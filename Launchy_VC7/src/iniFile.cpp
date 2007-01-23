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


// IniFile.cpp: implementation of the CIniFile class.
// Written by: Adam Clauss
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"

#include <string>
#include <sstream>
#include "UTF16File.h"
using namespace std;
//#include "fstream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////

//default constructor
CIniFile::CIniFile()
{
}

//constructor, can specify pathname here instead of using SetPath later
CIniFile::CIniFile(CString inipath)
{
	path = inipath;
}

//default destructor
CIniFile::~CIniFile()
{

}

/////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////

//sets path of ini file to read and write from
void CIniFile::SetPath(CString newpath)
{
	path = newpath;
}

//reads ini file specified using CIniFile::SetPath()
//returns true if successful, false otherwise
bool CIniFile::ReadFile()
{
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(path,status))
		return 0;
//	ifstream inifile;
	CString readinfo;
//	inifile.open(path);
	CUTF16File inifile;
	
	int curkey = -1, curval = -1;

	if ( inifile.Open(path, CFile::modeRead) == 0)
	{
		error = "Unable to open ini file.";
		return 0;
	}

	CString keyname, valuename, value;
	CString temp;
	while (inifile.ReadString(readinfo))
	{
		if (readinfo != "")
		{
			if (readinfo[0] == '[' && readinfo[readinfo.GetLength()-1] == ']') //if a section heading
			{
				keyname = readinfo;
				keyname.TrimLeft('[');
				keyname.TrimRight(']');
			}
			else //if a value
			{
				valuename = readinfo.Left(readinfo.Find(_T("=")));
				value = readinfo.Right(readinfo.GetLength()-valuename.GetLength()-1);
				SetValue(keyname,valuename,value);
			}
		}
	}
	inifile.Close();
	return 1;
}

//writes data stored in class to ini file
void CIniFile::WriteFile()
{
	CUTF16File inifile;
	inifile.Open(path, CFile::modeWrite|CFile::modeCreate);


//    WORD wBOM = 0xFEFF;
//	inifile.Write(&wBOM, sizeof(WORD));


	for (int keynum = 0; keynum <= names.GetUpperBound(); keynum++)
	{
		if (keys[keynum].names.GetSize() != 0)
		{
			CString blah;
			blah.Format(_T("[%s]\n"), names[keynum]);
//			inifile << L"[" << names[keynum] << L"]" << endl;
			inifile.WriteString(blah, true);
			for (int valuenum = 0; valuenum <= keys[keynum].names.GetUpperBound(); valuenum++)
			{
				blah.Format(_T("%s=%s"), keys[keynum].names[valuenum], keys[keynum].values[valuenum]);
				inifile.WriteString(blah, true);
//				inifile << keys[keynum].names[valuenum] << "=" << keys[keynum].values[valuenum];
				if (valuenum != keys[keynum].names.GetUpperBound())
					inifile.WriteString(_T("\n"), true);
//					inifile << endl;
				else
					if (keynum < names.GetSize())
						inifile.WriteString(_T("\n"), true);
						//						inifile << endl;
			}
			if (keynum < names.GetSize())
				inifile.WriteString(_T("\n"), true);
//				inifile << endl;
		}
	}
	inifile.Close();
}

//deletes all stored ini data
void CIniFile::Reset()
{
	keys.SetSize(0);
	names.SetSize(0);
}

//returns number of keys currently in the ini
int CIniFile::GetNumKeys()
{
	return (int) keys.GetSize();
}

//returns number of values stored for specified key, or -1 if key found
int CIniFile::GetNumValues(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return -1;
	else
		return (int) keys[keynum].names.GetSize();
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
CString CIniFile::GetValue(CString keyname, CString valuename, CString def)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);

	if (keynum == -1)
	{
		return def;
	}

	if (valuenum == -1)
	{
		return def;
	}
	return keys[keynum].values[valuenum];
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
int CIniFile::GetValueI(CString keyname, CString valuename, int def)
{
	CString ret = GetValue(keyname, valuename, _T("Asddfcnsd923"));
	if (ret == _T("Asddfcnsd923")) return def;

	return _wtoi(ret.GetBuffer());
}

__time64_t CIniFile::GetValueTime(CString keyname, CString valuename, __time64_t def)
{
	CString ret = GetValue(keyname, valuename, _T("Asddfcnsd923"));
	if (ret == _T("Asddfcnsd923")) return def;


	TCHAR* pend;


	return _wcstoi64(ret.GetBuffer(), &pend, 10); 
}


bool CIniFile::GetValueB(CString keyname, CString valuename, bool def)
{
	return (bool) GetValueI(keyname, valuename, def);
}

//gets value of [keyname] valuename = 
//overloaded to return CString, int, and double
double CIniFile::GetValueF(CString keyname, CString valuename, double def)
{
	CString ret = GetValue(keyname, valuename, _T("Asddfcnsd923"));
	if (ret == _T("Asddfcnsd923")) return def;
	return _wtof(ret.GetBuffer());
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CIniFile::SetValue(CString keyname, CString valuename, CString value, bool create)
{
	int keynum = FindKey(keyname), valuenum = 0;
	//find key
	if (keynum == -1) //if key doesn't exist
	{
		if (!create) //and user does not want to create it,
			return 0; //stop entering this key
		names.SetSize(names.GetSize()+1);
		keys.SetSize(keys.GetSize()+1);
		keynum = (int) names.GetSize()-1;
		names[keynum] = keyname;
	}

	//find value
	valuenum = FindValue(keynum,valuename);
	if (valuenum == -1)
	{
		if (!create)
			return 0;
		keys[keynum].names.SetSize(keys[keynum].names.GetSize()+1);
		keys[keynum].values.SetSize(keys[keynum].names.GetSize()+1);
		valuenum = (int) keys[keynum].names.GetSize()-1;
		keys[keynum].names[valuenum] = valuename;
	}
	keys[keynum].values[valuenum] = value;
	return 1;
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CIniFile::SetValueI(CString keyname, CString valuename, int value, bool create)
{
	CString temp;
	temp.Format(_T("%d"),value);
	return SetValue(keyname, valuename, temp, create);
}

bool CIniFile::SetValueTime(CString keyname, CString valuename, __time64_t value, bool create)
{
	CString temp;
	temp.Format(_T("%I64d"),value);
	return SetValue(keyname, valuename, temp, create);
}

bool CIniFile::SetValueB(CString keyname, CString valuename, bool value, bool create)
{
	CString temp;
	temp.Format(_T("%d"),value);
	return SetValue(keyname, valuename, temp, create);
}

//sets value of [keyname] valuename =.
//specify the optional paramter as false (0) if you do not want it to create
//the key if it doesn't exist. Returns true if data entered, false otherwise
//overloaded to accept CString, int, and double
bool CIniFile::SetValueF(CString keyname, CString valuename, double value, bool create)
{
	CString temp;
	temp.Format(_T("%e"),value);
	return SetValue(keyname, valuename, temp, create);
}

//deletes specified value
//returns true if value existed and deleted, false otherwise
bool CIniFile::DeleteValue(CString keyname, CString valuename)
{
	int keynum = FindKey(keyname), valuenum = FindValue(keynum,valuename);
	if (keynum == -1 || valuenum == -1)
		return 0;

	keys[keynum].names.RemoveAt(valuenum);
	keys[keynum].values.RemoveAt(valuenum);
	return 1;
}

//deletes specified key and all values contained within
//returns true if key existed and deleted, false otherwise
bool CIniFile::DeleteKey(CString keyname)
{
	int keynum = FindKey(keyname);
	if (keynum == -1)
		return 0;
	keys.RemoveAt(keynum);
	names.RemoveAt(keynum);
	return 1;
}

/////////////////////////////////////////////////////////////////////
// Private Functions
/////////////////////////////////////////////////////////////////////

//returns index of specified key, or -1 if not found
int CIniFile::FindKey(CString keyname)
{
	int keynum = 0;
	while ( keynum < keys.GetSize() && names[keynum] != keyname)
		keynum++;
	if (keynum == keys.GetSize())
		return -1;
	return keynum;
}

//returns index of specified value, in the specified key, or -1 if not found
int CIniFile::FindValue(int keynum, CString valuename)
{
	if (keynum == -1)
		return -1;
	int valuenum = 0;
	while (valuenum < keys[keynum].names.GetSize() && keys[keynum].names[valuenum] != valuename)
		valuenum++;
	if (valuenum == keys[keynum].names.GetSize())
		return -1;
	return valuenum;
}

