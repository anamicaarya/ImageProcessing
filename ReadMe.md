# ImageProcessing

A small Windows image processing demo in C++ using MFC. It loads BMP images, converts them to grayscale, and applies a simple edge-detection filter.

## Features
- Grayscale conversion
- Edge detection using a small convolution kernel
- Minimal MFC dialog application to run filters

## Tech Stack
- Language: C++
- Framework: Microsoft Foundation Class Library (MFC)
- Platform: Win32 desktop
- Project formats included: `.sln`, `.vcxproj`, `.vcproj`, `.dsw`, `.dsp`

## Project Structure
- `MyApp.sln`, `MyApp.vcxproj`, `MyApp.vcproj`, `MyApp.dsw`, `MyApp.dsp`  
  Solution and project files for different Visual Studio generations.
- `MyApp.cpp`, `MyApp.h`  
  Application entry point and app class.
- `MyAppDlg.cpp`, `MyAppDlg.h`  
  Main dialog and UI event handlers.
- `filter.cpp`, `filter.h`  
  Image filtering routines, including grayscale and edge detection.
- `bmpfile.cpp`, `bmpfile.h`  
  Simple BMP reader and writer utilities.
- `resource.h`, `MyApp.rc`, `MyApp.rc2`, `res/`  
  Icons, dialogs, and other resources.

## Getting Started

### Prerequisites
- Windows
- Visual Studio with the C++ Desktop Development workload

### Build and Run
1. Clone the repository.
   ```bash
   git clone https://github.com/anamicaarya/ImageProcessing.git
   cd ImageProcessing
