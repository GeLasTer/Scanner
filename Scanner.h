#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <zxing/ZXing.h>  // فرض می‌کنم این مسیر در سیستم تو درسته

class Scanner {
public:
    Scanner();
    ~Scanner();

    // اسکن QR Code از تصویر ورودی و برگرداندن متن خوانده شده
    std::string scanQRCode(const cv::Mat& image);

private:
    // می‌تونی اگر نیاز داری توابع یا متغیرهای خصوصی اضافه کنی
};
