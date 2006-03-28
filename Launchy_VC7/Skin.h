#pragma once
#include "Inifile.h"
#include "atltypes.h"
#include "IniFile.h"
#include <boost/smart_ptr.hpp>
#include "Skin.h"

using namespace boost;


class Skin
{
public:
	shared_ptr<CIniFile> ini;
	Skin(CString dir);
public:
	~Skin(void);
public:
	void parseSkinFile(void);
public:
	CRect inputRect;
public:
	CRect resultRect;
	CRect iconRect;

public:
	CString name;
	CString bgFile;
	CString alphaBorderFile;
	CRect alphaRect;
	CString directory;


	CString input_fontName;
	int input_fontSize;
	int input_italics;
	int input_bold;

	CString results_fontName;
	int results_fontSize;
	int results_italics;
	int results_bold;

	int inputFontRGB;
	int resultFontRGB;

	CRect backRect;
//	int width;
//	int height;
	int trans_rgb;
	int translucensy;
	int inputRGB;
	int resultRGB;

	bool resultTransparent;
	bool resultBorder;

	CRect stringToRect(wstring input);
	int stringToRGB(wstring input);
};
