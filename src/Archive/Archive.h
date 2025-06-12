#pragma once
#include <string>
#include <opencv2/core.hpp>
class Archive {
public:
    explicit Archive(const std::string& saveDir = "saved_frames");

    /**
     * ذخیره فریم تصویری به‌عنوان فایل PNG
     * @param frame: تصویر دریافتی (فریم شامل QR)
     * @param resultText: داده استخراج‌شده از QR (برای لاگ یا تست آینده)
     * @return مسیر فایل ذخیره‌شده
     */
    std::string save(const cv::Mat& frame, const std::string& resultText);

private:
    std::string directory;
    std::string generateTimestampFilename() const;
};