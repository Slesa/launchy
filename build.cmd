@echo off
%QTDIR%\bin\windeployqt --release --no-plugins --no-webkit bin\app
move bin\app\*.qm bin\app\tr
tools\nsis\bin\makensis setup\win\setup.nsi