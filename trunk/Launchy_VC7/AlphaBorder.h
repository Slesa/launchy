#pragma once

#include "atlimage.h"
#include "resource.h"
// AlphaBorder dialog

class AlphaBorder : public CDialog
{
	DECLARE_DYNAMIC(AlphaBorder)

public:
	AlphaBorder(CWnd* pParent = NULL);   // standard constructor
	virtual ~AlphaBorder();

// Dialog Data
	enum { IDD = IDD_ALPHA_BORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CImage image;
	CBitmap* pBitmap;
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetImage(CString name);
};
