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
	
	translucensy = ini->GetValueI(_T("Background"), _T("translucensy"),-1);

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

int wsToI(wstring val) {
  size_t len = val.size();
  string s;
  s.resize(len);
  for(size_t i = 0; i < len; i++)
		s[i] = static_cast<char>(val[i]);
  int x = atoi(s.c_str());
  return x;
}

int Skin::stringToRGB(CString in)
{
	wstring input = in.GetBuffer();
	vector<wstring> parts;
	Split(input, parts, 'x');
	return RGB(wsToI(parts[0]),wsToI(parts[1]),wsToI(parts[2]));
}

int wsToInt(wstring val) {
  size_t len = val.size();
  string s;
  s.resize(len);
  for(size_t i = 0; i < len; i++)
		s[i] = static_cast<char>(val[i]);
  int x = atoi(s.c_str());
  return x;
}

CRect Skin::stringToRect(CString in)
{
	wstring input = in.GetBuffer();
	vector<wstring> parts;
	Split(input, parts, ',');
	CRect r;
	int x1,y1,x2,y2;
	x1 = wsToInt(parts[0]);
	y1 = wsToInt(parts[1]);
	x2 = wsToInt(parts[2]);
	y2 = wsToInt(parts[3]);
	r.SetRect(x1,y1,x1+x2,y1+y2);
	return r;
}