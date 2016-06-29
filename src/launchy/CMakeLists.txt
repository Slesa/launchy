cmake_minimum_required (VERSION 2.8.12)
project (Launchy)

set(launchy_srcs
	AnimationLabel.cpp
	catalog_builder.cpp
	catalog_types.cpp
	CharLineEdit.cpp
	CharListWidget.cpp
	CommandHistory.cpp
	commandlineparser.cpp
	dsingleapplication.cpp
	Fader.cpp
	FileSearch.cpp
	icon_delegate.cpp
	icon_extractor.cpp
	InputDataList.cpp
	launchywidget.cpp
	LineEditMenu.cpp
	main.cpp
	optionsdialog.cpp
	platform_util.cpp
	plugin_handler.cpp
	plugin_interface.cpp
	singleapplication.cpp
   )
add_executable(Launchy ${launchy_srcs})
