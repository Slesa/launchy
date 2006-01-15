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
public:
	CString name;
	CString bgFile;
	CString directory;
	
	// Font information
	CFont* m_FontResult;
	CFont* m_FontInput;
	int inputFontRGB;
	int resultFontRGB;


	int width;
	int height;
	int trans_rgb;
	int translucensy;
	int inputRGB;
	int resultRGB;


	int stringToRGB(wstring input);
};
