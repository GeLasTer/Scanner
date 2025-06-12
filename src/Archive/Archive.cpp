#include "Archive.h"
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <opencv2/imgcodecs.hpp>

Archive::Archive(const std::string& saveDir) : directory(saveDir) {
    std::filesystem::create_directories(directory); // اطمینان از وجود مسیر ذخیره
}

std::string Archive::generateTimestampFilename() const {
    auto now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);

    std::ostringstream oss;
    oss << "frame_" << std::put_time(&localTime, "%Y%m%d_%H%M%S") << ".png";
    return directory + "/" + oss.str();
}

std::string Archive::save(const cv::Mat& frame, const std::string& resultText) {
    std::string filename = generateTimestampFilename();
    cv::imwrite(filename, frame); // TODO: در آینده: فشرده‌سازی، هندل ارورها
    return filename;
}