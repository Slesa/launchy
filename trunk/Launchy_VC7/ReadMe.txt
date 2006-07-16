Changes since 0.9.6:
. Brought back in the association code
. Fixed a no icon bug (looked up icon on c: rather than find the windows drive)
. Donate screen could come up twice if you rebooted, this is annoying.  I fixed this by forcing a store after the options change.

Changes since 0.9.3:

Code fixes to do once I can read up about them:

Still todo:
. Why does Launchy crash when started up quickly?!

Bug Fixes:
2) Spelled 'translucency' correctly for the skin ini file :p
3) Fixed up/down keys bringing forward a dropdown when no options are available
4) Explicit check for equality in sorting rather than partial match, otherwise usage wins

Others:
1) Now save ini after changing settings with menus
2) Now spaces are ignored from user input, in other words they don't help with matches
   -- this allows you to match johnny_cash.mp3 with 'johnny cash'
3) Removed association code
4) Added donate code
5) Added folder name indexing
1) Unicode support in the ini file!  Yay :)