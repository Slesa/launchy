/*
Launchy: Application Launcher
Copyright (C) 2005  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#pragma once
#include "Inifile.h"
#include "atltypes.h"
#include "IniFile.h"
#include "boost/smart_ptr.hpp"
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

	CString inputSmall_fontName;
	int inputSmall_fontSize;
	int inputSmall_italics;
	int inputSmall_bold;

	int listWidthInChars;


	CString results_fontName;
	int results_fontSize;
	int results_italics;
	int results_bold;

	int inputFontRGB;
	int inputSmallFontRGB;
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

	bool inputBorder;
	bool inputTransparent;

	CRect stringToRect(CString input);
	int stringToRGB(CString input);
};
