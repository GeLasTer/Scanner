#pragma once

#include <string>
#include <opencv2/core.hpp>

class Archive {
public:
    explicit Archive(const std::string& saveDir = "saved_frames");

    // ذخیره فریم با متن نتیجه اسکن و برگرداندن نام فایل ذخیره شده (خالی در صورت خطا)
    std::string save(const cv::Mat& frame, const std::string& resultText);

private:
    std::string directory;   // مسیر پوشه ذخیره‌سازی

    // تولید نام فایل یکتا بر اساس زمان فعلی
    std::string generateTimestampFilename() const;
};
