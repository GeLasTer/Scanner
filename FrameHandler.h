#pragma once

#include <opencv2/opencv.hpp>

class FrameHandler {
public:
    FrameHandler(int cameraIndex = 0);
    ~FrameHandler();

    // تلاش برای باز کردن دوربین، نتیجه موفقیت یا شکست
    bool openCamera(int cameraIndex);

    // دریافت فریم جدید، اگر فریم قابل دریافت نبود، ماتریس خالی برمی‌گرداند
    cv::Mat getFrame();

    // بررسی وضعیت دوربین
    bool isOpened() const noexcept;

private:
    cv::VideoCapture cap;
};
