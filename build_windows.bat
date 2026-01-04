@echo off
REM ================================================
REM Ahead - Windows Build Script (MinGW)
REM Run this on a Windows machine with Qt6 MinGW installed
REM ================================================

echo Building Ahead for Windows (MinGW)...

REM Adjust this path to your Qt6 MinGW installation
set QT_PATH=C:\Qt\6.6.0\mingw_64

REM Add MinGW to PATH
set PATH=%QT_PATH%\bin;C:\Qt\Tools\mingw1120_64\bin;%PATH%

cd qt_gui
if not exist build mkdir build
cd build

echo Configuring with CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_PATH%"

echo Building...
mingw32-make

echo.
echo Deploying Qt libraries...
%QT_PATH%\bin\windeployqt.exe Ahead.exe

echo.
echo ========================================
echo Build complete!
echo Executable: qt_gui\build\Ahead.exe
echo ========================================
pause
