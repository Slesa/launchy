// AdvancedOptions.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "AdvancedOptions.h"
#include "LaunchyDlg.h"
#include ".\advancedoptions.h"
#include "Options.h"


// AdvancedOptions dialog

IMPLEMENT_DYNAMIC(AdvancedOptions, CDialog)
AdvancedOptions::AdvancedOptions(CWnd* pParent /*=NULL*/)
	: CDialog(AdvancedOptions::IDD, pParent)
	, i_listLength(0)
{
}

AdvancedOptions::~AdvancedOptions()
{
}

void AdvancedOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADV_USB, cb_usb);
	DDX_Control(pDX, IDC_CHECK1, cb_sticky);
	DDX_Text(pDX, IDC_EDIT1, i_listLength);
}


BEGIN_MESSAGE_MAP(AdvancedOptions, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()




BOOL AdvancedOptions::OnInitDialog()
{
	CDialog::OnInitDialog();
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	
	if (ops->usbmode) 
		cb_usb.SetCheck(BST_CHECKED);
	else
		cb_usb.SetCheck(BST_UNCHECKED);

	if (ops->stickyWindow) 
		cb_sticky.SetCheck(BST_CHECKED);
	else
		cb_sticky.SetCheck(BST_UNCHECKED);


	i_listLength = ops->listLength;
	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AdvancedOptions::OnBnClickedOk()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	UpdateData(1);
	ops->listLength = i_listLength;
	
	// Are we transitioning into usb mode?
	if (!ops->usbmode && cb_usb.GetCheck()) {
		ops->changeUsbMode(true);		
	}

	// Are we transitioning out of usb mode?
	if (ops->usbmode && !cb_usb.GetCheck()) {
		ops->changeUsbMode(false);
	}

	ops->stickyWindow = cb_sticky.GetCheck();
	OnOK();
}
