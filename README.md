![Welcom](https://user-images.githubusercontent.com/43253719/166149703-4f02ed65-2c90-40c4-8b81-436f621770fb.PNG)

# Imaging-Art-Beyond-RGB-Project

  Spectral imaging is a powerful technique that enables higher color accuracy than conventional RGB capture, as well as material identification capabilities. However, it is still relatively unused for everyday museum photography due to perceived complexity and high cost. This project seeks to develop a standalone spectral image processing tool that is user-friendly, yet capable of handling complex image data. We will integrate existing image calibration routines and repackage them into a standalone application, enabling a simpler and more efficient user experience over current methods.  We hope that eliminating the current need for command line operation will enable non-expert users to process spectral images. A user will be able to import a number of RAW images and then will receive a true spectral image in the form of a TIFF file, as well as a color-managed RGB image. The user will only need to make a few basic selections before the images are processed. As the necessary image processing routines, which will be hidden under the hood, are executed, the application will provide progress updates in the form of status messages, Finally, the user will be able to view and interact with the output images inside, and also inspect reports detailing the accuracy of the color and spectral calibrations developed during the processing.


# About
This project is sponsored by The Studio for Scientific Imaging and Archiving of Cultural Heritage -- a facility within the RIT Program of Color Science’s Munsell Color Science Lab.
The project is being developed by a senior project team from the Software Engineering program at RIT.
### Sponsor
Olivia Kuzio
### Senior Project Team
Brendan Grau, Elijah Cooper, John Akey, Keenan Miller, SungMin Kim

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
- Window
    - TODO: UPDATE
- Mac
    - TODO: UPDATE
- Linux
    - Use your package manager to install the following
        - npm
        - make
        - git

### Clone the repository
- `git clone https://github.com/TristanKnox/Imaging-Art-beyond-RGB`
- `cd .\Imaging-Art-beyond-RGB\`

### Set-up development & release environment:
- `cd .\frontend\`
- `npm ci`

### Backend build
- `cd .\backend\` from project root
- If building for Windows
    - `make win`
- If building for a UNIX system (Linux, Mac, etc.)
    - `make`

### Fronted build
- `cd .\frontend\` from project root.

### Run:
- `cd .\frontend\` from project root.
- `npm run preview` or `npm run dev`

### Release
- TODO
