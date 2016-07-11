REM @echo off

call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64

cd src
mkdir build
cd build
cmake -G "NMake Makefiles" ..
if errorlevel 1 goto ERROR

nmake
if errorlevel 1 goto ERROR

ctest

:ERROR
cd ..\..

