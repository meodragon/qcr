@echo off
rem rtc Build Steps on Windows
rem to use a developer environment, it might be necessary to execute, for example:
rem SET ENV="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
rem CALL %ENV%

if "%1%"=="help" goto usage
if "%1%"=="debug" goto main

:usage
echo "usage: %0% debug"
goto done
rem to close the Command Prompt, execute: exit /b 0

:main
if not exist "build/" mkdir build
set FLAGS=/Fo"build/" /Fe:"build/"
set SOURCES=main.c surface.c
set LIBRARIES=vulkan-1.lib user32.lib
rc icon.rc || (
    echo "FAIL: rc icon.rc"
    goto done
)
cl %FLAGS% %SOURCES% /I %VULKAN_SDK%/Include /link /SUBSYSTEM:CONSOLE /LIBPATH:"%VULKAN_SDK%/Lib" %LIBRARIES% icon.res && (
    echo "BUILD: SUCCESS"
) || (
    echo "BUILD: FAILURE"
)
rem to close the Command Prompt, execute: exit /b 0

:done
