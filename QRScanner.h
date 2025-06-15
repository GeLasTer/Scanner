#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <ZXing/DecodeHints.h>

class QRScanner {
public:
    QRScanner();
    ~QRScanner() = default;

    // اسکن تصویر و بازگرداندن متن QR، اگر نبود رشته خالی برمی‌گرداند
    std::string scan(const cv::Mat& frame);

private:
    ZXing::DecodeHints hints;
};
