# Beyond-RGB-Backend

The backend for this projects is a WebSocket localhost server that listens for commands from the front end and spins off processing threads as tasks come in.

# Dev Env Setup

The following describes the steps needed to get the back end environment of this project up and running. The project itself is in CPP and is dependent on several thrid party libraries. It uses [CMake](https://cmake.org/) and [VCPKG](https://vcpkg.io/en/index.html) to manage the build process and dependeny managment respectivly. 

### Windows
- Prerequisites
  - Install [Visual Studio](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019?target=_blank). The build system uses the compilers that come with Visual Studio, although it is posible to install your own compiler and not user VS, doing so has not been tested for our setup and may lead to unexpeced issues.<br>
  **Note:** it is not required to use the full IDE. Alternatively, you can install Python, VS Build Tools, and CMake independently.
  - (optional) Install [Python](https://www.python.org/downloads/): At time of writing, the most recent confirmed working version is Python 3.12.
  - (optional) Install **Visual Studio Build Tools** with [Chocolaty](https://chocolatey.org): In an administrator command line, run: ```choco install visualstudio2022-workload-vctools -y```
  - (optional) Install [CMake](https://cmake.org/install/): If using the VS IDE you will have access to CMake commands within the DeveloperTerminal in the IDI. If you would like to be able to build this project from the command line you will need CMake installed.
- Setup
  - Clone this repository
  - ```cd Imaging-Art-beyond-RGB\backend```
  - run ```vcpkg_first_time_setup.bat``` this will first check to see if vcpkg has been set up for this project. If it has not it will first install it into the projects directory and then install all the dependencies that the project needs.<br>
  **Note:** this will take some time to complete, as some of the dependencies take a while to download and install. After this is complete you should see a new ```vcpkg``` folder within ```Imaging-Art-beyond-RGB\backend\```
  - run ```win10_debug.bat``` this will configure CMake and compile the application. When this is done you should see a new ```build``` folder within ```Imaging-Art-beyond-RGB\backend\```
  - ```cd Imaging-Art-beyond-RGB\backend\build\Debug```
  - run ```beyond-rgb-backend.exe``` you should see ```BeyondRBG Backend Started: listening on port 9002```, if so your all set up and the back end is currently running. ```ctr+c``` to terminate the program.
  - If building for release, run ```win10_release.bat``` instead of ```win10_debug.bat``` and ```beyond-rgb-backend.exe``` will be in the ```Release``` directory instead of ```Debug```.
- Update vcpkg
  - ```cd Imaging-Art-beyond-RGB\backend\vcpkg```
  - In a command line, run ```git pull --ff-only```
  - run ```bootstrap-vcpkg.bat```
  - ```cd ..```
  - run ```config_depends.bat```

### Mac OS Backend
- Setup Environment
  - Install the Brew package manager.
  - ```brew install cmake```
  - ```brew install pkg-config```
  - ```sh osx_config_environment.sh```
    - This downloads vcpkg and installs all the backend dependencies.
    - This will take some time.
- Update Environment (when needed)
  - ```sh osx_config_environment.sh```
- Compile
  - Debug: ```sh osx_build.sh```
  - Release: ```sh osx_release.sh```


### Linux
- Linux builds not currently supported.