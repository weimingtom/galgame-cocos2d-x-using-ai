@echo off
setlocal
cd /d "%~dp0"

if not exist "cocos2d\CMakeLists.txt" (
  echo [!] cocos2d-x engine not found in .\cocos2d
  echo.
  echo Download cocos2d-x 4.0 and extract so that this exists:
  echo   %cd%\cocos2d\CMakeLists.txt
  echo.
  echo Official: https://github.com/cocos2d/cocos2d-x/releases
  echo Or set environment variable COCOS2DX_ROOT to your engine path.
  exit /b 1
)

if not exist build mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
  echo.
  echo CMake failed. Try another generator, e.g.:
  echo   cmake .. -G "Visual Studio 16 2019" -A x64
  exit /b 1
)

cmake --build . --config Release
echo.
echo Done. Binary under build\bin\Release\ or similar.
endlocal
