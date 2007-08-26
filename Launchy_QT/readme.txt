Currently working on: 
Working on the plugin dialog box.  Currently getting options.cpp to call plugins.doDialog on plugin selection change (pluginChanged)


Trying to stylize the scrollbar is difficult

If you simply go to "switch user" and then come back in, the alpha border is fucked.  I tried recalling UpdateLayeredWindow and that didn't really help.  Reapplying the entire skin does work though.  WTF?

There are memory leaks everywhere.

If I keep the concept of labels, then the getIcon() function doesn't need to use QDir()::exists, it can just check for a directory label



Why does Qt::WindowStaysOnTopHint cause the program to close?

Todo:
. When reindexing, always update the current search afterwards
. Make sure gSearchTxt is set before sort

KNOWN PROBLEMS:
Icons don't show up if wrong color setting (16-bit)

LAPTOP CHANGES:
1) Set global main window variable in constructor of main 
