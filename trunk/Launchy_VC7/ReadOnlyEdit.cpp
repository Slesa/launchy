/*
	ReadOnlyEdit.cpp
	By: Kevin Bond
	kevinbond@gmail.com
	www.mechdesk.ath.cx

	Use and distribute freely.  
	Please don't remove my name and don't take credit for my work.	

	Send comments, questions or bugs to kevinbond@gmail.com 
	or visit my site www.mechdesk.ath.cx
*/

/*Instructions:	This class is derived from the CEdit class.  When creating
				the edit box you want to be white and read-only create it using
				CReadOnlyEdit instead of CEdit.  Make sure you remember to set 
				the dwStyle to ES_READONLY.
				
*/					

#include "stdafx.h"
#include "ReadOnlyEdit.h"
#include ".\readonlyedit.h"
#include "LaunchyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadOnlyEdit

CReadOnlyEdit::CReadOnlyEdit()
{
	//default text color
	m_crText = RGB(0,0,0);
	m_isBackTransparent = true;
}

CReadOnlyEdit::~CReadOnlyEdit()
{
	//delete brush
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CReadOnlyEdit, CEdit)
	//{{AFX_MSG_MAP(CReadOnlyEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(EN_UPDATE, OnEnUpdate)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadOnlyEdit message handlers



HBRUSH CReadOnlyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	
		//set text color
		pDC->SetTextColor(m_crText);
		//set the text's background color
		pDC->SetBkColor(m_crBackGnd);

    if (m_isBackTransparent) {
        m_Brush.CreateStockObject(HOLLOW_BRUSH);
        pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH) m_Brush;
    }
	else {
		return m_brBackGnd;
	}

	//return the brush used for background this sets control background
}


void CReadOnlyEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBackGnd = rgb;
	
	//free brush
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
	//set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
	Invalidate(TRUE);
}


void CReadOnlyEdit::SetTextColor(COLORREF rgb)
{
	//set text color ref
	m_crText = rgb;

	//redraw
	Invalidate(TRUE);
}

void CReadOnlyEdit::OnEnUpdate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
    CWnd* pParent = GetParent();
    CRect   rect;

    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);

	pParent->InvalidateRect(rect, TRUE); 
	// TODO:  Add your control notification handler code here
}

void CReadOnlyEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;
	pDlg->OnLButtonDown(nFlags, point);
//	CEdit::OnLButtonDown(nFlags, point);
}

void CReadOnlyEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnRButtonDown(nFlags, point);
}

void CReadOnlyEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	if (pDlg == NULL) return;
	pDlg->OnContextMenu(pWnd, point);

}
