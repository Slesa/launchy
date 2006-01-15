#include "StdAfx.h"
#include "Skin.h"
#include ".\skin.h"



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

	bgFile = directory + bgFile;

	width = ini->GetValueI(L"Background", L"width", 400);
	height = ini->GetValueI(L"Background", L"height", 50);

	trans_rgb = stringToRGB(ini->GetValue(L"Background", L"transparent_color",L"255x255x255"));
	
	translucensy = ini->GetValueI(L"Background", L"translucensy",-1);

	// Widget positions
	int tX = ini->GetValueI(L"Widgets", L"TextEntry_X", 25);
	int tY = ini->GetValueI(L"Widgets", L"TextEntry_Y", 25);
	int tW = tX + ini->GetValueI(L"Widgets", L"TextEntry_Width", 140);
	int tH = tY + ini->GetValueI(L"Widgets", L"TextEntry_Height", 50);
	inputRect.SetRect(tX,tY,tW,tH);

	tX = ini->GetValueI(L"Widgets", L"Results_X", 25);
	tY = ini->GetValueI(L"Widgets", L"Results_Y", 25);
	tW = tX + ini->GetValueI(L"Widgets", L"Results_Width", 140);
	tH = tY + ini->GetValueI(L"Widgets", L"Results_Height", 50);
	resultRect.SetRect(tX,tY,tW,tH);

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


	inputRGB = stringToRGB(ini->GetValue(L"Widgets", L"TextEntry_Color",L"255x255x255"));
	inputFontRGB = stringToRGB(ini->GetValue(L"Widgets", L"TextEntry_Font_Color",L"255x255x255"));
	resultRGB = stringToRGB(ini->GetValue(L"Widgets", L"Results_Color",L"255x255x255"));
	resultFontRGB = stringToRGB(ini->GetValue(L"Widgets", L"Results_Font_Color",L"255x255x255"));
}

void Split(wstring str, vector<wstring>& tokens, const char delim = ' ') {
	if (str.size() == 0) return;
	int start = 0;
	for(int i = 0; i < str.size(); i++) {
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
  int len = val.size();
  string s;
  s.resize(len);
  for(int i = 0; i < len; i++)
		s[i] = static_cast<char>(val[i]);
  int i = atoi(s.c_str());
  return i;
}

int Skin::stringToRGB(wstring input)
{
	vector<wstring> parts;
	Split(input, parts, 'x');
	return RGB(wsToI(parts[0]),wsToI(parts[1]),wsToI(parts[2]));
}
