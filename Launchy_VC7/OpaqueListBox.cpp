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


