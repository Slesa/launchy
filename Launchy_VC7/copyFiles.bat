@echo off
copy /Y release\launchy.exe "c:\program files\launchy\" 
copy /Y ..\bin\calcy.dll "c:\program files\launchy\plugins\"
copy /Y ..\bin\foxy.dll "c:\program files\launchy\plugins\"
copy /Y ..\bin\weby.dll "c:\program files\launchy\plugins\"
copy /Y ..\bin\explory.dll "c:\program files\launchy\plugins\"
pause