### QR Scanner App v1.0.0
The program is written in C++ and utilizes several powerful libraries to access the system's webcam, process images, and decode QR codes. When a QR code is presented to the camera, the program scans it, displays the decoded content on the screen, and saves both the image and its data as an archive in a JSON file.

___
##⚡ Version History
- **v1.0.0** – Initial release with basic QR code scanning and archiving features.

___
## 🔗 Dependencies
To build and run this project, make sure the following tools and libraries are installed:

- **MSVC** – Install with the "C++ Development" workload
- **CMake** – For building the project
- **vcpkg** – Used to manage the following C++ libraries:
  - `opencv`
  - `zxing-cpp`
  - `nlohmann-json`
___
## ⚙️ Installation Guide

### 1. Install Git
First, install **Git** from [git-scm.com](https://git-scm.com/), then add the `bin` folder path to your **Windows Environment Variables** so Git commands can be recognized in the terminal.

Clone the project using the command below:

```bash
git clone https://github.com/GeLasTer/Scanner.git
```
2. Install Microsoft Build Tools
Download and install Microsoft Build Tools from the official Microsoft website.
During installation, make sure to enable the C++ Development workload.
3. Install vcpkg
Clone vcpkg from GitHub:
```bash
git clone https://github.com/Microsoft/vcpkg.git
```
Build vcpkg using the instructions provided in its repository.

4. Install required libraries
You can install dependencies in two different ways:

🔸 Method 1 – Classic
Install the required packages manually:
```bash
vcpkg install opencv4
vcpkg install nu-book-zxing-cpp
vcpkg install nlohmann-json
```
🔹 Method 2 – Manifest Mode
This project includes a vcpkg.json file for Manifest mode.
Simply configure CMake to use vcpkg, and it will automatically install dependencies.

5. Configure CMake
Make sure to tell CMake where your vcpkg is installed by setting the toolchain file path:
```bash
-DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
```
```bash
cmake -B build -S .
cmake --build build
```
```bash
./build/ScannerApp
```
___
##📁 Project Structure
```bash
├── Archive.h              # Data archiving
├── FrameHandler.h         # Captures webcam frames
├── ScannerQRcode.cpp      # QR code scanning logic
├── Scanner.h              # Scanner core class
├── Ui.h                   # User interface (basic)
├── main.cpp               # Main entry point
├── CMakeLists.txt         # Build configuration
├── vcpkg.json             # Dependency manifest
├── LICENSE
├── README.md
└── .gitignore
```
___
##✍️ Author
. antique



