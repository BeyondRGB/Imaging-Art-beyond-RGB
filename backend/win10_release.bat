if exist build\Release\resources\app\backend\res\ (
	echo Found .\build\Release\resources\app\backend\res\
) else (
	::mkdir build\Release\resources\app\backend\res\
	mkdir -p build/{Release,resources,app,backend,res}
)
Xcopy /y /e .\res .\build\Release\resources\app\backend\res
cmake -B build -S .
cmake --build .\build\ --config Release

$SELL