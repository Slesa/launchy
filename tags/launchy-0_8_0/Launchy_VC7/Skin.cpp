#include "StdAfx.h"
#include "Skin.h"



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
	name = ini->GetValue(L"General", L"name", L"default skin name").c_str();

	// Background material
	bgFile = ini->GetValue(L"Background", L"bitmap", L"").c_str();

	bgFile = directory + bgFile;

	width = ini->GetValueI(L"Background", L"width", 400);
	height = ini->GetValueI(L"Background", L"height", 50);
	red = ini->GetValueI(L"Background", L"transparent_red", 0);
	green = ini->GetValueI(L"Background", L"transparent_green", 0);
	blue = ini->GetValueI(L"Background", L"transparent_blue", 0);

	transparency = ini->GetValueI(L"Background", L"transparency",-1);

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
	// Fonts
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
	fontName = ini->GetValue(L"Widgets", L"Results_Font", L"Trebuchet MS").c_str();
	fontSize = ini->GetValueI(L"Widgets", L"Results_Font_Size", 10);
	italics = ini->GetValueI(L"Widgets", L"Results_Font_Italics", 0);
	bold = ini->GetValueI(L"Widgets", L"Results_Font_Bold", 0);

	// Fonts
/*	m_FontResult.CreateFontW(
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

	// Widget background colors
	int r = ini->GetValueI(L"Widgets", L"TextEntry_Red", 0);
	int g = ini->GetValueI(L"Widgets", L"TextEntry_Green", 0);
	int b = ini->GetValueI(L"Widgets", L"TextEntry_Blue", 0);
	inputRGB = RGB(r,g,b);

	r = ini->GetValueI(L"Widgets", L"TextEntry_Font_Red", 255);
	g = ini->GetValueI(L"Widgets", L"TextEntry_Font_Green", 255);
	b = ini->GetValueI(L"Widgets", L"TextEntry_Font_Blue", 255);
	inputFontRGB = RGB(r,g,b);

	r = ini->GetValueI(L"Widgets", L"Results_Red", 0);
	g = ini->GetValueI(L"Widgets", L"Results_Green", 0);
	b = ini->GetValueI(L"Widgets", L"Results_Blue", 0);
	resultRGB = RGB(r,g,b);


	r = ini->GetValueI(L"Widgets", L"Results_Font_Red", 255);
	g = ini->GetValueI(L"Widgets", L"Results_Font_Green", 255);
	b = ini->GetValueI(L"Widgets", L"Results_Font_Blue", 255);
	resultFontRGB = RGB(r,g,b);

}
