#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Archive {
public:
    void save(const cv::Mat& frame, const std::string& data);
};