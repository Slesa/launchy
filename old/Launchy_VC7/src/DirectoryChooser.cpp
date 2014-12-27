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
//#include "Options.h"
#include "LaunchyDlg.h"
#include ".\directorychooser.h"

// DirectoryChooser dialog

IMPLEMENT_DYNAMIC(DirectoryChooser, CDialog)

DirectoryChooser::DirectoryChooser(CWnd* pParent /*=NULL*/)
: CDialog(DirectoryChooser::IDD, pParent)
, szLocalType(_T(""))
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
	DDX_Control(pDX, IDC_LIST3, LocalTypes);
	DDX_Text(pDX, IDC_EDIT3, szLocalType);
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
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_ADD_LOCAL_TYPE, OnBnClickedAddLocalType)
	ON_BN_CLICKED(IDC_REMOVE_LOCAL_TYPE, OnBnClickedRemoveLocalType)
	ON_BN_CLICKED(IDC_DEFAULT_LOCAL_TYPE, OnBnClickedDefaultLocalType)
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

	vector<DirOptions> D = ops->get_Directories();

	if ( pidl != 0 )
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if ( SHGetPathFromIDList ( pidl, path ) )
		{
			// ( "Selected Folder: %s\n", path );
			DirOptions d;
			d.dir = path;
			D.push_back(d);
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

	ops->set_Directories(D);
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

	vector<DirOptions> D = ops->get_Directories();
	for(vector<DirOptions>::iterator it = D.begin(); it != D.end(); ) {
		if (it->dir == name) {
			D.erase(it);
		} else {
			it++;
		}
	}

	ops->set_Directories(D);
	Directories.DeleteString(item);
	SetWidthByContentInListBox(this, IDC_LIST1);
}
void DirectoryChooser::OnBnClickedDefaultDirectory()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	vector<DirOptions> D;
	Directories.ResetContent();

	CString myMenu, allMenus;
	LaunchySmarts::GetShellDir(CSIDL_COMMON_STARTMENU, allMenus);
	LaunchySmarts::GetShellDir(CSIDL_STARTMENU, myMenu);

	D.push_back(DirOptions(allMenus));
	D.push_back(DirOptions(myMenu));
	D.push_back(DirOptions(_T("utilities\\")));

	ops->set_Directories(D);

	Directories.AddString(allMenus);
	Directories.AddString(myMenu);
	Directories.AddString(_T("utilities\\"));

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

	vector<CString> T = ops->get_Types();

	T.push_back(txt);
	Types.AddString(txt);
	TypeEdit.SetWindowTextW(_T(""));
	SetWidthByContentInListBox(this, IDC_LIST2);
	ops->set_Types(T);
}

void DirectoryChooser::OnBnClickedDefaultType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	vector<CString> T;
	Types.ResetContent();
	T.push_back(_T(".lnk"));
	Types.AddString(_T(".lnk"));
	ops->set_Types(T);
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

	vector<CString> T = ops->get_Types();
	for(vector<CString>::iterator it = T.begin(); it != T.end(); ) {
		if (*it == name) {
			T.erase(it);
		} else {
			it++;
		}
	}

	ops->set_Types(T);
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

	vector<DirOptions> D = ops->get_Directories();
	vector<CString> T = ops->get_Types();

	for(uint i = 0; i < D.size(); i++) {
		Directories.AddString(D[i].dir);
	}

	for(uint i = 0; i < T.size(); i++) {
		Types.AddString(T[i]);
	}

	dbak = D;
	tbak = T; 

	SetWidthByContentInListBox(this, IDC_LIST1);
	SetWidthByContentInListBox(this, IDC_LIST2);
	if (Directories.GetCount() > 0) {
		Directories.SetCurSel(0);
		OnLbnSelchangeList1();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DirectoryChooser::OnBnClickedOk()
{
	OnOK();
}


void DirectoryChooser::OnBnClickedCancel()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	ops->set_Directories(dbak);
	ops->set_Types(tbak);

	// TODO: Add your control notification handler code here
	OnCancel();
}




void DirectoryChooser::OnLbnSelchangeList1()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	vector<DirOptions> D = ops->get_Directories();

	CString name;
	Directories.GetText(Directories.GetCurSel(), name);

	LocalTypes.ResetContent();
	for(uint i = 0; i < D.size(); i++) {
		if (D[i].dir == name) {
			for (uint j = 0; j < D[i].types.size(); j++) {
				LocalTypes.AddString(D[i].types[j]);
			}
			break;
		}
	}
}

void DirectoryChooser::OnBnClickedAddLocalType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	UpdateData(1);
	LocalTypes.AddString(szLocalType);
	vector<DirOptions> D = ops->get_Directories();

	CString name;
	Directories.GetText(Directories.GetCurSel(), name);

	for(uint i = 0; i < D.size(); i++) {
		if (D[i].dir == name) {
			D[i].types.push_back(szLocalType);
			break;
		}
	}

	ops->set_Directories(D);

	szLocalType = _T("");
	UpdateData(0);
	SetWidthByContentInListBox(this, IDC_LIST3);
}

void DirectoryChooser::OnBnClickedRemoveLocalType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	CString name;
	int item = LocalTypes.GetCurSel();
	if (item < 0) {
		return;
	}
	LocalTypes.GetText(LocalTypes.GetCurSel(), name);

	LocalTypes.DeleteString(item);
	vector<DirOptions> D = ops->get_Directories();

	CString dirname;
	Directories.GetText(Directories.GetCurSel(), dirname);

	for(uint i = 0; i < D.size(); i++) {
		if (D[i].dir == dirname) {
			for(vector<CString>::iterator it = D[i].types.begin(); it != D[i].types.end(); ) {
				if (*it == name) {
					D[i].types.erase(it);
				} else {
					it++;
				}
			}
			break;
		}
	}

	ops->set_Directories(D);
	SetWidthByContentInListBox(this, IDC_LIST3);
}


void DirectoryChooser::OnBnClickedDefaultLocalType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	vector<DirOptions> D = ops->get_Directories();

	CString dirname;
	Directories.GetText(Directories.GetCurSel(), dirname);

	LocalTypes.ResetContent();

	for(uint i = 0; i < D.size(); i++) {
		if (D[i].dir == dirname) {
			D[i].types.clear();
			break;
		}
	}
	ops->set_Directories(D);
}
