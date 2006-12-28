// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the Weby_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// Weby_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef Weby_EXPORTS
#define Weby_API  __declspec(dllexport)
#else
#define Weby_API  __declspec(dllimport)
#endif

#include "LaunchyPlugin.h"

extern "C" __declspec(dllexport) SearchResult* PluginGetIdentifiers (int* iNumResults);
extern "C" __declspec(dllexport) SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults);
extern "C" __declspec(dllexport) void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath);
extern "C" __declspec(dllexport) TCHAR* PluginGetRegexs(int* iNumResults);