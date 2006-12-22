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
#include ".\smartcombobox.h"

// SmartComboBox

IMPLEMENT_DYNAMIC(SmartComboBox, CComboBox)

SmartComboBox::SmartComboBox()
: typed(_T("")), searchPath(_T(""))
{
	m_RemoveFrame = false;
	m_RemoveButton = false;
	cloneSelect = -1;

	CComboBox();
}

SmartComboBox::~SmartComboBox()
{
}


BEGIN_MESSAGE_MAP(SmartComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM_REFLECT()

	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &SmartComboBox::OnCbnEditupdate)
	//	ON_CONTROL_REFLECT(CBN_SELCHANGE, &SmartComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &SmartComboBox::OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, &SmartComboBox::OnCbnEditchange)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &SmartComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &SmartComboBox::OnCbnDropdown)
	ON_MESSAGE(WM_CHANGE_COMBO_SEL, &SmartComboBox::AfterSelChange)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()



// SmartComboBox message handlers




HBRUSH SmartComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{


	//HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetTextColor(m_crText);
	pDC->SetBkColor(m_crBackGnd);
	pDC->SetBkMode(TRANSPARENT);


	if (nCtlColor == CTLCOLOR_EDIT)
	{
		//[ASCII 160][ASCII 160][ASCII 160]Edit control
		if (m_edit.GetSafeHwnd() == NULL) {
			m_edit.SubclassWindow(pWnd->GetSafeHwnd());
//			m_edit.SetLimitText(512);
		}
		if (m_Transparent) {
			CBrush m_Brush;
			m_Brush.CreateStockObject(HOLLOW_BRUSH);
			return (HBRUSH) m_Brush;
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

/*
	The tab key was pressed and so we should
	reformat the string
*/
void SmartComboBox::TabSearchTxt()
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;

	SearchStrings.Add(pDlg->smarts->matches[0]->croppedName);
//	SearchStrings.Add(searchTxt);
	searchTxt = L"";
	SearchPluginID = pDlg->smarts->matches[0]->owner;
	ShowDropDown(false);
	pDlg->smarts->Update(searchTxt);
	ReformatDisplay();
}

void SmartComboBox::DeleteWord()
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;

	// Take it from searchTxt
	int rightPos = searchTxt.ReverseFind(L' ');
	if (rightPos == 1 || searchTxt == L"") {
		if (SearchStrings.GetCount() > 1) {
			searchTxt = SearchStrings[SearchStrings.GetCount() - 2];
			SearchStrings.RemoveAt(SearchStrings.GetCount()-1);
		} else {
			searchTxt == L"";
			SearchStrings.RemoveAll();
		}
	}
	else
		searchTxt.Delete(rightPos+1, searchTxt.GetLength() - rightPos - 1);
	ReformatDisplay();
}

void SmartComboBox::ReformatDisplay()
{

	CString out = L"";
	for(int i = 0; i < SearchStrings.GetCount(); i++) {
		out += SearchStrings[i];
		out += L" | ";
	}
	out += searchTxt;
	m_edit.SetWindowTextW(out);
	CleanText();
	SetEditSel(out.GetLength(), out.GetLength());
}

void SmartComboBox::ParseSearchTxt()
{

	CStringArray NewSearchStrings;
	CString prefix = L"";
	int pos = 0;
	for(int strNum = 0; strNum < SearchStrings.GetCount(); strNum++) {
		CString prefix = SearchStrings[strNum];

		for(int charNum = 0; charNum < prefix.GetLength(); charNum++) {
			if (prefix[charNum] != searchTxt[pos]) break;
			pos += 1;
			if (charNum == prefix.GetLength() - 1)
				NewSearchStrings.Add(prefix);
		}
		if (searchTxt[pos] == L' ') 
			pos += 1;
		if (searchTxt[pos] == L'|')
			pos += 1;
		if (searchTxt[pos] == L' ')
			pos += 1;
	}
	
	searchTxt = searchTxt.Right(searchTxt.GetLength() - pos);
	searchTxt.MakeLower();
	SearchStrings.Copy(NewSearchStrings);
}


void SmartComboBox::OnCbnEditupdate()
{

	m_edit.GetWindowTextW(searchTxt);

	ParseSearchTxt();

	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;

	pDlg->smarts->Update(searchTxt);

	CleanText();
}



void SmartComboBox::OnCbnCloseup()
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;
	if (!IsWindow(m_listbox.m_hWnd)) return;

	int sel = m_listbox.GetCurSel();
	if (sel != LB_ERR) {
		DropItem* data = (DropItem*) GetItemDataPtr(sel);

		m_listbox.GetText(sel, searchTxt);
		ParseSearchTxt();

		pDlg->smarts->Update(searchTxt, true, data->longpath);
	}
	SetCurSel(-1);
}

void SmartComboBox::OnCbnEditchange()
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;
	if (!IsWindow(m_listbox.m_hWnd)) return;


	m_edit.GetWindowTextW(typed);
	searchPath = pDlg->smarts->GetMatchPath(0);
}

void SmartComboBox::OnCbnSelchange()
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;
	if (!IsWindow(m_listbox.m_hWnd)) return;

	

	this->PostMessage(WM_CHANGE_COMBO_SEL,IDC_Input,(LPARAM)(LPCTSTR) searchTxt);

	// If it's closing, we've already taken care of this..
	if (GetDroppedState()) {
		m_listbox.GetText(m_listbox.GetCurSel(), searchTxt);
		ParseSearchTxt();

		pDlg->smarts->Update(searchTxt,false);
	}
}


void SmartComboBox::OnDrawSelchange(int itemID) {
	// If it's closing, we've already taken care of this..
	if (GetDroppedState()) {
		CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
		m_listbox.GetText(itemID, searchTxt);
		ParseSearchTxt();

		pDlg->smarts->Update(searchTxt,false);
	}

}

void SmartComboBox::OnCbnDropdown()
{
	//	SmartComboBox* pmyComboBox = this;
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
//	SetCurSel(-1);

	// Find the longest string in the combo box.
	CString str;
	CSize   sx,sz;
	int     dx=0;
	CDC*    pDC = GetDC();
	pDC->SelectObject(m_FontSmall);
	CString cStr;
	for (int i=0;i < GetCount();i++)
	{
		m_listbox.SetItemHeight(i, 40);
		//		pmyComboBox->GetLBText( i, str );		
		//		sz = pDC->GetTextExtent(str);
		DropItem* d = (DropItem*) GetItemDataPtr(i);
		sz = pDC->GetTextExtent(d->lesspath);

		m_listbox.GetText(i, cStr);
		sx = pDC->GetOutputTextExtent(cStr);

		if (sz.cx > sx.cx) {
			if (sz.cx > dx) {
				dx = sz.cx;
			}
		} else {
			if (sx.cx > dx) {
				dx = sx.cx;
			}
		}
	}
	ReleaseDC(pDC);


	// Adjust the width for the vertical scroll bar and the left and
	// right border.
	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 62;// + 2*::GetSystemMetrics(SM_CXEDGE);

	// Set the width of the list box so that every item is completely visible.
	SetDroppedWidth(dx);


	for(int i = 0; i < GetCount(); i++) {
		DropItem* data = (DropItem*) GetItemDataPtr(i);
		if (data->longpath.Find(_T(".directory")) != -1)
			data->icon = pDlg->IconInfo.GetFolderIconHandle(false);
		else
			data->icon = pDlg->IconInfo.GetIconHandleNoOverlay(data->longpath, false);
	}
}

void SmartComboBox::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CComboBox::OnPaint() for painting messages

	if (m_RemoveFrame) {
		CPaintDC dc(this); // device context for painting
		RECT         rect;

		// Find coordinates of client area
		GetClientRect(&rect);


		// Deflate the rectangle by the size of the borders
		InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

		// Remove the drop-down button as well
		rect.right -= GetSystemMetrics(SM_CXHSCROLL);

		// Make a mask from the rectangle, so the borders aren't included
		dc.IntersectClipRect(rect.left, rect.top, rect.right, rect.bottom);

		// Draw the combo-box into our DC
		CComboBox::OnPaint();

		// Remove the clipping region
		dc.SelectClipRgn(NULL);

		// now mask off the inside of the control so we can paint the borders
		dc.ExcludeClipRect(rect.left, rect.top, rect.right, rect.bottom);

		// paint a flat colour
		GetClientRect(&rect);
		CBrush b;
		b.CreateStockObject(HOLLOW_BRUSH);
		//b.CreateSolidBrush(RGB(124,124,124));
		dc.FillRect(&rect, &b);
	}
	else {
		CComboBox::OnPaint();
	}
}



void SmartComboBox::CleanText(void)
{

	if (m_Transparent) {
		CWnd* pParent = GetParent();
		CRect rect;
		GetWindowRect(rect);
		pParent->ScreenToClient(rect);
		rect.DeflateRect(2, 2);

		pParent->InvalidateRect(rect, TRUE); 
	}

}
void SmartComboBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//	OnDrawSelchange((int)lpDrawItemStruct->itemID);
	CRect rItem;
	CRect rText;
	CRect rIcon;
	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the List Box
		// based on whether the list box has Focus or not
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		return;
	}

	// String to store the text, which will be added to the CListBox
	CString strText;

	// Get the item text.
	m_listbox.GetText((int)lpDrawItemStruct->itemID, strText);
	//	AfxMessageBox(strText);

	//Initialize the Item's row
	rItem = lpDrawItemStruct->rcItem;

	//The icon that the sample has created has a width of 32 pixels
	rIcon = lpDrawItemStruct->rcItem;
	rIcon.bottom = rIcon.top + 32;
	rIcon.right = rIcon.left + 32;

	//Start drawing the text 2 pixels after the icon
	rText.left = rIcon.right + 2;
	rText.top = rIcon.top;

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(::GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(&rItem, &br);
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush br(dc->GetBkColor());
		//	CBrush br(::GetSysColor(COLOR_WINDOW));
		dc->FillRect(&rItem, &br);
	}

	// If the item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		dc->DrawFocusRect(&rItem);
	}

	// To draw the Text set the background mode to Transparent.
	int iBkMode = dc->SetBkMode(TRANSPARENT);


	DropItem* data = (DropItem*) m_listbox.GetItemDataPtr((int) lpDrawItemStruct->itemID);


	CPoint pt(rIcon.left,rIcon.top);
	//	icons.Draw(dc,0,pt,ILD_NORMAL);
	dc->DrawIcon(pt.x,pt.y,data->icon);




	dc->TextOut(rText.left,rText.top,strText);
	CSize cs = dc->GetTextExtent(strText);

	int oldColor = dc->GetTextColor();
	CFont* oldFont = dc->GetCurrentFont();

	dc->SelectObject(m_FontSmall);
	dc->SetTextColor(m_FontSmallRGB);
	dc->TextOut(rText.left,rText.top + cs.cy + 1,data->lesspath);

	dc->SelectObject(oldFont);
	dc->SetTextColor(oldColor);


}


void SmartComboBox::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 40;
}


void SmartComboBox::SetSmallFont(CFont* font, COLORREF rgb)
{
	m_FontSmall = font;	
	m_FontSmallRGB = rgb;
}

void SmartComboBox::DoSubclass(void)
{
	if (m_edit.GetSafeHwnd() == NULL) {
		m_edit.SubclassDlgItem( CTLCOLOR_EDIT,this);
	}
	//ListBox control
	if (m_listbox.GetSafeHwnd() == NULL) {
		m_listbox.SubclassDlgItem(CTLCOLOR_LISTBOX, this);
	}
}

LRESULT SmartComboBox::AfterSelChange(UINT wParam, LONG lParam) {
	m_edit.GetWindowTextW(searchTxt);
	ReformatDisplay();
//	m_edit.SetWindowTextW(L"This is a test");
	return true;
}