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
#include "TypeEdit.h"
#include "OpaqueListBox.h"
//#include "ColorCombo.h"

// SmartComboBox

class SmartComboBox : public CComboBox
{
	DECLARE_DYNAMIC(SmartComboBox)

public:
	SmartComboBox();
	virtual ~SmartComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
    CTypeEdit      m_edit;
    CListBox   m_listbox;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnCbnEditupdate();
//	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnEditchange();
	CString typed;
	afx_msg void OnCbnSelchange();

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
public:
	void CleanText(void);
};


