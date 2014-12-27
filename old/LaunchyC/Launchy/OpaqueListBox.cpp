// OpaqueListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "OpaqueListBox.h"
#include "CDialogSK.h"


lpfnSetLayeredWindowAttributes gg_pSetLayeredWindowAttributes;

// COpaqueListBox

IMPLEMENT_DYNAMIC(COpaqueListBox, CListBox)

COpaqueListBox::COpaqueListBox()
{
        //  call it with 0 alpha for the given color
	gg_pSetLayeredWindowAttributes(this->m_hWnd, 0, 255, 0);
}

COpaqueListBox::~COpaqueListBox()
{
}


BEGIN_MESSAGE_MAP(COpaqueListBox, CListBox)
END_MESSAGE_MAP()



// COpaqueListBox message handlers


