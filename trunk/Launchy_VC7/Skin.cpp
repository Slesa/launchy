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

#include "StdAfx.h"
#include "Skin.h"
#include ".\skin.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

Skin::Skin(CString dir) : ini(new CIniFile())
{
	directory = dir;
	CString path = dir + _T("skin.ini");
//	CString path = dir + _T("skin.ini");
	ini->SetPath(path);
	if (!ini->ReadFile()) {
		CString err;
		err.Format(_T("A skin.ini file was not found in the skin directory: %s"), dir);
//		AfxMessageBox(err);
		return;
	}

	parseSkinFile();
	ini.reset();
}

Skin::~Skin(void)
{
}



void Skin::parseSkinFile(void)
{
	// General Skin Information
	name = ini->GetValue(_T("General"), _T("name"), _T("default skin name"));

	// Background material
	bgFile = ini->GetValue(_T("Background"), _T("bitmap"), _T(""));
//	if (bgFile != "") 
		bgFile = directory + bgFile;

	backRect = stringToRect(ini->GetValue(_T("Background"), _T("backRect"), _T("0,0,400,50")));

	alphaBorderFile = ini->GetValue(_T("Background"), _T("alphaBorder"), _T(""));
	if (alphaBorderFile != "")
		alphaBorderFile = directory + alphaBorderFile;

	alphaRect = stringToRect(ini->GetValue(_T("Background"), _T("AlphaRect"), _T("0,0,400,50")));

	trans_rgb = stringToRGB(ini->GetValue(_T("Background"), _T("transparent_color"),_T("255x255x255")));
	
	translucensy = ini->GetValueI(_T("Background"), _T("translucency"),-1);

	// Widget positions
	inputRect = stringToRect(ini->GetValue(_T("Widgets"), _T("TextEntryRect"), _T("25,25,140,50")));
	resultRect = stringToRect(ini->GetValue(_T("Widgets"), _T("ResultsRect"), _T("25,25,140,50")));
	iconRect = stringToRect(ini->GetValue(_T("Widgets"),_T("IconRect"),_T("183,25,32,32")));

	// Widget fonts
	input_fontName = ini->GetValue(_T("Widgets"), _T("TextEntry_Font"), _T("Trebuchet MS"));
	input_fontSize = ini->GetValueI(_T("Widgets"), _T("TextEntry_Font_Size"), 10);
	input_italics = ini->GetValueI(_T("Widgets"), _T("TextEntry_Font_Italics"), 0);
	input_bold = ini->GetValueI(_T("Widgets"), _T("TextEntry_Font_Bold"), 0);
	if (input_bold == 1) input_bold = 700;

	results_fontName = ini->GetValue(_T("Widgets"), _T("Results_Font"), _T("Trebuchet MS"));
	results_fontSize = ini->GetValueI(_T("Widgets"), _T("Results_Font_Size"), 10);
	results_italics = ini->GetValueI(_T("Widgets"), _T("Results_Font_Italics"), 0);
	results_bold = ini->GetValueI(_T("Widgets"), _T("Results_Font_Bold"), 0);
	if (results_bold == 1) results_bold = 700;


	inputBorder = ini->GetValueB(_T("Widgets"), _T("TextEntry_ShowBorder"), true);
	inputTransparent = ini->GetValueB(_T("Widgets"), _T("TextEntry_TransparentBkgnd"), false);

	resultBorder = ini->GetValueB(_T("Widgets"), _T("Results_ShowBorder"), true);
	resultTransparent = ini->GetValueB(_T("Widgets"), _T("Results_TransparentBkgnd"), false);

	inputRGB = stringToRGB(ini->GetValue(_T("Widgets"), _T("TextEntry_Color"),_T("255x255x255")));
	inputFontRGB = stringToRGB(ini->GetValue(_T("Widgets"), _T("TextEntry_Font_Color"),_T("255x255x255")));
	resultRGB = stringToRGB(ini->GetValue(_T("Widgets"), _T("Results_Color"),_T("255x255x255")));
	resultFontRGB = stringToRGB(ini->GetValue(_T("Widgets"), _T("Results_Font_Color"),_T("255x255x255")));
}

void Split(wstring str, vector<wstring>& tokens, const TCHAR delim = ' ') {
	if (str.size() == 0) return;
	int start = 0;
	for(uint i = 0; i < str.size(); i++) {
		if (str[i] == delim) {
			if (i != 0) 
				tokens.push_back(str.substr(start,i-start));
			start = i+1;
		}
	}
	if (str[str.size()-1] != delim) {
		tokens.push_back(str.substr(start, str.size()-start));
	}
}


int Skin::stringToRGB(CString in)
{
	wstring input = in.GetBuffer();
	vector<wstring> parts;
	Split(input, parts, 'x');
	return RGB(_wtoi(parts[0].c_str()),_wtoi(parts[1].c_str()),_wtoi(parts[2].c_str()));
}



CRect Skin::stringToRect(CString in)
{
	wstring input = in.GetBuffer();
	vector<wstring> parts;
	Split(input, parts, ',');
	CRect r;
	int x1,y1,x2,y2;
	x1 = _wtoi(parts[0].c_str());
	y1 = _wtoi(parts[1].c_str());
	x2 = _wtoi(parts[2].c_str());
	y2 = _wtoi(parts[3].c_str());
	r.SetRect(x1,y1,x1+x2,y1+y2);
	return r;
}