#pragma once
#include "afxwin.h"


// CHotkeyDialog dialog

class CHotkeyDialog : public CDialog
{
	DECLARE_DYNAMIC(CHotkeyDialog)

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
