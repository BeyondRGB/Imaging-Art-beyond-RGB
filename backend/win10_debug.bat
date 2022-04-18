
:: Delete old build from frontend folder
if exist "..\frontend\lib\" RD /S /Q "..\frontend\lib\"
if exist "..\frontend\res\" RD /S /Q "..\frontend\res\"

:: Build 
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows
if errorlevel 1 exit
cmake --build .\build\
if errorlevel 1 exit

:: Copy .exe and libraries
mkdir "..\frontend\lib\"
Xcopy /y ".\build\Debug\" "..\frontend\lib\"

:: Copy all backend resource files
mkdir "..\frontend\res\"
Xcopy /y /e ".\res\" "..\frontend\res\"
