#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <ZXing/ReadBarcode.h>
#include <ZXing/BarcodeFormat.h>
#include <ZXing/DecodeHints.h>
#include <ZXing/Barcode.h>
#include <ZXing/ImageView.h>
#include <zxing/ZXingcpp.h>

class Scanner{
public:
    Scanner();
    ~Scanner();

    std::string scanQRCode(const cv::Mat& image);

private:
};
