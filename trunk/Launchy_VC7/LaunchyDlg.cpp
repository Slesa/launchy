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
#include "HotkeyDialog.h"
#include "Skin.h"
#include "SkinChooser.h"
#include "DirectoryChooser.h"

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
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	atLaunch = true;

	DelayTimer = 100;

	
	m_FontInput = NULL;
	m_FontResult = NULL;
}

void CLaunchyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Input, InputBox);
	DDX_Control(pDX, IDC_PREVIEW, Preview);
	DDX_Control(pDX, IDC_PIC, IconPreview);
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
	ON_WM_ENDSESSION()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(LAUNCHY_DB_DONE, OnDBDone)
END_MESSAGE_MAP()




// CLaunchyDlg message handlers

BOOL CLaunchyDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// In order to subclass the combobox list and edit controls
	// we have to first paint the controls to make sure the message
	// mapping is setup before we use the controls.
	InputBox.ShowDropDown(true);
	InputBox.ShowDropDown(false);

	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_TOOLWINDOW);


	// BEGIN SKINNING FUNCTIONS
	EnableEasyMove();                       // enable moving of
	// the dialog by
	// clicking
	// anywhere in
	// the dialog


	



	options.reset(new Options());
	smarts.reset(new LaunchySmarts());

	this->ShowWindows(false);

	applySkin();

	BOOL m_isKeyRegistered = RegisterHotKey(GetSafeHwnd(), 100,
		options->mod_key, options->vkey);

	ASSERT(m_isKeyRegistered != FALSE);


	SetTimer(UPDATE_TIMER, 60000, NULL);


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
			this->ShowWindows(true);
			this->ActivateTopParent();
			this->InputBox.SetFocus();
		}
		else {
			this->ShowWindows(false);
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
	options.reset();
	smarts.reset();
//	border.OnClose();
	// TODO: Add your message handler code here and/or call default
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
		if (pMsg->wParam==VK_DOWN) {
			if (!InputBox.GetDroppedState()) {
				InputBox.ShowDropDown(true);
//				InputBox.m_listbox.SetSel(1);
			}
		} 
		else if (pMsg->wParam==VK_UP) {
			if (!InputBox.GetDroppedState()) {
				InputBox.ShowDropDown(true);
//				InputBox.m_listbox.SetSel(1);
			}
		}
		else {
			if (InputBox.GetDroppedState()) {
				CString typed = InputBox.typed;
				InputBox.ShowDropDown(false);
				InputBox.m_edit.SetWindowText(typed);
				InputBox.SetEditSel(InputBox.m_edit.GetWindowTextLengthW(), InputBox.m_edit.GetWindowTextLengthW());
				InputBox.CleanText();
				//			InputBox.SetCurSel(-1);
			}
		}
		SetTimer(DELAY_TIMER, 1000, NULL);
		if(pMsg->wParam==VK_RETURN) {
			this->ShowWindows(SW_HIDE);
			this->Visible = false;


			if (InputBox.typed != searchTxt) {
				CString x;
				options->Associate(InputBox.typed, searchTxt);
			}

			KillTimer(DELAY_TIMER);
			smarts->Launch();
			pMsg->wParam = NULL;
		}

		if (pMsg->wParam==VK_ESCAPE) {
			this->ShowWindows(SW_HIDE);
			this->Visible = false;
			KillTimer(DELAY_TIMER);
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
		if (Visible && !InputBox.GetDroppedState() && InputBox.m_edit.GetWindowTextLengthW() > 0 &&
			InputBox.m_listbox.GetCount() > 1) {
	//			CString prev = InputBox.typed;
				InputBox.SetCurSel(-1);
//				InputBox.m_listbox.
			InputBox.ShowDropDown(true);
			InputBox.m_edit.SetWindowText(InputBox.typed);
			InputBox.SetEditSel(InputBox.typed.GetLength(), InputBox.typed.GetLength());
			InputBox.CleanText();
//			InputBox.typed = prev;
//			InputBox.m_edit.SetSel(0,-1);
//			InputBox.m_edit.SetSel(prev.GetLength(),prev.GetLength(),TRUE);
		}
		KillTimer(DELAY_TIMER);
		CDialogSK::OnTimer(nIDEvent);
	}
	else if (nIDEvent == UPDATE_TIMER) {
		smarts->LoadCatalog();
		KillTimer(UPDATE_TIMER);
		SetTimer(UPDATE_TIMER, 600000, NULL);
	}
}

void CLaunchyDlg::OnEndSession(BOOL bEnding)
{
	if (options != NULL) { options->Store(); }
	CDialogSK::OnEndSession(bEnding);

	// TODO: Add your message handler code here
}

void CLaunchyDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	// Load the desired menu
	CMenu mnuPopupSubmit;
	mnuPopupSubmit.LoadMenu(IDR_MENU1);


	// Get a pointer to the first item of the menu
	CMenu *mnuPopupMenu = mnuPopupSubmit.GetSubMenu(0);
	ASSERT(mnuPopupMenu);
	
	DWORD selection = mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON| TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);

	if (selection == ID_SETTINGS_SKINS) {
		SkinChooser dlg;
		dlg.DoModal();
		applySkin();
	}
	else if (selection == ID_SETTINGS_HOTKEY) {
		CHotkeyDialog dlg;
		dlg.DoModal();
	}

	else if (selection == ID_SETTINGS_DIRECTORIES) {
		DirectoryChooser dlg;
		dlg.DoModal();
		smarts->LoadCatalog();
	}

	else if (selection == ID_SETTINGS_REBUILD) {
		smarts->LoadCatalog();
	}

	else if (selection == ID_EXIT) {
		this->EndDialog(1);
	}
}

void CLaunchyDlg::applySkin()
{
	if (options->skin == NULL) {
			options->SetSkin(_T("Default"));
			if (options->skin == NULL) {
				return;
			}
	}

	if (border.inuse) {
		border.inuse = false;
		border.DestroyWindow();
	}



	if (options->skin->resultBorder) {
		SetWindowLong(Preview.GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(Preview.GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	} else {
		SetWindowLong(Preview.GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(Preview.GetSafeHwnd(), GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
	}

	InputBox.m_RemoveFrame = !options->skin->inputBorder;
	InputBox.m_Transparent = options->skin->inputTransparent;

	// After changing the windowlong, the window needs to get moved to update it.. so send
	// it to the top left corner for now and we'll move it back into position later.
	RECT r;
	r.top = 0;
	r.left = 0;

	Preview.MoveWindow(&r,1);

	SetBitmap(options->skin->bgFile);
	SetStyle (LO_STRETCH);                   // resize dialog to
	if (options->skin->translucensy != -1) {
		SetTransparent(options->skin->translucensy);
	} else {
		SetTransparentColor(options->skin->trans_rgb);    // set red as
	}

	if (options->skin->alphaBorderFile == "") {
		border.inuse = false;
	} else {
		if (!border.inuse) {
			int ret = border.Create(IDD_ALPHA_BORDER, this);
		}
		border.SetImage(options->skin->alphaBorderFile);
		border.inuse = true;

		RECT r;
		GetWindowRect(&r);
		border.MoveWindow(r.left + options->skin->alphaRect.left, r.top + options->skin->alphaRect.top, r.right, r.bottom, 1);
		ShowWindows(IsWindowVisible() != 0);
	}


	MoveWindow(options->posX, options->posY, options->skin->backRect.Width(), options->skin->backRect.Height(),1);
	InputBox.MoveWindow(options->skin->inputRect,1);
	Preview.MoveWindow(options->skin->resultRect,1);
	IconPreview.MoveWindow(options->skin->iconRect,1);


//	InputBox.SetFont(options->skin->m_FontInput,1);
//	Preview.SetFont(options->skin->m_FontResult,1);



	CFont* old1;
	CFont* old2;

	old1 = m_FontInput;
	old2 = m_FontResult;

	m_FontInput = new CFont;
	m_FontResult = new CFont;

	m_FontInput->CreateFontW(
		options->skin->input_fontSize,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   options->skin->input_bold,						// nWeight
	   options->skin->input_italics,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   options->skin->input_fontName);                 // lpszFacename	


	// Fonts
	m_FontResult->CreateFontW(
	   options->skin->results_fontSize,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   options->skin->results_bold,                 // nWeight
	   options->skin->results_italics,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   options->skin->results_fontName);                 // lpszFacename	

	InputBox.SetFont(m_FontInput,1);
	Preview.SetFont(m_FontResult,1);

	// Free the old fonts!
	if (old1 != NULL && old2 != NULL) {
		old1->DeleteObject();
		old2->DeleteObject();
		delete(old1);
		delete(old2);
	}



	InputBox.SetTextColor(options->skin->inputFontRGB);
	Preview.SetTextColor(options->skin->resultFontRGB);



	Preview.m_isBackTransparent = options->skin->resultTransparent;

	InputBox.SetBackColor(options->skin->inputRGB);

	Preview.SetBackColor(options->skin->resultRGB);	


	IconPreview.m_GrabBkgnd = true;

	RedrawWindow();

}
LRESULT CLaunchyDlg::OnDBDone(UINT wParam, LONG lParam) {
	smarts->Update(searchTxt, false);
	return true;
}