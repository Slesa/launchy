// PluginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "PluginDialog.h"
#include ".\plugindialog.h"
#include "LaunchyDlg.h"
#include "Plugin.h"
#include <map>

using namespace std;


// CPluginDialog dialog

IMPLEMENT_DYNAMIC(CPluginDialog, CDialog)
CPluginDialog::CPluginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginDialog::IDD, pParent)
{
}

CPluginDialog::~CPluginDialog()
{
}

void CPluginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGINLIST, PluginList);
	DDX_Control(pDX, IDC_OPTIONS, OptionsButton);
}


BEGIN_MESSAGE_MAP(CPluginDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPTIONS, OnBnClickedOptions)
	ON_LBN_SELCHANGE(IDC_PLUGINLIST, OnSelChange)
END_MESSAGE_MAP()


// CPluginDialog message handlers



BOOL CPluginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here


	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	if (plugins == NULL) return true;
	plugins->LoadDlls(false);
	PluginList.ResetContent();
	PluginList.SetCheckStyle( BS_AUTOCHECKBOX  );

	for(int i = 0; i < plugins->allPlugins.size(); i++) {
		CString str;
		str.Format(L" %s - %s", plugins->allPlugins[i].name, plugins->allPlugins[i].description);
		PluginList.AddString(str);
		PluginList.SetCheck(i, plugins->allPlugins[i].loaded);	
	}

	if (plugins->allPlugins.size() > 0)
		PluginList.SetSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPluginDialog::OnBnClickedOk()
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	if (plugins == NULL) return;

	// Tell the plugin manager of the 
	int changes = 0;
	for(int i = 0; i < PluginList.GetCount(); i++) {
		if (PluginList.GetCheck(i) != plugins->allPlugins[i].loaded)
			changes++;
		plugins->allPlugins[i].loaded = PluginList.GetCheck(i);
	}

	if (changes > 0) {

		AfxMessageBox(L"Launchy will now be restarted to apply the plugin changes you have made");

		// Load a new Launchy
		int numArgs;
		LPWSTR*  strings = CommandLineToArgvW(GetCommandLine(), &numArgs);


		SHELLEXECUTEINFO ShExecInfo;

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = strings[0];
		ShExecInfo.lpParameters = L"/wait";
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_NORMAL;
		ShExecInfo.hInstApp = NULL;

		BOOL ret = ShellExecuteEx(&ShExecInfo);	


	((CLaunchyDlg*)AfxGetMainWnd())->options->Store();
	((CLaunchyDlg*)AfxGetMainWnd())->EndDialog(1);
    //    ASSERT(AfxGetApp()->m_pMainWnd != NULL);
    //    AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE); 
	}
	OnOK();
}

void CPluginDialog::OnBnClickedOptions()
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	if (plugins == NULL) return;

	int id = PluginList.GetCurSel();
	if (id == LB_ERR) return;
	plugins->CallOptionsDlg(plugins->allPlugins[id], this->GetSafeHwnd());
}

void CPluginDialog::OnSelChange()
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	if (plugins == NULL) {
		OptionsButton.EnableWindow(false);
		return;
	}

	int id = PluginList.GetCurSel();
	if (id == LB_ERR) return;

	if (plugins->allPlugins[id].hasOptionsDlg)
		OptionsButton.EnableWindow(true);
	else
		OptionsButton.EnableWindow(false);
}
