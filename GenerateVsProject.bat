@echo off
cls
echo [35mGenerate Project Script running...
echo Checking system...
if %OS% == Windows_NT (
   echo [36m
   echo [3F
   ver
) else (
   echo [91mSidekick is designed for the Windows OS
   echo Canceling Operation[0m
   goto :end
)

call git --version
call cmake --version

echo [0m
echo [35mGenerating _build folder in %cd%[0m
if exist _build\ (
   echo [91mFolder already exists
   echo Canceling Operation[0m
   goto :end
)

mkdir _build
cd _build
echo [35mGenerating project structure from ../src/CMakeLists.txt[36m
cmake ../src
echo [32m
echo Finished[0m

:end
   pause
