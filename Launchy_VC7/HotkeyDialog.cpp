// HotkeyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "HotkeyDialog.h"
#include "LaunchyDlg.h"

// CHotkeyDialog dialog

IMPLEMENT_DYNAMIC(CHotkeyDialog, CDialog)

CHotkeyDialog::CHotkeyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHotkeyDialog::IDD, pParent)
{
}

BOOL CHotkeyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	modifier.InsertString(0,_T("CONTROL"));
	modifier.InsertString(0,_T("SHIFT"));
	modifier.InsertString(0,_T("WIN"));
	modifier.InsertString(0,_T("ALT"));

	modifier.SetCurSel(0);

	for(int i='Z';i>='A';i--)
		vchar.InsertString(0,(CString)(char)i);

	vchar.InsertString(0,_T("F1"));
	vchar.InsertString(0,_T("F2"));
	vchar.InsertString(0,_T("F3"));
	vchar.InsertString(0,_T("F4"));
	vchar.InsertString(0,_T("F5"));
	vchar.InsertString(0,_T("F6"));
	vchar.InsertString(0,_T("F7"));
	vchar.InsertString(0,_T("F8"));
	vchar.InsertString(0,_T("F9"));
	vchar.InsertString(0,_T("F10"));
	vchar.InsertString(0,_T("Esc"));
	vchar.InsertString(0,_T("Space"));


	vchar.SetCurSel(0);

	return TRUE;
}


CHotkeyDialog::~CHotkeyDialog()
{
}

void CHotkeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOD_COMBO, modifier);
	DDX_Control(pDX, IDC_KEY_COMBO, vchar);
}


BEGIN_MESSAGE_MAP(CHotkeyDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_MOD_COMBO, &CHotkeyDialog::OnCbnSelchangeModCombo)
	ON_BN_CLICKED(IDOK, &CHotkeyDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CHotkeyDialog message handlers


void CHotkeyDialog::OnCbnSelchangeModCombo()
{
	// TODO: Add your control notification handler code here
}

void CHotkeyDialog::OnBnClickedOk()
{
	UINT mod, vkey;
	CString modifierstring;
	modifier.GetLBText(modifier.GetCurSel(),modifierstring);
	
	if(modifierstring==_T("ALT"))
		mod=MOD_ALT;
	else if (modifierstring==_T("CONTROL"))
		mod=MOD_CONTROL;
	else if(modifierstring==_T("SHIFT"))
		mod=MOD_SHIFT;
	else if(modifierstring==_T("WIN"))
		mod=MOD_WIN;

	CString vkeystr;
	vchar.GetLBText(vchar.GetCurSel(),vkeystr);

	if (vkeystr == _T("Space")) 
		vkey = VK_SPACE;
	else if (vkeystr == _T("Esc"))
		vkey = VK_ESCAPE;
	else if (vkeystr == _T("F1"))
		vkey = VK_F1;
	else if (vkeystr == _T("F2"))
		vkey = VK_F2;
	else if (vkeystr == _T("F3"))
		vkey = VK_F3;
	else if (vkeystr == _T("F4"))
		vkey = VK_F4;
	else if (vkeystr == _T("F5"))
		vkey = VK_F5;
	else if (vkeystr == _T("F6"))
		vkey = VK_F6;
	else if (vkeystr == _T("F7"))
		vkey = VK_F7;
	else if (vkeystr == _T("F8"))
		vkey = VK_F8;
	else if (vkeystr == _T("F9"))
		vkey = VK_F9;
	else if (vkeystr == _T("F10"))
		vkey = VK_F10;
	else {
		vkey = vkeystr[0];
	}
	UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), 100);
	BOOL m_isKeyRegistered = RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), 100,
	mod, vkey);

	CLaunchyDlg* main_dlg = (CLaunchyDlg*) AfxGetMainWnd();

	if (!m_isKeyRegistered) {
		AfxMessageBox(_T("Sorry, this hotkey will not work, please choose another"));
		// ReRegister the old hot key
		UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), 100);
		BOOL m_isKeyRegistered = RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), 100,
		main_dlg->options->mod_key, main_dlg->options->vkey);
	}
	else {
		// Save the new hot key to ini
		main_dlg->options->vkey = vkey;
		main_dlg->options->mod_key = mod;
		OnOK();
	}
}
