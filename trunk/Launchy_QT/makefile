ifdef DESTDIR
PREFIX=$(DESTDIR)
else
PREFIX=/usr
endif

SKINS_PATH=$(PREFIX)/share/launchy/skins
PLUGINS_PATH=$(PREFIX)/lib/launchy/plugins
PLATFORMS_PATH=$(PREFIX)/lib/launchy/

QMAKE=qmake-qt4

DEFS=SKINS_PATH=\\\"$(SKINS_PATH)\\\" PLUGINS_PATH=\\\"$(PLUGINS_PATH)\\\" \
     PLATFORMS_PATH=\\\"$(PLATFORMS_PATH)\\\"

release:
	$(QMAKE) Launchy.pro && $(DEFS) make release
	cd platforms/gnome && $(QMAKE) gnome.pro && $(DEFS) make release
	cd ../../plugins/runner && $(QMAKE) runner.pro && $(DEFS) make release
	cd ../weby && $(QMAKE) weby.pro && $(DEFS) make release
	cd ../calcy && $(QMAKE) calcy.pro && $(DEFS) make release
	cd ../gcalc && $(QMAKE) gcalc.pro && $(DEFS) make release

debug:
	$(QMAKE) Launchy.pro && $(DEFS) make debug
	cd platforms/gnome && $(QMAKE) gnome.pro && $(DEFS) make debug
	cd ../../plugins/runner && $(QMAKE) runner.pro && $(DEFS) make debug
	cd ../weby && $(QMAKE) weby.pro && $(DEFS) make debug
	cd ../calcy && $(QMAKE) calcy.pro && $(DEFS) make debug
	cd ../gcalc && $(QMAKE) gcalc.pro && $(DEFS) make debug

clean:
	$(QMAKE) Launchy.pro && $(DEFS) make clean
	cd platforms/gnome && $(QMAKE) gnome.pro && $(DEFS) make clean
	cd ../../plugins/runner && $(QMAKE) runner.pro && $(DEFS) make clean
	cd ../weby && $(QMAKE) weby.pro && $(DEFS) make clean
	cd ../calcy && $(QMAKE) calcy.pro && $(DEFS) make clean
	cd ../gcalc && $(QMAKE) gcalc.pro && $(DEFS) make clean

install:
	-install -d $(PREFIX)/bin/
	install -m 755 release/Launchy $(PREFIX)/bin/launchy
	-install -d $(PLATFORMS_PATH)
	install -m 644 -t release/libplatform_*.so $(PLATFORMS_PATH)/
	-install -d $(PLUGINS_PATH)
	install -m 644 -t release/plugins/*.so $(PLUGINS_PATH)/ 
	-install -d $(SKINS_PATH)
	install -m 644 release/skins $(SKINS_PATH)/

uninstall:
	-rm $(DESTDIR)/bin/launchy
	-rm -rf $(PLUGINS_PATH)
	-rm -rf $(PLATFORMS_PATH)
	-rm -rf $(SKINS_PATH)
