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

#ifndef _DISKOBJECT_H_
#define _DISKOBJECT_H_

#define EF_ONLY_FILENAMES	0
#define EF_FULLY_QUALIFIED	1

class CDiskObject 
{
public:
	// ctor(s)
	CDiskObject();
	CDiskObject( CWnd* feedbackWindow );

	// dtor
	~CDiskObject();

	// - - -
	// Operations

	// Files

	BOOL FileExists( const CString& file );
	BOOL FileInformation( const CString& file, BY_HANDLE_FILE_INFORMATION &file_info);
	BOOL CreateFile( const CString& file );
	BOOL CopyFile( const CString& sourceFile, const CString& destDirectory );
	BOOL CopyFile( const CString& sourceFile, const CString& destDirectory, const CString& destFile );
	BOOL RemoveFile( const CString& sourceFile );
	BOOL RenameFile( const CString& sourceFile, const CString& destFile );
	BOOL MoveFile( const CString& sourceFile, const CString& destFile );

	BOOL CopyFiles( const CString& sourceDirectory, const CString& destDirectory );
	BOOL CopyFiles( CStringArray& files, const CString& destDirectory );

	// Directories

	BOOL DirectoryExists( const CString& file );

	BOOL CreateDirectory( const CString& directory );
	BOOL CopyDirectory( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectory( const CString& directory );
	BOOL RemoveDirectory( const CString& directory );

	BOOL CopyDirectories( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectories( const CString& directory );
	BOOL RemoveDirectories( const CString& directory );

	BOOL EnumFilesInDirectoryWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllDirectories( const CString& sourceDirectory, CStringArray& directories );
	BOOL EnumDirectories( const CString& sourceDirectory, CStringArray& directories );
	BOOL EnumFilesInDirectory( const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllFiles( const CString& sourceDirectory, CStringArray& files );
	BOOL EnumAllFilesWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files );

	// Error handling
	CString GetErrorMessage();	

private:
	// Error handling
	CString m_errorMessage;

	void	ClearError();
	void	SetInternalErrorMessage();
	void	SetSystemErrorMessage( int error, const CString& elements = _T( "" ) );

	// Helpers

	CWnd*	m_feedbackWindow;
	void	QualifyPath( CString& str );
	void	QualifyFile( CString& str );
	CString GetFileName( const CString& str );
	CString GetDirectoryName( const CString& str );

};

#endif // _DISKOBJECT_H_
