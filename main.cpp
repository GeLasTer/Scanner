#include "src/Archive/Archive.h"
#include <opencv2/opencv.hpp>

int main() {
    Archive arch;
    cv::Mat testFrame = cv::Mat::zeros(480, 640, CV_8UC3); // یک فریم مشکی تست
    arch.save(testFrame, "TEST_DATA");
    return 0;
}