@echo off

PATH=%QTDIR%\bin;%PATH%
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"

echo Creating NSIS script...
cd src
%QTDIR%\bin\qmake -recursive Launchy.pro "CONFIG+=release" 

rem echo Building...
rem nmake
cd ..

echo Deploying Qt...
%QTDIR%\bin\windeployqt --release --no-plugins --no-webkit bin\release\app

echo Moving translation files
move bin\release\app\*.qm bin\release\app\tr

echo Creating setup file...
tools\nsis\bin\makensis setup\win\setup.nsi
echo Done
pause

