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
#include "afxwin.h"


// CHotkeyDialog dialog

class CHotkeyDialog : public CDialog
{
	DECLARE_DYNAMIC(CHotkeyDialog)
private:
	uint szKeyToIdent(CString vkeystr);
	CString identToKeyString(uint vkeystr);

public:
	CHotkeyDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotkeyDialog();
	BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_HOTKEY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnCbnSelchangeModCombo();
public:
	CComboBox modifier;
public:
	CComboBox vchar;
public:
	afx_msg void OnBnClickedOk();
};
