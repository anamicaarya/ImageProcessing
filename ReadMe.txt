# ImageProcessing

A small Windows image processing demo in C++ using MFC. It converts images to grayscale and applies a basic edge-detection filter.

## Features
- Grayscale conversion
- Edge detection (simple kernel-based approach)
- Minimal MFC dialog app to wire the operations

## Tech Stack
- C++ with Microsoft Foundation Class Library (MFC)
- Win32 desktop application
- No external image libraries

## Project Structure
- `MyApp.sln`, `MyApp.vcxproj`, `MyApp.vcproj`, `MyApp.dsw`, `MyApp.dsp` — Solution and project files for different Visual Studio generations
- `MyApp.cpp`, `MyApp.h` — Application entry and app class
- `MyAppDlg.cpp`, `MyAppDlg.h` — Main dialog and UI event handlers
- `filter.cpp`, `filter.h` — Image filtering routines (grayscale, edge detection)
- `bmpfile.cpp`, `bmpfile.h` — Simple BMP reader/writer utilities
- `resource.h`, `MyApp.rc`, `MyApp.rc2`, `res/` — App resources (icons, dialogs)

## Getting Started

### Prerequisites
- Windows
- Visual Studio (any version that can open one of the included project formats)
- C++ Desktop Development workload

### Build and Run
1. Clone the repository:
   ```bash
   git clone https://github.com/anamicaarya/ImageProcessing.git
   cd ImageProcessing


### How It Works

Grayscale: converts each pixel to a single luminance value and writes it back to the image buffer.

Edge detection: applies a small convolution kernel to approximate intensity gradients and highlights edges.

BMP handling: bmpfile.* provides simple routines to read and write uncompressed BMP files used by the demo.
