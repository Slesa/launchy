PREFIX=/usr
SKINS_PATH=$(PREFIX)/share/launchy/skins
PLUGINS_PATH=$(PREFIX)/lib/launchy/plugins
PLATFORMS_PATH=$(PREFIX)/lib/launchy/
DESKTOP_PATH=$(PREFIX)/share/applications
ICON_PATH=$(PREFIX)/share/pixmaps
QMAKE=qmake-qt4

DEFS=SKINS_PATH=\\\"$(SKINS_PATH)\\\" PLUGINS_PATH=\\\"$(PLUGINS_PATH)\\\" \
	PLATFORMS_PATH=\\\"$(PLATFORMS_PATH)\\\"

DIRS=. platforms/unix plugins/runner plugins/weby plugins/calcy plugins/gcalc


release:: 
	cd src && $(DEFS) $(QMAKE) src.pro && $(DEFS) make release
	cd platforms/unix && $(DEFS) $(QMAKE) unix.pro && $(DEFS) make release
	cd plugins/runner && $(QMAKE) runner.pro && make release
	cd plugins/weby && $(QMAKE) weby.pro && make release
	cd plugins/calcy && $(QMAKE) calcy.pro && make release
	cd plugins/gcalc && $(QMAKE) gcalc.pro && make release

debug::
	cd src && $(DEFS) $(QMAKE) src.pro && $(DEFS) make debug
	cd platforms/unix && $(DEFS) $(QMAKE) unix.pro && $(DEFS) make debug
	cd plugins/runner && $(QMAKE) runner.pro && make debug
	cd plugins/weby && $(QMAKE) weby.pro && make debug
	cd plugins/calcy && $(QMAKE) calcy.pro && make debug
	cd plugins/gcalc && $(QMAKE) gcalc.pro && make debug

clean::
	cd src && $(QMAKE) src.pro && $(DEFS) make clean
	cd platforms/unix && $(QMAKE) unix.pro && $(DEFS) make clean
	cd plugins/runner && $(QMAKE) runner.pro &&  make clean
	cd plugins/weby && $(QMAKE) weby.pro &&  make clean
	cd plugins/calcy && $(QMAKE) calcy.pro && make clean
	cd plugins/gcalc && $(QMAKE) gcalc.pro &&  make clean

install:: release
	cd src && $(DEFS) $(QMAKE) src.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install
	cd platforms/unix && $(QMAKE) unix.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install
	cd plugins/runner && $(QMAKE) runner.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install
	cd plugins/weby && $(QMAKE) weby.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install
	cd plugins/calcy && $(QMAKE) calcy.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install
	cd plugins/gcalc && $(QMAKE) gcalc.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-install

uninstall::
	cd src && $(DEFS) $(QMAKE) src.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall
	cd platforms/unix && $(QMAKE) unix.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall
	cd plugins/runner && $(QMAKE) runner.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall
	cd plugins/weby && $(QMAKE) weby.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall
	cd plugins/calcy && $(QMAKE) calcy.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall
	cd plugins/gcalc && $(QMAKE) gcalc.pro && $(DEFS) make INSTALL_ROOT=$(DESTDIR) release-uninstall