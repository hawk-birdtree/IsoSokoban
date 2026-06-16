@echo off
:: Set up paths
SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin
SET PATH=%COMPILER_PATH%;%PATH%

:: Ensure the output directory exists
if not exist out mkdir out

:: Compile with GCC (matching your Notepad++ setup)
gcc -o out/isoSokoban.exe src/*.c ^
    -I%RAYLIB_PATH%/src ^
    -L%RAYLIB_PATH%/src ^
    %RAYLIB_PATH%/src/raylib.rc.data ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -static -O2 -std=c99 -Wall

echo Build complete!