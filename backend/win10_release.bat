if exist build\Release\resources\app\backend\res\ (
	echo Found .\build\Release\resources\app\backend\res\
) else (
	mkdir build\Release\resources\app\backend\res\
)
:: Copy all resrouce files to a path relative to the .exe as they will be in the deployed app
Xcopy /y /e .\res .\build\Release\resources\app\backend\res\
cmake -B build -S .
cmake --build .\build\ --config Release