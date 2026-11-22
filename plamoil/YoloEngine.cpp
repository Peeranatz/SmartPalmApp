#include "YoloEngine.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;
using namespace cv::dnn;

// ---------------------------------------------------------------------------
// Caches
//  1) The ONNX network is loaded once and reused (loading it costs more than
//     running it).
//  2) The raw detections of the last analysed image are kept, so toggling
//     "show box" / "show mask" / class filter only re-draws instead of
//     running the model again.
// Both caches are only touched from the UI thread.
// ---------------------------------------------------------------------------
namespace {

const int kNumClasses = 6;
const int kMaskCoeffs = 32;
const int kInput = 640;
// Keep these equal to model.track(conf=..., iou=...) in ai_server.py so that
// image mode (C++) and video mode (Python) give the same detections.
const float kConfThreshold = 0.40f;
const float kNmsIouThreshold = 0.50f;
const int kMaxDetections = 300;   // Ultralytics default max_det

struct NetCache {
    std::string modelPath;
    long long modelStamp = -1;
    Net net;
    bool loaded = false;
} g_net;

struct DetectionCache {
    std::string imagePath;
    long long imageStamp = -1;
    std::string modelPath;
    Mat letterboxImg;
    Mat protos2D;               // 32 x (160*160), owned copy
    bool isSegmentation = false;
    vector<Rect> boxes;         // after NMS
    vector<float> confidences;
    vector<int> classIds;
    vector<vector<float>> maskCoeffs;
    bool valid = false;
} g_det;

// Last-write time + size, so a file replaced under the same name is re-read.
long long FileStamp(const std::string& path) {
    struct _stat64 st;
    if (_stat64(path.c_str(), &st) != 0) return -1;
    return (long long)st.st_mtime * 1000003LL + (long long)st.st_size;
}

bool EnsureNet(const std::string& modelPath) {
    long long stamp = FileStamp(modelPath);
    if (g_net.loaded && g_net.modelPath == modelPath && g_net.modelStamp == stamp) return true;
    try {
        g_net.net = readNetFromONNX(modelPath);
    }
    catch (...) {
        g_net.loaded = false;
        return false;
    }
    g_net.modelPath = modelPath;
    g_net.modelStamp = stamp;
    g_net.loaded = !g_net.net.empty();
    g_det.valid = false;  // new model -> old detections are stale
    return g_net.loaded;
}

bool Detect(const std::string& imagePath, const std::string& modelPath) {
    long long stamp = FileStamp(imagePath);
    if (g_det.valid && g_det.imagePath == imagePath && g_det.imageStamp == stamp &&
        g_det.modelPath == modelPath && g_net.loaded && g_net.modelPath == modelPath &&
        g_net.modelStamp == FileStamp(modelPath)) {
        return true;  // cache hit
    }

    Mat img = imread(imagePath);
    if (img.empty()) return false;
    if (!EnsureNet(modelPath)) return false;

    // Letterbox to 640x640
    float scale = min((float)kInput / img.cols, (float)kInput / img.rows);
    int newW = (int)std::lround(img.cols * scale), newH = (int)std::lround(img.rows * scale);
    Mat resizedImg, letterboxImg(kInput, kInput, CV_8UC3, Scalar(114, 114, 114));
    resize(img, resizedImg, Size(newW, newH));
    int padX = (kInput - newW) / 2, padY = (kInput - newH) / 2;
    resizedImg.copyTo(letterboxImg(Rect(padX, padY, newW, newH)));

    Mat blob;
    blobFromImage(letterboxImg, blob, 1.0 / 255.0, Size(kInput, kInput), Scalar(), true, false);
    g_net.net.setInput(blob);

    vector<Mat> outputs;
    try {
        g_net.net.forward(outputs, g_net.net.getUnconnectedOutLayersNames());
    }
    catch (...) {
        return false;
    }

    Mat detections, prototypes;
    for (size_t i = 0; i < outputs.size(); i++) {
        if (outputs[i].dims == 3) detections = outputs[i];
        else if (outputs[i].dims == 4) prototypes = outputs[i];
    }
    if (detections.empty()) return false;
    bool isSegmentation = !prototypes.empty();

    // [1, 4 + classes + 32, 8400] -> rows = candidates
    Mat dets(detections.size[1], detections.size[2], CV_32F, detections.ptr<float>());
    dets = dets.t();

    vector<Rect> boxes; vector<float> confidences; vector<int> classIds; vector<vector<float>> coeffs;
    for (int i = 0; i < dets.rows; i++) {
        float* rowPtr = dets.ptr<float>(i);
        float* classesScores = rowPtr + 4;
        Point classIdPoint; double maxClassScore;
        minMaxLoc(Mat(1, kNumClasses, CV_32F, classesScores), nullptr, &maxClassScore, nullptr, &classIdPoint);

        if (maxClassScore > kConfThreshold) {
            float cx = rowPtr[0], cy = rowPtr[1], cw = rowPtr[2], ch = rowPtr[3];
            boxes.push_back(Rect(int(cx - cw / 2), int(cy - ch / 2), int(cw), int(ch)));
            confidences.push_back((float)maxClassScore);
            classIds.push_back(classIdPoint.x);
            if (isSegmentation) {
                vector<float> c(kMaskCoeffs);
                for (int j = 0; j < kMaskCoeffs; j++) c[j] = rowPtr[4 + kNumClasses + j];
                coeffs.push_back(c);
            }
        }
    }

    vector<int> indices;
    // Per-class NMS, like Ultralytics (agnostic_nms=False): a ripe and an underripe
    // box on the same spot do not suppress each other.
    NMSBoxesBatched(boxes, confidences, classIds, kConfThreshold, kNmsIouThreshold, indices);
    if ((int)indices.size() > kMaxDetections) indices.resize(kMaxDetections);

    g_det.boxes.clear(); g_det.confidences.clear(); g_det.classIds.clear(); g_det.maskCoeffs.clear();
    for (int idx : indices) {
        g_det.boxes.push_back(boxes[idx]);
        g_det.confidences.push_back(confidences[idx]);
        g_det.classIds.push_back(classIds[idx]);
        if (isSegmentation) g_det.maskCoeffs.push_back(coeffs[idx]);
    }
    g_det.isSegmentation = isSegmentation;
    g_det.protos2D = isSegmentation ? Mat(kMaskCoeffs, 160 * 160, CV_32F, prototypes.ptr<float>()).clone() : Mat();
    g_det.letterboxImg = letterboxImg;
    g_det.imagePath = imagePath;
    g_det.imageStamp = stamp;
    g_det.modelPath = modelPath;
    g_det.valid = true;
    return true;
}

void ReadDisplayConfig(bool& showBox, bool& showMask, int& filterId) {
    std::ifstream configFile("C:\\projectsky\\display_config.txt");
    if (!configFile.is_open()) return;
    std::string line;
    if (!std::getline(configFile, line)) return;
    std::vector<std::string> tokens; size_t pos = 0;
    while ((pos = line.find(',')) != std::string::npos) {
        tokens.push_back(line.substr(0, pos)); line.erase(0, pos + 1);
    }
    tokens.push_back(line);
    if (tokens.size() >= 1) showBox = (tokens[0] == "1");
    if (tokens.size() >= 2) showMask = (tokens[1] == "1");
    if (tokens.size() >= 3) {
        try { filterId = std::stoi(tokens[2]); }
        catch (...) { filterId = -1; }
    }
}

}  // namespace

bool RunYOLOv8_AI(const std::string& imagePath, const std::string& modelPath, cv::Mat& resultImg,
    int& countEmpty, int& countOverripe, int& countRipe, int& countRotten, int& countUnderripe, int& countUnripe,
    std::string& boxDataStr)
{
    // Fixed BGR class colors (match the Python palette and History view)
    const std::vector<cv::Scalar> classColors = {
        cv::Scalar(128, 128, 240), cv::Scalar(0, 165, 255), cv::Scalar(50, 205, 50),
        cv::Scalar(192, 192, 192), cv::Scalar(0, 215, 255), cv::Scalar(255, 191, 0)
    };

    bool showBox = true, showMask = true;
    int filterId = -1;
    ReadDisplayConfig(showBox, showMask, filterId);

    if (!Detect(imagePath, modelPath)) return false;

    Mat drawImg = g_det.letterboxImg.clone();

    for (size_t k = 0; k < g_det.boxes.size(); k++) {
        int cId = g_det.classIds[k];
        if (filterId != -1 && cId != filterId) continue;

        Rect box = g_det.boxes[k];
        float conf = g_det.confidences[k];

        if (cId == 0) countEmpty++; else if (cId == 1) countOverripe++; else if (cId == 2) countRipe++;
        else if (cId == 3) countRotten++; else if (cId == 4) countUnderripe++; else if (cId == 5) countUnripe++;

        boxDataStr += to_string(cId) + "," + to_string(box.x + box.width / 2.0f) + "," + to_string(box.y + box.height / 2.0f) + "," + to_string((float)box.width) + "," + to_string((float)box.height) + "\n";

        Scalar color = classColors[cId % classColors.size()];
        Rect clippedBox = box & Rect(0, 0, drawImg.cols, drawImg.rows);

        if (showMask && g_det.isSegmentation && clippedBox.area() > 0) {
            Mat coeffsMat(1, kMaskCoeffs, CV_32F, const_cast<float*>(g_det.maskCoeffs[k].data()));
            Mat maskMat = coeffsMat * g_det.protos2D;
            maskMat = maskMat.reshape(1, 160);
            exp(-maskMat, maskMat);
            maskMat = 1.0f / (1.0f + maskMat);

            Mat mask640;
            resize(maskMat, mask640, Size(kInput, kInput), 0, 0, INTER_LINEAR);
            Mat boxMask = mask640(clippedBox) > 0.5f;
            Mat roiImg = drawImg(clippedBox);

            for (int y = 0; y < boxMask.rows; ++y) {
                const uchar* maskPtr = boxMask.ptr<uchar>(y);
                Vec3b* imgPtr = roiImg.ptr<Vec3b>(y);
                for (int x = 0; x < boxMask.cols; ++x) {
                    if (maskPtr[x]) {
                        imgPtr[x][0] = saturate_cast<uchar>(imgPtr[x][0] * 0.5 + color[0] * 0.5);
                        imgPtr[x][1] = saturate_cast<uchar>(imgPtr[x][1] * 0.5 + color[1] * 0.5);
                        imgPtr[x][2] = saturate_cast<uchar>(imgPtr[x][2] * 0.5 + color[2] * 0.5);
                    }
                }
            }
        }

        if (showBox) {
            rectangle(drawImg, box, color, 2);
            string labelStr = to_string(cId) + " " + format("%.2f", conf);
            int baseLine = 0;
            Size labelSize = getTextSize(labelStr, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            Rect labelBg(box.x, box.y - labelSize.height - 5, labelSize.width, labelSize.height + 5);
            rectangle(drawImg, labelBg, color, FILLED);
            putText(drawImg, labelStr, Point(box.x, box.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        }
    }

    resultImg = drawImg;
    return true;
}
