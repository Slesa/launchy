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
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	bool inuse;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
