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

// UTF16File.cpp: implementation of the CUTF16File class.
//
// Version 5.0, 2 February 2004.
//
// Jeffrey Walton
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UTF16File.h"

#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUTF16File::CUTF16File(): CStdioFile(),
        m_bIsUnicode( FALSE ), m_bByteSwapped ( FALSE )
{
}

CUTF16File::CUTF16File(LPCTSTR lpszFileName,UINT nOpenFlags)
        :CStdioFile(lpszFileName, nOpenFlags), 
        m_bIsUnicode( FALSE ), m_bByteSwapped ( FALSE )
{

    WCHAR wcBOM = L'\0';

    // We only need the BOM check if reading.
    if( CFile::modeWrite == ( nOpenFlags & CFile::modeWrite ) ) { return; }

    // BOM is two bytes
    if( CFile::GetLength() < 2 ) { return; }

    CFile::Read( reinterpret_cast<LPVOID>( &wcBOM ), sizeof( wcBOM ) );

    if( wcBOM == UNICODE_BOM ) {

        m_bIsUnicode   = TRUE;
        m_bByteSwapped = FALSE;
    } 

    if( wcBOM == UNICODE_RBOM ) {

        m_bIsUnicode   = TRUE;
        m_bByteSwapped = TRUE;
    }

    // Not a BOM mark - its an ANSI file
    //   so punt to CStdioFile...
    if( FALSE == m_bIsUnicode ) {
       
        CStdioFile::Seek( 0, CFile::begin );
    }
}

BOOL CUTF16File::Open(LPCTSTR lpszFileName,UINT nOpenFlags,CFileException* pError /*=NULL*/)
{

	BOOL bResult = FALSE;

    WCHAR wcBOM = L'\0';

    bResult = CStdioFile::Open(lpszFileName, nOpenFlags, pError);

	if (!bResult) return bResult;

    // We only need the BOM check if reading.
    if( CFile::modeWrite == ( nOpenFlags & CFile::modeWrite ) ) { return bResult; }

    // BOM is two bytes
    if( CFile::GetLength() < 2 ) { return bResult; }

    if( TRUE == bResult ) {

        CStdioFile::Read( &wcBOM, sizeof( WCHAR ) );

        if( wcBOM == UNICODE_BOM ) {

            m_bIsUnicode   = TRUE;
            m_bByteSwapped = FALSE;
        }

        if( wcBOM == UNICODE_RBOM ) {

            m_bIsUnicode   = TRUE;
            m_bByteSwapped = TRUE;
        }

        // Not a BOM mark - its an ANSI file
        //   so punt to CStdioFile...
        if( FALSE == m_bIsUnicode ) {

            CStdioFile::Seek( 0, CFile::begin );
        }
    }

    return bResult;
}

BOOL CUTF16File::ReadString( CString& rString ) {

    if( TRUE == m_bIsUnicode ) {

        return ReadUnicodeString( rString );
    }

    return CStdioFile::ReadString( rString );
}

LPTSTR CUTF16File::ReadString( LPTSTR lpsz, UINT nMax ) {

    if( TRUE == m_bIsUnicode ) {

        return ReadUnicodeString( lpsz, nMax );
    }

    return CStdioFile::ReadString( lpsz, nMax );
}

BOOL CUTF16File::ReadUnicodeString( CString& rString ) {

    BOOL bRead = FALSE;

    WCHAR c = L'0';

    rString.Empty();

    LoadAccumulator();

    while( FALSE == m_Accumulator.empty() ) {

        bRead = TRUE;

        c = m_Accumulator.front();

        m_Accumulator.pop_front();

		// Added the check for '\r' since that's what windows does..
        if( L'\n' == c || L'\r' == c) { break; }

        rString += c;

        if( TRUE == m_Accumulator.empty() ) {
            
            LoadAccumulator();
        }
    }

    return bRead;;
}

/***
*char *fgets(string, count, stream) - input string from a stream
*
*Purpose:
*       get a string, up to count-1 chars or '\n', whichever comes first,
*       append '\0' and put the whole thing into string. the '\n' IS included
*       in the string. if count<=1 no input is requested. if EOF is found
*       immediately, return NULL. if EOF found after chars read, let EOF
*       finish the string as '\n' would.
*
***/

LPTSTR CUTF16File::ReadUnicodeString( LPWSTR lpsz, UINT nMax ) {
BOOL bRead = FALSE;
LPWSTR p = lpsz;
WCHAR c = L'\0';

	ASSERT(lpsz != NULL);
	ASSERT(AfxIsValidAddress(lpsz, nMax));
	ASSERT(m_pStream != NULL);

    if( nMax <= 1 ) { return lpsz; }

    LoadAccumulator();

    while( FALSE == m_Accumulator.empty() && --nMax ) {
        
        bRead = TRUE;        

        *p++ = c = m_Accumulator.front();

        m_Accumulator.pop_front();

        if( L'\n' == c ) { break; }

        if( TRUE == m_Accumulator.empty() ) {
            
            LoadAccumulator();
        }
    }

    *p = L'\0';

    return TRUE == bRead ? lpsz : NULL;
}

VOID CUTF16File::WriteString( LPCTSTR lpsz, BOOL bAsUnicode /*= FALSE */ )
{
    if( TRUE == bAsUnicode ) {

        WriteUnicodeString( lpsz );

    } else {

        WriteANSIString( lpsz );
    }
}

BOOL CUTF16File::LoadAccumulator()
{
    BYTE cbBuffer[ ACCUMULATOR_CHAR_COUNT * sizeof( WCHAR ) ];

    UINT uCount = CStdioFile::Read( cbBuffer, ACCUMULATOR_CHAR_COUNT * sizeof( WCHAR ) );

    WCHAR* pwszBuffer = reinterpret_cast<WCHAR*>( cbBuffer );

    for( UINT i = 0; i < uCount / 2; i++ ) {

        WCHAR c = *pwszBuffer++;

        if( TRUE == m_bByteSwapped ) {

            BYTE b1 = c >> 8;   // high order
            BYTE b2 = c & 0xFF; // low order

            c = b1 | ( b2 << 8 );
        }

        m_Accumulator.push_back( c );
    }

    return 0 == uCount;
}

VOID CUTF16File::ClearAccumulator()
{
    m_Accumulator.clear();
}

ULONGLONG CUTF16File::Seek(LONG lOff, UINT nFrom)
{
    ULONGLONG lResult = CStdioFile::Seek( lOff, nFrom );

    ClearAccumulator();

    LoadAccumulator();

    // Should there be a test here to set fp = 2 if Unicode,
    //  and the user asks for CFile::begin???

    return lResult;
}

VOID CUTF16File::WriteANSIString( LPCWSTR lpsz )
{
	
    CStdioFile::WriteString( lpsz );
}

VOID CUTF16File::WriteUnicodeString( LPCWSTR lpsz )
{

    if( 0 == CFile::GetPosition() ) {

        WCHAR wcBOF = UNICODE_BOM;

        CFile::Write( static_cast<LPVOID>( &wcBOF ), sizeof( UNICODE_BOM ) );
    }

    CFile::Write( lpsz, (unsigned int) wcslen( lpsz ) * sizeof( WCHAR ) );
}

