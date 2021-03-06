#include "../stdafx.h"
#include "BinaryFile.h"
#ifdef _WIN32
#include <io.h>
#include <time.h>
#endif
#include "../Android/nv_file.h"
//p4 test
namespace FATMING_CORE
{
	cBinaryFile::cBinaryFile(bool e_bBigEndian)
	{
		m_bBigEndian = e_bBigEndian;
		if( e_bBigEndian )
			SwapToPc();
		else
			SwapToXBOX();
		m_pData = 0;
		m_pFile = 0;
#ifdef WIN32
		m_FileHandle = 0;
#endif
	}

	cBinaryFile::~cBinaryFile()
	{
		CloseFile();
	};

	void cBinaryFile::SwapToPc()
	{
		D0 = 0;D1 = 1;D2 = 2;D3 = 3;
		D4 = 4;D5 = 5;D6 = 6;D7 = 7;
		W0 = 0;W1 = 1;
	}

	void cBinaryFile::SwapToXBOX()
	{
		D0 = 7;D1 = 6;D2 = 5;D3 = 4;
		D4 = 3;D5 = 2;D6 = 1;D7 = 0;
		W0 = 1;W1 = 0;
	}

	void cBinaryFile::WriteToFile(FLOAT val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	//VOID cBinaryFile::WriteToFile(double val )
	//{
	//	//fwrite( &val, sizeof(double), 1, m_pFile );
	//	WriteToFile((int64) val );
	//}
	//VOID cBinaryFile::WriteToFile(WCHAR val )
	//{
	//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
	//	fwrite( data, sizeof(data), 1, m_pFile ); 
	//}

	void cBinaryFile::WriteToFile(int	  val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	void cBinaryFile::WriteToFile(DWORD val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		NvFWrite( data, sizeof(data), 1, m_pFile ); 
	}


	void cBinaryFile::WriteToFile(SHORT val )
	{
		CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
		NvFWrite( data, sizeof(data), 1, m_pFile ); 
	}


	void cBinaryFile::WriteToFile(WORD val )
	{
		CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
		NvFWrite( data, sizeof(data), 1, m_pFile ); 
	}

	void cBinaryFile::WriteToFileImmediatelyWithLine(const char* val ,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)strlen(val);
		NvFWrite( val, sizeof(char),l_iLength, m_pFile );
		NvFWrite( "\n", sizeof(char),1, m_pFile );
		Flush();
	}

	void cBinaryFile::WriteToFileImmediatelyWithLine(const WCHAR* val,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			NvFWrite( val, sizeof(WCHAR),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile((int64)val[i]);
		}
		if( m_bBigEndian )
			NvFWrite( L"\n", sizeof(WCHAR),1, m_pFile );
		else
			WriteToFile(L"\n");
		Flush();
	}

	void cBinaryFile::WriteToFile(char* val)
	{
		if( !m_pFile )
			return;
		int	l_iLength = (int)strlen(val);
		NvFWrite( val, sizeof(char),l_iLength, m_pFile );
	}

	void cBinaryFile::WriteToFile(int64	  val )
	{
		if( !m_pFile )
			return;
		CHAR data[8] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3],
						 ((CHAR*)&val)[D4], ((CHAR*)&val)[D5],
						 ((CHAR*)&val)[D6], ((CHAR*)&val)[D7]};
		NvFWrite( data, sizeof(data), 1, m_pFile );
	}

	void cBinaryFile::WriteToFile(WCHAR* val )
	{
		if( !m_pFile )
			return;
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			NvFWrite( val, sizeof(WCHAR),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile((int64)val[i]);
		}
	}

	bool	cBinaryFile::Flush()
	{
		if(m_pFile)
		{
			NvFFlush(m_pFile);
#ifdef WIN32
			if( m_FileHandle )
				FlushFileBuffers(m_FileHandle);
#endif
			return true;
		}
		return false;
	}
	bool	cBinaryFile::Writefile(const char*e_str,bool e_bBinary,bool e_bForceToWrite)
	{
		CloseFile();
#ifdef WIN32
		if( e_bForceToWrite )
		{
			int	l_iFlag = FILE_ATTRIBUTE_NORMAL;
			if( e_bForceToWrite )
				l_iFlag |= (FILE_FLAG_WRITE_THROUGH);
			m_FileHandle = CreateFile(UT::CharToWchar(e_str).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, l_iFlag, NULL);
			if (m_FileHandle != INVALID_HANDLE_VALUE) 
			{
				int file_descriptor = _open_osfhandle((intptr_t)m_FileHandle, 0);
				if (file_descriptor != -1) 
				{
					if( e_bBinary )
						m_pFile = _fdopen(file_descriptor, "w+b");
					else
						this->m_pFile = _fdopen(file_descriptor,"w");
				}
			}
		}
		else
		{
			if( e_bBinary )
				this->m_pFile = MyFileOpen(e_str,"w+b");
			else
				this->m_pFile = MyFileOpen(e_str,"w");		
		}
#else
		std::string	l_strFileName = e_str;
		if( e_bBinary )
			this->m_pFile = MyFileOpen(l_strFileName.c_str(),"w+b" );
		else
			this->m_pFile = MyFileOpen(l_strFileName.c_str(),"w" );
#endif
		if(m_pFile)
			return true;
		return false;
	}

	void	cBinaryFile::CloseFile()
	{
		if( m_pData )
			free(m_pData);
		m_pData = 0;
#ifdef WIN32
		if( m_FileHandle )
			CloseHandle(m_FileHandle);
		else
		if( m_pFile )
			fclose( m_pFile );
#else
		if( m_pFile )
			NvFClose( m_pFile );
#endif
		m_pFile = 0;
	}

	bool	cBinaryFile::Openfile(const char*e_strFileName)
	{
		this->m_pFile = MyFileOpen(e_strFileName,"r+b" );
		if(m_pFile)
			return true;
		return false;
	}

	int		cBinaryFile::GetDataFileSize()
	{
		if(  !m_pFile )
			return 0;
		return UT::GetFileSize(this->m_pFile);

	}
	void*	cBinaryFile::GetDataFile(UINT e_uiStart)
	{
		if(!m_pData)
		{
			assert(m_pFile&&"file is not existed");
			if( !m_pFile )
				return 0;
			long l_lFileSize = UT::GetFileSize(this->m_pFile);
			m_pData = malloc(l_lFileSize);
			NvFRead(m_pData,sizeof(char),l_lFileSize,m_pFile);
		}
		return (void*)((BYTE*)m_pData+e_uiStart);
	}

	//push data into e_pDestData
	void*	cBinaryFile::GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData)
	{
	#ifdef WIN32
		fpos_t pos = e_uiStart;
		fsetpos(m_pFile,&pos);
		fread(e_pDestData,sizeof(BYTE),e_uiSLength,m_pFile);
		rewind(m_pFile);
		return e_pDestData;
	#else
			//or just indtead by pos.__pos = e_uiStart;
		//and call as win32
			unsigned short*l_pData = (unsigned short*)GetDataFile(e_uiStart);
			WCHAR*l_Transto2Wchar = (WCHAR*)e_pDestData;        
			for(int i=0;i<e_uiSLength/2;++i )
			{
				l_Transto2Wchar[i] = l_pData[i];
			}
			l_Transto2Wchar[e_uiSLength/2] = L'\0';
		return e_pDestData;
	#endif
	}

	void	cBinaryFile::CreateTimestamp()
	{
	#ifdef WIN32
			char buffer[32];
			time_t ltime; time(&ltime);
			struct tm now; _localtime64_s(&now, &ltime);
			strftime(buffer, 32, "[%H:%M:%S] ", &now);
			int	l_iLength = (int)strlen(buffer);
			NvFWrite( buffer, sizeof(char),l_iLength, m_pFile );
	#endif //WIN32
	}

	bool	cBinaryFile::SetFilePos(int e_iPos)
	{
		return NvFSeek(m_pFile,e_iPos,SEEK_SET)==0?true:false;
	}

	int64		cBinaryFile::GetInt64(void*e_pData)
	{
		return *(int64*)e_pData;
	}

	float		cBinaryFile::GetFloat(void*e_pData)
	{
		return *(float*)e_pData;
	}

	//double		cBinaryFile::GetDouble(void*e_pData)
	//{
	//	double	l_dbValue = 0.f;
	//	memcpy(&l_dbValue,e_pData,sizeof(double));
	//	//return *(double*)e_pData;
	//	return l_dbValue;
	//}

	int			cBinaryFile::GetInt(void*e_pData)
	{
		return *(int*)e_pData;
	}

	char		cBinaryFile::GetChar(void*e_pData)
	{
		return *(char*)e_pData;
	}

	cSkipHeaderBinaryFile::cSkipHeaderBinaryFile(int e_iMagicID)
	{
		m_iMagicNumberID = e_iMagicID;
		m_iSkpiHeaderSize = rand();
	}

	cSkipHeaderBinaryFile::~cSkipHeaderBinaryFile()
	{
	
	}

	void*		cSkipHeaderBinaryFile::GetDataFile(UINT e_uiStart)
	{
		void*l_pData = cBinaryFile::GetDataFile(e_uiStart);
		if( !l_pData )
			return 0;
		char*l_pOutputData = (char*)l_pData;
		if( GetDataFileSize() < sizeof(m_iMagicNumberID) )
		{//not cSkipHeaderBinaryFile file.
			return 0;
		}
		int	l_iHeaderNum = 0;
		int	l_iSkipHeaderData = 0;
		memcpy(&l_iHeaderNum,l_pOutputData,sizeof(int));
		if( m_iMagicNumberID != l_iHeaderNum)
		{
			return 0;
		}
		l_pOutputData += sizeof(m_iMagicNumberID);
		//not fetch second int data then skip random data
		memcpy(&l_iSkipHeaderData,l_pOutputData,sizeof(int));
		l_pOutputData += sizeof(m_iSkpiHeaderSize);
		l_pOutputData += l_iSkipHeaderData;
		l_pOutputData += e_uiStart;
		return l_pData;
	}

	bool		cSkipHeaderBinaryFile::Writefile(const char*e_strFileName,bool e_bBinary,bool e_bForceToWrite)
	{
		if(cBinaryFile::Writefile(e_strFileName,e_bBinary,e_bForceToWrite))
		{
			this->m_iSkpiHeaderSize	= rand(1,1024);
			this->WriteToFile(this->m_iMagicNumberID);
			this->WriteToFile(this->m_iSkpiHeaderSize);
			char*l_pRandData = new char[m_iSkpiHeaderSize];
			for( int i=0;i<m_iSkpiHeaderSize;++i )
				l_pRandData[i] = (char)rand()%255;
			NvFWrite( l_pRandData, sizeof(char)*m_iSkpiHeaderSize, 1, m_pFile );
			delete l_pRandData;
			return true;
		}
		return false;
	}

	int			cSkipHeaderBinaryFile::GetSkipHeaderAndHMagicNumberHeader()
	{
		if( m_pFile )
			return sizeof(m_iMagicNumberID)+sizeof(m_iSkpiHeaderSize)+m_iSkpiHeaderSize;
		return -1;
	}
}