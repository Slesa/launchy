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

/* ==========================================================================
	Class :			CDiskObject

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-12

	Purpose :		"CDiskObject" encapsulates several high-level file- 
					and directory operations. 	

	Description :	All error handling is managed internally, and the 
					different API-functions return a "BOOL" to signal success 
					or failure. In case of failure, "FALSE" returned, the 
					member function "GetErrorMessage" can be called to get a
					"CString" with the error message.

	Usage :			The class is used by creating a "CDiskObject", and 
					calling the methods of the class. No other setup is 
					necessary. If a "CWnd"-pointer is submitted to the "ctor", 
					"CDiskObject" will give feedback by calling "SetWindowText" 
					with, for example, filenames during processing.This 
					means that a "CStatic" (or other appropriate control) can 
					be set up to display the file currently copied, for 
					example.
   ========================================================================
	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-14

	Purpose :		1. Added const-correctness for inparams.
					2. Replacing "/" with "\" when qualifying file names/
					   directories
					3. Skipping directory qualify when the path starts 
					   with \\. Can't add a drive letter then... The path 
					   is assumed to be fully qualified.
					4. Added pragma to get rid of C4706 assignment warning.
					5. Added "RemoveFile" for reasons of symmetry.
   ========================================================================
	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-14

	Purpose :		1. Added default ctor
					2. Changed name of "EnumDirectories" to "EnumAllDirectories" 
					   and added a non-recursive "EnumDirectories".
   ========================================================================
	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-19

	Purpose			1. Checking FILE_ATTRIBUTE_DIRECTORY as a flag instead 
					   of a value.
   ========================================================================
	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-08-04

	Purpose			1. Changed drive length from _MAX_DRIVE to _MAX_PATH 
					   in UNC parsing. (jkaspzyk)
					2. Added UNICODE macro to SetSystemErrorMessage 
					   declaration. (nuhi)
   ========================================================================
	Author :		Allen Rossouw

	Date :			2005-04-16

	Purpose			1.	Sorting the output in EnumFilesInDirectoryWithFilter
					2.	Added a DirectoryExists function, returning TRUE if
						the inparam directory exists.
					3.	Added a CopyFile with an extra parameter for a new 
						filename
					4.	Added a FileInformation function, returning a 
						BY_HANDLE_FILE_INFORMATION for the inparam file.
   ========================================================================
	Author :		Johan Rosengren

	Date :			2005-05-15

	Purpose			1.	Corrected bug in CopyFile
					2.	Added RenameFile 
   ========================================================================*/

#include "stdafx.h"
#include <tchar.h>
#include "DiskObject.h"

#pragma warning( disable : 4706 )

//#define Trigger( a)	if( m_feedbackWindow ) m_feedbackWindow->SetWindowText( a );
#define Trigger(a) 
////////////////////////////////////////////////////////////////////
// CDiskObject construction/destruction/initialization

CDiskObject::CDiskObject()
/* ============================================================
	Function :		CDiskObject::CDiskObject
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none
					
	Usage :			Should normally be created on the stack.

   ============================================================*/
{
	m_feedbackWindow = NULL;
}

CDiskObject::CDiskObject( CWnd* feedbackWindow )
/* ============================================================
	Function :		CDiskObject::CDiskObject
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* hwndFeedback	-	"CWnd" to feedback 
											window

 	Usage :			Should normally be created on the stack.

  ============================================================*/
{
	m_feedbackWindow = feedbackWindow;
}

CDiskObject::~CDiskObject( )
/* ============================================================
	Function :		CDiskObject::~CDiskObject
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Should normally be created on the stack.

   ============================================================*/
{
}

////////////////////////////////////////////////////////////////////
// CDiskObject operations
//

// File operations

BOOL CDiskObject::CopyFiles( const CString& sourceDirectory,const CString& destDirectory )
/* ============================================================
	Function :		CDiskObject::CopyFiles
	Description :	The member copies all files from 
					"sourceDirectory" to "destDirectory". 
					Subdirectories will not be copied.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will return 
												errors.
	Parameters :	CString sourceDirectory	-	Copy from. If
												empty, current.
					CString destDirectory	-	Copy to. If 
												empty, current.
					
	Usage :			Call to copy the files from one directory to 
					another.

   ============================================================*/
{
	ClearError( );
	CString source( sourceDirectory );
	CString dest( destDirectory );
	QualifyPath( source );
	QualifyPath( dest );

	// First, we enumerate all files
	CStringArray files;
	BOOL result = EnumFilesInDirectory( source, files );
	if( result )
	{
		// Create the destination directory, if necessary
		if( ( result = CreateDirectory( dest ) ) )
		{
			INT_PTR max = files.GetSize( );
			for( INT_PTR t = 0 ; t < max ; t++ )
			{
				// Copy the files
				CString file;
				file = files[ t ];
				Trigger( file );
				if( !( result = ::CopyFile( source + file, dest + file, FALSE ) ) )
				{
					// Set error message
					SetSystemErrorMessage( ::GetLastError( ), 
											source + 
											file + 
											_T( " -> " ) + 
											dest + 
											file );
					t = max;
				}
			}
		}
	}

	return result;
}

BOOL CDiskObject::CopyFiles( CStringArray& files,const CString& destDirectory )
/* ============================================================
	Function :		CDiskObject::CopyFiles
	Description :	The function copies the files in the 
					"CStringArray" "files" to the directory 
					"destDirectory". Existing files will be
					overwritten. The destination will be 
					created if it doesn't already exist.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will return 
												errors.
	Parameters :	CStringArray& files		-	a string array 
												with the files 
												to copy
					CString destDirectory	-	destination
					
	Usage :			Copy a list of files to a directory.

   ============================================================*/
{
	ClearError( );
	CString dest( destDirectory );

	BOOL result = TRUE;
	if( files.GetSize( ) )
	{
		QualifyPath( dest );

		// Create destination, if necessary
		if( ( result = CreateDirectory( dest ) ) )
		{
			INT_PTR max = files.GetSize( );
			for( INT_PTR t = 0 ; t < max ; t++ )
			{
				// Loop and copy the files
				CString file;
				file = files[ t ];
				if( file.GetLength( ) )
				{
					Trigger( file );
					QualifyFile( file );

					// Create destination filename
					CString to = dest + GetFileName( file );
					if( !( result = ::CopyFile( file, to, FALSE ) ) )
					{
						// Set error message
						SetSystemErrorMessage( ::GetLastError( ),
												file + 
												_T( " -> " ) + 
												dest + 
												file );
						t = max;
					}
				}
			}
		}
	}
	else
	{
		SetInternalErrorMessage( );
		result =  FALSE;
	}

	return result;
}

BOOL CDiskObject::CopyFile( const CString& sourceFile,const CString& destDirectory )
/* ============================================================
	Function :		CDiskObject::CopyFile
	Description :	Will copy "sourceFile" to "destDirectory". 
					An existing file will be overwritten. The 
					directory will be created if it doesn't exist.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will contain 
												errors
	Parameters :	CString sourceFile		-	file to copy
					CString destDirectory	-	destination
					
	Usage :			Call to copy a file to a directory.

   ============================================================*/
{
	ClearError( );
	CString source( sourceFile );
	CString dest( destDirectory );

	BOOL result = TRUE;
	if( sourceFile.GetLength( ) )
	{
		QualifyFile( source );
		QualifyPath( dest );

		// Creating destDirectory if necessary.
		if( ( result = CreateDirectory( dest ) ) )
		{
			CString filePart = GetFileName( source );
			// Copy the file
			Trigger( filePart );
			if( !( result = ::CopyFile( source, dest + filePart, FALSE ) ) )
				SetSystemErrorMessage( ::GetLastError( ), source );
		}
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::CopyFile( const CString& sourceFile,const CString& destDirectory, const CString& destFile )
/* ============================================================
	Function :		CDiskObject::CopyFile
	Description :	Will copy "sourceFile" to "destDirectory" 
					with the new name "destFile". 
					An existing file will be overwritten. The 
					directory will be created if it doesn't exist.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will contain 
												errors
	Parameters :	CString sourceFile		-	file to copy
					CString destDirectory	-	destination
					CString destFile		-	destination file name 
												(in "destDirectory")
					
	Usage :			Call to copy a file to a directory.

   ============================================================*/
{
	ClearError( );
	CString source( sourceFile );
	CString destDir( destDirectory );
	CString dest( destFile );

	BOOL result = TRUE;
	if( sourceFile.GetLength( ) )
	{
		QualifyFile( source );
		QualifyFile( dest );
		QualifyPath( destDir );

		// Creating destDirectory if necessary.
		if( ( result = CreateDirectory( destDir ) ) )
		{
			// Copy the file
			Trigger( dest );
			if( !( result = ::CopyFile( source, dest, FALSE ) ) )
				SetSystemErrorMessage( ::GetLastError( ), source );
		}
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::RemoveFile( const CString& sourceFile )
/* ============================================================
	Function :		CDiskObject::RemoveFile
	Description :	Will remove "sourceFile".
	Access :		Public
					
	Return :		BOOL						-	"TRUE" if 
													removed ok
	Parameters :	const CString& sourceFile	-	File to
													remove
					
	Usage :			Call to delete a file. Added for reasons 
					of symmetry.

   ============================================================*/
{
	ClearError( );
	CString source( sourceFile );

	BOOL result = TRUE;
	if( sourceFile.GetLength( ) )
	{
		QualifyFile( source );

		if( !( result = ::DeleteFile( source ) ) )
			SetSystemErrorMessage( ::GetLastError( ), source );
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

////////////////////////////////////////
// Directory operations
//

BOOL CDiskObject::DirectoryExists( const CString& directory )
/* ============================================================
	Function :		CDiskObject::DirectoryExists
	Description :	Returns "TRUE" if the directory "directory"
					exists
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if found
	Parameters :	CString directory	-	directory to check

	Usage :			Call to check for directory existence.

   ============================================================*/
{
	ClearError( );

	BOOL result = FALSE;

	if( directory.GetLength( ) )
	{
		CString indir( directory );
		QualifyPath( indir );

		TCHAR buff[ _MAX_PATH ];
		::GetCurrentDirectory(_MAX_PATH, buff );

		if( ::SetCurrentDirectory( indir ) )
			result = TRUE;

		::SetCurrentDirectory( buff );

	}
	else
	{
		SetInternalErrorMessage( );
	}

	return result;
}

BOOL CDiskObject::CreateDirectory( const CString& directory )
/* ============================================================
	Function :		CDiskObject::CreateDirectory
	Description :	Will recursively create the directory 
					"directory".
	Access :		Public

	Return :		BOOL				-	"TRUE" if OK. 
											"GetErrorMessage" 
											will get an 
											error string if 
											"FALSE"
	Parameters :	CString directory	-	directory to 
											create

	Usage :			Call to create a directory chain.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;
	CString indir( directory );
	if( indir.GetLength( ) )
	{
		QualifyPath( indir );

		_TCHAR drive[ _MAX_PATH ];
		_TCHAR dir[ _MAX_DIR ];
		_TCHAR fname[ _MAX_FNAME ];
		_TCHAR ext[ _MAX_EXT ];

		// Split directory into parts
		_tsplitpath( indir, drive, dir, fname, ext );

		TCHAR currentDirectory[ _MAX_PATH ];
		::GetCurrentDirectory( _MAX_PATH, currentDirectory );

		CStringArray directories;
		CString parts = dir;

		if( parts.GetLength( ) > 2 )
		{
			if( parts.Left( 2 ) == _T( "\\\\" ) )
			{
				// We have an UNC name
				CString strComputer;
				parts = parts.Right( parts.GetLength( ) - 2 );
				int findDir = parts.Find( _TCHAR( '\\' ) );
				if( findDir!=-1)
				{
					strComputer = _T( "\\\\" ) + parts.Left( findDir );
					parts = parts.Right( parts.GetLength( ) - ( findDir + 1 ) );
				}
				_tcscpy( drive, strComputer );
			}
		}

		CString strRoot( drive );

		// Strip leading \'s
		while( parts.GetLength( ) && parts[0] == _TCHAR( '\\' ) )
			parts = parts.Right( parts.GetLength( ) - 1 );

		// Cut into separate directories
		int find = parts.Find( _TCHAR( '\\' ) );
		while( find != -1 )
		{
			directories.Add( parts.Left( find ) );
			parts = parts.Right( parts.GetLength( ) - ( find + 1 ) );
			find = parts.Find( _TCHAR( '\\' ) );
		}

		if( parts.GetLength( ) )
			directories.Add( parts );

		if( fname )
			directories.Add( fname );

		// Loop directories one-by-one, creating as necessary
		INT_PTR max = directories.GetSize( );
		CString strCurrentDirectory( strRoot );

		for( INT_PTR t = 0 ; t < max ; t++ )
		{
			strCurrentDirectory += _TCHAR( '\\' ) + directories[ t ];
			Trigger( strCurrentDirectory );
			if( !( result = ::SetCurrentDirectory( strCurrentDirectory ) ) )
			{
				if( !( result = ::CreateDirectory( strCurrentDirectory, NULL ) ) )
				{
					SetSystemErrorMessage( ::GetLastError( ), strCurrentDirectory );
					t = max;
				}
			}
		}

		::SetCurrentDirectory( currentDirectory );

	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}
	return result;
}

BOOL CDiskObject::EmptyDirectory( const CString& directory )
/* ============================================================
	Function :		CDiskObject::EmptyDirectory
	Description :	Will delete all files in directory. 
	Access :		Public

	Return :		BOOL				-	"TRUE" if OK. 
											"GetErrorMessage" will 
											get an error 
											string if "FALSE"
	Parameters :	CString directory	-	the directory to
											empty.

	Usage :			Call to empty a directory.

   ============================================================*/
{
	ClearError( );
	CString indir( directory );
	QualifyPath( indir );

	// Enumerate all files
	CStringArray files;
	BOOL result = EnumFilesInDirectory( indir, files );
	if( result )
	{
		INT_PTR max = files.GetSize( );
		for( INT_PTR t = 0 ; t < max ; t++ )
		{
			// Loop and delete
			CString file = files[ t ];
			Trigger( file );
			if( !( result = ::DeleteFile( indir + file ) ) )
			{
				SetSystemErrorMessage( ::GetLastError( ), indir + file );
				t = max;
			}
		}
	}

	return result;
}

BOOL CDiskObject::RemoveDirectory( const CString& directory )
/* ============================================================
	Function :		CDiskObject::RemoveDirectory
	Description :	Will remove the directory "directory", even
					if not empty. Will not remove 
					subdirectories.
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if OK. 
											"GetErrorMessage"
											will get an error 
											string if "FALSE"
	Parameters :	CString directory	-	directory to 
											remove.

	Usage :			Call to remove a directory.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;
	CString indir( directory );
	if( indir.GetLength( ) )
	{
		QualifyPath( indir );

		// Wipe and remove directory
		if( ( result = EmptyDirectory( indir ) ) )
		{
			Trigger( indir );
			if( !( result = ::RemoveDirectory( indir ) ) )
				SetSystemErrorMessage( ::GetLastError( ), indir );
		}
	}
	else
	{
		// Small sanity check, we can't
		// delete the current directory.
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::RemoveDirectories( const CString& directory )
/* ============================================================
	Function :		CDiskObject::RemoveDirectories
	Description :	Will remove the directory "directory", even
					if not empty. Will remove subdirectories.
	Access :		Public

	Return :		BOOL				-	"TRUE" if OK. 
											"GetErrorMessage" 
											will get an error 
											string if "FALSE"
	Parameters :	CString directory	-	root directory to 
											remove.

	Usage :			Call to remove a directory tree.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;

	CString indir( directory );
	if( indir.GetLength( ) )
	{
		QualifyPath( indir );

		CStringArray directories;

		// Get all directories
		;
		if( ( result = EnumAllDirectories( indir, directories ) ) )
		{
			// Loop and remove
			INT_PTR max = directories.GetSize( );
			for( INT_PTR t = max - 1; t >= 0 ; t-- )
				if( !( result = RemoveDirectory( directories[ t ] ) ) )
					t = -1;
			
			if( result )
				result = RemoveDirectory( indir );
		}
	}
	else
	{
		// Small sanity check, we can't
		// delete the current directory.
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::EmptyDirectories( const CString& directory )
/* ============================================================
	Function :		CDiskObject::EmptyDirectories
	Description :	Will delete all files in "directory". Will 
					also empty subdirectories.
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if OK. 
											"GetErrorMessage" 
											will get an error 
											string if "FALSE"
	Parameters :	CString directory	-	the root directory 
											to empty.

	Usage :			Call to empty a directory tree.

   ============================================================*/
{
	ClearError( );

	CString indir( directory );

	QualifyPath( indir );
	CStringArray directories;

	// Get all directories
	BOOL result = EnumAllDirectories( indir, directories );
	if( result )
	{
		INT_PTR max = directories.GetSize( );
		// Loop and empty
		for( INT_PTR t = max - 1 ; t >= 0 ; t-- )
			if( !( result = EmptyDirectory( directories[ t ] ) ) )
				t = -1;

		if( result )
			result = EmptyDirectory( indir );
	}

	return result;
}

BOOL CDiskObject::CopyDirectory( const CString& sourceDirectory,const CString& destDirectory )
/* ============================================================
	Function :		CDiskObject::CopyDirectory
	Description :	Copies all the files from "sourceDirectory" 
					to "destDirectory". Existing files will be
					overwritten. "destDirectory" will be created
					if necessary. Subdirectories will not be
					copied.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will get an error 
												string if "FALSE"
	Parameters :	CString sourceDirectory	-	copy from.
					CString destDirectory	-	copy to.

	Usage :			Call to copy a directory to another 
					directory.

   ============================================================*/
{
	ClearError( );
	CString source( sourceDirectory );
	CString dest( destDirectory );
	QualifyPath( source );
	QualifyPath( dest );

	Trigger( dest );

	// We don't care if this fails - CopyFiles will 
	// return an error in that case.
	::CreateDirectory( dest, NULL );

	return CopyFiles( source, dest );

}

BOOL CDiskObject::CopyDirectories( const CString& sourceDirectory,const CString& destDirectory )
/* ============================================================
	Function :		CDiskObject::CopyDirectories
	Description :	Copies all the files and subdirectories 
					from "sourceDirectory" to "destDirectory", 
					keeping the directory structure. Existing 
					files will be overwritten. "destDirectory" 
					and subdirectories will be created if 
					necessary.
	Access :		Public

					
	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will get an 
												error string 
												if "FALSE"
	Parameters :	CString sourceDirectory	-	copy from.
					CString destDirectory	-	copy to.

	Usage :			Call to copy a directory tree to a new 
					directory tree.

   ============================================================*/
{
	ClearError( );

	CString source( sourceDirectory );
	CString dest( destDirectory );
	QualifyPath( source );
	QualifyPath( dest );

	// Enumerate all directories and files below sourceDirectory
	CStringArray directories;
	directories.Add( source );

	BOOL result = EnumAllDirectories( source, directories );
	if( result )
	{
		// Create and copy directories
		INT_PTR max = directories.GetSize( );
		for( INT_PTR t = 0 ; t < max ; t++ )
		{
			// Create names and copy
			CString from = directories[ t ];
			CString part = from.Right( from.GetLength( ) - source.GetLength( ) );
			CString to = dest + part;

			if( !( result = CopyFiles( from, to ) ) )
				t = max;
		}
	}

	return result;
}

////////////////////////////////////////
// File-oriented operations
//

BOOL CDiskObject::FileExists( const CString& file )
/* ============================================================
	Function :		CDiskObject::FileExists
	Description :	Returns "TRUE" if the file file exists
	Access :		Public
					
	Return :		BOOL			-	"TRUE" if found
	Parameters :	CString file	-	file to check

	Usage :			Call to check for file existence.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;

	if( file.GetLength( ) )
	{
		CString infile( file );
		QualifyFile( infile );
		HANDLE filehandle = ::CreateFile( 
								infile,
								GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
							);

		if( filehandle == INVALID_HANDLE_VALUE )
			result = FALSE;
		else
			CloseHandle( filehandle );
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::FileInformation( const CString& file, BY_HANDLE_FILE_INFORMATION &file_info )
/* ============================================================
	Function :		CDiskObject::FileInformation
	Description :	Returns "TRUE" if the file file exists
					At this time file_info is filled out
	Access :		Public

	Return :		BOOL									-	"TRUE" if found
	Parameters :	CString file							-	file to check
					BY_HANDLE_FILE_INFORMATION &file_info	-	info filled out

	Usage :			Call to check for file existence and if there
					return data about that file.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;

	if( file.GetLength( ) )
	{
		CString infile( file );
		QualifyFile( infile );
		HANDLE filehandle = ::CreateFile(infile,
											GENERIC_READ,
											FILE_SHARE_READ | FILE_SHARE_WRITE,
											NULL,
											OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL,
											NULL);

		if( filehandle == INVALID_HANDLE_VALUE )
			result = FALSE;
		else
		{
			BY_HANDLE_FILE_INFORMATION file_info_local;
			if (GetFileInformationByHandle( filehandle, &file_info_local ))
			{
				memcpy(&file_info, &file_info_local, sizeof(file_info_local));
			}
			else
			{
				result = FALSE;
				SetInternalErrorMessage( );
			}
			CloseHandle( filehandle );
		}
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;

}

BOOL CDiskObject::CreateFile( const CString& file )
/* ============================================================
	Function :		CDiskObject::CreateFile
	Description :	Creates the file "file", as well as the 
					directories necessary
	Access :		Public

	Return :		BOOL			-	"TRUE" if OK
	Parameters :	CString file	-	file to create

	Usage :			Call to create a file.

   ============================================================*/
{
	ClearError( );

	BOOL result = TRUE;

	if( file.GetLength( ) )
	{

		CString infile( file );
		QualifyFile( infile );

		// Split into directory and file
		CString directory = GetDirectoryName( infile );
		CString filename = GetFileName( infile );

		if( ( result = CreateDirectory( directory ) ) )
		{
			Trigger( file );
			HANDLE filehandle = ::CreateFile( 
									infile,
									GENERIC_WRITE,
									0,
									NULL,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL
								);

			if( filehandle == INVALID_HANDLE_VALUE )
				result = FALSE;
			else
				CloseHandle( filehandle );
		}
	}
	else
	{
		SetInternalErrorMessage( );
		result = FALSE;
	}

	return result;
}

BOOL CDiskObject::MoveFile( const CString& sourceFile, const CString& destFile )
/* ============================================================
	Function :		CDiskObject::MoveFile
	Description :	Move the file "sourceFile" to "destFile". 
					Creates the destination directory if 
					necessary

	Access :		Public

	Return :		BOOL						- "TRUE" if renamed OK
	Parameters :	const CString& sourceFile	- File to move
					const CString& desFile		- New name/location

	Usage :			Call to move a file.

   ============================================================*/
{

	ClearError( );

	BOOL result = FALSE;

	if( sourceFile.GetLength() && destFile.GetLength() )
	{
		CString insourcefile( sourceFile );
		QualifyFile( insourcefile );
		CString indestdir = GetDirectoryName( destFile );
		CString indestfilename = GetFileName( destFile );

		if( CopyFile( insourcefile, indestdir, indestfilename ) )
		{
			result = RemoveFile( sourceFile );
		}

	}

	return result;

}

BOOL CDiskObject::RenameFile( const CString& sourceFile, const CString& destFile )
/* ============================================================
	Function :		CDiskObject::RenameFile
	Description :	Renames the file "sourceFile" to "destFile". 
					"destFile" is assumed to be a filename rather 
					than a fully qualified path.

	Access :		Public

	Return :		BOOL						- "TRUE" if renamed OK
	Parameters :	const CString& sourceFile	- file to rename
					const CString& desFile		- New name

	Usage :			Call to rename a file.

   ============================================================*/
{

	ClearError( );

	BOOL result = FALSE;

	if( sourceFile.GetLength() && destFile.GetLength() )
	{
		CString insourcefile( sourceFile );
		QualifyFile( insourcefile );
		CString insourcedir = GetDirectoryName( insourcefile );
		if( CopyFile( sourceFile, insourcedir, destFile ) )
		{
			result = RemoveFile( sourceFile );
		}
	}

	return result;

}

////////////////////////////////////////
// Helpers
//

BOOL CDiskObject::EnumFilesInDirectory( const CString& sourceDirectory, CStringArray& files, int mode )
/* ============================================================
	Function :		CDiskObject::EnumFilesInDirectory
	Description :	Enumerates all files in directory 
					"sourceDirectory", and adds them to the 
					"CStringArray" "files". Will not add 
					subdirectories
	Access :		Public

	Return :		BOOL					-	"TRUE" if OK. 
												"GetErrorMessage" 
												will contain errors
	Parameters :	CString sourceDirectory	-	The directory to 
												enumerate
					CStringArray& files		-	The resulting files
					
	Usage :			Call to get a list of files in a directory.

   ============================================================*/
{
	return EnumFilesInDirectoryWithFilter( _T( "*.*" ), sourceDirectory, files, mode );
}

BOOL CDiskObject::EnumFilesInDirectoryWithFilter( const CString& strFilter,const CString& sourceDirectory, CStringArray& files, int mode )
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

	ClearError( );
	CString source( sourceDirectory );
	QualifyPath( source );
	CString sourceFiles( source + strFilter );
	BOOL result = TRUE;
	WIN32_FIND_DATA ff;

	HANDLE findhandle = FindFirstFile( sourceFiles, &ff );
	if( findhandle != INVALID_HANDLE_VALUE )
	{
		BOOL res = TRUE;
		while( res )
		{
			// We only want files
			if( !( ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				BOOL    added = FALSE;
				CString file;
				file = ff.cFileName;

				if( mode == EF_FULLY_QUALIFIED )
					file = sourceDirectory + file;

				// Return the array alphabetically ordered
				for( int i = 0; i < files.GetSize(); i++ )
				{
					if ( files.GetAt( i ) > file )
					{
						files.InsertAt( i, file );
						added = TRUE;
						break;
					}
				}
				if ( !added )
				{
					files.Add( file );
				}

				Trigger( file );
			}

			res = FindNextFile( findhandle, &ff );
		}

		FindClose( findhandle );
	}
	else
	{
		// Set error message
		SetSystemErrorMessage( ::GetLastError( ), sourceFiles );
		result = FALSE;
	}

	return result;

}
 
BOOL CDiskObject::EnumAllDirectories( const CString& sourceDirectory, CStringArray& directories )
/* ============================================================
	Function :		CDiskObject::EnumAllDirectories
	Description :	Enumerates all directories recursively in 
					directory "sourceDirectory", and adds them 
					to the "CStringArray" "directories". Each entry 
					will be a fully qualified name
	Access :		Public

	Return :		BOOL						-	"TRUE" if OK. 
													"GetErrorMessage" 
													will contain errors
	Parameters :	CString sourceDirectory		-	start directory
					CStringArray& directories	-	"CStringArray" to be 
													filled with the 
													directory names.

	Usage :			Call to get the directory names in a 
					directory tree.

   ============================================================*/
{
	ClearError( );

	CString source( sourceDirectory );
	QualifyPath( source );
	CString sourceFiles( source + _T( "*.*" ) );
	BOOL result = TRUE;
	WIN32_FIND_DATA ff;

	HANDLE findhandle = FindFirstFile( sourceFiles, &ff );
	if( findhandle != INVALID_HANDLE_VALUE)
	{

		BOOL res = TRUE;
		while( res)
		{
			// We only want directories
			if( ( ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && _tcscmp( ff.cFileName, _T( "." ) ) && _tcscmp( ff.cFileName, _T( ".." ) ) )
			{
				CString directory( source + ff.cFileName );
				directories.Add( directory + _TCHAR( '\\' ) );
				Trigger( ff.cFileName );
				if( !( result = EnumAllDirectories( directory, directories ) ) )
					res = FALSE;
			}

			if( res )
				res = FindNextFile( findhandle, &ff );
			else
				result = FALSE;
		}

		FindClose( findhandle );
	}
	else
	{
		// Set error message
		SetSystemErrorMessage( ::GetLastError( ), sourceFiles );
		result = FALSE;
	}

	return result;
}
 
BOOL CDiskObject::EnumDirectories( const CString& sourceDirectory, CStringArray& directories )
/* ============================================================
	Function :		CDiskObject::EnumDirectories
	Description :	Enumerates all directories in directory 
					"sourceDirectory", and adds them to the 
					"CStringArray" "directories". Each entry will
					be a fully qualified name
	Access :		Public

	Return :		BOOL						-	"TRUE" if OK. 
													"GetErrorMessage" 
													will contain errors
	Parameters :	CString sourceDirectory		-	start directory
					CStringArray& directories	-	"CStringArray" to be 
													filled with the 
													directory names.

	Usage :			Call to get a list of directories in a 
					directory tree.

   ============================================================*/
{
	ClearError( );

	CString source( sourceDirectory );
	QualifyPath( source );
	CString sourceFiles( source + _T( "*.*" ) );
	BOOL result = TRUE;
	WIN32_FIND_DATA ff;

	HANDLE findhandle = FindFirstFile( sourceFiles, &ff );
	if( findhandle != INVALID_HANDLE_VALUE)
	{

		BOOL res = TRUE;
		while( res)
		{
			// We only want directories
			if( ( ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && _tcscmp( ff.cFileName, _T( "." ) ) && _tcscmp( ff.cFileName, _T( ".." ) ) )
			{
				directories.Add( ff.cFileName );
				Trigger( ff.cFileName );
			}

			if( res )
				res = FindNextFile( findhandle, &ff );
			else
				result = FALSE;
		}

		FindClose( findhandle );
	}
	else
	{
		// Set error message
		SetSystemErrorMessage( ::GetLastError( ), sourceFiles );
		result = FALSE;
	}

	return result;
}
 
BOOL CDiskObject::EnumAllFiles( const CString& sourceDirectory, CStringArray& files )
/* ============================================================
	Function :		CDiskObject::EnumAllFiles
	Description :	Returns, recursively, the fully qualified 
					names of all files found under 
					"sourceDirectory".
	Access :		Public

	Return :		BOOL					-	"TRUE" if OK.
	Parameters :	CString sourceDirectory	-	Root dir	
					CStringArray& files		-	output list 
												of all files 
												found

	Usage :			Call to get the names of all files in a 
					directory tree.

   ============================================================*/
{
	return EnumAllFilesWithFilter( _T( "*.*" ), sourceDirectory, files );
}

BOOL CDiskObject::EnumAllFilesWithFilter( const CString& filter,const CString& sourceDirectory, CStringArray& files )
/* ============================================================
	Function :		CDiskObject::EnumAllFiles
	Description :	Returns, recursively, the fully qualified 
					names of all files matching the filter
					found under "sourceDirectory".
	Access :		Public

	Return :		BOOL					-	"TRUE" if OK.
	Parameters :	CString filter			-	Filter for files
												to find
					CString sourceDirectory	-	Root dir	
					CStringArray& files		-	output list 
												of all files 
												found

	Usage :			Call to get a filtered list of all files in 
					a directory tree.

   ============================================================*/
{
	CStringArray dirs;
	BOOL result = EnumAllDirectories( sourceDirectory, dirs );
	dirs.Add(sourceDirectory);
	if( result )
	{
		INT_PTR max1 = dirs.GetSize( );
		for( int t = 0 ; t < max1 ; t++ )
		{
			CString dir = dirs[ t ];
			QualifyPath( dir );
			CStringArray dirfiles;
			if( EnumFilesInDirectoryWithFilter( filter, dir, dirfiles, EF_FULLY_QUALIFIED ) ) 
			{
				INT_PTR max2 = dirfiles.GetSize( );
				for( int i = 0 ; i < max2 ; i++ )
				{
					CString file = dirfiles[ i ];
					Trigger( file );
					files.Add( file );
				}
			}
//			Sleep(100);
		}
	}

	return result;
}

////////////////////////////////////////
// Error handling

CString CDiskObject::GetErrorMessage( )
/* ============================================================
	Function :		CDiskObject::GetErrorMessage
	Description :	Will return the current error message for 
					the "CDiskObject".
	Access :		Public
					
	Return :		CString	-	current error message.
	Parameters :	none

	Usage :			Call to get the error message.

   ============================================================*/
{
	return m_errorMessage;
}

// Private methods

void CDiskObject::ClearError( )
/* ============================================================
	Function :		CDiskObject::ClearError
	Description :	Clears the internal error message string
	Access :		Private
					
	Return :		void
	Parameters :	none

	Usage :			Call to clear the internal error message.

   ============================================================*/
{
	m_errorMessage = _T( "" );
}

void CDiskObject::SetSystemErrorMessage( int error,const CString& elements)
/* ============================================================
	Function :		CDiskObject::SetSystemErrorMessage
	Description :	Sets the error message string with the 
					system error message as gotten from 
					"GetLastError". "elements" will be appended.
	Access :		Private

	Return :		void
	Parameters :	int error			-	error number from 
											"GetLastError"
					CString elements	-	files or directories 
											to add to the message
					
	Usage :			Call to set the error message.

   ============================================================*/
{
	LPVOID msgBuff;

	// Format error message from system
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		( LPTSTR ) &msgBuff,
		0,
		NULL
	);

	// Create the error string
	m_errorMessage = CString( ( LPTSTR ) msgBuff );
	m_errorMessage.TrimLeft( );
	m_errorMessage.TrimRight( );

	if( elements.GetLength( ) )
		m_errorMessage += _TCHAR( ' ' ) + elements;

	Trigger ( m_errorMessage );

	// Free the buffer.
	LocalFree( msgBuff );
}

void CDiskObject::SetInternalErrorMessage( )
/* ============================================================
	Function :		CDiskObject::SetInternalErrorMessage
	Description :	Sets the error message to the internal error
					message
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to set the error message.

   ============================================================*/
{
	m_errorMessage = _T( "CDiskObject internal error" );
	Trigger ( m_errorMessage );
}

////////////////////////////////////////
// Helpers

void CDiskObject::QualifyPath( CString& str )
/* ============================================================
	Function :		CDiskObject::QualifyPath
	Description :	Creates a fully qualified path from "str". 
	Access :		Private
					
	Return :		void
	Parameters :	CString& str	-	the path to qualify

	Usage :			Call to create a fully qualified path.

   ============================================================*/
{

	str.Replace( _TCHAR( '/' ), _TCHAR( '\\' ) );

	// Append a trailing backslash to directory name
	if( str.GetLength( ) )
		if( str[ str.GetLength( ) - 1 ] != _TCHAR( '\\' ) )
			str += _TCHAR( '\\' );

	if( str.GetLength( ) < 2 || str.Left( 2) != "\\\\" )
		QualifyFile( str );

}

void CDiskObject::QualifyFile( CString& str )
/* ============================================================
	Function :		CDiskObject::QualifyFile
	Description :	Creates a fully qualified path from "str". 
	Access :		Private

	Return :		void
	Parameters :	CString& str	-	the filename to qualify

	Usage :			Call to create a fully qualified filename.

   ============================================================*/
{
	str.Replace( _TCHAR( '/' ), _TCHAR( '\\' ) );

	_TCHAR drive[ _MAX_DRIVE ];
	_TCHAR dir[ _MAX_DIR ];
	_TCHAR fname[ _MAX_FNAME ];
	_TCHAR ext[ _MAX_EXT ];

	_tsplitpath( str, drive, dir, fname, ext );

	if( !_tcsclen( drive ) )
	{
		CString test( dir );
		// The name is not fully qualified.
		// We assume it to be relative the 
		// current directory

		TCHAR currentDirectory[ _MAX_PATH ];
		::GetCurrentDirectory( _MAX_PATH, currentDirectory );
		if( test.GetLength( ) > 2 && test.Left( 3 ) == _T( "..\\" ) )
		{

			TCHAR upDirectory[ _MAX_PATH ];
			while( test.GetLength( ) > 2 && test.Left( 3 ) == _T( "..\\" ) )
			{
				::SetCurrentDirectory( _T( ".." ) );
				test = test.Right( test.GetLength( ) - 3 );
			}

			::GetCurrentDirectory( _MAX_PATH, upDirectory );

			str = upDirectory;
			str += _TCHAR( '\\' );
			str += fname;
			str += ext;
			::SetCurrentDirectory( currentDirectory );
		}
		else if ( test.GetLength( ) && test[ 0 ] == _TCHAR( '\\' ) )
		{
			// Start from root
			_tsplitpath( currentDirectory, drive, dir, fname, ext );
			str = drive + str;
		}
		else
		{
			str = _TCHAR( '\\' ) + str;
			str = currentDirectory + str;
		}
	}
}

CString CDiskObject::GetFileName( const CString& str )
/* ============================================================
	Function :		CDiskObject::GetFileName
	Description :	Returns the file name part of "str".
	Access :		Private
					
	Return :		CString	-	
	Parameters :	CString str	-	The complete name to get

	Usage :			Call to get the filename from a fully 
					qualified path.

   ============================================================*/
{
	CString instr( str );
	QualifyFile( instr );
	CString file;
	int found = instr.ReverseFind( _TCHAR( '\\' ) );
	if( found != -1 )
		file = instr.Right( str.GetLength( ) - ( found + 1 ) );

	return file;
}

CString CDiskObject::GetDirectoryName( const CString& str )
/* ============================================================
	Function :		CDiskObject::GetDirectoryName
	Description :	Returns the directory name part of "str"
	Access :		Private
					
	Return :		CString	-	
	Parameters :	CString str	-	The complete name to check	

	Usage :			Call to get the directory from a fully 
					qualified path.

   ============================================================*/
{
	CString instr( str );
	QualifyFile( instr );
	CString directory;
	int found = instr.ReverseFind( _TCHAR( '\\' ) );
	if( found != -1 )
		directory = instr.Left( found );

	return directory;
}

#pragma warning( default : 4706 )
