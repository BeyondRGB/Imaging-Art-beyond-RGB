
if exist ..\backend\build\Release\*.exe (
	@echo off
	
	if exist ".\lib\" RD /S /Q ".\lib\"
	mkdir ".\lib\"

	if exist ".\res\" RD /S /Q ".\res\"
	mkdir ".\res\"

	:: Copy .exe and libraries
	Xcopy /y "..\backend\build\Release\" ".\lib\"
	
	:: Copy all backend resource files
	Xcopy /y /e "..\backend\res\" ".\res\"

	npm run make

) else (
	echo Backend Release build does not exist. Run Imaging-Art-beyond-RGB\backend\win10_release.bat and try again
)
