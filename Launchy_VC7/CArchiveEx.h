#if !defined(_CARCHIVE_H_)
#define _CARCHIVE_H_

struct ArcHeader
{
	_TCHAR			chSignature[3];		//	"akz"
	BYTE			uchFlag;			//	0-plane text; 1-compr; 2-crypt; 3-compr & crypt;
	DWORD			dwOriginalSize;		//	decompr size
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