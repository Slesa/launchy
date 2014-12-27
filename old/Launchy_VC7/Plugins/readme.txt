------------------------------------
   Visual Studio Project Renamer
 
          For Win9x/NT/2k

           By Niek Albers 
       nieka@daansystems.com

        (C)2001 DaanSystems

Homepage: http://www.daansystems.com
------------------------------------

Use with care and on your own risk! 
Make backups before you use this program.
Do a 'rebuild all' on your renamed project.

Usage: vsrename <oldname> <newname> <dir>

Where:

oldname : The old name of your project (typically the xxx from xxx.dsw)
newname : The new name for your project
dir	: The directory tree that holds the project.

The program will replace all occurences of oldname to newname in all
files and rename all filenames that have the old name in it.
It will walk subdirectories.

--- History

12.Jul.2001 v0.5

+ Even better handling of binary files. When a binary file
  is detected, it will not try to replace in it.

30.Jun.2001 v0.4

bugfixes:

+ Better handling of binary files. They should be alright after
  renaming the project.

15.Jun.2001 v0.3

bugfixes:

+ Better renaming of projects with the same name in it,
  so you can rename a project from 'myproject' to 'supermyproject'.
  It should go ok.

-----------
14.Jun.2001 v0.2

improvements:

+ Added renaming of main project directory.

