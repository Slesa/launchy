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
			__int64 offset = (row * Pitch) + i;
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
	inuse = false;
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

	image.Destroy();
	image.Load(name);	
	::PreMultiplyRGBChannels(image);

	POINT pt={0, 0};
	SIZE sz = {image.GetWidth(), image.GetHeight()};
	POINT ptSource = {0};
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), NULL/*dcScreen*/, &pt, &sz, image.GetDC()/*dcMemory*/,
		&ptSource, 0, &bf, ULW_ALPHA);

}

void AlphaBorder::OnDestroy()
{
	CDialog::OnDestroy();
	image.ReleaseDC();
	if (!image.IsNull())
		image.Destroy();
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



BOOL AlphaBorder::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CLaunchyDlg* pDlg = (CLaunchyDlg*) AfxGetMainWnd();
	pDlg->PreTranslateMessage(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
