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
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READONLYEDIT_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_)
