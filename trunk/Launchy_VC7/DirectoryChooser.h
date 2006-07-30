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
#include <vector>
#include "Options.h"

using namespace std;

// DirectoryChooser dialog

class DirectoryChooser : public CDialog
{
	DECLARE_DYNAMIC(DirectoryChooser)
private:
	vector<CString> tbak;
	vector<DirOptions> dbak;


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
	afx_msg void OnBnClickedDefaultDirectory();
public:
	afx_msg void OnBnClickedAddType();
public:
	afx_msg void OnBnClickedRemoveType();
	afx_msg void OnBnClickedDirIndex();
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
	afx_msg void OnBnClickedDefaultType();
	afx_msg void OnBnDoubleclickedOk();
	afx_msg void OnLbnSelchangeList1();
	CListBox LocalTypes;
	afx_msg void OnBnClickedAddLocalType();
	afx_msg void OnBnClickedRemoveLocalType();
	afx_msg void OnBnClickedDefaultLocalType();
	CString szLocalType;
};
