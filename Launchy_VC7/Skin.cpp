#include "StdAfx.h"
#include "Skin.h"
#include ".\skin.h"
#include <sstream>


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
	// Need to properly dispose of fonts
//	delete this->m_FontInput;
//	delete this->m_FontResult;
}



void Skin::parseSkinFile(void)
{
	// General Skin Information
	name = ini->GetValue(L"General", L"name", L"default skin name").c_str();

	// Background material
	bgFile = ini->GetValue(L"Background", L"bitmap", L"").c_str();
//	if (bgFile != "") 
		bgFile = directory + bgFile;

	backRect = stringToRect(ini->GetValue(L"Background", L"backRect", L"0,0,400,50"));

	alphaBorderFile = ini->GetValue(L"Background", L"alphaBorder", L"").c_str();
	if (alphaBorderFile != "")
		alphaBorderFile = directory + alphaBorderFile;

	alphaRect = stringToRect(ini->GetValue(L"Background", L"AlphaRect", L"0,0,400,50"));

	trans_rgb = stringToRGB(ini->GetValue(L"Background", L"transparent_color",L"255x255x255"));
	
	translucensy = ini->GetValueI(L"Background", L"translucensy",-1);

	// Widget positions
	inputRect = stringToRect(ini->GetValue(L"Widgets", L"TextEntryRect", L"25,25,140,50"));
	resultRect = stringToRect(ini->GetValue(L"Widgets", L"ResultsRect", L"25,25,140,50"));
	iconRect = stringToRect(ini->GetValue(L"Widgets",L"IconRect",L"183,25,32,32"));

	// Widget fonts
	CString fontName = ini->GetValue(L"Widgets", L"TextEntry_Font", L"Trebuchet MS").c_str();
	int fontSize = ini->GetValueI(L"Widgets", L"TextEntry_Font_Size", 10);
	int italics = ini->GetValueI(L"Widgets", L"TextEntry_Font_Italics", 0);
	int bold = ini->GetValueI(L"Widgets", L"TextEntry_Font_Bold", 0);
	if (bold == 1) bold = 700;
	// Fonts
	m_FontInput = new CFont;
	m_FontResult = new CFont;
	m_FontInput->CreateFontW(
	   fontSize,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   bold,						// nWeight
	   italics,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   fontName);                 // lpszFacename	

	/*	m_FontInput.CreateFontW(
	   fontSize,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   bold,                 // nWeight
	   italics,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   fontName);                 // lpszFacename	
*/
	fontName = ini->GetValue(_T("Widgets"), _T("Results_Font"), _T("Trebuchet MS")).c_str();
	fontSize = ini->GetValueI(L"Widgets", L"Results_Font_Size", 10);
	italics = ini->GetValueI(L"Widgets", L"Results_Font_Italics", 0);
	bold = ini->GetValueI(L"Widgets", L"Results_Font_Bold", 0);

	

	if (bold == 1) bold = 700;

	// Fonts
	m_FontResult->CreateFontW(
	   fontSize,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   bold,                 // nWeight
	   italics,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   fontName);                 // lpszFacename	

	resultBorder = ini->GetValueB(L"Widgets", L"Results_ShowBorder", true);
	resultTransparent = ini->GetValueB(L"Widgets", L"Results_TransparentBkgnd", false);

	inputRGB = stringToRGB(ini->GetValue(L"Widgets", L"TextEntry_Color",L"255x255x255"));
	inputFontRGB = stringToRGB(ini->GetValue(L"Widgets", L"TextEntry_Font_Color",L"255x255x255"));
	resultRGB = stringToRGB(ini->GetValue(L"Widgets", L"Results_Color",L"255x255x255"));
	resultFontRGB = stringToRGB(ini->GetValue(L"Widgets", L"Results_Font_Color",L"255x255x255"));
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

int Skin::stringToRGB(wstring input)
{
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

CRect Skin::stringToRect(wstring input)
{
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