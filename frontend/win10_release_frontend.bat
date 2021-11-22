
if exist ..\backend\build\Release\*.exe (
	@echo off
	if exist lib\ (
		echo Found .\lib\
	) else (
		@echo on
		mkdir lib\
		Xcopy  ..\backend\build\Release .\lib
	)
	npm run make
)else (
	echo Backend Release build does not exist. Run Imaging-Art-beyond-RGB\backend\win10_release.bat and try again
)
