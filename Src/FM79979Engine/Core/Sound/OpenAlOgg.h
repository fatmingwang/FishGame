#ifndef OPENAL_OGG_H
#define OPENAL_OGG_H
#include "SoundShell.h"
namespace FATMING_CORE
{
	void LoadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);

	class	cOpanalOgg:public cBasicSound
	{
	public:
		DEFINE_TYPE_INFO();
		cOpanalOgg(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName);
		virtual ~cOpanalOgg();
		virtual	NamedTypedObject*	Clone(){ return 0; }
		virtual void	OpenFile(const char*e_strileName);
	};

}

//fslStream_OGG::fslStream_OGG()
//{
// className = "fslSound";
// iMemSize = 0;
// iLength = 0;
// bSourceRelative = false;
// bIsLooping = false;
// bForceStop = false;
// bActive = false;
// source = buffer = 0;
// current_gain = 1.0f;
// outer_gain = 0;
// snd_info.uiChannels = snd_info.uiFrequency = snd_info.uiSampling = 0;
//}
//
//fslStream_OGG::~fslStream_OGG()
//{
// if (bStreamCreated)
// {
//  alSourceStop(source);
//  Empty();
//  alDeleteSources(1,&source);
//  alDeleteBuffers(2,buffers);
//  ov_clear(&oggStream);
// }
//}
//
//bool fslStream_OGG::Update()
//{
// ALenum state; 
// alGetSourcei(source,AL_SOURCE_STATE,&state);
//
// if (state == AL_PAUSED || !bActive) return false;
//
// int processed;
//    alGetSourcei(source,AL_BUFFERS_PROCESSED,&processed);
//
//    while (processed--)
//    {
//        ALuint bufferI;
//        alSourceUnqueueBuffers(source,1,&bufferI);
//        Stream(bufferI);
//  if (bActive) alSourceQueueBuffers(source,1,&bufferI);
//    }
//
// if (state == AL_STOPPED || !bActive)
// {
//  bActive = false;
//  StreamSetPos(0.0f);
//  if (bForceStop) return false;
//
//  if (bIsLooping)
//  {
//   alSourceStop(source); // <- *** note these ***
//   Empty();    // <- *** 2 lines of code ***
//   StreamPlay();
//   alSourcePlay(source);
//  }
//  else
//  {
//   return true;
//  }
// }
//
// return false;
//}
//
//void fslStream_OGG::StreamPlay()
//{
// if (!bActive)
// {
//  bActive = true;
//  bForceStop = false;
//  Stream(buffers[0]);
//  Stream(buffers[1]);
//
//  alSourceQueueBuffers(source,2,buffers);
// }
//}
//
//bool fslStream_OGG::Open(const char* strFile)
//{
// bStreamCreated = false;
// vorbis_info* vorbisInfo;
// oggFile = MyFileOpen(strFile, "rb" );
//
// if (!oggFile) return false;
//
// if (ov_open_callbacks(oggFile,&oggStream,NULL,0,OV_CALLBACKS_DEFAULT) != 0)
// {
//  fclose(oggFile);
//  return false;
// }
//
// vorbisInfo = ov_info(&oggStream,-1);
//
// if (vorbisInfo->channels == 1)
//  format = AL_FORMAT_MONO16;
// else format = AL_FORMAT_STEREO16;
//
// alGenBuffers(2,buffers);
// alGenSources(1,&source);
//
// iLength = (long)(ov_time_total(&oggStream,-1) * 1000.0);
// snd_info.uiChannels = (format == AL_FORMAT_MONO8 || format == AL_FORMAT_MONO16)? 1:2;
// snd_info.uiSampling = (format == AL_FORMAT_MONO8 || format == AL_FORMAT_STEREO8)? 8:16;
// snd_info.uiFrequency = vorbisInfo->rate;
//
// bStreamCreated = true;
// bIsStream = true;
//
// return true;
//}
//
//void fslStream_OGG::Stream(ALuint bufferI)
//{
// int  size = 0;
// int  section;
// int  result;
//
// bActive = true;
//
// while (size < OGG_STREAM_BUFFER_SIZE)
// {
//  result = ov_read(&oggStream,data + size,OGG_STREAM_BUFFER_SIZE - size,0,2,1,&section);
//
//  if (result > 0)
//  {
//   size += result;
//  }
//  else
//  {
//   if (result < 0) return; else break;
//  }
// }
//
// if (size == 0) { bActive = false; return; }
//
//    alBufferData(bufferI,format,data,size,snd_info.uiFrequency);
//}
//
//void fslStream_OGG::Empty()
//{
// int queued;
// alGetSourcei(source,AL_BUFFERS_QUEUED,&queued);
//
// while (queued--)
// {
//        ALuint bufferI;
//  alSourceUnqueueBuffers(source,1,&bufferI);
// }
//}
//
//void fslStream_OGG::StreamSetPos(float p)
//{
// ov_time_seek(&oggStream,p);
//}
//
//float fslStream_OGG::StreamGetPos()
//{
// return (float)ov_time_tell(&oggStream);
//}
#endif