![Welcome](https://user-images.githubusercontent.com/43253719/166149703-4f02ed65-2c90-40c4-8b81-436f621770fb.PNG)

# Imaging-Art-Beyond-RGB-Project

Spectral imaging is a powerful technique that enables higher color accuracy than conventional RGB capture, as well as material identification capabilities. However, it is still relatively unused for everyday museum photography due to perceived complexity and high cost. This project seeks to develop a standalone spectral image processing tool that is user-friendly, yet capable of handling complex image data. We will integrate existing image calibration routines and repackage them into a standalone application, enabling a simpler and more efficient user experience over current methods.  We hope that eliminating the current need for command line operation will enable non-expert users to process spectral images. A user will be able to import a number of RAW images and then will receive a true spectral image in the form of a TIFF file, as well as a color-managed RGB image. The user will only need to make a few basic selections before the images are processed. As the necessary image processing routines, which will be hidden under the hood, are executed, the application will provide progress updates in the form of status messages, Finally, the user will be able to view and interact with the output images inside, and also inspect reports detailing the accuracy of the color and spectral calibrations developed during the processing.


# About
This project is sponsored by The Studio for Scientific Imaging and Archiving of Cultural Heritage -- a facility within the RIT Program of Color Science’s Munsell Color Science Lab.
The project is being developed by a senior project team from the Software Engineering program at RIT.
### Sponsors
Olivia Kuzio, Susan Farnand, Leah Humenuck, Sahara Smith
### Senior Project Team 1
Tristan Knox, Jacob O'Neil, Andrew Hammerstonm, Jared Ponzetti, Patrick Dalesio
### Senior Project Team 2
Brendan Grau, Sungmin Kim, Keenan Miller, John Akey, Elijah Cooper
### Senior Project Team 3
Sam McQuate, Jacob Auger, Josh Greco, Robert Loos, Jonathan Pofcher
### Senior Project Team 4
Ray Axmann, Noah Pelletier, Mitchell Sulkowski, Christopher Cabonilas, Cameron Robinson, Adam Cropp
### Senior Project Team 5
Jack Ganger-Spivak, Raynard Miot, Parker Noffke, Nat Manoucheri, Taylor Lineman, Jakob Langtry


# Documentaion
* [Architecture](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki)
* [CommunicationsAPI](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/Communications-API)
* [BackendDesign](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/Backend-Overview)
* [TIFF Output](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/TIFF-Output-Images)
* [BeyondRGB Under The Hood](https://github.com/TristanKnox/Imaging-Art-beyond-RGB/wiki/BeyondRGB-Under-The-Hood)

# Development and release environment set-up
## Windows 10/11
- This set-up process was tested on a clean install of Windows with no prior Visual Studio installations.
### Install tools
- https://gitforwindows.org/
  - Command line git is still needed for our scripts (even if you use GitHub desktop).
- https://cmake.org/download/
  - If cmake gives you pkg-config errors, run `winget install bloodrock.pkg-config-lite` and accept agreements
- https://visualstudio.microsoft.com/downloads/?q=build+tools
  - Scroll down and install only the "Build Tools"
  - Within the installer, check "Desktop development with C++"
  	- MSVC (latest)
  	- Windows 10 SDK
    - Uncheck/no need for Cmake and other defaults
- Install [LLVM Compiler](https://releases.llvm.org/). This compiler is used for clang-format, the backend formatter for the project. LLVM release 21.1.0 is confirmed to work (any releases afterwards have not been tested). Make to select to add to PATH during installation
- https://nodejs.org/en/
	- Do not need/uncheck optional build tools (python and MSVC 2017)
- Reboot or log-out and log-in
- Make sure your editor is configured for `C++20` syntax

### Clone our github repository
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd .\Imaging-Art-beyond-RGB\`
- `git submodule sync`
- `git submodule update --init --recursive`

### Set-up development & release environment:
- `cd .\backend\`
- `.\vcpkg_first_time_setup.bat`
  - Locally install Vcpkg and our dependencies (\~17.5 GB and \~30 minutes)
- `cd ..\frontend\`
- `npm install`

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
- Git and CMake dependencies: `brew install git cmake pkg-config node`
- Make sure your editor is configured for `C++20` syntax

### Clone our github repository
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd Imaging-Art-beyond-RGB`
- `git submodule sync`
- `git submodule update --init --recursive`

### Set-up development & release environment:
- `cd backend`
- `sh unix_config_environment.sh`
  - Locally install Vcpkg and our dependencies (\~5 GB and \~15 minutes)
- `cd ../frontend`
- `npm install`

### Backend build
- `cd backend` from project root.
- If an additional dependency is ever added to "dependencies.txt", first run:
  - `sh unix_config_environment.sh`
- To build:
  - `sh unix_debug.sh` or `sh unix_release.sh`
  - Build automatically copied to frontend for testing.

### Frontend build
- `cd frontend` from project root.
- If frontend configuration is ever changed, first run:
  - `npm install`

### Run:
- `cd frontend` from project root.
- `npm run preview`

### Release
- `cd backend` from project root.
- `sh unix_release.sh`
- `cd ../frontend`
- `sh release_frontend.sh`
  - The release output can be found in "Imaging-Art-beyond-RGB\frontend\out\".

#### Mac Release issues
- Because Apple is greedy, apps released using M1 macs will result in users seeing a message saying that the app is "damaged and cannot be opened"
  - Fix this by either signing the release of this application with your apple developer id or simply compiling and releasing the application with x64 (intel) architecture (note that M1 macs will still be able to run apps released this way).

## Ubuntu (tested on 24.04 and 25.04)

### Install tools
- Clone repository: `git clone https://github.com/BeyondRGB/Imaging-Art-beyond-RGB.git`
- `cd Imaging-Art-beyond-RGB`
- `git submodule sync`
- `git submodule update --init --recursive`
- `sudo apt install $(cat backend/apt-dependencies.txt)` from project root.
- Make sure your editor is configured for `C++20` syntax
- If you are on Ubuntu 24.04 LTS, you additionally need to configure GCC 14:
  - `sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 14`
  - `sudo sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 14`

### Set-up
- `cd backend`
- `sh unix_config_environment.sh`
  - This locally installs Vcpkg and our dependencies, it will take a while (\~4.8 GB and \~10 minutes)
- `cd ../frontend`
- `npm install`

### Backend build
- `cd backend` from project root.
- If an additional dependency is ever added to "dependencies.txt", first run:
  - `sh unix_config_environment.sh`
- To build:
  - `sh unix_debug.sh` or `sh unix_release.sh`
  - Build automatically copied to frontend for testing.

### Frontend build
- `cd frontend` from project root.
- If frontend configuration is ever changed, first run:
  - `npm install`

### Run:
- `cd frontend` from project root.
- `npm run preview`

### Release
- `cd backend` from project root.
- `sh unix_release.sh`
- `cd ../frontend`
- `sh release_frontend.sh`
- Built packages are in frontend/out/make/ for Debian/Ubuntu, Fedora, and as a generic zip file for other distributions.
- LibOpenCV is required as an additional dependency if you install from these packages.
  - `sudo apt install libopencv-dev` or equivalent

## Additional Notes

### Formatting
There is a pre-commit git hook (found in ./husky folder in the project's frontend directory) which verifies changed C++ files were formatted correctly. If your files aren't formatted, the commit fails and the hook attempts to format using clang-format (found using PATH). The formatted files can then be staged for a valid commit.