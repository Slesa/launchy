@echo off
%QTDIR%\bin\windeployqt --release --no-plugins --no-webkit bin\release\app
move bin\release\app\*.qm bin\release\app\tr
cd src
qmake Launchy.pro
cd ..
tools\nsis\bin\makensis setup\win\setup.nsi
pause

