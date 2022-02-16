#include "../stdafx.h"
#include "OpenAlOgg.h"

#ifdef IOS
//#include "ogg.h"
//#include "vorbisfile.h"
#else
#ifdef WIN32
#include "../../../Include/ogg/ogg.h"
#include "../../../Include/vorbis/vorbisfile.h"
#else//linux
//#include "ogg.h"
//#include "vorbisfile.h"
#endif
#endif
namespace FATMING_CORE
{

	#define SOUND_BUFFER_SIZE     32768       // 32 KB buffers
	void LoadOGG(const char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq,float&e_fTotalPlayTime)
	{
#ifdef _OGG_
		int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
		int bitStream;
		long bytes;
		char array[SOUND_BUFFER_SIZE];                // Local fixed size array
		FILE *f;

		// Open for binary reading
		f = MyFileOpen(fileName, "rb" );

		if (f == NULL)
		{
			UT::ErrorMsg("can't open Sound file",fileName);
		}
		// end if

		vorbis_info *pInfo;
		OggVorbis_File oggFile;

		// Try opening the given file
		if (ov_open(f, &oggFile, NULL, 0) != 0)
		{
			UT::ErrorMsg("can't open ogg file",fileName);
		}
		e_fTotalPlayTime = (float)ov_time_total(&oggFile,0);
		// end if

		// Get some information about the OGG file
		pInfo = ov_info(&oggFile, -1);

		// Check the number of channels... always use 16-bit samples
		if (pInfo->channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
		// end if

		// The frequency of the sampling rate
		freq = pInfo->rate;

		// Keep reading until all is read
		do
			{
			// Read up to a buffer's worth of decoded sound data
			bytes = ov_read(&oggFile, array, SOUND_BUFFER_SIZE, endian, 2, 1, &bitStream);

			if (bytes < 0)
			{
				UT::ErrorMsg("file is empty",fileName);
			}
			// end if

			// Append to end of buffer
			buffer.insert(buffer.end(), array, array + bytes);
			}
		while (bytes > 0);
		//l_fTime = ov_time_total(&oggFile,1);
		// Clean up!
		ov_clear(&oggFile);
	//#else
	//	assert(0&&"apple ogg file........Z_Z");
#endif
	}

	const wchar_t*     cOpanalOgg::TypeID( L"cOpanalOgg" );
	cOpanalOgg::cOpanalOgg(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName):cBasicSound(e_pNamedTypedObject)
	{
		OpenFile(e_strileName);
	}

	cOpanalOgg::~cOpanalOgg()
	{

	}

	void	cOpanalOgg::OpenFile(const char*e_strileName)
	{
		this->Destroy();
#ifndef ANDROID
		alGenBuffers(1, &m_uiBufferID);
		alGenSources(1, &m_uiSourceID);
#endif
		vector<char> buffer;
		LoadOGG(e_strileName,buffer,m_iFormat,m_iFreq,m_fTimeLength);
		m_iSize = static_cast<ALsizei>(buffer.size());
		if( buffer.size() )
		{
			AssignBufferData(&buffer[0]);
			SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strileName)).c_str());
		}
	}
}