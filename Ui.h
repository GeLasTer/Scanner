#ifndef UI_H
#define UI_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class UI {
public:
    void displayBarcodeText(const std::vector<std::string>& barcodeContents);

    void displayFrame(const cv::Mat& frame, const std::vector<std::string>& results);
};

#endif