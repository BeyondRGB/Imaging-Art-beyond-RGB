@echo off
if exist vcpkg\ (
  echo Vcpkg is already setup, run vcpkg_libraries.bat to install missing libraries.
) else (
  @echo on
  git clone https://github.com/microsoft/vcpkg
  cmd /c .\vcpkg\bootstrap-vcpkg.bat
  cmd /c config_depends.bat
  cmd /c websocketpp_bundle.bat
)
