@echo off
REM ================================================
REM Ahead - Windows Build Script
REM Run this on a Windows machine with Qt6 installed
REM ================================================

echo Building Ahead for Windows...

REM Adjust this path to your Qt6 installation
set QT_PATH=C:\Qt\6.6.0\msvc2019_64

cd qt_gui
if not exist build mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH="%QT_PATH%"
cmake --build . --config Release

echo.
echo Deploying Qt libraries...
%QT_PATH%\bin\windeployqt.exe Release\Ahead.exe

echo.
echo ========================================
echo Build complete!
echo Executable: qt_gui\build\Release\Ahead.exe
echo ========================================
pause
