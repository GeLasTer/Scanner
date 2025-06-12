cmake_minimum_required(VERSION 3.15)
project(QRScanner)

# ابزار vcpkg برای نصب کتابخانه‌ها
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# یافتن کتابخانه‌ها (طبق config-style vcpkg)
find_package(opencv4 CONFIG REQUIRED)
find_package(Zxing CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# اضافه کردن ماژول‌ها
add_subdirectory(src/Archive)

# ساخت اجرایی اصلی پروژه
add_executable(QRScanner
        main.cpp
)

# لینک دادن کتابخانه‌ها به executable
target_link_libraries(QRScanner
        PRIVATE
        Archive
        opencv4::opencv4
        Zxing::Zxing
        nlohmann_json::nlohmann_json
)