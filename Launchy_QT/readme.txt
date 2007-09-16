To deal with for alpha release:
. Seems like DB isn't always saved correctly (webstats)
. Changing skins in options (then cancel I guess?) causes the alpha border to move
. Add %s to weby for custom searches


Currently working on: 

Seems like the database is getting hosed every now and then, "webstats" loses its count from time to time

Add "open with" support

Are the files being written/read in unicode?  Check on firefox bookmarks too.

when there are no suggested results, run them as arguments to the first thing

When a db is done building, redo the search on the current text

Trying to stylize the scrollbar is difficult

If you simply go to "switch user" and then come back in, the alpha border is fucked.  I tried recalling UpdateLayeredWindow and that didn't really help.  Reapplying the entire skin does work though.  WTF?

Playing with the options tab on xp machine caused crashes when the plugins weren't loaded

Rescanning the database caused the rescan to freeze at around 76%, couldn't do any more scans after that

Allow for caps in input letters

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

