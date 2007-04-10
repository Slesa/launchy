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
#include "Options.h"
#include "LaunchyDlg.h"
#include "LaunchySmarts.h"
#include ".\options.h"





Options::Options(void) : ini(new CIniFile())
, vkey(0)
, mod_key(0)
, usbmode(false)
, Indexing(false)
{
	
	firstRun = false;

	hMutex = CreateMutex( 
		NULL,                       // default security attributes
		FALSE,                      // initially not owned
		NULL);                      // unnamed mutex


	CDiskObject disk;
	if (disk.FileExists(_T("launchy.ini"))) {
		ini->SetPath(_T("launchy.ini"));
		usbmode = true;
	} else { 
		CString dir;
		LaunchySmarts::GetShellDir(CSIDL_APPDATA, dir);
		dir += _T("\\Launchy\\");
		disk.CreateDirectory(dir);
		set_dataPath(dir);
		dir += _T("\\launchy.ini");
		ini->SetPath(dir);
	}

	if (!ini->ReadFile()) {
		firstRun = true;
	}

	ParseIni();
	LoadSkins();
	UpgradeCleanup();
}


void Options::changeUsbMode(bool toUSB)
{
	if (toUSB) {
		set_dataPath(_T(""));		
		ini->SetPath(_T("launchy.ini"));
		usbmode = true;
	} else {
		CDiskObject disk;
		CString dir;
		LaunchySmarts::GetShellDir(CSIDL_APPDATA, dir);
		dir += _T("\\Launchy\\");
		disk.CreateDirectory(dir);
		set_dataPath(dir);
		dir += _T("launchy.ini");
		ini->SetPath(dir);
		usbmode = false;

		// Need to delete the old files else they will
		// be used when next Launchy starts!
		getLock();
		disk.RemoveFile(_T("launchy.ini"));
		disk.RemoveFile(_T("launchy.db"));
		relLock();
	}
}

Options::~Options(void)
{
	Store();
}

vector<CString> DeSerializeStringArray(CString input) {
	vector<CString> output;

	CString tmp;
	int cur = 0;
	while(true) {
		int c = input.Find(_T(";"), cur);
		if (c > 0 && c < input.GetLength()) {
			tmp = input.Mid(cur,c-cur);
			output.push_back(tmp);
		} else {
			break;
		}
		cur = c+1;
	}
	return output;
}

CString SerializeStringArray(vector<CString> input) {
	CString output = _T("");
	for(uint i = 0; i < input.size(); i++) {

		output.Append(input[i]);
		output.Append(_T(";"));
	}
	return output;
}


vector<DirOptions> DeSerializeDirArray(CString input) {
	vector<DirOptions> output;

	CString tmp;
	int cur = 0;
	while(true) {
		DirOptions d;
		int c = input.Find(_T(";"), cur);
		if (c > 0 && c < input.GetLength()) {
			tmp = input.Mid(cur,c-cur);
			d.dir = tmp;
			cur = c+1;

			int end = input.Find(_T("|"),cur);
			if (end != -1) {
				c = input.Find(_T(","),cur);
				while(cur < end && c < end && c != -1) {
					CString type = input.Mid(cur, c-cur);
					d.types.push_back(type);
					cur = c+1;
					c = input.Find(_T(","),cur);
				}
				cur = end + 1;
			}
			output.push_back(d);
		} else {
			break;
		}
	}
	return output;
}

CString SerializeDirArray(vector<DirOptions> input) {
	CString output = _T("");
	for(uint i = 0; i < input.size(); i++) {

		output.Append(input[i].dir);
		output.Append(_T(";"));
		for(uint j = 0; j < input[i].types.size(); j++) {
			output.Append(input[i].types[j]);
			output.Append(_T(","));
		}
		output.Append(_T("|"));
	}
	return output;
}

bool Options::LoadPlugin(CString PluginName)
{
	return  ini->GetValueB(_T("Plugins"), PluginName, true);
}

void Options::ParseIni(void)
{
	CString DefaultDirs;
	CString myMenu, allMenus;
	LaunchySmarts::GetShellDir(CSIDL_COMMON_STARTMENU, allMenus);
	LaunchySmarts::GetShellDir(CSIDL_STARTMENU, myMenu);
	DefaultDirs.Format(_T("%s;.lnk,|%s;.lnk,|Utilities\\;.lnk,|"), myMenu, allMenus);

	ver = ini->GetValueI(_T("Launchy Information"), _T("Version"), 0);

	posX = ini->GetValueI(_T("Position"), _T("pos_x"));
	posY = ini->GetValueI(_T("Position"), _T("pos_y"));

	listLength = ini->GetValueI(_T("Advanced"), _T("List Length"), 10);
	stickyWindow = ini->GetValueB(_T("Advanced"), _T("Sticky Window"), false);
	chkupdate = ini->GetValueB(_T("Advanced"), _T("Check For Updates"), true);
	aot = ini->GetValueB(_T("Advanced"), _T("Always on Top"), false);
	indexTime = ini->GetValueI(_T("Advanced"), _T("Index Time"), 20);

	mod_key =  ini->GetValueI(_T("Hotkey"), _T("mod_key"), MOD_ALT);
	vkey =  ini->GetValueI(_T("Hotkey"), _T("vkey"), VK_SPACE);

	skinName = ini->GetValue(_T("Skin"), _T("name"), _T("Default"));

	set_Directories(DeSerializeDirArray(ini->GetValue(_T("General"), _T("Directories"), DefaultDirs)));
	set_Types(DeSerializeStringArray(ini->GetValue(_T("General"), _T("Types"), _T(".lnk;"))));



	CTime t = CTime::GetCurrentTime();

	installTime = CTime( ini->GetValueTime(_T("Launchy Information"), _T("InstallTime"), t.GetTime()));


}

void Options::Store(void)
{
	CWinApp* pApp = AfxGetApp();
	if (pApp == NULL) return;
	RECT location;
	pApp->GetMainWnd()->GetWindowRect(&location);

	ini->SetValueI(_T("Launchy Information"), _T("Version"), LAUNCHY_VERSION);
	ini->SetValueI(_T("Position"), _T("pos_x"), location.left);
	ini->SetValueI(_T("Position"), _T("pos_y"), location.top);

	ini->SetValueI(_T("Advanced"), _T("List Length"),  listLength);
	ini->SetValueB(_T("Advanced"), _T("Sticky Window"), stickyWindow);
	ini->SetValueB(_T("Advanced"), _T("Check For Updates"), chkupdate);
	ini->SetValueB(_T("Advanced"), _T("Always on Top"), aot);
	ini->SetValueI(_T("Advanced"), _T("Index Time"), indexTime);

	ini->SetValueI(_T("Hotkey"), _T("mod_key"), mod_key);
	ini->SetValueI(_T("Hotkey"), _T("vkey"), vkey);

	ini->SetValue(_T("Skin"), _T("name"), skinName);

	ini->SetValue(_T("General"), _T("Directories"), SerializeDirArray(get_Directories()));
	ini->SetValue(_T("General"), _T("Types"), SerializeStringArray(get_Types()));


	ini->SetValueTime(_T("Launchy Information"), _T("InstallTime"), installTime.GetTime());

	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;

	if (plugins != NULL) {
		for(int i = 0; i < plugins->allPlugins.size(); i++) {
			ini->SetValueB(_T("Plugins"), plugins->allPlugins[i].name, plugins->allPlugins[i].loaded);
		}
	}
	ini->WriteFile();
}

void Options::LoadSkins(void)
{
	wchar_t buffer[_MAX_PATH];
	
	/* Get the current working directory: */
	_wgetcwd( buffer, _MAX_PATH );

	CString dir = buffer;
	dir += "\\Skins\\";

	CDiskObject disk;
	CStringArray skinDirs;
	disk.EnumAllDirectories(dir, skinDirs);

	INT_PTR count = skinDirs.GetCount();
	for(int i = 0; i < count; i++) {
		shared_ptr<Skin> x(new Skin(skinDirs[i]));
		if (x->name == _T("")) continue;
		skins.push_back(x);
		if (x->name == skinName) {
			skin = x;
		}
	}


}

void Options::UpgradeCleanup(void)
{
	if (firstRun) {
/*		AfxMessageBox(_T("Welcome to Launchy!\n \
			Launchy is currently running in the background and can be brought forward by pressing Alt+Space\n \
			Enjoy!"));
			*/
	} 

	// Ver == 0 for all versions below 0.91 (ver wasn't added until 0.91)

	// Delete the Users/ config files
	if (ver < 98) {
		// Remove the old configuration directories if they exist
		CDiskObject disk;
		disk.RemoveDirectory(_T("Users"));
	}

	if (ver < 102) {
		CString file;
		LaunchySmarts::GetShellDir(CSIDL_APPDATA, file);
		file += _T("\\Launchy\\");
		file += L"launchy.db";
		CDiskObject disk;
		disk.RemoveFile(file);
	}

	if (ver < LAUNCHY_VERSION) {
		installTime = CTime::GetCurrentTime();
	}
}

void Options::SetSkin(CString name)
{
	// Select this skin
	for(uint i = 0; i < skins.size(); i++) {
		if (skins[i]->name == name) {
			skin = skins[i];
			skinName = name;
			break;
		}
	}
}

void Options::Associate(CString entry, CString destination) 	 
{ 	 
	ini->SetValue(_T("Associations"), entry, destination); 	 
} 	 


CString Options::GetAssociation(CString query) 	 
{ 	 
	CString res = ini->GetValue(_T("Associations"), query, _T("")); 	 
	if (res != _T("")) 	 
		return res; 	 
	return _T(""); 	 

	return res; 	 
}

void Options::getLock(void)
{
	WaitForSingleObject(hMutex, INFINITE);
}

void Options::relLock(void)
{
	ReleaseMutex(hMutex);
}
