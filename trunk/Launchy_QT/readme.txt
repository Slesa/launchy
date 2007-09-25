For beta release:
. Add thanks to Tyler for graphics stuff in change log
. Tyler's new skin


Low priority:
. Allow multiple directory selection for file extensions
. Can get rid of extra data (e.g. icon info) in alternatives item's if I clear the items after options exits
. Append to file plugin or .bat file or something
. Control+Enter to complete website
. Confirmation on Alt+F4
. Launchy commands inline (e.g. rescan/add dir/exit)


For next alpha:
. Use environment variables and use them by default in Launchy (for runner's cmd too) (can use them but no gui way to add them)
. Make sure skin and alpha are always together (seem to separate at launch)



Done for alpha 5:
. Controly should pick up more items now
. Starting path of application is its home directory
. Errors hidden on appliation launches
. Alternate search options now get selected correctly
. After catalog update, update current search result
. Can edit input
. Plugin event of Launchy showing/hiding
. Unicode in Foxy reads


Done for alpha 4:
. Convert ".mp3" into "*.mp3"
. resize options dialog?
. version information
. Add extra characters for hotkey
. Fix hotkeys which weren't working
. Fast indexer causes duplicates?
. 16bit icon support

Done for alpha 3:
. Use old style dropdown



For next alpha:
. Expand on links, e.g. my documents in explory
//. Maybe alphabetize lists of files?  Hmm
. Controly is too strict


Crashes:
. I crashed Launchy by adding qt4.3.1\bin, checking "Executables" and hitting "ok"




Currently working on: 

Seems like the database is getting hosed every now and then, "webstats" loses its count from time to time

Add "open with" support

Are the files being written/read in unicode?  Check on firefox bookmarks too.

Trying to stylize the scrollbar is difficult

Playing with the options tab on xp machine caused crashes when the plugins weren't loaded

Rescanning the database caused the rescan to freeze at around 76%, couldn't do any more scans after that


Memory leaks?


Why does Qt::WindowStaysOnTopHint cause the program to close?

Todo:
. When reindexing, always update the current search afterwards
. Make sure gSearchTxt is set before sort

KNOWN PROBLEMS:
Icons don't show up if wrong color setting (16-bit)
Options not being saved on laptop



XP issue:
I've installed the vc_redist.. so the SXS for the VC runtime is there.. and still plugins won't load.  Is it a QT issue then?

