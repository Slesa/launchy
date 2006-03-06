// TransparentStatic2.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentStatic2.h"
#include ".\transparentstatic2.h"


// CTransparentStatic2

IMPLEMENT_DYNAMIC(CTransparentStatic2, CStatic)
CTransparentStatic2::CTransparentStatic2()
{
	m_GrabBkgnd = false;
}

CTransparentStatic2::~CTransparentStatic2()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic2, CStatic)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransparentStatic2 message handlers

LRESULT CTransparentStatic2::OnSetText(WPARAM wParam,LPARAM lParam)
{
	LRESULT Result = Default();
	Invalidate();
	UpdateWindow();
	return Result;
}

HBRUSH CTransparentStatic2::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CTransparentStatic2::OnEraseBkgnd(CDC* pDC)
{
	
	if (m_Bmp.GetSafeHandle() == NULL || m_GrabBkgnd)
	{
		if (m_GrabBkgnd) {
			ShowWindow(false);
		}

		CRect Rect;
		GetWindowRect(&Rect);
		CWnd *pParent = GetParent();
		ASSERT(pParent);
		pParent->ScreenToClient(&Rect);  //convert our corrdinates to our parents

		//copy what's on the parents at this point
		CDC *spDC = pParent->GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(spDC);
		m_Bmp.DeleteObject();
		m_Bmp.CreateCompatibleBitmap(spDC,Rect.Width(),Rect.Height());
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		MemDC.BitBlt(0,0,Rect.Width(),Rect.Height(),spDC,Rect.left,Rect.top,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		pParent->ReleaseDC(spDC);

		if (m_GrabBkgnd) {
			ShowWindow(true);
			m_GrabBkgnd = false;
		}
	}
	else //copy what we copied off the parent the first time back onto the parent
	{
		CRect Rect;
		GetClientRect(Rect);
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
	}

//	return	CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}


void CTransparentStatic2::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BOOL bfont = FALSE;
	BOOL bIcon = FALSE;
	BOOL bBmp = FALSE;

	// Where to draw text
	CRect client_rect;
	GetClientRect(client_rect);
	// Get the caption
	CString szText;
	GetWindowText(szText);

	HICON hIcon;
	hIcon = GetIcon();
	if(hIcon)
	{
		// mean icon type
		bIcon = TRUE;
	}

	HBITMAP hBmp;
	hBmp = GetBitmap();
	if(hBmp)
	{
		// mean bitmap type
//		bIcon = TRUE;
		bBmp = TRUE;
	}

	// Get the font
	CFont *pFont, *pOldFont;
	pFont = GetFont();
	if(pFont)
	{
		bfont = TRUE;
	}

	if(bIcon)
	{
//		Invalidate();
//		UpdateWindow();
//		GetParent()->InvalidateRect(&client_rect, TRUE);
		/*
    CRect   rect;
    GetClientRect(rect);
    ICONINFO IconI;
	GetIconInfo(hIcon, &IconI);
    CDC dc2;
    dc2.CreateCompatibleDC(&dc);
	BITMAP bmp;
	GetObject(IconI.hbmColor, sizeof(BITMAP), &bmp);



	SelectObject(dc2.m_hDC, IconI.hbmColor);

	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc2, 0, 0, 32, 32, SRCCOPY);
*/
		
		dc.DrawIcon(0, 0, hIcon);
	}

	if(bBmp)
	{
		// Left for an exercise for interested users
		//
	}

	if(bfont)
	{
		pOldFont = dc.SelectObject(pFont);

		// Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

		DWORD dwStyle = GetStyle(), dwText = 0;

		MAP_STYLE( SS_RIGHT, DT_RIGHT );
		MAP_STYLE( SS_CENTER, DT_CENTER );
		MAP_STYLE( SS_CENTERIMAGE, DT_VCENTER | DT_SINGLELINE );
		MAP_STYLE( SS_NOPREFIX, DT_NOPREFIX );
		MAP_STYLE( SS_WORDELLIPSIS, DT_WORD_ELLIPSIS );
		MAP_STYLE( SS_ENDELLIPSIS, DT_END_ELLIPSIS );
		MAP_STYLE( SS_PATHELLIPSIS, DT_PATH_ELLIPSIS );

		NMAP_STYLE( SS_LEFTNOWORDWRAP |
			SS_CENTERIMAGE |
			SS_WORDELLIPSIS |
			SS_ENDELLIPSIS |
			SS_PATHELLIPSIS, DT_WORDBREAK );

		// Set transparent background
		dc.SetBkMode(TRANSPARENT);

		// Draw the text
		dc.DrawText(szText, client_rect, dwText);

		// Select old font
		dc.SelectObject(pOldFont);
	}
}

/*
void CTransparentStatic2::OnPaint()
{
CPaintDC dc(this); // device context for painting
// TODO: Add your message handler code here
// Do not call CStatic::OnPaint() for painting messages
}
*/
