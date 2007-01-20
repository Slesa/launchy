// Foxy.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Foxy.h"

#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <string>
#include <vector>
#include <assert.h>
#include <shlobj.h>
#include <map>
#include "UTF16File.h"
#include <boost/regex.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CFoxyApp

BEGIN_MESSAGE_MAP(CFoxyApp, CWinApp)
END_MESSAGE_MAP()


// CFoxyApp construction

CFoxyApp::CFoxyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFoxyApp object

CFoxyApp theApp;


// CFoxyApp initialization

BOOL CFoxyApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


bool PluginOwnsSearch (TCHAR* txt) 
{
	return false;
}

BOOL GetShellDir(int iType, wstring& szPath)
{
	HINSTANCE hInst = ::LoadLibrary( L"shell32.dll" );
	if ( NULL == hInst )
	{
		return FALSE;
	}

	HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPWSTR );


	pfnSHGetFolderPath = reinterpret_cast<HRESULT (__stdcall *)( HWND, int, HANDLE, DWORD, LPWSTR )>( GetProcAddress( hInst, "SHGetFolderPathW" ) );

	if ( NULL == pfnSHGetFolderPath )
	{

		FreeLibrary( hInst ); // <-- here
		return FALSE;
	}

	TCHAR tmp[_MAX_PATH];
	HRESULT hRet = pfnSHGetFolderPath( NULL, iType, NULL, 0, tmp );
	szPath = tmp;
	FreeLibrary( hInst ); // <-- and here
	return TRUE;
	return 0;
}

bool DirectoryExists( const wstring& directory )
{

	bool result = false;

	if( directory.length( ) )
	{
		wstring indir( directory );

		TCHAR buff[ _MAX_PATH ];
		::GetCurrentDirectory(_MAX_PATH, buff );

		if( ::SetCurrentDirectory( indir.c_str() ) )
			result = true;

		::SetCurrentDirectory( buff );

	}

	return result;
}

/*
	sourceDirectory = directory to search
	files = files found in directory
	DirsOnly = only return directories
*/
bool EnumFilesInDirectory(const wstring& sourceDirectory, vector<wstring>& files, bool DirsOnly )
{

	wstring source( sourceDirectory );
	source += L"\\";
	wstring sourceFiles( source + L"*.*" );
	bool result = true;
	WIN32_FIND_DATA ff;

	HANDLE findhandle = FindFirstFile( sourceFiles.c_str(), &ff );
	if( findhandle != INVALID_HANDLE_VALUE )
	{
		BOOL res = TRUE;
		while( res )
		{
			wstring file;
			file = ff.cFileName;
			if (DirsOnly) {
				if (!(ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					res = FindNextFile(findhandle, &ff);
					continue;
				} else {
				if (file != L"." && file != L"..")
					files.push_back(file);					
				}
			} else {
				if (file != L"." && file != L"..")
					files.push_back(file);
			}

			res = FindNextFile( findhandle, &ff );
		}

		FindClose( findhandle );
	}
	else
	{
		// Set error message
		result = FALSE;
	}

	return result;
}

struct BookMark {
	wstring dest;
	wstring shortcut;
	wstring name;
};

void LoadBookMarks(wstring filename, vector<BookMark>& marks) {
	CUTF16File inifile;
	if ( inifile.Open(filename.c_str(), CFile::modeRead) == 0) return;
	
	CString line;
	wstring wline;

	boost::wregex regex_url(L"<a href=\"(.*?\")",boost::regex::perl|boost::regex::icase);	
	boost::wregex regex_urlname(L"\">(.*?)<\/A>", boost::regex::perl|boost::regex::icase);
	boost::wregex regex_shortcut(L"SHORTCUTURL=\"(.*?)\"", boost::regex::perl|boost::regex::icase);
	boost::wregex regex_postdata(L"POST_DATA", boost::regex::perl|boost::regex::icase);

	while (inifile.ReadString(line)) {
		wline = line.GetBuffer();
		boost::wsmatch what;
		if(boost::regex_search(wline, what, regex_url)) {
			BookMark mark;
			mark.dest = what[1];
			CString tmp = mark.dest.c_str();
			tmp.TrimRight(L"\"");
			mark.dest = tmp;
			if (boost::regex_search(wline, what, regex_urlname)) {
				mark.name = what[1];
				if (boost::regex_search(wline, what, regex_postdata))
					continue;
				if (boost::regex_search(wline, what, regex_shortcut)) {
					mark.shortcut = what[1];
				} else {
					mark.shortcut = L"";
				}
				marks.push_back(mark);
			}
		}		

	}

	inifile.Close();
}

map<wstring, wstring> Shortcuts;
map<wstring, wstring> Bookmarks;

SearchResult* PluginGetIdentifiers (int* iNumResults)
{
	Shortcuts.clear();
	// Find the bookmarks file(s)
	wstring dir = L"";
	GetShellDir(CSIDL_APPDATA, dir);
	dir += L"\\Mozilla\\Firefox\\Profiles\\";

	vector<wstring> ProfileDirs;
	EnumFilesInDirectory(dir, ProfileDirs, true);


	vector<BookMark> BookMarks;

	for(int i = 0; i < ProfileDirs.size(); i++) {
		wstring bookmarkfile = dir;
		bookmarkfile += ProfileDirs[i];
		bookmarkfile += L"\\bookmarks.html";

		LoadBookMarks(bookmarkfile, BookMarks);
	}

	vector<SearchResult> results;

	for(int i = 0; i < BookMarks.size(); i++) {
		if (BookMarks[i].shortcut != L"") {
			Shortcuts[BookMarks[i].shortcut] = BookMarks[i].dest;
			Shortcuts[BookMarks[i].name] = BookMarks[i].dest;
		} else {
			Bookmarks[BookMarks[i].name] = BookMarks[i].dest;
		}
		results.push_back(makeResult(BookMarks[i].name, BookMarks[i].dest, L"Firefox Bookmark", NULL));
	}

	*iNumResults = results.size();
	return ResultVectorToArray(results);
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	*iNumResults = 0;

	return NULL;
}


SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) {
	*NumResults = 0;
	return NULL;
}
SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) 
 {
	*NumResults = 0;
	return NULL;
}


void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {
	vector<wstring> VStrings = TCHARListToVector(NumStrings, Strings);

	wstring url = L"";

	wstring fs = FinalString;

	// Doing a search
	if (NumStrings > 0 && fs != L"") {
		if (Shortcuts.count(VStrings[0]) != 0) {
			url = Shortcuts[VStrings[0]];
			boost::wregex reg(L"%s");
			url = boost::regex_replace(url, reg, fs);
		}
	}

	// No search text, just go to the main site
	else if (NumStrings > 0) {
		if (Bookmarks.count(VStrings[0]) != 0){
			url = Bookmarks[VStrings[0]];
		} 	
	}

	// Just a bookmark
	else {
		url = FullPath;
	}

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = url.c_str();
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);
	
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L" | ";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = L"Foxy";
	return string2TCHAR(tmp);
}