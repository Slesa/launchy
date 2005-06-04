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

// LaunchyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "LaunchyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLaunchyDlg dialog

#define DELAY_TIMER 100
#define UPDATE_TIMER 101

CLaunchyDlg::CLaunchyDlg(CWnd* pParent /*=NULL*/)
: CDialogSK(CLaunchyDlg::IDD, pParent)
, DelayTimer(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	atLaunch = true;

	m_Font.CreatePointFont(100,_T("Trebuchet MS"));

	DelayTimer = 100;
}

void CLaunchyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Input, InputBox);
	DDX_Control(pDX, IDC_PREVIEW, Preview);
}

BEGIN_MESSAGE_MAP(CLaunchyDlg, CDialogSK)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_Input, &CLaunchyDlg::OnCbnSelchangeInput)
	ON_WM_TIMER()
END_MESSAGE_MAP()




// CLaunchyDlg message handlers

BOOL CLaunchyDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	// BEGIN SKINNING FUNCTIONS
	EnableEasyMove();                       // enable moving of
	// the dialog by
	// clicking
	// anywhere in
	// the dialog
	SetBitmap (IDB_BACKGROUND);             // set background
	// bitmap
	SetStyle (LO_STRETCH);                   // resize dialog to
	// the size of
	// the bitmap
	SetTransparentColor(RGB(255, 0, 0));    // set red as
	// the transparent
	// color
	// END SKINNING FUNCTIONS

	options = new Options();



	BOOL m_isKeyRegistered = RegisterHotKey(GetSafeHwnd(), 100,
		MOD_ALT, VK_SPACE);

	ASSERT(m_isKeyRegistered != FALSE);



	InputBox.SetFont(&m_Font);
	Preview.SetFont(&m_Font);

	SetTimer(UPDATE_TIMER, 1200000, NULL);

	// In order to subclass the combobox list and edit controls
	// we have to first paint the controls to make sure the message
	// mapping is setup before we use the controls.
	InputBox.ShowDropDown(true);
	InputBox.ShowDropDown(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunchyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSK::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaunchyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CLaunchyDlg::OnHotKey(WPARAM wParam, LPARAM lParam) {
	if (wParam == 100) {
		if (atLaunch)
		{
			this->Visible = false;
			atLaunch = false;
		}
		this->Visible = !this->Visible;
		if (Visible)
		{
			this->ShowWindow(SW_SHOW);
			this->ActivateTopParent();
			this->InputBox.SetFocus();
		}
		else {
			this->ShowWindow(SW_HIDE);
		}
	}
	return 1;
}

void CLaunchyDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if(this->atLaunch) {
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
		if (options->posX != -1 && options->posY != -1) {
			lpwndpos->x = options->posX;
			lpwndpos->y = options->posY;
		}
	}
	CDialogSK::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here
}

void CLaunchyDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	delete options;
	options = NULL;
	CDialogSK::OnClose();
}

void CLaunchyDlg::OnDestroy()
{
	OnClose();
	CDialogSK::OnDestroy();

	// TODO: Add your message handler code here
}




void CLaunchyDlg::OnCbnSelchangeInput()
{
	// TODO: Add your control notification handler code here
}




BOOL CLaunchyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
	{
		SetTimer(DELAY_TIMER, 1000, NULL);
		if(pMsg->wParam==VK_RETURN) {
			this->ShowWindow(SW_HIDE);
			this->Visible = false;
			smarts.Launch();
			pMsg->wParam = NULL;
		}

		if (pMsg->wParam==VK_ESCAPE) {
			this->ShowWindow(SW_HIDE);
			this->Visible = false;
			pMsg->wParam = NULL;
		}

	} 

	if (pMsg->message == WM_CHAR) {
		SetTimer(DELAY_TIMER, 1000, NULL);

	}
	return CDialogSK::PreTranslateMessage(pMsg);
}

void CLaunchyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == DELAY_TIMER) {
		if (Visible && InputBox.m_edit.GetWindowTextLengthW() > 0 &&
			InputBox.m_listbox.GetCount() > 1) {
			InputBox.ShowDropDown(true);
		}
		KillTimer(DELAY_TIMER);
		CDialogSK::OnTimer(nIDEvent);
	}
	else if (nIDEvent == UPDATE_TIMER) {
		smarts.LoadCatalog();
	}
}
