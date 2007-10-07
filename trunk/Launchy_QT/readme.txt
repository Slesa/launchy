For beta release:
. Add thanks to Tyler for graphics stuff in change log
. Tyler's new skin


Low priority:
. Can get rid of extra data (e.g. icon info) in alternatives item's if I clear the items after options exits
. Append to file plugin or .bat file or something
. Confirmation on Alt+F4
. Launchy commands inline (e.g. rescan/add dir/exit)


For next alpha:
. Use environment variables and use them by default in Launchy (for runner's cmd too) (can use them but no gui way to add them)
. Esc from dropdown should only exit dropdown, not hide launchy
. Launchy index/options/exit
. Fix animation problem
. Thread animation
. Fix Launchy detached window problem


Done for alpha 8:
. Fix font support for arrow problem?

Done for alpha 7:
. Use mutex for running launchy, instead of network code 
. Fix animation problems
. Enter key works for launching
. Add .weby to weby sites
. Alternatives should display above the program if no room below
. Make sure skin and alpha are always together (seem to separate at launch)
. Sorts by directory names first



Done for alpha 6:
. Completes folders (e.g. my documents) into paths
. Changed " | " to unicode arrow
. Wikipedia entry fixed for new installations
. Fade in / out
. User adjustable opaqueness
. Enter and tab work as expected in the dropdown menu
. Adjustable dropdown length
. Recycle bin and other system folders now show up


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




Currently working on: 

Add "open with" support

Trying to stylize the scrollbar is difficult

Playing with the options tab on xp machine caused crashes when the plugins weren't loaded

Rescanning the database caused the rescan to freeze at around 76%, couldn't do any more scans after that


Memory leaks?

Why does Qt::WindowStaysOnTopHint cause the program to close?

Todo:
. When reindexing, always update the current search afterwards
. Make sure gSearchTxt is set before sort

KNOWN PROBLEMS:
Options not being saved on laptop

