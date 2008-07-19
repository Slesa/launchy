/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

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

#include "catalog.h"
#include "globals.h"
#include <qdebug.h>

#include <QtAlgorithms>

bool CatLessNoPtr (CatItem & a, CatItem & b) {
	return CatLess(&a, &b);
}

bool CatLess (CatItem* a, CatItem* b)  {
/*
	if (a->isHistory) { return true; }
	if (b->isHistory) { return false; }
*/
	bool localEqual = a->lowName == gSearchTxt;
	bool otherEqual = b->lowName == gSearchTxt;

	if (localEqual && !otherEqual)
		return true;
	if (!localEqual && otherEqual)
		return false;


	if(a->usage > b->usage)
		return true;
	if (a->usage < b->usage)
		return false;



	int localFind = a->lowName.indexOf(gSearchTxt);
	int otherFind = b->lowName.indexOf(gSearchTxt);

	if (localFind != -1  && otherFind == -1)
		return true;
	else if (localFind == -1 && otherFind != -1)
		return false;
	
	if (localFind != -1 && otherFind != -1) {
		if (localFind < otherFind)
			return true;
		else
			return false;
	}

	int localLen = a->lowName.count();
	int otherLen = b->lowName.count();

	if (localLen < otherLen)
		return true;
	if (localLen > otherLen)
		return false;

	
	// Absolute tiebreaker to prevent loops
	if (a->fullPath < b->fullPath)
		return true;
	return false;
}



