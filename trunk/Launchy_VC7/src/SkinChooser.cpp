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

// SkinChooser.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "SkinChooser.h"
#include "Options.h"
#include "LaunchyDlg.h"

// SkinChooser dialog

IMPLEMENT_DYNAMIC(SkinChooser, CDialog)

SkinChooser::SkinChooser(CWnd* pParent /*=NULL*/)
	: CDialog(SkinChooser::IDD, pParent)
{

}

SkinChooser::~SkinChooser()
{
}

void SkinChooser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, skinlist);
}


BEGIN_MESSAGE_MAP(SkinChooser, CDialog)
	ON_BN_CLICKED(IDOK, &SkinChooser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SkinChooser::OnBnClickedCancel)
END_MESSAGE_MAP()




BOOL SkinChooser::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	for(uint i = 0; i < ops->skins.size(); i++) {
		skinlist.AddString(ops->skins[i]->name);
//		AfxMessageBox(ops->skins[i]->name);
		
	}

	return TRUE;
	
}

void SkinChooser::OnBnClickedOk()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	CString name;
	int item = skinlist.GetCurSel();
	if (item < 0) {
		OnOK();
		return;
	}
	skinlist.GetText(skinlist.GetCurSel(), name);

	ops->SetSkin(name);
	OnOK();
}

void SkinChooser::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
