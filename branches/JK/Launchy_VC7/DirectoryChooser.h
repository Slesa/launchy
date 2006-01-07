#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;

// DirectoryChooser dialog

class DirectoryChooser : public CDialog
{
	DECLARE_DYNAMIC(DirectoryChooser)
private:
	vector<CString> tbak;
	vector<CString> dbak;


public:
	DirectoryChooser(CWnd* pParent = NULL);   // standard constructor
	virtual ~DirectoryChooser();

// Dialog Data
	enum { IDD = IDD_DIRECTORIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CListBox Directories;
public:
	CListBox Types;
public:
	afx_msg void OnBnClickedAddDirectory();
public:
	afx_msg void OnBnClickedRemoveDirectory();
public:
	afx_msg void OnBnClickedAddType();
public:
	afx_msg void OnBnClickedRemoveType();
public:
	virtual BOOL OnInitDialog();
public:
	CEdit TypeEdit;
public:
	afx_msg void OnBnClickedOk();

public:
	afx_msg void OnBnClickedCancel();
public:
	CEdit numFiles;
};
