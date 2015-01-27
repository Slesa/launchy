TEMPLATE			= lib

UI_DIR				= .ui
MOC_DIR				= .moc

debug:DLLDESTDIR	= ../../../debug/plugins
release:D::DESTDIR	= ../../../release/plugins

macx {
	debug:DLLDESTDIR	= ../../../debug/Launchy.app/Contents/MacOS/plugins
	relase:DLLDESTDIR	= ../../../release/Launchy.app/Contents/MacOS/plugins	
}