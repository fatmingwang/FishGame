#ifndef SOUND_SHELL_H
#define SOUND_SHELL_H




#if defined(WIN32)
#include "../../../Include/AL/alut.h"
#elif defined(IOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif defined(ANDROID)//set project path to \Sound\OpenAL_Android\ALSoft
#include "AL/al.h"
#include "AL/alc.h"
//#include "al.h"
//#include "alc.h"
//#else//linux
//#include "alut.h"
#endif

#include "../SmartPointer.h"
namespace FATMING_CORE
{
	//========================
	//whole sound have implement this,because we need quickly to sort,the sound buffer nad source data
	//it's quick tightly to combine with cSoundParser's static members
	//
	//========================
	class	cBasicSound:public cSmartObject
	{
		static	ALuint	m_suiTotalSoundIndex;		//how many sound we have created
		ALuint	m_uiIndex;							//to check the index is not same as destination
	protected:
		//sound data
		GET_SET_DEC(ALenum,m_iFormat,GetFormat,SetFormat);
		GET_SET_DEC(ALsizei,m_iSize,GetSize,SetSize);
		GET_SET_DEC(ALsizei,m_iFreq,GetFreq,SetFreq);
		//
		ALuint	m_uiBufferID;	//unique ID
		ALuint	m_uiSourceID;	//it could be dynamic
		ALuint	m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex;
		ALuint	m_uiUsingSourceIDIndex;	//while play is called,set source ID Index and set up value fo m_uiSourceID,it's related to ALuint*	cSoundParser::m_psuiSourceUsingIDIndex
		bool	m_bLoop;		//
		float	m_fTimeLength;	//
		bool	m_bPause;
		float	m_fVolume;
		//
		GET_SET_DEC(int,m_iPiority,GetPiority,SetPiority);
		GET_SET_DEC(bool,m_bStopWhileLeave,IsStopWhileLeave,SetStopWhileLeave);
	public:
		DEFINE_TYPE_INFO();
		cBasicSound(NamedTypedObject*e_pNamedTypedObject);
		virtual ~cBasicSound();
		bool	IsUsing();//check is it still on using(occupied source ID)
		virtual	void	Pause(bool e_bPause = true);
		virtual	void	Rewind();
		virtual	void	AssignBufferData(void*e_pBuffer);
		virtual	void	Destroy();
		virtual	void	OpenFile(const char*e_strileName){}
		virtual	void	Play(bool e_bPlay);
		virtual	bool	IsPlay();
		virtual	void	SetLoop(bool e_bLoop);
		virtual	bool	IsLoop();
		//iphone support?
		virtual	void	SetVolume(float e_fVolume);
		float			GetVolume(){return m_fVolume;}
		ALenum			PerSampleBitAndChannelToOpenALFormat(int e_iBit,int e_iChannel);
		void check();
	};
	std::string    GetFileNameWithExtension(cBasicSound*e_pBasicSound);
}




#endif