![Welcom](https://user-images.githubusercontent.com/43253719/166149703-4f02ed65-2c90-40c4-8b81-436f621770fb.PNG)

# Imaging-Art-Beyond-RGB-Project

  Spectral imaging is a powerful technique that enables higher color accuracy than conventional RGB capture, as well as material identification capabilities. However, it is still relatively unused for everyday museum photography due to perceived complexity and high cost. This project seeks to develop a standalone spectral image processing tool that is user-friendly, yet capable of handling complex image data. We will integrate existing image calibration routines and repackage them into a standalone application, enabling a simpler and more efficient user experience over current methods.  We hope that eliminating the current need for command line operation will enable non-expert users to process spectral images. A user will be able to import a number of RAW images and then will receive a true spectral image in the form of a TIFF file, as well as a color-managed RGB image. The user will only need to make a few basic selections before the images are processed. As the necessary image processing routines, which will be hidden under the hood, are executed, the application will provide progress updates in the form of status messages, Finally, the user will be able to view and interact with the output images inside, and also inspect reports detailing the accuracy of the color and spectral calibrations developed during the processing.


# About
This project is sponsored by The Studio for Scientific Imaging and Archiving of Cultural Heritage -- a facility within the RIT Program of Color Science’s Munsell Color Science Lab.
The project is being developed by a senior project team from the Software Engineering program at RIT.
### Sponsors
Olivia Kuzio
Susan Farnand
Leah Humenuck
### Senior Project Team 1
Tristan Knox, Jacob O'Neil, Andrew Hammerstonm, Jared Ponzetti, Patrick Dalesio
### Senior Project Team 2
Brendan Grau, Sungmin Kim, Keenan Miller, John Akey, Elijah Cooper

# Documentaion
* [Architecture](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki)
* [CommunicationsAPI](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/Communications-API)
* [BackednDesign](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/Backend-Overview)
* [TIFF Output](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/TIFF-Output-Images)
* [BeyondRGB Under The Hood](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/BeyondRGB-Under-The-Hood)

# Development and release environment set-up
## Windows 10/11
- This set-up process was tested on a clean install of Windows with no prior Visual Studio installations.
### Install tools
- https://gitforwindows.org/
  - Command line git is still needed for our scripts (even if you use GitHub desktop).
- https://cmake.org/download/
- https://visualstudio.microsoft.com/downloads/?q=build+tools
  - Scroll down and install only the "Build Tools"
  - Within the installer, check "Desktop development with C++"
  	- MSVC (latest)
  	- Windows 10 SDK
    - Uncheck/no need for Cmake and other defaults
- https://nodejs.org/en/
	- Do not need/uncheck optional build tools (python and MSVC 2017)
- Reboot or log-out and log-in
- Make sure your editor is configured for `C++20` syntax

### Clone our github repository
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd .\Imaging-Art-beyond-RGB\`

### Set-up development & release environment:
- `cd .\backend\`
- `.\vcpkg_first_time_setup.bat`
  - Locally install Vcpkg and our dependencies (\~10 GB and \~30 minutes)
- `cd ..\frontend\`
- `npm ci`

### Backend build
- `cd .\backend\` from project root.
- If an additional dependency is ever added to "dependencies.txt", first run:
  - `.\config_depends.bat`
- To build:
  - `.\win10_debug.bat` or `.\win10_release.bat`
  - Build automatically copied to fronted for testing.

### Fronted build
- `cd .\frontend\` from project root.
- If frontend configuration is ever changed, first run:
  - `npm ci`

### Run:
- `cd .\frontend\` from project root.
- `npm run preview` or `npm run dev`

### Release
- `cd .\backend\` from project root.
- `.\win10_release.bat`
- `cd ..\frontend\`
- `.\win10_release_frontend.bat`
  - The release output can be found in "Imaging-Art-beyond-RGB\frontend\out\".
  - The release is a folder called "Beyond RGB-win32-x64" with an executable "Beyond RGB.exe".

## Mac
### Install tools
- Xcode command line developer tools
- Homebrew: https://brew.sh/
- Git: `brew install git`
- CMake: `brew install cmake`
- pkgconfig: `brew install pkg-config`
- Node.js: `brew install node`
- Make sure your editor is configured for `C++20` syntax

### Clone our github repository
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd Imaging-Art-beyond-RGB`

### Set-up development & release environment:
- `cd backend`
- `sh osx_config_environment.sh`
  - Locally install Vcpkg and our dependencies (\~10 GB and \~30 minutes)
- `cd ../frontend`
- `npm ci`

### Backend build
- `cd backend` from project root.
- If an additional dependency is ever added to "dependencies.txt", first run:
  - `sh osx_config_environment.sh`
- To build:
  - `sh osx_debug.sh` or `sh osx_release.sh`
  - Build automatically copied to frontend for testing.
  - If you encounter an error with either of the above, crosscheck the specified architecture of your system with the `VCPKG_TARGET_TRIPLET` flag specified in both `osx_debug.sh` and `osx_release.sh`:
    - For example, if you are using an M1 mac, replace `x64-osx` with `arm64-osx`

### Fronted build
- `cd frontend` from project root.
- If frontend configuration is ever changed, first run:
  - `npm ci`

### Run:
- `cd frontend` from project root.
- `npm run preview` or `npm run dev`

### Release
- `cd backend` from project root.
- `sh osx_release.sh`
- `cd ../frontend`
- `sh release_frontend.sh`
  - The release output can be found in "Imaging-Art-beyond-RGB\frontend\out\".

#### Mac Release issues
- Because Apple is greedy, apps released using M1 macs will result in users seeing a message saying that the app is "damaged and cannot be opened"
  - Fix this by either signing the release of this application with your apple developer id or simply compiling and releasing the application with x64 (intel) architecture (note that M1 macs will still be able to run apps released this way).

## Ubuntu

### Install tools
- `sudo apt install build-essential git cmake`
- Node.js: https://nodejs.org/en/download/
- Make sure your editor is configured for `C++20` syntax

### Set-up
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd Imaging-Art-beyond-RGB`
- `cd backend`
- `sh linux_config_environment.sh`
  - Vcpkg may fail and require you to install another command line tool.
  - Once you install it, run the script again. It may fail again and require more command line tools to be installed.
  - Locally install Vcpkg and our dependencies (\~10 GB and \~30 minutes)
- `cd ../frontend`
- `npm ci`

### Backend build
- `cd backend` from project root.
- If an additional dependency is ever added to "dependencies.txt", first run:
  - `sh linux_config_environment.sh`
- To build:
  - `sh linux_debug.sh` or `sh linux_release.sh`
  - Build automatically copied to fronted for testing.

### Fronted build
- `cd frontend` from project root.
- If frontend configuration is ever changed, first run:
  - `npm ci`

### Run:
- `cd frontend` from project root.
- `npm run preview` or `npm run dev`

### Release
- `cd backend` from project root.
- `sh linux_release.sh`
- `cd ../frontend`
- `sh release_frontend.sh`
