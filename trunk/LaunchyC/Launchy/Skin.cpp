#include "StdAfx.h"
#include "Skin.h"



Skin::Skin(CString dir) : ini(new CIniFile())
{
	directory = dir;
	CString path = dir + _T("skin.ini");
	ini->SetPath(path.GetBuffer());
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
	int tX = ini->GetValueI(L"WidgetPosition", L"TextEntry_X", 25);
	int tY = ini->GetValueI(L"WidgetPosition", L"TextEntry_Y", 25);
	int tW = tX + ini->GetValueI(L"WidgetPosition", L"TextEntry_Width", 140);
	int tH = tY + ini->GetValueI(L"WidgetPosition", L"TextEntry_Height", 50);
	inputRect.SetRect(tX,tY,tW,tH);

	tX = ini->GetValueI(L"WidgetPosition", L"Results_X", 25);
	tY = ini->GetValueI(L"WidgetPosition", L"Results_Y", 25);
	tW = tX + ini->GetValueI(L"WidgetPosition", L"Results_Width", 140);
	tH = tY + ini->GetValueI(L"WidgetPosition", L"Results_Height", 50);
	resultRect.SetRect(tX,tY,tW,tH);
}
