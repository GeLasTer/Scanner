cmake_minimum_required(VERSION 3.15)

project(ScannerQRcode VERSION 1.0 LANGUAGES CXX)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "{YOUR PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
endif()

# زبان
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ScannerQRcode
        main.cpp
        ScannerQRcode.cpp
)

find_package(OpenCV REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(ZXing CONFIG REQUIRED)

target_link_libraries(ScannerQRcode PRIVATE
        ${OpenCV_LIBS}

        nlohmann_json::nlohmann_json
        ZXing::ZXing
)


target_include_directories(ScannerQRcode PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)
