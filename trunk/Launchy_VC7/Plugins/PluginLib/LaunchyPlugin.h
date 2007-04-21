#ifndef LAUNCHY_PLUGIN_H
#define LAUNCHY_PLUGIN_H

// Create a Win32 dll
// Use Unicode Character Set

#include <string>
#include <vector>
#include <map>
//#include "stdafx.h"

using namespace std;
typedef unsigned int uint;

/**
	Internal structure used for storage of plugin defined options
*/
extern map<wstring,wstring> Storage;

/**
	@brief When you want to add an index item to Launchy, a SearchResult is used.
	@warning This is meant for internal Launchy use
	*/
struct SearchResult {
	/// The shortened string that Launchy will search on
	TCHAR* DisplayString; // Must be set
	/// The full path (including filename) of the file pointed at if it is a file.
	/// Important: Make sure that FullPath is unique, e.g. append ".YourPluginName" to DisplayString
	//                  otherwise multiple files with the same name will get confused by Launchy
	TCHAR* FullPath;
	/// The location of your file, set to your Plugin Name if it is not for a file
	TCHAR* Location; // Can be NULL
	/// This is currently unused
	HICON DisplayIcon; // Make sure you 0 this on creation!!
};

/// Internally used by Launchy to free results after you have returned them
void FreeSearchResult (SearchResult* sr);

/** 
	@brief Use this function to quickly build SearchResults
	@see SearchResult
	@param DisplayString The shortened string that Launchy will search on
	@param FullPath	The full path (including filename) of the file pointed at if it is a file.
	  Important: Make sure that FullPath is unique, e.g. append ".YourPluginName" to DisplayString 
	                  otherwise multiple files with the same name will get confused by Launchy
    @param Location The location of your file, set to your Plugin Name if it is not for a file
	@param icon Currently unused
	@return The composed SearchResult
*/
SearchResult makeResult(wstring DisplayString, wstring FullPath, wstring Location, HICON icon);

/**
	@brief Convert a vector of SearchResults into a C SearchResult array
	@see makeResult
	@param results The vector of search results to convert
	@return The array of SearchResults
*/
SearchResult* ResultVectorToArray(vector<SearchResult> results);

/**
	@brief Convert a list of TCHAR strings to a vector of wstrings
	@see StringVectorToTCHAR()
	@param numStrings The number of strings in the TCHAR to convert
	@param Strings The TCHAR* to convert
	@return The vector of strings
	*/
vector<wstring> TCHARListToVector(int numStrings, const TCHAR* Strings);

/**
	@brief Convert a wstring to a TCHAR*
	@param str The wstring to convert
	@return The THCAR* representation of the string
*/
TCHAR* string2TCHAR(wstring str);

/**
	@brief Convert a vector of wstrings to a TCHAR*
	@see TCHARListToVector()
	@param Strings The vector of wstrings to convert
	@return The TCHAR* representation of the string vector
	*/
TCHAR* StringVectorToTCHAR( vector<wstring>& Strings);

/**
	@brief Create a generated ID number from your plugin name
	@warning This is meant for internal Launchy use
	@param str Your plugin name
	@return The generated ID
*/
unsigned long GenerateID(wstring str);

/**
	@brief Convert a map of string->string into a TCHAR*
	@warning This is meant for internal Launchy use
	@param m The map to convert
	@return The serialized map as a TCHAR*
*/
TCHAR* SerializeStringMap(map<wstring,wstring> m);

/**
	@brief Have Launchy store a string value in its archive
	@param name The identifier of the value to store
	@param value The value of the identifier
	@return void
*/
void StoreString(wstring name, wstring value);

/**
	@brief Have Launchy retrieve a string value from its archive
	@param name The identifier of the value to retrieve
	@return The string associated with the identifier in Launchy's archive
*/
wstring RetrieveString(wstring name);

/**
	@brief Have Launchy store an int into its archive
	@param name The identifier of the value to store
	@param value The value of the identifier
	@return void
*/
void StoreInt(wstring name, int value);
/**
	@brief Have Launchy retrieve an int from its archive
	@param name The identifier of the value to retrieve
	@param exists This value is set to whether or not the identifier was found in the archive
	@return The retrieved integer
*/
int RetrieveInt(wstring name, bool& exists);

/**
	@brief Have Launchy store a float into its archive
	@param name The identifier of the value to store
	@param value The value of the identifier
	@return void
*/
void StoreFloat(wstring name, float value);

/**
	@brief Have Launchy retrieve a float from its archive
	@param name The identifier of the value to retrieve
	@param exists This value is set to whether or not the identifier was found in the archive
	@return The retrieved float
*/
float RetrieveFloat(wstring name, bool& exists);

/**
@brief This event is called when Launchy is about to archive its options
@brief If you have not yet performed your StoreInt/Float/String functions
@brief this is the time to do so
*/
void PluginSaveOptions();

extern "C" {
/**
	@brief Every time Launchy reindexes it clears the list of Plugin provided files and asks for them again
	@brief This function is where you tell Launchy what "files" you would like it to index
	@see SearchResult
	@see makeResult()
	@see ResultVectorToArray()
	@param iNumResults Be sure to set the value of iNumResults to the number of results you have returned in the SearchResult*
	@return You must return a list of SearchResults
*/
__declspec(dllexport) SearchResult* PluginGetIdentifiers (int* iNumResults);
/**
	@brief If your plugin would like to add some items to the main Launchy index, such as "Google" or "Amazon" then this is the place to do it.
	@brief PluginGetIdentifiers() is called at plugin initialization time and each time the index is reloaded.  
	@see TCHARListToVector()
	@param NumStrings The number of strings in parameter "Strings"
	@param Strings Each time the user hits the TAB key the user's current string is appended to Strings
	@param FinalString This is the user's current string, e.g. for "Google | Launchy" "Google" would be in Strings and FinalString would equal "Launchy"
	@param NumResults Set this value to the number of SearchResults that you are returning
	@return The set of SearchResults for this user search
*/
__declspec(dllexport) SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
/**
	@brief When a user presses TAB on a file owned by the normal Launchy index, Launchy asks each of the plugins if they would like to provide SearchResults
	@brief for this file.  For instance, Explory will return results if the file in question is a .lnk to a directory such as My Documents
	@see PluginGetIdentifiers()
	@param NumStrings The number of strings in parameter "Strings"
	@param Strings Each time the user hits the TAB key the user's current string is appended to Strings
	@param FinalString This is the user's current string, e.g. for "Google | Launchy" "Google" would be in Strings and FinalString would equal "Launchy"
	@param NumResults Set this value to the number of SearchResults that you are returning
	@param FullPath The FullPath of the SearchResult in question
	@return The set of SearchResults for this user search
*/
__declspec(dllexport) SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
/**
	@brief This is called when the user has pressed enter on a SearchResult that this plugin created
	@brief It is up to the plugin to do something with the file, such as execute it
	@see PluginGetIdentifiers()
	@see PluginUpdateSearch()
	@see SearchResult
	@see makeResults()
	@param NumStrings The number of strings in parameter "Strings"
	@param Strings Each time the user hits the TAB key the user's current string is appended to Strings
	@param FinalString This is the user's current string, e.g. for "Google | Launchy" "Google" would be in Strings and FinalString would equal "Launchy"
	@param FullPath This is the FullPath value of the SearchResult supplied by the plugin
	@return nothing
*/
__declspec(dllexport) void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath);
/**
	@brief If your plugin would like to claim ownership of all user input that matches a regular expression, then provide Launchy with a list of regular expressions here
	@brief  An example is Weby which owns all searches that include www or http
	@brief The regular expressions are PERL regular expressions to be interpreted by the BOOST regex library
	@see StringVectorToTCHAR()
	@warning Be sure to escape your \\'s with \\'s, in other words an \ is actually \\\\ in your returned string
	@param iNumResults The number of regular expressions you are returning
	@return The list of regular expressions	
*/
__declspec(dllexport) TCHAR* PluginGetRegexs(int* iNumResults);
/**
	@brief The character used to separate input items when the user hits the TAB key, typically L'|'
	@return The separator character
*/
__declspec(dllexport) TCHAR* PluginGetSeparator();
/**
	@brief This is used interally by Launchy to free the SearchResults that you have returned
	@warning This function is internal to Launchy
*/
__declspec(dllexport) void PluginFreeResults (SearchResult*, int num);
/**
	@brief This is used internally by Launchy to free any strings that you have returned
	@warning This function is internal to Launchy
*/
__declspec(dllexport) void PluginFreeStrings (TCHAR* str);
/**
	@brief This is called at load when Launchy would like to know the name of your plugin
	@warning Any name is fine, but please try to keep it short and do not use the same name
	@warning as other known plugins
	@return The name of your plugin
*/
__declspec(dllexport) TCHAR* PluginGetName();
/**
	@brief Launchy will list a brief description of your plugin in the "Plugins" dialog, this is
	@brief where Launchy gets that description
	@return A very brief description of your plugin
*/
__declspec(dllexport) TCHAR* PluginGetDescription();
/**
	@brief This function is called when the plugin is first loaded
	
*/
__declspec(dllexport) void PluginInitialize();
/**
	@brief This function is called just before the plugin is unloaded
*/
__declspec(dllexport) void PluginClose();
/**
	@brief This function is called when the user has pressed the "plugin options" button on your plugin
	@brief You should create a dialog window to interface with this user from this function
	@brief An example plugin that has such a dialog is Runny
	@param hwnd This is Launchy's hwnd and should be the parent window of your window
*/
__declspec(dllexport) void PluginCallOptionsDlg(HWND hwnd);
/**
	@brief Called from Launchy when it wants the plugin's information to be archived
	@warning This function is internal to Launchy
*/
__declspec(dllexport) void PluginGetStorage(int* NumItems, TCHAR** ItemNames, TCHAR** ItemValues);
/**
	@brief Called from Launchy when it wants to tell the plugin what information it has archived
	@warning This function is internal to Launchy
*/
__declspec(dllexport) void PluginSetStorage(int NumItems, TCHAR* ItemNames, TCHAR* ItemValues);
/**
	@brief Used by Launchy to determine if it should call your plugin's PluginCallOptionsDlg function
	@return Return true if your plugin has a functioning PluginCallOptionsDlg function, else false
*/
__declspec(dllexport) bool PluginHasOptionsDlg(void);
}




/*
extern "C" __declspec(dllexport) SearchResult* PluginGetIdentifiers (int* iNumResults);
extern "C" __declspec(dllexport) SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath);
extern "C" __declspec(dllexport) TCHAR* PluginGetRegexs(int* iNumResults);
extern "C" __declspec(dllexport) TCHAR* PluginGetSeparator();
extern "C" __declspec(dllexport) void PluginFreeResults (SearchResult*, int num);
extern "C" __declspec(dllexport) void PluginFreeStrings (TCHAR* str);
extern "C" __declspec(dllexport) TCHAR* PluginGetName();
extern "C" __declspec(dllexport) TCHAR* PluginGetDescription();
extern "C" __declspec(dllexport) void PluginInitialize();
extern "C" __declspec(dllexport) void PluginClose();
extern "C" __declspec(dllexport) void PluginCallOptionsDlg(HWND hwnd);
extern "C" __declspec(dllexport) void PluginGetStorage(int* NumItems, TCHAR** ItemNames, TCHAR** ItemValues);
extern "C" __declspec(dllexport) void PluginSetStorage(int NumItems, TCHAR* ItemNames, TCHAR* ItemValues);
extern "C" __declspec(dllexport) bool PluginHasOptionsDlg(void);
*/
#endif