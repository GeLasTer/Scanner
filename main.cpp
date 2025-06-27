#include "FrameHandler.h"
#include "Scanner.h"
#include "Ui.h"
#include "Archive.h"

#include <opencv2/highgui.hpp>
#include <iostream>

int main() {
    FrameHandler camera;
    Scanner scanner;
    UI ui;
    Archive archive;

    while (true) {
        cv::Mat frame = camera.getFrame();

        if (frame.empty()) {
            std::cerr << "Frame empty!" << std::endl;
            break;
        }

        std::string result = scanner.scanQRCode(frame);
        std::vector<std::string> results;

        if (!result.empty()) {
            results.push_back(result);
            archive.save(frame, result);
        }

        ui.displayFrame(frame, results);

        int key = cv::waitKey(10);
        if (key == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
