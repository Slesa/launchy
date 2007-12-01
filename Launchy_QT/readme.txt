2.0+:
. Add memory for runny/launchy (rebuild/exit/etc..)
. Add a plugin for open with etc..


For 2.0 Final:
Easy stuff -
	. Add thanks to Tyler for graphics stuff on webpage
	. Add thanks to testers
Hard stuff -
	
For Beta 2:
	. Fix controly problem
	
Done for beta 2:
. Shortcuts to directories are treated as normal files
. Undid relative positioning
. Wrong skin would periodically get selected
. Always center Launchy setting added (allows you to move it but will recenter every time you hit the hotkey)
. Fixed skin color problem in dropdown box
. Installer is more Vista friendly for non-admin users

Done for beta 1:
. Thanks to Tyler Sticka for Launchy's beautiful new skin and icon!
. If hotkey pressed to hide Launchy when options are open, Launchy would close when you exited the options dialog.  Fixed.
. Added tooltips for alternative items
. Bring the font back?  Greek fonts seem to have problems
. If skin doesn't exist, use default
. Changing skins reset position (fixed)
. Dropdown is also transparent
. Double click runs item
. Right click on output area opens popup menu
. Fixed a GDI resource leak
. If there is no text, clear the icon
. Position is saved relative to screen resolution, and saved periodically in case of fast reboot
. Controly caches names, hopefully will prevent crashes from repeat scans
. Hitting tab from the dropdown when selecting a file should extend the file

Done for alpha 9:
. Expand to short name on tab
. Fixed split windows problem
. Fixed problem with adding directories
. Runner no longer appends ".runner"
. Possibly fixed remaining fade glitch



Done for alpha 8:
. Fix font support for arrow problem?
. Thread animation
. Launchy index/options/exit
. Esc from dropdown works correctly now
. Can now edit directories by double clicking them 
. Can use environment variables in directories


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

