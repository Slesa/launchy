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

// DirectoryChooser.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "DirectoryChooser.h"
#include "Options.h"
#include "LaunchyDlg.h"
#include ".\directorychooser.h"

// DirectoryChooser dialog

IMPLEMENT_DYNAMIC(DirectoryChooser, CDialog)

DirectoryChooser::DirectoryChooser(CWnd* pParent /*=NULL*/)
	: CDialog(DirectoryChooser::IDD, pParent)
{

}

DirectoryChooser::~DirectoryChooser()
{
}

void DirectoryChooser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Directories);
	DDX_Control(pDX, IDC_LIST2, Types);
	DDX_Control(pDX, IDC_EDIT1, TypeEdit);
	DDX_Control(pDX, IDC_INDEXED, numFiles);
	DDX_Control(pDX, IDC_INDEX_NAMES, IndexNames);
}


BEGIN_MESSAGE_MAP(DirectoryChooser, CDialog)
	ON_BN_CLICKED(IDC_ADD_DIRECTORY, &DirectoryChooser::OnBnClickedAddDirectory)
	ON_BN_CLICKED(IDC_REMOVE_DIRECTORY, &DirectoryChooser::OnBnClickedRemoveDirectory)
	ON_BN_CLICKED(IDC_DEFAULT_DIRECTORY, &DirectoryChooser::OnBnClickedDefaultDirectory)
	ON_BN_CLICKED(IDC_ADD_TYPE, &DirectoryChooser::OnBnClickedAddType)
	ON_BN_CLICKED(IDC_REMOVE_TYPE, &DirectoryChooser::OnBnClickedRemoveType)
	ON_BN_CLICKED(IDOK, &DirectoryChooser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DirectoryChooser::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DEFAULT_TYPE, OnBnClickedDefaultType)
	ON_BN_CLICKED(IDC_INDEX_NAMES, &DirectoryChooser::OnBnClickedDirIndex)
END_MESSAGE_MAP()

void SetWidthByContentInListBox(CDialog* pDlg, int nID)
{
if (nID <= 0|| pDlg == NULL) return;
CWnd* pWnd = (CWnd*)pDlg;

CListBox* pCB = (CListBox*)pWnd->GetDlgItem(nID);
if (pCB == NULL) return;

int origSize = pCB->GetHorizontalExtent();
int maxSize = origSize;

CDC* pDC = pWnd->GetDC();
CString cStr;
int str_num = pCB->GetCount();
for (int i = 0; i < str_num; i++)
{
pCB->GetText(i, cStr);

CSize strSize = pDC->GetOutputTextExtent(cStr);
if (strSize.cx > maxSize)
maxSize = strSize.cx;
}

// in case when adding to list, existing items may be
// longer than newer ones, so may not want to resize
if (maxSize > origSize)
pCB->SetHorizontalExtent(maxSize);
}

// DirectoryChooser message handlers

void DirectoryChooser::OnBnClickedAddDirectory()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	// TODO: Add your control notification handler code here
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory");
	bi.hwndOwner = GetSafeHwnd();
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList ( pidl, path ) )
        {
            // ( "Selected Folder: %s\n", path );
			ops->Directories.push_back(path);
			Directories.AddString(path);
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
	SetWidthByContentInListBox(this, IDC_LIST1);
}

void DirectoryChooser::OnBnClickedRemoveDirectory()
{
	CString name;
	int item = Directories.GetCurSel();
	if (item < 0) {
		return;
	}
	Directories.GetText(Directories.GetCurSel(), name);
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	for(vector<CString>::iterator it = ops->Directories.begin(); it != ops->Directories.end(); ) {
		if (*it == name) {
			ops->Directories.erase(it);
		} else {
			it++;
		}
	}

	Directories.DeleteString(item);
	SetWidthByContentInListBox(this, IDC_LIST1);
}
void DirectoryChooser::OnBnClickedDefaultDirectory()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
//	for(vector<CString>::iterator it = ops->Directories.begin(); it != ops->Directories.end(); ) {
//		ops->Directories.erase(it);
//	}
	ops->Directories.clear();
	Directories.ResetContent();

	CString myMenu, allMenus;
	LaunchySmarts::GetShellDir(CSIDL_COMMON_STARTMENU, allMenus);
	LaunchySmarts::GetShellDir(CSIDL_STARTMENU, myMenu);

	ops->Directories.push_back(allMenus);
	ops->Directories.push_back(myMenu);

	Directories.AddString(allMenus);
	Directories.AddString(myMenu);


	SetWidthByContentInListBox(this, IDC_LIST1);
}

void DirectoryChooser::OnBnClickedAddType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	CString txt;
	TypeEdit.GetWindowTextW(txt);
	if (txt == "")
		return;
	txt.MakeLower();
	ops->Types.push_back(txt);
	Types.AddString(txt);
	TypeEdit.SetWindowTextW(_T(""));
	SetWidthByContentInListBox(this, IDC_LIST2);
}

void DirectoryChooser::OnBnClickedDefaultType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	ops->Types.clear();
	Types.ResetContent();
	ops->Types.push_back(_T(".lnk"));
	Types.AddString(_T(".lnk"));
}

void DirectoryChooser::OnBnClickedRemoveType()
{
	CString name;
	int item = Types.GetCurSel();
	if (item < 0) {
		return;
	}
	Types.GetText(Types.GetCurSel(), name);
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	for(vector<CString>::iterator it = ops->Types.begin(); it != ops->Types.end(); ) {
		if (*it == name) {
			ops->Types.erase(it);
		} else {
			it++;
		}
	}

	Types.DeleteString(item);
	SetWidthByContentInListBox(this, IDC_LIST2);
}

BOOL DirectoryChooser::OnInitDialog()
{
	CDialog::OnInitDialog();

	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	shared_ptr<LaunchySmarts> smarts = ((CLaunchyDlg*)AfxGetMainWnd())->smarts;

	CString d;
	d.Format(_T("Indexing %d files"), smarts->catFiles);
	numFiles.SetWindowTextW(d);

	for(uint i = 0; i < ops->Directories.size(); i++) {
		Directories.AddString(ops->Directories[i]);
	}

	for(uint i = 0; i < ops->Types.size(); i++) {
		Types.AddString(ops->Types[i]);
		if (ops->Types[i] == _T(".directory")) {
			IndexNames.SetCheck(BST_CHECKED);
		}
	}

	dbak = ops->Directories;
	tbak = ops->Types; 

	SetWidthByContentInListBox(this, IDC_LIST1);
	SetWidthByContentInListBox(this, IDC_LIST2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DirectoryChooser::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	shared_ptr<LaunchySmarts> smarts = ((CLaunchyDlg*)AfxGetMainWnd())->smarts;
//	smarts->LoadCatalog();
	OnOK();
}


void DirectoryChooser::OnBnClickedCancel()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	ops->Directories = dbak;
	ops->Types = tbak;

	// TODO: Add your control notification handler code here
	OnCancel();
}

void DirectoryChooser::OnBnClickedDirIndex()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	if (IndexNames.GetCheck() == BST_CHECKED) {
		ops->Types.push_back(_T(".directory"));
		Types.AddString(_T(".directory"));
	} else {
		for(vector<CString>::iterator it = ops->Types.begin(); it != ops->Types.end(); ) {
			if (*it == _T(".directory")) {
				ops->Types.erase(it);
			} else {
				it++;
			}
		}
		int id = Types.FindStringExact(-1, _T(".directory"));
		if (id != -1) {
			Types.DeleteString(id);
		}
	}
	
	SetWidthByContentInListBox(this, IDC_LIST2);
}
