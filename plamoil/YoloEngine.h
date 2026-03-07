#pragma once
#include <opencv2/opencv.hpp>
#include <string>

// อัปเกรดเพิ่ม std::string& outBoxData ไว้ท้ายสุด
bool RunYOLOv8_AI(const std::string& imgPath, const std::string& modelPath, cv::Mat& outImg,
    int& cEmpty, int& cOverripe, int& cRipe, int& cRotten, int& cUnderripe, int& cUnripe,
    std::string& outBoxData);