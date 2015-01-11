#pragma once
#include "afxwin.h"


// SkinChooser dialog

class SkinChooser : public CDialog
{
	DECLARE_DYNAMIC(SkinChooser)

public:
	SkinChooser(CWnd* pParent = NULL);   // standard constructor
	virtual ~SkinChooser();

// Dialog Data
	enum { IDD = IDD_SKIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog(void);
public:
	CListBox skinlist;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};
