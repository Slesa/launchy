#include "stdafx.h"
#include "resource.h"
#include "Runny.h"
#include <tchar.h>
#include <string>

using namespace std;

void AddString(HWND hwnd) {
    int nlen = GetWindowTextLength(GetDlgItem(hwnd, IDC_ENAME)) + 1;
	int clen = GetWindowTextLength(GetDlgItem(hwnd, IDC_ECOMMAND)) + 1;

	if(nlen > 1 && clen > 1)
    {
		TCHAR* buff = (TCHAR*) malloc(sizeof(TCHAR) * nlen);
		GetDlgItemText(hwnd, IDC_ENAME, buff, nlen);

		wstring name = buff;
		free(buff);


		buff = (TCHAR*) malloc(sizeof(TCHAR) * clen);
		GetDlgItemText(hwnd, IDC_ECOMMAND, buff, clen);

		wstring command = buff;
		free(buff);

		wstring combined = name;
		combined += L" | ";
		combined += command;

		SendDlgItemMessage(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM) combined.c_str());
    }

	SetDlgItemText(hwnd, IDC_ENAME, L"");
	SetDlgItemText(hwnd, IDC_ECOMMAND, L"");
}

void RemoveString(HWND hwnd) {
	int sel = SendDlgItemMessage(hwnd, IDC_LIST, LB_GETCURSEL, 0, 0);
	if (sel != LB_ERR) {
		SendDlgItemMessage(hwnd, IDC_LIST, LB_DELETESTRING, sel, 0);
	}
}

void OnInitDialog(HWND hwnd) {

	// Add all of the strings
	for(map<wstring,wstring>::iterator it = Commands.begin(); it != Commands.end(); ++it) {
		wstring tmp = it->first;
		tmp += L" | ";
		tmp += it->second;
		SendDlgItemMessage(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM) tmp.c_str());
	}
}

wstring TrimSides(wstring str) {
	if (str.find_first_not_of(L" \t\n") == string::npos) return L"";
	str = str.substr(str.find_first_not_of(L" \t\n"));
	str = str.substr(0, str.find_last_not_of(L" \t\n")+1);
	return str;
}

void StoreCommands(HWND hwnd) {
	Commands.clear();
	int numStrings = SendDlgItemMessage(hwnd, IDC_LIST, LB_GETCOUNT, 0, 0);
	for(int i = 0; i < numStrings; i++) {
		int len = SendDlgItemMessage(hwnd, IDC_LIST, LB_GETTEXTLEN, i, 0) + 1;
		TCHAR* buff = (TCHAR*) malloc(sizeof(TCHAR) * len);
		SendDlgItemMessage(hwnd, IDC_LIST, LB_GETTEXT, i, (LPARAM) buff);
	
		wstring str = buff;
		free(buff);

		int sepPos = str.find(L"|", 0);
		if (sepPos == string::npos) continue;
		Commands[TrimSides(str.substr(0,sepPos))] = TrimSides(str.substr(sepPos + 1));

	}
}

BOOL CALLBACK OptionsDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	
    switch(Message)
    {
        case WM_INITDIALOG:
			OnInitDialog(hwnd);
	        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
					StoreCommands(hwnd);
                    EndDialog(hwnd, IDOK);
	                break;
                case IDC_BADD:
					AddString(hwnd);
					break;
				case IDC_BREMOVE:
					RemoveString(hwnd);
					break;
				case IDCANCEL:
					EndDialog(hwnd, IDOK);
					break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}