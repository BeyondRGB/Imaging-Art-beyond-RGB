if exist build\Debug\resources\app\backend\res\ (
	echo Found .\build\Debug\resources\app\backend\res\
) else (
	mkdir build\Debug\resources\app\backend\res\
)
:: Copy all resrouce files to a path relative to the .exe as they will be in the deployed app
Xcopy /y /e .\res .\build\Debug\resources\app\backend\res
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build .\build\