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

	// Select this skin
	for(uint i = 0; i < ops->skins.size(); i++) {
		if (ops->skins[i]->name == name) {
			ops->skin = ops->skins[i];
			ops->skinName = name;
		}
	}
	OnOK();
}

void SkinChooser::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
