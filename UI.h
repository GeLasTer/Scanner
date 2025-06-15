#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

// ساختار برای نگهداری نتیجه اسکن و موقعیت QR
struct ScanResult {
    std::string text;
    cv::RotatedRect position;
};

class UI {
public:
    explicit UI(const std::string& window_name = "QR Code Scanner");

    // نمایش فریم همراه با موقعیت‌های اسکن شده و متن وضعیت روی پنجره
    void displayFrame(const cv::Mat& frame, const std::vector<ScanResult>& results, const std::string& status);

private:
    std::string window_name_;

    // رسم مستطیل و متن روی فریم برای هر کد QR اسکن شده
    void drawScanResults(cv::Mat& frame, const std::vector<ScanResult>& results);

    // رسم متن وضعیت روی فریم
    void drawStatus(cv::Mat& frame, const std::string& status);
};
