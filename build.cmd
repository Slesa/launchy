@echo off

PATH=%QTDIR%\bin;%PATH%
echo Deploying Qt...
%QTDIR%\bin\windeployqt --release --no-plugins --no-webkit bin\release\app

echo Moving translation files
move bin\release\app\*.qm bin\release\app\tr

echo Creating NSIS script...
cd src
%QTDIR%\bin\qmake Launchy.pro
cd ..

echo Creating setup file...
tools\nsis\bin\makensis setup\win\setup.nsi
echo Done
pause

