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

// UTF16File.h: interface for the CUTF16File class.
//
// Version 5.0, 2 February 2004.
//
// Jeffrey Walton
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTF16File_H__32BEF8AC_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
#define AFX_UTF16File_H__32BEF8AC_25E0_482F_8B00_C40775BCDB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

//
// Under a hex editor, file[0] = 0xFF
//                     file[1] = 0xFE
//
// for a proper UTF-16 BOM
//
// This is different than the in-memory
//   representation of: mem[0] = 0xFE
//                      mem[1] = 0xFF
//
// on an Intel CPU
//
const WCHAR UNICODE_BOM             = 0xFEFF;
const WCHAR UNICODE_RBOM            = 0xFFFE;

const INT ACCUMULATOR_CHAR_COUNT    = 2048;

class CUTF16File: public CStdioFile
{
public:
	
	CUTF16File();
	CUTF16File( LPCTSTR lpszFileName, UINT nOpenFlags );

	virtual BOOL	Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );
	virtual BOOL	ReadString(CString& rString);
    virtual LPTSTR  ReadString( LPTSTR lpsz, UINT nMax );
	virtual VOID	WriteString( LPCTSTR lpsz, BOOL bAsUnicode = FALSE );

    virtual ULONGLONG    Seek( LONG lOff, UINT nFrom );

    BOOL            IsUnicodeFile( ) { return m_bIsUnicode; }

protected:

	BOOL            ReadUnicodeString( CString& szString );
    LPTSTR          ReadUnicodeString( LPWSTR lpsz, UINT nMax );

    virtual VOID    WriteANSIString( LPCWSTR lpsz );
    virtual VOID    WriteUnicodeString( LPCWSTR lpsz );

	BOOL m_bIsUnicode;
    BOOL m_bByteSwapped;

private:

	BOOL LoadAccumulator();
    VOID ClearAccumulator();

    std::list< WCHAR > m_Accumulator;
};

#endif // !defined(AFX_UTF16File_H__32BEF8AC_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
