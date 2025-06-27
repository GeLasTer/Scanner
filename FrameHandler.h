#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H

#include <opencv2/opencv.hpp>

class FrameHandler {
public:
    FrameHandler();
    cv::Mat getFrame();

private:
    cv::VideoCapture cap;
};

#endif