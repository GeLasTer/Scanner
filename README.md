# Scanner
# Archive Module

This module handles saving image frames that contain scanned QR codes.

## Responsibilities

- Save frames as .png images using OpenCV.
- Automatically name the images using timestamp-based filenames (e.g. frame_20250613_184503.png).
- Store the images inside a dedicated directory (saved_frames/).
- Intended to work alongside other modules, receiving the frame and scan results from the UI layer.

## Usage

`cpp
Archive archive("saved_frames");
std::string savedPath = archive.save(frame, "QR result");