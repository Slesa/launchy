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

// TypeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "TypeEdit.h"


// CTypeEdit

//IMPLEMENT_DYNAMIC(CTypeEdit, CReadOnlyEdit)

CTypeEdit::CTypeEdit()
{

}

CTypeEdit::~CTypeEdit()
{
}


BEGIN_MESSAGE_MAP(CTypeEdit, CEdit)
//	ON_WM_CHAR()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CTypeEdit message handlers



//void CTypeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	BOOL handled = false;
//	if (nChar == VK_ESCAPE) {
//		AfxMessageBox(_T("Got it!"));
//		AfxGetMainWnd()->ShowWindow(SW_HIDE);
//		handled = true;
//	}
//	if (!handled) {
//		// TODO: Add your message handler code here and/or call default
//		CEdit::OnChar(nChar, nRepCnt, nFlags);
//	}
//}

//void CTypeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//	BOOL handled = false;
//	if (nChar == VK_DELETE) {
//		AfxMessageBox(_T("Got it!"));
//		AfxGetMainWnd()->ShowWindow(SW_HIDE);
//		handled = true;
//	}
//	if (!handled) {
//		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
//	}
//}
