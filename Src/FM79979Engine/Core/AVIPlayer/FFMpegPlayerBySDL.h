#ifdef THIS_IS_FOR_SDL
#ifndef FFMPEG_PLAYER_H
#define FFMPEG_PLAYER_H
#ifdef WIN32
//#define inline _inline
//#define bool _Bool
//#define true 1
//#define false 0
extern "C"
{
#include "../../Include/libavcodec/avcodec.h"
#include "../../Include/libavformat/avformat.h"
#include "../../Include/libswscale/swscale.h"
};
#else
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};
#endif
//
//#include <SDL.h>
//#include <SDL_thread.h>
//#include <stdio.h>
//#include <math.h>

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0
#define SAMPLE_CORRECTION_PERCENT_MAX 10
#define AUDIO_DIFF_AVG_NB 20
#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT (SDL_USEREVENT + 2)
#define VIDEO_PICTURE_QUEUE_SIZE 1
#define DEFAULT_AV_SYNC_TYPE AV_SYNC_VIDEO_MASTER

class PacketQueue;
void packet_queue_flush(PacketQueue *q);
class PacketQueue
{
public:
	PacketQueue(){ first_pkt = 0; last_pkt = 0;  mutex = 0; cond = 0; }
	void	Destroy()
	{
		if(this->first_pkt)
			packet_queue_flush(this);
		if(mutex)
			SDL_DestroyMutex(mutex);
		if(cond)
			SDL_DestroyCond(cond);
	}
	~PacketQueue()
	{
		this->Destroy();
		assert(!last_pkt);
		assert(!first_pkt);
	}
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
  SDL_mutex *mutex;
  SDL_cond *cond;
};

class VideoPicture
{
public:
  SDL_Overlay *bmp;
  int width, height; /* source height & width */
  int allocated;
  double pts;
  VideoPicture()
  {
	bmp = 0;
  }
  void	Destroy()
  {
	  SDL_FreeYUVOverlay(bmp);
	  bmp = 0;
  }
  ~VideoPicture()
  {
	assert(!bmp);
  }
  
};
class VideoState 
{
public:
  AVFormatContext *pFormatCtx;
  int             videoStream, audioStream;

  int             av_sync_type;
  double          external_clock; /* external clock base */
  int64_t         external_clock_time;
  int             seek_req;
  int             seek_flags;
  int64_t         seek_pos;

  double          audio_clock;
  AVStream        *audio_st;	//pFormatCtx->streams[iAudioCodecIndex]
  PacketQueue     *pAudioq;
  uint8_t         audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
  unsigned int    audio_buf_size;
  unsigned int    audio_buf_index;
  AVPacket        audio_pkt;
  uint8_t         *audio_pkt_data;
  int             audio_pkt_size;
  int             audio_hw_buf_size;  
  double          audio_diff_cum; /* used for AV difference average computation */
  double          audio_diff_avg_coef;
  double          audio_diff_threshold;
  int             audio_diff_avg_count;
  double          frame_timer;
  double          frame_last_pts;
  double          frame_last_delay;
  double          video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
  double          video_current_pts; ///<current displayed pts (different from video_clock if frame fifos are used)
  int64_t         video_current_pts_time;  ///<time (av_gettime) at which we updated video_current_pts - used to have running video pts
  AVStream        *video_st;//pFormatCtx->streams[iVideoCodecIndex]
  PacketQueue     *pVideoq;
  VideoPicture    *pictq;//[VIDEO_PICTURE_QUEUE_SIZE];
  int             pictq_size, pictq_rindex, pictq_windex;
  SDL_mutex       *pictq_mutex;
  SDL_cond        *pictq_cond;
  SDL_Thread      *parse_tid;
  SDL_Thread      *video_tid;
  //wait for file open and close all thread
  SDL_mutex       *pMutex;
  SDL_cond        *pCond;
  //SDL_mutex       *pVideoEndMutex;
  //SDL_cond        *pVideoEndCond;

  char            filename[1024];
  int             quit;
  //ture if file open
  bool			  bFileOpen;
  //index for video and audio
  int	iVideoCodecIndex;
  int	iAudioCodecIndex;
  //for video frame
  AVFrame*	pFrame;
  bool	bVideoThreadEnd;
  void	Init()
  {
	bVideoThreadEnd = false;
	pMutex = 0;
	pCond = 0;
	//pVideoEndMutex = 0;
	//pVideoEndCond = 0;
	pFrame = 0;
	audio_st = 0;
	video_st = 0;
	bFileOpen = false;
	quit = 0;
	pFormatCtx = 0;
	audio_pkt_data = 0;
	pictq_mutex = 0;
	pictq_cond = 0;
	parse_tid = 0;
	video_tid = 0;
	iVideoCodecIndex = -1;
	iAudioCodecIndex = -1;
	pAudioq = new PacketQueue();
	pVideoq = new PacketQueue();
	pictq = new VideoPicture();
  }
  void	Destroy()
  {
	if( iVideoCodecIndex!=-1 )
		avcodec_close(pFormatCtx->streams[iVideoCodecIndex]->codec);
	if( iAudioCodecIndex!=-1 )
		avcodec_close(pFormatCtx->streams[iAudioCodecIndex]->codec);
	if( pFormatCtx )
		av_close_input_file(pFormatCtx);
	if( pFrame )
	{
		av_free(pFrame);
		pFrame = 0;
	}
	av_free_packet(&audio_pkt);
	//if(audio_pkt_data)
	if(pictq_mutex)
		SDL_DestroyMutex(pictq_mutex);
	if(pictq_cond)
		SDL_DestroyCond(pictq_cond);
	SAFE_DELETE(pAudioq);
	SAFE_DELETE(pVideoq);
	//for(int i=0;i<VIDEO_PICTURE_QUEUE_SIZE;++i)
	//	pictq[i].Destroy();
	pictq->Destroy();
	SAFE_DELETE(pictq);
	if( pMutex )
		SDL_DestroyMutex(pMutex);
	if( pCond )
		SDL_DestroyCond(pCond);
	//if( pVideoEndMutex )
	//	SDL_DestroyMutex(pVideoEndMutex);
	//if( pVideoEndCond )
	//	SDL_DestroyCond(pVideoEndCond);
	//if(!parse_tid)
	//if(!video_tid)
  }
};

enum {
  AV_SYNC_AUDIO_MASTER,
  AV_SYNC_VIDEO_MASTER,
  AV_SYNC_EXTERNAL_MASTER,
};

void alloc_picture(void *userdata);
void video_refresh_timer();
class	cFFMpegPlayer
{
	  //it decide the render frequence
	  GET_SET_DEC(float,m_fFPS,GetFPS,SetFPS);
	  //current play progess
	  float m_fCurrentime;
	  //is it time to render
	  float	m_fCurrentTimeToDetectedTimeForRender;
	  bool	m_bRender;
public:
	cFFMpegPlayer();
	virtual ~cFFMpegPlayer();
//below is single thread
	bool			m_bPlayOver;
	Uint32 rmask, gmask, bmask, amask;
	SwsContext *img_convert_ctx;
    AVFormatContext *pFormatCtx;
    int             videoStream;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame         *pFrame; 
    AVFrame         *pFrameRGB;
    AVPacket        packet;
    int             frameFinished;
    int             numBytes;
	int				ret;
    uint8_t         *buffer;
	SDL_Surface		*src;
	bool	OpenFileBySingleThread(char*e_strFileName);
    void    UpdateBySingleThread(float e_fElpaseTime);
    void    RenderBySingleThread();
	void	DestroyBySingleThread();
//below is multithreads
	bool	OpenFile(char*e_strFileName);
    float	GetDuration();
	float	GetCurrenTime(){ return this->m_fCurrentime; }
	void	SetCurrentTime(float e_fCurrentime);
    void    Update(float e_fElpaseTime)
    {
		m_fCurrentime+=e_fElpaseTime;
		float l_fDestTime = 1/m_fFPS;
		m_fCurrentTimeToDetectedTimeForRender+=e_fElpaseTime;
		if(m_fCurrentTimeToDetectedTimeForRender>=l_fDestTime)
		{
			m_fCurrentTimeToDetectedTimeForRender -= l_fDestTime;
			m_bRender = true;
		}
		else
			m_bRender = false;
    }
    void    Render()
    {
      if( m_bRender )
        video_refresh_timer();
    }
};


#endif//end fuck
#endif//end FFMPEG_PLAYER_H