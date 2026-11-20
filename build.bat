@echo off
setlocal enabledelayedexpansion

echo =======================================================
echo          Otterus2D - Automated Build Script
echo =======================================================

:: 1. Locate CMake
set "CMAKE_CMD=cmake"
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "!VSWHERE!" (
        for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -property installationPath`) do (
            if exist "%%i\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
                set "CMAKE_CMD=%%i\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
            )
        )
    )
)

"!CMAKE_CMD!" --version >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake was not found in PATH or Visual Studio installation.
    echo Please install CMake 3.20+ or Visual Studio with C++ CMake tools.
    exit /b 1
)

:: 2. Locate vcpkg toolchain
set "VCPKG_TOOLCHAIN="

if defined VCPKG_ROOT (
    if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
        set "VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
    )
)

if "%VCPKG_TOOLCHAIN%"=="" (
    if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set "VCPKG_TOOLCHAIN=C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
    ) else if exist "%~dp0vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set "VCPKG_TOOLCHAIN=%~dp0vcpkg\scripts\buildsystems\vcpkg.cmake"
    )
)

:: If vcpkg is still not found, offer to clone and bootstrap locally
if "%VCPKG_TOOLCHAIN%"=="" (
    echo [INFO] vcpkg was not detected in VCPKG_ROOT or C:\vcpkg.
    echo [INFO] Bootstrapping a local vcpkg instance in .\vcpkg...
    git clone https://github.com/microsoft/vcpkg.git "%~dp0vcpkg"
    if %errorlevel% neq 0 (
        echo [ERROR] Failed to clone vcpkg repository.
        exit /b 1
    )
    call "%~dp0vcpkg\bootstrap-vcpkg.bat" -disableMetrics
    set "VCPKG_TOOLCHAIN=%~dp0vcpkg\scripts\buildsystems\vcpkg.cmake"
)

echo [INFO] Using vcpkg toolchain: %VCPKG_TOOLCHAIN%

:: 3. Configure CMake
set "CONFIG=Release"
if not "%~1"=="" (
    set "CONFIG=%~1"
)

echo [INFO] Configuring project (%CONFIG% mode)...
"!CMAKE_CMD!" -B "%~dp0build" -S "%~dp0." -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET="x64-windows"
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %errorlevel%
)

:: 4. Build Project
echo [INFO] Building project (%CONFIG%)...
"!CMAKE_CMD!" --build "%~dp0build" --config %CONFIG%
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b %errorlevel%
)

echo =======================================================
echo  Build Succeeded!
echo  Executable: build\bin\%CONFIG%\OTTERUS_EDITOR.exe
echo =======================================================
