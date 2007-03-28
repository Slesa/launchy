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

	// Make a list of the turned on plugins and tell the plugin manager to update
	map<CString,bool> ids;
	for(int i = 0; i < PluginList.GetCount(); i++) {
		ids[plugins->allPlugins[i].name] = PluginList.GetCheck(i);
	}

	plugins->ReloadPlugins(ids);

	OnOK();
}

void CPluginDialog::OnBnClickedOptions()
{
	shared_ptr<Plugin> plugins = ((CLaunchyDlg*)AfxGetMainWnd())->plugins;
	if (plugins == NULL) return;

	int id = PluginList.GetCurSel();
	if (id == LB_ERR) return;
	plugins->CallOptionsDlg(plugins->allPlugins[id]);
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
