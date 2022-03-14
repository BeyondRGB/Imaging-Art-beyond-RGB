if exist build\Release\res\ (
	echo Found .\build\Release\res\
) else (
	mkdir build\Release\res\
)
:: Copy all resrouce files to a path relative to the .exe as they will be in the deployed app
Xcopy /y /e .\res .\build\Release\res\
cmake -B build -S .
cmake --build .\build\ --config Release