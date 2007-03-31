#include "stdafx.h"
#include "resource.h"
#include "Runny.h"
#include <tchar.h>
#include <string>

using namespace std;

void AddString(HWND hwnd) {
    int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT)) + 1;
    if(len > 0)
    {
		TCHAR* buff = (TCHAR*) malloc(sizeof(TCHAR) * len);
		GetDlgItemText(hwnd, IDC_EDIT, buff, len);

		wstring tmp = buff;
		free(buff);
		if (tmp == L"" || tmp.find(L"|") == string::npos) {
			MessageBox(hwnd, L"The proper format of a Runny command is \"<command name> | <program to run>\"", L"Error in command", MB_OK);
			return;
		}

		SendDlgItemMessage(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM) tmp.c_str());
    }
	SetDlgItemText(hwnd, IDC_EDIT, L"");
}

void RemoveString(HWND hwnd) {
	int sel = SendDlgItemMessage(hwnd, IDC_LIST, LB_GETCURSEL, 0, 0);
	if (sel != LB_ERR) {
		SendDlgItemMessage(hwnd, IDC_LIST, LB_DELETESTRING, sel, 0);
	}
}

void OnInitDialog(HWND hwnd) {

	// Add all of the strings
	for(int i = 0; i < Commands.size(); i++) {
		wstring tmp = Commands[i].name;
		tmp += L" | ";
		tmp += Commands[i].format;
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
		cmd c;
		c.name = TrimSides(str.substr(0, sepPos));
		c.format = TrimSides(str.substr(sepPos + 1));
		Commands.push_back(c);
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
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}