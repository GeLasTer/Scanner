#include "Scanner.h"

Scanner::Scanner() {
    // سازنده در صورت نیاز می‌تونه کارهای اولیه رو انجام بده
}

Scanner::~Scanner() {
    // پاکسازی‌ها (اگر نیاز باشه)
}

std::string Scanner::scanQRCode(const cv::Mat& image) {
    if (image.empty()) {
        return "";
    }

    // تبدیل تصویر opencv به ZXing::ImageView یا ZXing::Image (بسته به نسخه zxing شما)
    // فرض می‌کنیم از نسخه zxing-cpp جدید استفاده می‌کنی

    // تبدیل cv::Mat به GrayScale
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else if (image.channels() == 4) {
        cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = image;
    }

    ZXing::ImageView zxingImage(gray.data, gray.cols, gray.rows, ZXing::ImageFormat::Lum);

    auto result = ZXing::ReadBarcode(zxingImage, ZXing::BarcodeFormat::QR_CODE);

    if (result.isValid()) {
        return result.text();
    }

    return "";
}
