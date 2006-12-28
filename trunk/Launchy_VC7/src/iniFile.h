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

// IniFile.h: interface for the CIniFile class.
// Written by: Adam Clauss
// Email: pandcc3@comwerx.net
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//////////////////////////////////////////////////////////////////////


#pragma once

#include "StdAfx.h"
#include <afxtempl.h>




class CIniFile  
{
	//all private variables
private:

	//stores pathname of ini file to read/write
	CString path;
	
	//all keys are of this time
	struct key
	{
		//list of values in key
		CArray<CString, CString> values; 

		//corresponding list of value names
		CArray<CString, CString> names;
	};

	//list of keys in ini
	CArray<key, key> keys; 

	//corresponding list of keynames
	CArray<CString, CString> names; 
	
	
	//all private functions
private:
	//overloaded to take CString

	//returns index of specified value, in the specified key, or -1 if not found
	int FindValue(int keynum, CString valuename);

	//returns index of specified key, or -1 if not found
	int FindKey(CString keyname);


	//public variables
public:

	//will contain error info if one occurs
	//ended up not using much, just in ReadFile and GetValue
	CString error;


	//public functions
public:
	//default constructor
	CIniFile();

	//constructor, can specify pathname here instead of using SetPath later
	CIniFile(CString inipath);

	//default destructor
	virtual ~CIniFile();

	//sets path of ini file to read and write from
	void SetPath(CString newpath);

	//reads ini file specified using CIniFile::SetPath()
	//returns true if successful, false otherwise
	bool ReadFile();

	//writes data stored in class to ini file
	void WriteFile(); 

	//deletes all stored ini data
	void Reset();

	//returns number of keys currently in the ini
	int GetNumKeys();

	//returns number of values stored for specified key
	int GetNumValues(CString keyname);

	//gets value of [keyname] valuename = 
	//overloaded to return CString, int, and double,
	//returns "", or 0 if key/value not found.  Sets error member to show problem
	CString GetValue(CString keyname, CString valuename, CString def); 
	int GetValueI(CString keyname, CString valuename, int def = 0); 
	__time64_t GetValueTime(CString keyname, CString valuename, __time64_t def = 0);
	double GetValueF(CString keyname, CString valuename, double def = 0.0);
	bool GetValueB(CString keyname, CString valuename, bool def = false);

	//sets value of [keyname] valuename =.
	//specify the optional paramter as false (0) if you do not want it to create
	//the key if it doesn't exist. Returns true if data entered, false otherwise
	//overloaded to accept CString, int, and double
	bool SetValue(CString key, CString valuename, CString value, bool create = 1);
	bool SetValueI(CString key, CString valuename, int value, bool create = 1);
	bool SetValueTime(CString key, CString valuename, __time64_t value, bool create = 1);
	bool SetValueF(CString key, CString valuename, double value, bool create = 1);
	bool SetValueB(CString key, CString valuename, bool value, bool create = 1);
	//deletes specified value
	//returns true if value existed and deleted, false otherwise
	bool DeleteValue(CString keyname, CString valuename);

	//deletes specified key and all values contained within
	//returns true if key existed and deleted, false otherwise
	bool DeleteKey(CString keyname);
};

