cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0074 NEW)

project(ScannerApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/002/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(OpenCV REQUIRED)
find_package(ZXing CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(Scanner STATIC Scanner.cpp)

target_link_libraries(Scanner PRIVATE
        ZXing::ZXing
        ${OpenCV_LIBS}
        nlohmann_json::nlohmann_json
)

add_executable(MainApp main.cpp)

target_link_libraries(MainApp PRIVATE Scanner)
