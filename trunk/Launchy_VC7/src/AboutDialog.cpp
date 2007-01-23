// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "AboutDialog.h"
#include ".\aboutdialog.h"
#include "ExeLauncher.h"

// AboutDialog dialog

IMPLEMENT_DYNAMIC(AboutDialog, CDialog)
AboutDialog::AboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDialog::IDD, pParent)
	, fVersion(LAUNCHY_VERSION_STRING)
{
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, fVersion);
}


BEGIN_MESSAGE_MAP(AboutDialog, CDialog)
	ON_STN_CLICKED(IDC_STATIC2, OnStnClickedStatic2)
	ON_STN_CLICKED(IDC_STATIC4, OnStnClickedStatic4)
	ON_STN_CLICKED(IDC_STATIC7, OnStnClickedStatic7)
END_MESSAGE_MAP()


// AboutDialog message handlers

BOOL AboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AboutDialog::OnStnClickedStatic2()
{
	ExeLauncher exe;
	exe.Run(_T("http://www.launchy.net"));
}

void AboutDialog::OnStnClickedStatic4()
{
	ExeLauncher exe;
	exe.Run(_T("http://cs.unm.edu/~karlinjf/"));
}

void AboutDialog::OnStnClickedStatic7()
{
	ExeLauncher exe;
	exe.Run(_T("http://www.launchy.net/#donate"));
}
