Features to complete before release:
. Plugin SDK Package
. Make plugin section on the webpage
. Add the chinese character stuff from Bborn

New Features:
. Added AlwaysOnTop Toggle
. Add plugins dialog with enable/disable and options button
. Runny plugin
. Command line arguments
. Options support for plugins
. Added .* as a filetype
. Update notification
. Control Panel plugin


Bug Fixes/Improvements:
. Fill up the textbox with words and then use ctrl+left/right arrow and it scrambles the text as it scrolls
. Removed startup crash bug
. Fixed Google not getting selected bug
. Fixed mouse problems
. Launchy no longer jumps to original position when skins are changed
. Fixed the problem with launchy in the corner of the screen moving elsewhere on reload
. Calculator took priority over filenames with numbers, fixed
. Launchy no longer flashes on screen when starting up

WHERE I LEFT OFF:
The plugin dialog is almost done.. except for the hard part: live update of plugins.  It's probably a really bad idea
to allow dynamic loading/unloading of plugins but I think it's what users would expect.  So, take a look at void Plugin::ReloadPlugins(map<CString, bool> ids) {
next time you get back to the code.




How to fix the startup crash bug:
. I added a return on all SmartComboBox functions if the inputbox subclass m_hWnds weren't valid
. On EditChange and EditUpdate I made it showlaunchy, dropdown, come back up, hidelaunchy instead of return
. That worked but it made launchy dissappear, now I'm trying to ensure that the subclassing happens while
within the initinstance of LaunchyDlg but that's  not working yet.  Perhaps it should be done from within the smartcombobox 
when it initializes?  Perhaps onintinstance isn't the proper place to do it.


Things need to be done:
. Selecting a file other than default is not working, such as the Launchy spreadsheet

. Set the working directory to wherever the executable is on startup

. Fix the problem with the directory not searching everything

. typing "my docu" tab "recp" while debugging kills it on the p (that's the first non-matchable letter)
. my doc <tab> causes some weird icon crash

. Explory: If I type c:\dev\ whatever without hitting tab, launchy doesn't know what to do with it :(

. I got a crash once when I typed in "my do" then hit tab for my documents :/  It had been in use for awhile.
. I typed in "weath" and when I hit tab launchy froze so I then typed in 87110 and a few seconds 
later it gave me the crash dialog.  Why the crash?  Why the long freeze?

. If Launchy starts off the screen, move it!




. Every time I use Launchy for the first time it crashes.
 For example... I reboot my computer, wait for everything to load, 
 then hit ALT+SPACE. Launchy shows up as normal, so I start typing.
  Then it locks up and crashes. 
  I get a Runtime error "R6025 -pure virtual function call". 
  If I restart Launchy and try again it works just fine. 
  But it's always the first time it's used that it crashes. Any ideas? :(
  
  -- This must be the dropdown menu not being properly subclassed, and hence
  there are virtual function problems.  Ick!

> * It is weird that unless you type TAB after you type c: it does not seem to
> go into Explory mode. That is, you cannot type c:\windows and start
> exploring from there. As : cannot be part of any filename in windows, I
> think that if the second character typed into launchy is : you could
> automatically go into Explory mode. 




PLUGIN DETAIL:
Okay, I seem to properly recognize ownership of a string and tab sort of works but I need to 
handle deletion of characters

TODO to finish Google plugin:
. Properly display the text, when I hit tab it should auto-complete from "goo" to "Google"
. Transfer the icon from the dll to Launchy somehow, I have to figure out how this works.





Where I've left off:


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