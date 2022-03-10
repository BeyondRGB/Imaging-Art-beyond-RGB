
if exist ..\backend\build\Release\*.exe (
	@echo off
	if exist lib\ (
		echo Found .\lib\
	) else (
		@echo on
		mkdir lib\
	)
	if exist res\ (
		echo Found .\res
	) else (
		@echo on
		mkdir res\
	)
	:: The /y flag supresses the promt to verify that it is ok to overwrite files
	:: This will always just replace the res and lib with whatever is Release and res
	
	:: Copy .exe and libraries
	Xcopy /y ..\backend\build\Release .\lib
	:: Copy all backend resource files
	Xcopy /y /e ..\backend\res .\res
	npm run make
)else (
	echo Backend Release build does not exist. Run Imaging-Art-beyond-RGB\backend\win10_release.bat and try again
)
