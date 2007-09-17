For beta release:
. Add thanks to Tyler for graphics stuff in change log
. Tyler's new skin
. Plugin event of Launchy coming forward.  Plugins should be allowed to provide search results at that time.
. Append to file plugin or .bat file or something


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

