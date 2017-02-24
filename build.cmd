REM @echo off

REM call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64

cd src
mkdir build
cd build
REM cmake -G "NMake Makefiles" ..
cmake -G "NMake Makefiles"  -DCMAKE_BUILD_TYPE=Release -config Release ..
if errorlevel 1 goto ERROR

nmake
if errorlevel 1 goto ERROR


ctest --output-on-failure
if errorlevel 1 goto ERROR

cpack

:ERROR
cd ..\..

