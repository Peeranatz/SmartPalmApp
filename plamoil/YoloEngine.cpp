#include "YoloEngine.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;
using namespace cv::dnn;

bool RunYOLOv8_AI(const std::string& imagePath, const std::string& modelPath, cv::Mat& resultImg,
    int& countEmpty, int& countOverripe, int& countRipe, int& countRotten, int& countUnderripe, int& countUnripe,
    std::string& boxDataStr)
{
    // Initialize fixed BGR object class colors
    const std::vector<cv::Scalar> classColors = {
        cv::Scalar(128, 128, 240), cv::Scalar(0, 165, 255), cv::Scalar(50, 205, 50),
        cv::Scalar(192, 192, 192), cv::Scalar(0, 215, 255), cv::Scalar(255, 191, 0)
    };

    // 1. Read Display Configuration (Boxes, Masks, Class Filters)
    bool showBox = true, showMask = true;
    int filterId = -1;
    std::ifstream configFile("C:\\projectsky\\display_config.txt");
    if (configFile.is_open()) {
        std::string line;
        if (std::getline(configFile, line)) {
            std::vector<std::string> tokens; size_t pos = 0;
            while ((pos = line.find(',')) != std::string::npos) {
                tokens.push_back(line.substr(0, pos)); line.erase(0, pos + 1);
            }
            tokens.push_back(line);

            if (tokens.size() >= 1) showBox = (tokens[0] == "1");
            if (tokens.size() >= 2) showMask = (tokens[1] == "1");
            if (tokens.size() >= 3) filterId = std::stoi(tokens[2]);
        }
        configFile.close();
    }

    Mat img = imread(imagePath);
    if (img.empty()) return false;

    float scale = min(640.0f / img.cols, 640.0f / img.rows);
    int newW = round(img.cols * scale), newH = round(img.rows * scale);
    Mat resizedImg, letterboxImg(640, 640, CV_8UC3, Scalar(114, 114, 114));
    resize(img, resizedImg, Size(newW, newH));
    int padX = (640 - newW) / 2, padY = (640 - newH) / 2;
    resizedImg.copyTo(letterboxImg(Rect(padX, padY, newW, newH)));

    Net net;
    try { net = readNetFromONNX(modelPath); }
    catch (...) { return false; }

    Mat blob;
    blobFromImage(letterboxImg, blob, 1.0 / 255.0, Size(640, 640), Scalar(), true, false);
    net.setInput(blob);

    vector<Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    Mat detections, prototypes;
    for (size_t i = 0; i < outputs.size(); i++) {
        if (outputs[i].dims == 3) detections = outputs[i];
        else if (outputs[i].dims == 4) prototypes = outputs[i];
    }
    bool isSegmentation = !prototypes.empty();

    Mat dets(detections.size[1], detections.size[2], CV_32F, detections.ptr<float>());
    dets = dets.t();

    vector<Rect> boxes; vector<float> confidences; vector<int> classIds; vector<vector<float>> maskCoeffs;
    for (int i = 0; i < dets.rows; i++) {
        float* rowPtr = dets.ptr<float>(i);
        float* classesScores = rowPtr + 4;
        Point classIdPoint; double maxClassScore;
        minMaxLoc(Mat(1, 6, CV_32F, classesScores), nullptr, &maxClassScore, nullptr, &classIdPoint);

        if (maxClassScore > 0.45) {
            float cx = rowPtr[0], cy = rowPtr[1], cw = rowPtr[2], ch = rowPtr[3];
            boxes.push_back(Rect(int(cx - cw / 2), int(cy - ch / 2), int(cw), int(ch)));
            confidences.push_back((float)maxClassScore);
            classIds.push_back(classIdPoint.x);

            if (isSegmentation) {
                vector<float> coeffs(32);
                for (int j = 0; j < 32; j++) coeffs[j] = rowPtr[10 + j];
                maskCoeffs.push_back(coeffs);
            }
        }
    }

    vector<int> indices;
    NMSBoxes(boxes, confidences, 0.45f, 0.5f, indices);

    Mat drawImg = letterboxImg.clone();
    Mat protos2D;
    if (isSegmentation) protos2D = Mat(32, 160 * 160, CV_32F, prototypes.ptr<float>());

    for (int idx : indices) {
        int cId = classIds[idx];

        if (filterId != -1 && cId != filterId) continue;

        Rect box = boxes[idx];
        float conf = confidences[idx];

        if (cId == 0) countEmpty++; else if (cId == 1) countOverripe++; else if (cId == 2) countRipe++;
        else if (cId == 3) countRotten++; else if (cId == 4) countUnderripe++; else if (cId == 5) countUnripe++;

        boxDataStr += to_string(cId) + "," + to_string(box.x + box.width / 2.0f) + "," + to_string(box.y + box.height / 2.0f) + "," + to_string((float)box.width) + "," + to_string((float)box.height) + "\n";

        Scalar color = classColors[cId % classColors.size()];
        Rect clippedBox = box & Rect(0, 0, drawImg.cols, drawImg.rows);

        if (showMask && isSegmentation && clippedBox.area() > 0) {
            Mat coeffsMat(1, 32, CV_32F, maskCoeffs[idx].data());
            Mat maskMat = coeffsMat * protos2D;
            maskMat = maskMat.reshape(1, 160);
            exp(-maskMat, maskMat);
            maskMat = 1.0f / (1.0f + maskMat);

            Mat mask640;
            resize(maskMat, mask640, Size(640, 640), 0, 0, INTER_LINEAR);
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

    resultImg = drawImg.clone();
    return true;
}