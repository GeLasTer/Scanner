#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Archive/Archive.h"

int main() {
    // مرحله ۱: ساخت یک تصویر آزمایشی (مثلاً زمینه سیاه با متن)
    cv::Mat image = cv::Mat::zeros(300, 400, CV_8UC3);
    cv::putText(image, "Test QR Frame", cv::Point(50, 150), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

    // مرحله ۲: ساخت آرشیو و ذخیره تصویر
    Archive archiver("saved_frames");
    std::string savedPath = archiver.save(image, "Sample QR data");

    // مرحله ۳: نمایش خروجی برای اطمینان
    std::cout << "✅ تصویر ذخیره شد در مسیر: " << savedPath << std::endl;

    // مرحله ۴: (اختیاری) نمایش تصویر برای تست
    cv::imshow("Saved Frame", image);
    cv::waitKey(2000); // نمایش ۲ ثانیه
    return 0;
}