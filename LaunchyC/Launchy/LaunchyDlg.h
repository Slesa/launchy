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

// LaunchyDlg.h : header file
//

#pragma once

#include "CDialogSK.h"
#include "afxwin.h"
#include "Options.h"
#include "smartcombobox.h"
#include "LaunchySmarts.h"
#include "Resource.h"
#include <boost/smart_ptr.hpp>

using namespace boost;

// CLaunchyDlg dialog
class CLaunchyDlg : public CDialogSK
{
// Construction
public:
	CLaunchyDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	enum { IDD = IDD_LAUNCHY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	CFont m_Font;
	HICON m_hIcon;
	BOOL atLaunch;
	BOOL Visible;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	
	scoped_ptr<LaunchySmarts> smarts;
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

	scoped_ptr<Options> options;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
//	afx_msg void OnCbnEditchangeInput();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCbnSelchangeInput();
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	SmartComboBox InputBox;
protected:
//	virtual void OnCancel();
//	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit Preview;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	unsigned int DelayTimer;
	afx_msg void OnEndSession(BOOL bEnding);
};