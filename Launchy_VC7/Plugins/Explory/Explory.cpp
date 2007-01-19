// Explory.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Explory.h"
#include "LaunchyPlugin.h"
#include <shellapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <assert.h>
#include <algorithm>

#include "stdafx.h"
#include <Shlobj.h>
#include <crtdbg.h>
#include <atldef.h>
#include <atlconv.h>



using namespace std;

wstring TakeOverPath;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

bool PluginOwnsSearch (TCHAR* txt) 
{
	return false;
}

SearchResult* PluginGetIdentifiers (int* iNumResults)
{
	vector<SearchResult> results;
	for(int i = L'a'; i <= L'z'; i++) {
		wstring tmp;
		tmp += (TCHAR) i;
		tmp += L":";
		results.push_back(makeResult(tmp, L"", L"", NULL));
	}

	*iNumResults = 26;
	return ResultVectorToArray(results);
	/*
	*iNumResults = 0;
	return NULL;
	*/
}

TCHAR* PluginGetRegexs(int* iNumResults)
{

	*iNumResults = 0;
	return NULL;
	
}


HRESULT ResolveShortcut(const TCHAR* LnkFile, TCHAR* FilePath,
                        TCHAR* LnkDesc, TCHAR* WorkDir) 
{ 
    CoInitialize(NULL);
    HRESULT hres; 
    IShellLink* psl; 
    WIN32_FIND_DATA wfd; 
    TCHAR strfilepath[MAX_PATH];     
    TCHAR strlnkdesc[INFOTIPSIZE];
    TCHAR strworkdir[MAX_PATH];

    USES_CONVERSION;
 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, 
            CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf;         
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf); 
        if (SUCCEEDED(hres)) 
        { 
            hres = ppf->Load(LnkFile, STGM_READ); 
            if (SUCCEEDED(hres)) 
            {               
                hres = psl->Resolve(GetDesktopWindow(), 0); 
                if (SUCCEEDED(hres)) 
                { 
                    hres = psl->GetPath(strfilepath,MAX_PATH, &wfd, 
                                           SLGP_UNCPRIORITY );
                    
                    if (SUCCEEDED(hres)) 
                    {      
						_tcscpy(FilePath, strfilepath); 
                        hres = psl->GetDescription(strlnkdesc,INFOTIPSIZE);
                    }

                    if (SUCCEEDED(hres)) 
                    {
                        _tcscpy(LnkDesc,strlnkdesc);
                        hres = psl->GetWorkingDirectory(strworkdir,MAX_PATH);
                    }

                    if (SUCCEEDED(hres)) 
                    {
                        _tcscpy(WorkDir,strworkdir);
                    }
                } 
            }         
            ppf->Release(); 
        }     
        psl->Release(); 
    } 
    CoUninitialize();
    return hres; 
} 

bool FileExists( const wstring& file )

{
	bool result = true;

	if( file.length() )
	{
		wstring infile( file );
		HANDLE filehandle = ::CreateFile( 
								infile.c_str(),
								GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
							);

		if( filehandle == INVALID_HANDLE_VALUE )
			result = false;
		else
			CloseHandle( filehandle );
	}
	else
	{
		result = false;
	}

	return result;
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
bool EnumFilesInDirectory(const wstring& sourceDirectory, vector<wstring>& files )
/* ============================================================
	Function :		CDiskObject::EnumFilesInDirectoryWithFilter
	Description :	Enumerates all files matching "strFilter" in 
					the directory "sourceDirectory", and adds 
					them to the "CStringArray" "files". Will not 
					add subdirectories.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will contain errors
	Parameters :	CString strFilter		-	the file name filter
					CString sourceDirectory	-	source directory
					CStringArray& files		-	output array
					
	Usage :			Call to get a filtered list of files from 
					a directory.

   ============================================================*/
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
			// We only want files
		//	if( !( ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		//	{
				BOOL    added = FALSE;
				wstring file;
				file = ff.cFileName;

//					file = sourceDirectory + file;

				// Return the array alphabetically ordered
				if (file != L"." && file != L"..")
					files.push_back(file);

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

bool FixPath(wstring& path) {
	// Fix unix paths
	std::replace( path.begin(), path.end(), L'/', L'\\' );

	// Remove double slashes
	while(true) {
		size_t p = path.find(L"\\\\");
		if (p == string::npos) break;
		path.replace(p, 2, L"\\");
	}


	// Are we pointing at a real destination?
	if (DirectoryExists(path)) {
		if (path[path.length()-1] != L'\\')
			path += L'\\';
		return true;
	} else if (path.at(path.length() - 1) == L'\\') {
		// It says its a directory but it's not, must be a file
		path = path.substr(0, path.length() - 1);
	}

	return FileExists(path);
	
}

SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) {
	vector<wstring> VStrings = TCHARListToVector(NumStrings, Strings);

	wstring lowFinal(FinalString);
	transform(lowFinal.begin(), lowFinal.end(), lowFinal.begin(), tolower);

	// Build the path
	wstring dirs = L"";
	wstring path = L"";

	// Did we take this string over from a PluginFileOptions?
	if (VStrings.size() > 0 && VStrings[0].length() > 0 && VStrings[0].at(1) != L':') {
		dirs += TakeOverPath;
		dirs += L"\\";
		for(int i = 1; i < NumStrings; i++) {
			dirs += VStrings[i];
			dirs += L"\\";
		}
	} else {
		for(int i = 0; i < NumStrings; i++) {
			dirs += VStrings[i];
			dirs += L"\\";
		}
	}

	path = dirs;
	path += FinalString;

	vector<wstring> matches;

	bool ValidPath = FixPath(path);

	if(ValidPath && DirectoryExists(path)) {
		matches.push_back(FinalString);
		EnumFilesInDirectory(path, matches);
	} else if (ValidPath) {
		matches.push_back(FinalString);
	}
	else {
		EnumFilesInDirectory(dirs, matches);
	}

	vector<SearchResult> results;
	for(int i = 0; i < matches.size(); i++) {
		if (!ValidPath) {
			wstring pre = matches[i].substr(0, _tcslen(FinalString));
			transform(pre.begin(), pre.end(), pre.begin(), tolower);
			if (pre != lowFinal)
				continue;
		}
		wstring tmp = path;
		if (!ValidPath) {
			tmp = tmp.substr(0, tmp.rfind(L"\\") + 1);
			tmp += matches[i];
		}	
		results.push_back(makeResult(matches[i], tmp, tmp, NULL));
	}
	*NumResults = results.size();
	return ResultVectorToArray(results);
}

SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings,const TCHAR* FinalString, int* NumResults) {
	wstring path = FullPath;

	if (path.rfind(L".directory") != string::npos) {
		path += L"\\";
		TakeOverPath = path.substr(0, path.length() - 11);
		// Looks like we have a directory!
		int numRes;
		SearchResult* sr = PluginUpdateSearch(1, TakeOverPath.c_str(), FinalString, NumResults);
		return sr;	
	}
	if (path.rfind(L".lnk") != string::npos) {
		TCHAR strfilepath[MAX_PATH];
		TCHAR strworkdir[MAX_PATH];
		TCHAR strlnkdesc[INFOTIPSIZE];
		if (SUCCEEDED(ResolveShortcut(FullPath, strfilepath, strlnkdesc, strworkdir))) {
			TakeOverPath = strfilepath;
			SearchResult* sr = PluginUpdateSearch(1, strfilepath, FinalString, NumResults);
			return sr;
		}
	}
	*NumResults = 0;
	return NULL;	
}
	

void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {


	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = FullPath;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);
	
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L"\\";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = L"Explory";
	return string2TCHAR(tmp);
}