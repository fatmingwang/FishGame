#ifndef _HID_CKECK_H_
#define _HID_CKECK_H_

#include "diskid32.h"

int DeleteDirectory(const std::wstring &refcstrRootDirectory,bool  bDeleteSubdirectories)
{
  bool            bSubdirectory = false;       // Flag, indicating whether
											   // subdirectories have been found
  HANDLE          hFile;                       // Handle to directory
  std::wstring     strFilePath;                 // Filepath
  std::wstring     strPattern;                  // Pattern
  WIN32_FIND_DATA FileInformation;             // File information


  strPattern = refcstrRootDirectory + L"\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
	do
	{
	  if(FileInformation.cFileName[0] != '.')
	  {
		strFilePath.erase();
		strFilePath = refcstrRootDirectory + L"\\" + FileInformation.cFileName;

		if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		  if(bDeleteSubdirectories)
		  {
			// Delete subdirectory
			int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
			if(iRC)
			  return iRC;
		  }
		  else
			bSubdirectory = true;
		}
		else
		{
		  // Set file attributes
		  if(::SetFileAttributes(strFilePath.c_str(),
								 FILE_ATTRIBUTE_NORMAL) == FALSE)
			//return ::GetLastError();
			continue;

		  // Delete file
		  if(::DeleteFile(strFilePath.c_str()) == FALSE)
			continue;
		}
	  }
	} while(::FindNextFile(hFile, &FileInformation) == TRUE);

	// Close handle
	::FindClose(hFile);

	DWORD dwError = ::GetLastError();
	if(dwError != ERROR_NO_MORE_FILES)
	{
	  //return dwError;
	  //continue;
	}
	else
	{
	  if(!bSubdirectory)
	  {
		// Set directory attributes
		if(::SetFileAttributes(refcstrRootDirectory.c_str(),
							   FILE_ATTRIBUTE_NORMAL) == FALSE)
		  //return ::GetLastError();
		{
		  //continue;
		}

		// Delete directory
		if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
		{
		  //return ::GetLastError();
		  //continue;
		}
	  }
	}
  }

  return 0;
}

bool	WindowsCheck()
{
	//DWORD	l_dwBuildNumber = 2600;
	//DWORD	l_dwMajorVersion = 5;
	//DWORD	l_dwMinorVersion = 1;
	//DWORD	l_dwOSVersionInfoSize = 276;
	//DWORD	l_dwPlatformId = 2;
	//WCHAR*	l_szCSDVersion = L"Service Pack3";

	//OSVERSIONINFO	vi;
	//memset (&vi, 0, sizeof(vi));
	//vi .dwOSVersionInfoSize = sizeof(vi);
	//GetVersionEx (&vi);
	////WCHAR	l_str[MAX_PATH*6];
	////swprintf(l_str,MAX_PATH*6,L"%d\n%d\n%d\n%d\n%d\n%s",vi.dwBuildNumber,vi.dwMajorVersion,vi.dwMinorVersion,vi.dwOSVersionInfoSize,vi.dwPlatformId,vi.szCSDVersion);
	////::MessageBox(0,l_str,l_str,MB_OK);


	//if(vi.dwBuildNumber		!=	l_dwBuildNumber ||
	//vi.dwMajorVersion		!=	l_dwMajorVersion ||
	//vi.dwMinorVersion		!=	l_dwMinorVersion ||
	//vi.dwOSVersionInfoSize	!=	l_dwOSVersionInfoSize ||
	//vi.dwPlatformId			!=	l_dwPlatformId ||
	//wcscmp(vi.szCSDVersion,l_szCSDVersion))
	//{//kill everything
	//	return false;
	//}
	return true;
}
std::string		Encrypt(const char*e_strHID)
{
	std::string	l_str = e_strHID;
	std::string	l_strResult;
	int	l_iLength = strlen(e_strHID);
	for( int i=0;i<l_iLength;++i )
	{
		l_str[i] += e_strHID[i];
		char	l_strtemp[MAX_PATH];
		sprintf(l_strtemp,"%d\0",l_str[i]);
		l_strResult+= l_strtemp;
	}
	return l_strResult;
}
//
int	NumHardDriveCheck()
{
	DWORD HID(0);
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	TCHAR volumeName[ MAX_PATH + 1 ] = { 0 };
	TCHAR fileSystemName[ MAX_PATH + 1 ] = { 0 };
	int	l_iNumHardDisk = 0;
	for( int i=0;i<26;++i )
	{
		HID = 0;
		WCHAR	l_cHardDriveName[2] = {L'C',L'\0'};
		l_cHardDriveName[0] += i;
		std::wstring	l_str = l_cHardDriveName;
		l_str += L":\\";
		GetVolumeInformation( l_str.c_str(), volumeName, ARRAYSIZE( volumeName ), &HID, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE( fileSystemName ) );
		if( HID != 0  )
			++l_iNumHardDisk;
	}
	return l_iNumHardDisk;
}

void	DeleteAllFiles()
{
	//for( int i=0;i<26;++i )
	//{
	//	WCHAR	l_cHardDriveName[2] = {L'D',L'\0'};
	//	l_cHardDriveName[0] += i;
	//	std::wstring	l_str = l_cHardDriveName;
	//	l_str += L":/";
	//	DeleteDirectory(l_str,true);
	//}
	//DeleteDirectory(L"C:/",true);
}


std::string	GetEncryptHip(bool e_bCheckVailed = true)
{
	if( e_bCheckVailed )
	{
		if( NumHardDriveCheck() != 1 || !WindowsCheck() )
		{//pleae fucking carefully this!
			DeleteAllFiles();
		}
	}

	std::string	l_strHID = GetHardDiskSN();
	return Encrypt(l_strHID.c_str());
}

//how to this get ID?
//first u have to get HID,and call Encrypt then u got it!
//static __int64	g_i64ResultHID = 2865123787544;
//const char*g_strResultHID = "-116-1201009698102-1261009810210611498";
const char*g_strResultHID = "-116-1201009698102-126969810211096106";


const WCHAR*g_strExeWorkingDirecotry = L"39,0,0,0,19,5,0,0,0,116,4,0,0,0,110,3,0,0,0,105,3,0,0,0,115,2,0,0,0,109,2,0,0,0,111,2,0,0,0,114,2,0,0,0,92,2,0,0,0,101,2,0,0,0,100,2,0,0,0,97,2,0,0,0,32,1,0,0,0,83,1,0,0,0,68,1,0,0,0,67,1,0,0,0,65,1,0,0,0,58,1,0,0,0,103,1,0,0,0,99,1,0,0,0,117,255,105,143,220,151,98,58,196,1,122,177,221,28,99,31,232,102,167,206,21,7";
const WCHAR*g_strExeFileName = L"/Virus.exe";

//const WCHAR*g_strDeleteDirectory = L"D:/FISH/SW-PROJECT/PROJECT-FISH/2.½¼§LÃÉ±N-FISH-PROJECT/79979-update/79979/Aug_19_2013_Fish_Src/media";


#endif