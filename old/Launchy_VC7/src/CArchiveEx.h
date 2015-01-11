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

#if !defined(_CARCHIVE_H_)
#define _CARCHIVE_H_

struct ArcHeader
{
	_TCHAR			chSignature[3];		//	"akz"
	BYTE			uchFlag;			//	0-plane text; 1-compr; 2-crypt; 3-compr & crypt;
	ULONGLONG			dwOriginalSize;		//	decompr size
	ULONG			ulCRC;				//	CRC
};

class CArchiveExt : public CArchive
{
	CString m_strKey;
	BOOL	m_bCompress;
	CFile*	m_pFileTmp;
	BYTE*	m_pBuf;

public:
	CArchiveExt(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL, CString Key = _TEXT(""), BOOL bCompress = FALSE);
	virtual ~CArchiveExt();

	virtual void Close();
	virtual void Abort();

protected:
	virtual bool Crypto(LPSTR lpstrBuffer, ULONG ulLen, CString strKey = _TEXT(""));
};

#endif