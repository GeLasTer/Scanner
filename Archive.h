#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>

class Archive {
public:
    explicit Archive(const std::string& saveDir = "saved_frames");


    std::string save(const cv::Mat& frame, const std::string& resultText);

private:
    std::string directory;
    std::string jsonPath;

    std::string generateTimestampFilename() const;
    void updateJson(const std::string& imagePath, const std::string& content);
};
