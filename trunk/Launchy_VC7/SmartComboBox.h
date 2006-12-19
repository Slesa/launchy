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

#include "stdafx.h"
#include "ReadOnlyEdit.h"
//#include "TypeEdit.h"
//#include "OpaqueListBox.h"
//#include "LListBox.h"
//#include "ColorCombo.h"

// SmartComboBox

#define WM_LAUNCHY_MOUSE_SELECT (LAUNCHY_DB_DONE + 1)
#define WM_CHANGE_COMBO_SEL (WM_LAUNCHY_MOUSE_SELECT + 1)

struct DropItem {
	HICON icon;
	CString longpath;
	CString lesspath;

	~DropItem() {
		if (icon != NULL) {
			DestroyIcon(icon);
		}
	}
};

class SmartComboBox : public CComboBox
{
	DECLARE_DYNAMIC(SmartComboBox)

public:
	SmartComboBox();
	virtual ~SmartComboBox();
private:
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
    CEdit      m_edit;
    CListBox   m_listbox;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnCbnEditupdate();
//	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnEditchange();
	CString typed;
	CString searchPath;
	afx_msg void OnCbnSelchange();
	afx_msg void OnDrawSelchange(int id);
	void SetSmallFont(CFont* font, COLORREF rgb);
	COLORREF m_FontSmallRGB;
	CFont* m_FontSmall;
	int cloneSelect;

	bool m_RemoveFrame;
	bool m_RemoveButton;
	bool m_Transparent;
	afx_msg void OnCbnDropdown();
private:
	//text and text background colors
	COLORREF m_crText;
	COLORREF m_crBackGnd;
	//background brush
	CBrush m_brBackGnd;
	afx_msg void OnPaint();
	LRESULT AfterSelChange(UINT wParam, LONG lParam);
	void ParseSearchTxt();
public:
	void CleanText(void);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	void DoSubclass(void);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


