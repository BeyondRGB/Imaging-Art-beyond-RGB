
if exist ..\backend\build\Release\*.exe (
	@echo off
	if exist backend\lib\ (
		echo Found .\backend\lib\
	) else (
		@echo on
		mkdir backend\lib\
	)
	if exist backend\res\ (
		echo Found .\backend\res
	) else (
		@echo on
		mkdir .\backend\res\
	)
	:: The /y flag supresses the promt to verify that it is ok to overwrite files
	:: This will always just replace the res and lib with whatever is Release and res
	
	:: Copy .exe and libraries
	Xcopy /y ..\backend\build\Release .\backend\lib
	:: Copy all backend resource files
	Xcopy /y /e ..\backend\res .\backend\res
	npm run make
)else (
	echo Backend Release build does not exist. Run Imaging-Art-beyond-RGB\backend\win10_release.bat and try again
)
