// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows/Linux/Irix
//////////////////////////////////////////////////////////////////////

#ifndef CIniFile_H
#define CIniFile_H

//#define _UNICODE 
//#include <tchar.h>

#include "stdafx.h"

// C++ Includes
#include <string>
#include <vector>

// C Includes
#include <stdlib.h>

#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048

using namespace std;

class CIniFile  
{
private:
  bool   caseInsensitive;
  CString path;
  struct key {
    vector<wstring> names;
    vector<wstring> values; 
    vector<wstring> comments;
  };
  vector<key>    keys; 
  vector<wstring> names; 
  vector<wstring> comments;
  wstring CheckCase( wstring s) const;

public:
  enum errors{ noID = -1};
  CIniFile( CString const iniPath = _T(""));
  ~CIniFile()  {
	WriteFile();
  }

  // Sets whether or not keynames and valuenames should be case sensitive.
  // The default is case insensitive.
  void CaseSensitive()                           {caseInsensitive = false;}
  void CaseInsensitive()                         {caseInsensitive = true;}

  // Sets path of ini file to read and write from.
  void Path(CString const newPath)                {path = newPath;}
  CString Path() const                            {return path;}
  void SetPath(CString const newPath)             {Path( newPath);}

  // Reads ini file specified using path.
  // Returns true if successful, false otherwise.
  bool ReadFile();
  
  // Writes data stored in class to ini file.
  bool WriteFile(); 
  
  // Deletes all stored ini data.
  void Erase();
  void Clear()                                   {Erase();}
  void Reset()                                   {Erase();}

  // Returns index of specified key, or noID if not found.
  long FindKey( wstring const keyname) const;

  // Returns index of specified value, in the specified key, or noID if not found.
  long FindValue( unsigned const keyID, wstring const valuename) const;

  // Returns number of keys currently in the ini.
  size_t NumKeys() const                       {return names.size();}
  size_t GetNumKeys() const                    {return NumKeys();}

  // Add a key name.
  size_t AddKeyName( wstring const keyname);

  // Returns key names by index.
  wstring KeyName( unsigned const keyID) const;
  wstring GetKeyName( unsigned const keyID) const {return KeyName(keyID);}

  // Returns number of values stored for specified key.
  size_t NumValues( unsigned const keyID);
  size_t GetNumValues( unsigned const keyID)   {return NumValues( keyID);}
  size_t NumValues( wstring const keyname);
  size_t GetNumValues( wstring const keyname)   {return NumValues( keyname);}

  // Returns value name by index for a given keyname or keyID.
  wstring ValueName( unsigned const keyID, unsigned const valueID) const;
  wstring GetValueName( unsigned const keyID, unsigned const valueID) const {
    return ValueName( keyID, valueID);
  }
  wstring ValueName( wstring const keyname, unsigned const valueID) const;
  wstring GetValueName( wstring const keyname, unsigned const valueID) const {
    return ValueName( keyname, valueID);
  }

  // Gets value of [keyname] valuename =.
  // Overloaded to return wstring, int, and double.
  // Returns defValue if key/value not found.
  wstring GetValue( unsigned const keyID, unsigned const valueID, wstring const defValue = L"") const;
  wstring GetValue(wstring const keyname, wstring const valuename, wstring const defValue = L"") const; 
  int    GetValueI(wstring const keyname, wstring const valuename, int const defValue = 0) const;
  bool   GetValueB(wstring const keyname, wstring const valuename, bool const defValue = false) const {
    return GetValueI( keyname, valuename, int( defValue)) != 0;
  }
  double   GetValueF(wstring const keyname, wstring const valuename, double const defValue = 0.0) const;
  // This is a variable length formatted GetValue routine. All these voids
  // are required because there is no vsscanf() like there is a vsprintf().
  // Only a maximum of 8 variable can be read.
/*  unsigned GetValueV( wstring const keyname, wstring const valuename, char *format,
		      void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
  		      void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
  		      void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
  		      void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);
*/
  // Sets value of [keyname] valuename =.
  // Specify the optional paramter as false (0) if you do not want it to create
  // the key if it doesn't exist. Returns true if data entered, false otherwise.
  // Overloaded to accept string, int, and double.
  bool SetValue( wstring const keyname, wstring const valuename, wstring const value, bool const create = true);
  bool SetValueI( wstring const keyname, wstring const valuename, int const value, bool const create = true);
  bool SetValueB( wstring const keyname, wstring const valuename, bool const value, bool const create = true) {
    return SetValueI( keyname, valuename, int(value), create);
  }
  bool SetValueF( wstring const keyname, wstring const valuename, double const value, bool const create = true);
  bool SetValueV( wstring const keyname, wstring const valuename, TCHAR *format, ...);
  bool SetValue( unsigned const keyID, unsigned const valueID, wstring const value);

  // Deletes specified value.
  // Returns true if value existed and deleted, false otherwise.
  bool DeleteValue( wstring const keyname, wstring const valuename);
  
  // Deletes specified key and all values contained within.
  // Returns true if key existed and deleted, false otherwise.
  bool DeleteKey(wstring keyname);

  // Header comment functions.
  // Header comments are those comments before the first key.
  //
  // Number of header comments.
  size_t NumHeaderComments()                  {return comments.size();}
  // Add a header comment.
  void     HeaderComment( wstring const comment);
  // Return a header comment.
  wstring   HeaderComment( unsigned const commentID) const;
  // Delete a header comment.
  bool     DeleteHeaderComment( unsigned commentID);
  // Delete all header comments.
  void     DeleteHeaderComments()               {comments.clear();}

  // Key comment functions.
  // Key comments are those comments within a key. Any comments
  // defined within value names will be added to this list. Therefore,
  // these comments will be moved to the top of the key definition when
  // the CIniFile::WriteFile() is called.
  //
  // Number of key comments.
  size_t NumKeyComments( unsigned const keyID) const;
  size_t NumKeyComments( wstring const keyname) const;
  // Add a key comment.
  bool     KeyComment( unsigned const keyID, wstring const comment);
  bool     KeyComment( wstring const keyname, wstring const comment);
  // Return a key comment.
  wstring   KeyComment( unsigned const keyID, unsigned const commentID) const;
  wstring   KeyComment( wstring const keyname, unsigned const commentID) const;
  // Delete a key comment.
  bool     DeleteKeyComment( unsigned const keyID, unsigned const commentID);
  bool     DeleteKeyComment( wstring const keyname, unsigned const commentID);
  // Delete all comments for a key.
  bool     DeleteKeyComments( unsigned const keyID);
  bool     DeleteKeyComments( wstring const keyname);
};

#endif
