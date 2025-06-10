cmake_minimum_required(VERSION 3.10)
project(QRScanner)

# تنظیم vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

# پیدا کردن کتابخانه‌ها
find_package(OpenCV REQUIRED)
find_package(Zxing REQUIRED)          # برای zxing-cpp
find_package(nlohmann_json REQUIRED)  # برای json

# اضافه کردن ماژول Archive
add_subdirectory(src/Archive)

# برنامه اصلی
add_executable(QRScanner main.cpp)
target_link_libraries(QRScanner PRIVATE 
    Archive 
    OpenCV::OpenCV 
    Zxing::Zxing 
    nlohmann_json::nlohmann_json
)