#pragma once
#include "afxwin.h"


// AdvancedOptions dialog

class AdvancedOptions : public CDialog
{
	DECLARE_DYNAMIC(AdvancedOptions)

public:
	AdvancedOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~AdvancedOptions();

// Dialog Data
	enum { IDD = IDD_ADVANCED_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton cb_usb;
	CButton cb_sticky;
	virtual BOOL OnInitDialog();
//	int cs_listlength;
	afx_msg void OnBnClickedOk();
	int i_listLength;
};
