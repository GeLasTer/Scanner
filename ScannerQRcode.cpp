#pragma once

#include <opencv2/opencv.hpp>
#include <ZXing/ReadBarcode.h>
#include <ZXing/TextUtfEncoding.h>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iomanip>
#include <sstream>
#include <iostream>

// ------------------ Configuration ------------------
struct Config {
    int cameraIndex = 0;
    int frameWidth = 1280;
    int frameHeight = 720;
    std::string saveDirectory = "saved_frames";
    std::string windowName = "QR Code Scanner";
    float scanInterval = 0.1f; // seconds
    bool saveScannedFrames = true;
    bool showDebugInfo = true;
};

// ------------------ FrameHandler ------------------
class FrameHandler {
public:
    FrameHandler(const Config& config) : config_(config) {
        cap_.open(config.cameraIndex);
        if (!cap_.isOpened()) {
            throw std::runtime_error("⛔ Failed to open camera!");
        }
        cap_.set(cv::CAP_PROP_FRAME_WIDTH, config.frameWidth);
        cap_.set(cv::CAP_PROP_FRAME_HEIGHT, config.frameHeight);
    }

    cv::Mat getFrame() {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> lock(cap_mutex_);
            cap_ >> frame;
        }

        if (frame.empty() && config_.showDebugInfo) {
            std::cerr << "⚠️ Empty frame received!" << std::endl;
        }
        return frame;
    }

    bool isOpened() const {
        return cap_.isOpened();
    }

private:
    cv::VideoCapture cap_;
    std::mutex cap_mutex_;
    const Config& config_;
};

// ------------------ QRScanner ------------------
struct ScanResult {
    std::string text;
    std::vector<cv::Point> position;
    double decodeTime = 0.0;
};

class QRScanner {
public:
    QRScanner(const Config& config) : config_(config) {
        hints_.setFormats(ZXing::BarcodeFormat::QRCode);
        hints_.setTryHarder(true);
        hints_.setTryRotate(true);
    }

    std::vector<ScanResult> scan(const cv::Mat& frame) {
        std::vector<ScanResult> results;

        if (frame.empty())
            return results;

        auto start = std::chrono::high_resolution_clock::now();

        try {
            cv::Mat gray;
            if (frame.channels() == 3)
                cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            else if (frame.channels() == 4)
                cv::cvtColor(frame, gray, cv::COLOR_BGRA2GRAY);
            else
                gray = frame;

            ZXing::ImageView imageView(gray.data, gray.cols, gray.rows, ZXing::ImageFormat::Lum);
            ZXing::Barcode barcode = ZXing::ReadBarcode(imageView, hints_);

            if (barcode.isValid()) {
                ScanResult result;
                result.text = ZXing::TextUtfEncoding::ToUtf8(barcode.text());

                auto points = barcode.position().points();
                for (const auto& p : points) {
                    result.position.emplace_back(p.x, p.y);
                }

                results.push_back(result);
            }

        } catch (const std::exception& e) {
            if (config_.showDebugInfo) {
                std::cerr << "❌ Scanner Error: " << e.what() << std::endl;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (!results.empty()) {
            results.back().decodeTime = elapsed.count();
        }

        return results;
    }

private:
    ZXing::DecodeHints hints_;
    const Config& config_;
};

// ------------------ UI ------------------
class UI {
public:
    UI(const Config& config) : config_(config) {
        cv::namedWindow(config.windowName, cv::WINDOW_NORMAL);
        cv::resizeWindow(config.windowName, config.frameWidth, config.frameHeight);
        lastTime_ = std::chrono::system_clock::now();
    }

    void displayFrame(const cv::Mat& frame, const std::vector<ScanResult>& results, const std::string& status) {
        cv::Mat displayFrame = frame.clone();

        // Draw QR code boundaries and text
        for (const auto& result : results) {
            if (result.position.size() >= 4) {
                // Draw bounding polygon
                cv::polylines(displayFrame, result.position, true, cv::Scalar(0, 255, 0), 2);

                // Draw center point
                cv::Point center(0, 0);
                for (const auto& p : result.position) {
                    center += p;
                }
                center.x /= static_cast<int>(result.position.size());
                center.y /= static_cast<int>(result.position.size());
                cv::circle(displayFrame, center, 5, cv::Scalar(0, 255, 255), -1);

                // Draw text (split if too long)
                std::string displayText = result.text.length() > 30 ?
                    result.text.substr(0, 27) + "..." : result.text;
                cv::putText(displayFrame, displayText, result.position[0] - cv::Point(0, 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 1);

                // Display decode time if debug info is enabled
                if (config_.showDebugInfo) {
                    std::ostringstream ss;
                    ss << "Decode: " << std::fixed << std::setprecision(1) << (result.decodeTime * 1000) << "ms";
                    cv::putText(displayFrame, ss.str(), center + cv::Point(20, 0),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1);
                }
            }
        }

        // Draw status bar
        cv::Rect statusBar(0, 0, displayFrame.cols, 30);
        cv::rectangle(displayFrame, statusBar, cv::Scalar(0, 0, 0), cv::FILLED);
        cv::putText(displayFrame, status, cv::Point(10, 20),
            cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);

        // Display FPS counter
        if (config_.showDebugInfo) {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime_).count();
            if (duration > 0) {
                double fps = 1000.0 / duration;
                std::ostringstream ss;
                ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
                cv::putText(displayFrame, ss.str(), cv::Point(displayFrame.cols - 100, 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
            }
            lastTime_ = now;
        }

        cv::imshow(config_.windowName, displayFrame);
    }

    void close() {
        cv::destroyWindow(config_.windowName);
    }

private:
    const Config& config_;
    std::chrono::time_point<std::chrono::system_clock> lastTime_;
};

// ------------------ Archive ------------------
class Archive {
public:
    explicit Archive(const Config& config) : config_(config) {
        try {
            std::filesystem::create_directories(config.saveDirectory);
        }
        catch (const std::filesystem::filesystem_error& e) {
            if (config_.showDebugInfo) {
                std::cerr << "⛔ Archive Error: " << e.what() << std::endl;
            }
        }
    }

    std::string save(const cv::Mat& frame, const std::string& resultText) {
        if (!config_.saveScannedFrames || resultText.empty()) {
            return "";
        }

        std::string filename = generateFilename(resultText);
        try {
            if (!cv::imwrite(filename, frame)) {
                if (config_.showDebugInfo) {
                    std::cerr << "⛔ Failed to save image: " << filename << std::endl;
                }
                return "";
            }
            return filename;
        }
        catch (const cv::Exception& e) {
            if (config_.showDebugInfo) {
                std::cerr << "⛔ OpenCV Exception: " << e.what() << std::endl;
            }
            return "";
        }
    }

private:
    const Config& config_;

    std::string sanitizeFilename(const std::string& input) const {
        std::string output;
        for (char c : input) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.') {
                output += c;
            }
            else if (c == ' ') {
                output += '_';
            }
        }
        return output.substr(0, 50); // Limit filename length
    }

    std::string generateFilename(const std::string& resultText) const {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::tm localTime;
#if defined(_WIN32)
        localtime_s(&localTime, &time);
#else
        localtime_r(&time, &localTime);
#endif

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count() % 1000;

        std::ostringstream oss;
        oss << config_.saveDirectory << "/QR_"
            << std::put_time(&localTime, "%Y%m%d_%H%M%S")
            << "_" << std::setw(3) << std::setfill('0') << ms
            << "_" << sanitizeFilename(resultText) << ".png";

        return oss.str();
    }
};

// ------------------ Main Application ------------------
class QRScannerApp {
public:
    QRScannerApp(const Config& config = Config{})
        : config_(config),
        frameHandler_(config),
        scanner_(config),
        ui_(config),
        archive_(config),
        running_(false) {}

    ~QRScannerApp() {
        stop();
    }

    void start() {
        if (!frameHandler_.isOpened()) {
            std::cerr << "⛔ Camera not available!" << std::endl;
            return;
        }

        running_ = true;
        processingThread_ = std::thread(&QRScannerApp::processingLoop, this);

        mainLoop();
    }

    void stop() {
        running_ = false;
        queueCondition_.notify_all();
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
        ui_.close();
    }

private:
    void mainLoop() {
        while (running_) {
            cv::Mat frame = frameHandler_.getFrame();
            if (frame.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(queueMutex_);
                frameQueue_.push(frame.clone());
            }
            queueCondition_.notify_one();

            std::vector<ScanResult> currentResults;
            {
                std::lock_guard<std::mutex> lock(resultsMutex_);
                currentResults = lastResults_;
            }

            std::string status = currentResults.empty() ?
                "Scanning..." : "Found: " + std::to_string(currentResults.size()) + " QR codes";

            ui_.displayFrame(frame, currentResults, status);

            int key = cv::waitKey(1);
            if (key == 27) { // ESC key
                stop();
                break;
            }
        }
    }

    void processingLoop() {
        while (running_) {
            cv::Mat frame;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                queueCondition_.wait_for(lock, std::chrono::milliseconds(100),
                    [this]() { return !frameQueue_.empty() || !running_; });

                if (!running_) break;
                if (frameQueue_.empty()) continue;

                frame = frameQueue_.front();
                frameQueue_.pop();
            }

            auto results = scanner_.scan(frame);

            if (!results.empty()) {
                std::string firstResult = results[0].text;
                archive_.save(frame, firstResult);
            }

            {
                std::lock_guard<std::mutex> lock(resultsMutex_);
                lastResults_ = results;
            }
        }
    }

    Config config_;
    FrameHandler frameHandler_;
    QRScanner scanner_;
    UI ui_;
    Archive archive_;

    std::atomic<bool> running_;
    std::thread processingThread_;
    std::queue<cv::Mat> frameQueue_;
    std::mutex queueMutex_;
    std::condition_variable queueCondition_;
    std::vector<ScanResult> lastResults_;
    std::mutex resultsMutex_;
};

// ------------------ Usage Example ------------------
/*
int main() {
    Config config;
    config.cameraIndex = 0;
    config.frameWidth = 1280;
    config.frameHeight = 720;
    config.showDebugInfo = true;
    config.saveScannedFrames = true;

    QRScannerApp app(config);
    app.start();

    return 0;
}
*/
