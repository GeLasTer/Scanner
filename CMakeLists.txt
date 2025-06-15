cmake_minimum_required(VERSION 3.15)
project(ScannerQR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# پشتیبانی از vcpkg اگر تعریف شده باشد
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# پیدا کردن پکیج‌ها
find_package(OpenCV REQUIRED)
find_package(ZXing CONFIG REQUIRED)

# فایل‌های سورس و هدرها
set(SOURCES
        main.cpp
        src/FrameHandler/FrameHandler.cpp
        src/QRScanner/QRScanner.cpp
        src/UI/UI.cpp
        src/Archive/Archive.cpp
)

set(HEADERS
        src/FrameHandler/FrameHandler.h
        src/QRScanner/QRScanner.h
        src/UI/UI.h
        src/Archive/Archive.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        src/
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ZXing::Core            # اگر این خط خطا داد، به ZXing::ZXing تغییر بده
        ${OpenCV_LIBS}
)

# فعال کردن هشدارها و بهینه‌سازی کامپایلر
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
