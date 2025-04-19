@echo off
robocopy .\websocket .\vcpkg\packages /s /e
robocopy .\websocket\websocketpp_x64-windows\include .\vcpkg\installed\x64-windows\include /s /e
robocopy .\websocket\websocketpp_x64-windows\share .\vcpkg\installed\x64-windows\share /s /e