if exist build\Debug\res\ (
	echo Found .\build\Debug\res\
) else (
	mkdir build\Debug\res\
)
:: Copy all resrouce files to a path relative to the .exe as they will be in the deployed app
Xcopy /y /e .\res .\build\Debug\res
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build .\build\