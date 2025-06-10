#include "Archive.h"
#include <filesystem>

void Archive::save(const cv::Mat& frame, const std::string& data) {
    std::string filename = "scan_" + std::to_string(time(0)) + ".png";
    cv::imwrite(filename, frame);
}