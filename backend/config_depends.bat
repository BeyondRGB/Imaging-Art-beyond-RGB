for /F "tokens=*" %%A in (dependencies.txt) do .\vcpkg\vcpkg.exe install %%A:x64-windows