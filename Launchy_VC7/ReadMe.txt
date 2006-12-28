
Seems like this used to work and perhaps I broke it somehow.. either way I'm too tired to work now.

PLUGIN DETAIL:
Okay, I seem to properly recognize ownership of a string and tab sort of works but I need to 
handle deletion of characters

TODO to finish Google plugin:
. Properly display the text, when I hit tab it should auto-complete from "goo" to "Google"
. Transfer the icon from the dll to Launchy somehow, I have to figure out how this works.





Where I've left off:

Hmm.  Still have the clone problem.  Need a real fix for this.

Trying to get sticky window to work.  It mostly works except when you start launchy it doesn't 
show right away and also it's possible to get it to not reset its timer so sometimes the drop list
won't show.



Changes since 0.9.6:
. Brought back in the association code
. Sticky window (always on top) now possible
. Now handles duplicates of filenames 
. Dropdown window are much more useful
. USB Mode Option
. Configurable Database Update Rate

BUG FIXES:
. Fixed a no icon bug (looked up icon on c: rather than find the windows drive)
. Donate screen could come up twice if you rebooted, this is annoying.  I fixed this by forcing a store after the options change.
. Fixed the launchy crash bug?
. Fixed LUA situation
. Now the hotkey window will initialize with your current hotkey

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