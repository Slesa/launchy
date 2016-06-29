@echo off

call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
SET LAUNCHY_VERSION=3.0.0
SET PPATH=%PATH%


:BUILD

echo "Creating 64 bit..."
SET QTDIR="%QTPATH%\msvc2013_64"
SET PATH=%QTDIR%\bin;%PPATH%

CALL :MakeAll bin\release win32-msvc2013
if errorlevel 1 (
  goto MENU
) 

REM echo "Creating 32 bit..."
REM call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
REM SET QTDIR="%QTPATH%\msvc2013"
REM SET PATH=%QTDIR%\bin;%PPATH%

REM CALL :MakeAll bin\release.32 win32-msvc2013

SET PATH=%PPATH%

rem Loop the build script.
:MENU
set CHOICE=nothing
echo (Q)uit, (Enter) runs the build again
set /P CHOICE= 
if /i "%CHOICE%"=="Q" goto :Quit

GOTO Build

:Quit
exit /b %errorlevel% 



:MakeAll

echo Cleaning %1 ...
if EXIST %1 rmdir /s /q %1

echo Creating Makefiles ...
cd src
%QTDIR%\bin\qmake -recursive -spec %2 Launchy.pro "CONFIG+=release" 
if errorlevel 1 (
  cd ..
  exit /b %errorlevel%
) 

echo Building...
nmake
if errorlevel 1 (
  cd ..
  exit /b %errorlevel%
) 
cd ..

echo Deploying Qt...
%QTDIR%\bin\windeployqt --release --no-plugins --no-webkit bin\release\app
if errorlevel 1 (
  exit /b %errorlevel%
) 

echo Moving translation files
move bin\release\app\*.qm bin\release\app\tr
if errorlevel 1 (
  exit /b %errorlevel%
) 

echo Creating setup file...
tools\nsis\bin\makensis setup\win\setup.nsi

