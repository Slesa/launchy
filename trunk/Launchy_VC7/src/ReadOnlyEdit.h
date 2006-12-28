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

/*
	ReadOnlyEdit.h
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

#if !defined(AFX_READONLYEDIT_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_)
#define AFX_READONLYEDIT_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadOnlyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadOnlyEdit window

class CReadOnlyEdit : public CEdit
{
// Construction
public:
	CReadOnlyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadOnlyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:			
	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
	virtual ~CReadOnlyEdit();
	bool m_isBackTransparent;
	CBrush        m_Brush;
	// Generated message map functions
protected:
	//text and text background colors
	COLORREF m_crText;
	COLORREF m_crBackGnd;
	//background brush
	CBrush m_brBackGnd;
	//{{AFX_MSG(CReadOnlyEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READONLYEDIT_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_)
