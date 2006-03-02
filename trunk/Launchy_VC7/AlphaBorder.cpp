// AlphaBorder.cpp : implementation file
//

#include "stdafx.h"
//#include "ab_border.h"
#include "Launchy.h"
#include "LaunchyDlg.h"
#include "AlphaBorder.h"
#include ".\alphaborder.h"
//#include ".\alphaborder.h"


void PreMultiplyRGBChannels(CImage& bmp)
{
	BYTE* bits = (BYTE*) bmp.GetBits();
	int Pitch = bmp.GetPitch();

	for(int row = 0; row < bmp.GetHeight(); row++) {
		for(long long i = 0; i < bmp.GetWidth() * 4; i += 4) {
				int offset = (row * Pitch) + i;
				bits[offset] = bits[offset]*bits[offset+3]/255;
				bits[offset+1] = bits[offset+1]*bits[offset+3]/255;
				bits[offset+2] = bits[offset+2]*bits[offset+3]/255;
		}
	}
}


// AlphaBorder dialog

IMPLEMENT_DYNAMIC(AlphaBorder, CDialog)
AlphaBorder::AlphaBorder(CWnd* pParent /*=NULL*/)
	: CDialog(AlphaBorder::IDD, pParent)
{
}

AlphaBorder::~AlphaBorder()
{
}

void AlphaBorder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AlphaBorder, CDialog)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// AlphaBorder message handlers

BOOL AlphaBorder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// setup the blend function

	



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AlphaBorder::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);
	// TODO: Add your message handler code here
}


void AlphaBorder::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
		CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
		if (pDlg == NULL) return;
		pDlg->OnLButtonDown(nFlags, point);

	CDialog::OnLButtonDown(nFlags, point);

}

void AlphaBorder::SetImage(CString name)
{
	ModifyStyleEx(0, WS_EX_LAYERED);
	image.Load(name);	
	::PreMultiplyRGBChannels(image);

	pBitmap = CBitmap::FromHandle((HBITMAP) image);
//	CBitmap& bmp = (CBitmap&) pBitmap;





	// ok... into the per-pixel-alpha bendling....

	// Create/setup the DC's

	CDC dcScreen;
	CDC dcMemory;

	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);

	CBitmap *pOldBitmap= dcMemory.SelectObject(pBitmap);

	CString str("Blah");
	RECT rect;
	rect.top = 70;
	rect.left = 50;
	rect.bottom = 95;
	rect.right = 80;

	dcMemory.SetBkMode(TRANSPARENT);
//	dcMemory.
	dcMemory.SetBkColor(0x00ffffff);
	dcMemory.SetTextColor(0x00000000);
//	dcMemory.DrawText(str, &rect, DT_CENTER);

	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg;
	GetWindowRect(rectDlg);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(image.GetWidth(), image.GetHeight());


	// Perform the alpha blend

	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 0xff, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
	BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
}

void AlphaBorder::OnDestroy()
{
	CDialog::OnDestroy();
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	pDlg->OnClose();
	// TODO: Add your message handler code here
}

void AlphaBorder::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	pDlg->OnClose();
	CDialog::OnClose();
}

void AlphaBorder::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	pDlg->OnContextMenu(pDlg, point);
}
