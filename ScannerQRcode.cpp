// Merged.cpp
// FrameHandler.cpp, Scanner.cpp, UI.cpp, Archive.cpp

#include "FrameHandler.h"
#include "Scanner.h"
#include "UI.h"
#include "Archive.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <opencv2/imgcodecs.hpp>
#include <nlohmann/json.hpp>

// ======= FrameHandler.cpp =======

FrameHandler::FrameHandler() {
    cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "Error to Open Camera" << std::endl;
    }
}

cv::Mat FrameHandler::getFrame() {
    cv::Mat frame;
    cap >> frame;

    if (frame.empty()) {
        std::cerr << "frame empty!" << std::endl;
    }

    return frame;
}

// ======= Scanner.cpp =======

Scanner::Scanner() {}

Scanner::~Scanner() {}

std::string Scanner::scanQRCode(const cv::Mat& image) {
    if (image.empty()) {
        return "";
    }

    cv::Mat gray;
    if (image.channels() == 3)
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else if (image.channels() == 4)
        cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
    else
        gray = image;

    ZXing::ImageView zxingImage(gray.data, gray.cols, gray.rows, ZXing::ImageFormat::Lum);

    ZXing::DecodeHints hints;
    hints.setFormats(ZXing::BarcodeFormat::QRCode);

    ZXing::Barcode result = ZXing::ReadBarcode(zxingImage, hints);

    if (result.isValid())
        return result.text();

    return "";
}

// ======= UI.cpp =======

void UI::displayFrame(const cv::Mat& frame, const std::vector<std::string>& results) {
    cv::Mat displayFrame = frame.clone();

    if (results.empty()) {
        std::string status_text = "[Scanning...]";
        cv::Point text_position(20, 40);
        cv::putText(displayFrame, status_text, text_position, cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 255), 3);
    } else {
        std::string status_text = "[Code Detected!]";
        cv::Point status_position(20, 40);
        cv::putText(displayFrame, status_text, status_position, cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 255, 0), 3);

        for (size_t i = 0; i < results.size(); ++i) {
            cv::Point data_position(20, 80 + static_cast<int>(i) * 40);
            cv::putText(displayFrame, results[i], data_position, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 0), 2);
        }
    }

    cv::imshow("QR Code Scanner", displayFrame);
}

// ======= Archive.cpp =======


namespace fs = std::filesystem;
using json = nlohmann::json;

Archive::Archive(const std::string& saveDir)
    : directory(saveDir), jsonPath(saveDir + "/data.json") {

    if (!fs::exists(directory)) {
        try {
            fs::create_directories(directory);
            std::cout << "Directory created: " << directory << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
        }
    }
}

std::string Archive::generateTimestampFilename() const {
    auto now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);

    std::ostringstream oss;
    oss << "qr_" << std::put_time(&localTime, "%Y%m%d_%H%M%S") << ".png";
    return (fs::path(directory) / oss.str()).string();
}

std::string Archive::save(const cv::Mat& frame, const std::string& resultText) {
    if (frame.empty()) {
        std::cerr << "Error: Cannot save empty frame" << std::endl;
        return "";
    }

    if (resultText.empty()) {
        std::cerr << "Warning: QR content is empty" << std::endl;
    }

    std::string imagePath = generateTimestampFilename();
    try {
        if (!cv::imwrite(imagePath, frame)) {
            std::cerr << "Failed to save image: " << imagePath << std::endl;
            return "";
        }
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV error: " << e.what() << std::endl;
        return "";
    }

    updateJson(imagePath, resultText);
    return imagePath;
}

void Archive::updateJson(const std::string& imagePath, const std::string& content) {
    json jArray;

    std::ifstream inFile(jsonPath);
    if (inFile.good()) {
        try {
            inFile >> jArray;
        } catch (const json::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << " - Creating new file." << std::endl;
            jArray = json::array();
        }
    } else {
        jArray = json::array();
    }

    auto now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &localTime);

    json newEntry = {
        {"content", content},
        {"image_path", fs::path(imagePath).filename().string()},
        {"full_path", imagePath},
        {"timestamp", timestamp}
    };

    jArray.push_back(newEntry);

    std::ofstream outFile(jsonPath);
    if (!outFile) {
        std::cerr << "Failed to open JSON file for writing: " << jsonPath << std::endl;
        return;
    }
    outFile << std::setw(4) << jArray << std::endl;
}