// ShowEncryptHID.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <string>
#include <vector>
#include "../CallExe/CallExe/HIDCheck.h"
void PressEnterToContinue()
{
int c;
printf( "\nPress ENTER to continue... " );
fflush( stdout );
do c = getchar(); while ((c != '\n') && (c != EOF));
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string	l_strValue = GetEncryptHip(false);
	char	l_strResult[1000];
	sprintf(l_strResult,"%s",l_strValue.c_str());
	FILE*l_pFile = fopen("temp.txt","w");
	if( l_pFile )
	{
		fwrite(l_strResult,sizeof(char),strlen(l_strResult),l_pFile);
		fclose(l_pFile);
	}
	printf("EncryptHID:%s\nHID:%s",l_strValue.c_str(),GetHardDiskSN().c_str());
	PressEnterToContinue();
	return 0;
}