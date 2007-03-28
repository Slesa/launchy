#pragma once
#include "afxwin.h"


// CPluginDialog dialog

class CPluginDialog : public CDialog
{
	DECLARE_DYNAMIC(CPluginDialog)

public:
	CPluginDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPluginDialog();

// Dialog Data
	enum { IDD = IDD_PLUGINS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CCheckListBox  PluginList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOptions();

	afx_msg void OnSelChange();
	CButton OptionsButton;
};
