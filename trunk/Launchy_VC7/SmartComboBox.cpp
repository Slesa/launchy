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

// SmartComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "SmartComboBox.h"
#include "LaunchyDlg.h"

// SmartComboBox

IMPLEMENT_DYNAMIC(SmartComboBox, CComboBox)

SmartComboBox::SmartComboBox()
: typed(_T(""))
{
	CComboBox();
}

SmartComboBox::~SmartComboBox()
{
}


BEGIN_MESSAGE_MAP(SmartComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &SmartComboBox::OnCbnEditupdate)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, &SmartComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &SmartComboBox::OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, &SmartComboBox::OnCbnEditchange)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &SmartComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &SmartComboBox::OnCbnDropdown)
END_MESSAGE_MAP()



// SmartComboBox message handlers




HBRUSH SmartComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetTextColor(m_crText);
//	pDC->SetBkColor(m_crBackGnd);
	pDC->SetBkMode(TRANSPARENT);


	if (nCtlColor == CTLCOLOR_EDIT)
	{
		//[ASCII 160][ASCII 160][ASCII 160]Edit control
		if (m_edit.GetSafeHwnd() == NULL) {
			m_edit.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		//ListBox control
		if (m_listbox.GetSafeHwnd() == NULL)
			m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
	}


	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return m_brBackGnd;
}

void SmartComboBox::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBackGnd = rgb;
	
	//free brush
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
	//set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
//	Invalidate(TRUE);
}


void SmartComboBox::SetTextColor(COLORREF rgb)
{
	//set text color ref
	m_crText = rgb;

	//redraw
//	Invalidate(TRUE);
}

void SmartComboBox::OnDestroy()
{
	if (m_edit.GetSafeHwnd() != NULL)
		m_edit.UnsubclassWindow();
	if (m_listbox.GetSafeHwnd() != NULL)
		m_listbox.UnsubclassWindow();

	CComboBox::OnDestroy();

	// TODO: Add your message handler code here
}

void SmartComboBox::OnCbnEditupdate()
{
	m_edit.GetWindowTextW(searchTxt);

	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;

	pDlg->smarts->Update(searchTxt);
}



void SmartComboBox::OnCbnCloseup()
{
	int sel = m_listbox.GetCurSel();
	if (sel != LB_ERR) {

		m_listbox.GetText(sel, searchTxt);
		CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
		if (pDlg == NULL) return;

		pDlg->smarts->Update(searchTxt);
	}
	SetCurSel(-1);
}

void SmartComboBox::OnCbnEditchange()
{
	m_edit.GetWindowTextW(typed);
}

void SmartComboBox::OnCbnSelchange()
{
	// If it's closing, we've already taken care of this..
	if (GetDroppedState()) {
		CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
		m_listbox.GetText(m_listbox.GetCurSel(), searchTxt);
		pDlg->smarts->Update(searchTxt,false);
	}	
}
void SmartComboBox::OnCbnDropdown()
{
	SmartComboBox* pmyComboBox = this;
//	pmyComboBox->SetCurSel(-1);

	// Find the longest string in the combo box.
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = pmyComboBox->GetDC();
	for (int i=0;i < pmyComboBox->GetCount();i++)
	{
	pmyComboBox->GetLBText( i, str );
	sz = pDC->GetTextExtent(str);

	if (sz.cx > dx)
		dx = sz.cx;
	}
	pmyComboBox->ReleaseDC(pDC);

	// Adjust the width for the vertical scroll bar and the left and
	// right border.
	dx += ::GetSystemMetrics(SM_CXVSCROLL);// + 2*::GetSystemMetrics(SM_CXEDGE);

	// Set the width of the list box so that every item is completely visible.
	pmyComboBox->SetDroppedWidth(dx);
}

