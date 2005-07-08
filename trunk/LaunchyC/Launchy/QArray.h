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
	With a quick fix by me, the while loops were going too far and causing
	null references -- Josh
*/

/********************************************************************************************
* MOD-NAME      : QArray.h
* LONG-NAME     : QuickSort algorithm enabled CArray
*
* AUTHOR        : Martin Ziacek, Martin.Ziacek@swh.sk, http://www.swh.sk
* COPYRIGHT     : 1999 Martin Ziacek
* DEPARTMENT    : SWH s.r.o
* TELEPHONE     : +421 7 59684147
* CREATION-DATE : 1.5.1999 8:27:23
* SP-NO         : 
* FUNCTION      : Implementation of QuickSort algorithm as template for array class
*                 and template for simple function QuickSort()
* 
*********************************************************************************************/

#ifndef _QuickSortCArrayAndFunctionTemplate_7EB8E364_1A47_11d3_AFD1_0080ADB99E81_
#define _QuickSortCArrayAndFunctionTemplate_7EB8E364_1A47_11d3_AFD1_0080ADB99E81_

//////////////////////////////////////////////////////////////////////////
// QuickSort functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// QuickSortRecursive - core of algorithm, do not call it, use QuickSort,
// see below
template <class T> void QuickSortRecursive(T *pArr, int d, int h, BOOL bAscending)
{
	int i,j;
	T str;

	i = h;
	j = d;

	str = pArr[((int) ((d+h) / 2))];

	do {
		if (bAscending) {
			while ((j <= h) && (*pArr[j].get() < *str.get())) j++;
			while ((i >= 0) && (*pArr[i].get() > *str.get())) i--;
		} else {
			while (j <= h && *pArr[j].get() > *str.get()) j++;
			while (i >= 0 && *pArr[i].get() < *str.get()) i--;
		}

		if ( i >= j ) {

			if ( i != j ) {
				T zal;

				zal = pArr[i];
				pArr[i] = pArr[j];
				pArr[j] = zal;

			}

			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) QuickSortRecursive(pArr,d,i,bAscending);
	if (j < h) QuickSortRecursive(pArr,j,h,bAscending);
}

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// T *pArr			... pointer of array to sort
// int iSize		... size of array T *pArr
// BOOL bAscending	... if bAscending == TRUE, then sort ascending,
//						otherwise descending
//
// return TRUE if no error, error can be bad parameter, call ::GetLastError()
// if QuickSort returned FALSE
template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) {

		try {

			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr,low,high,bAscending);

		} catch (...) {
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} else {
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

//////////////////////////////////////////////////////////////////////////
// CQArray
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CQArray declaration

template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

//////////////////////////////////////////////////////////////////////////
// CQArray implementation

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// BOOL bAscending	... if bAscending == TRUE, then sort ascending, 
//						otherwise descending
//
template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1) {
		::QuickSort(GetData(),GetSize(),bAscending);
	}
}

#endif /* _QuickSortCArrayAndFunctionTemplate_7EB8E364_1A47_11d3_AFD1_0080ADB99E81_ */
