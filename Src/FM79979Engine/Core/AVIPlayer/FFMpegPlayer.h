#ifndef FFMPEG_PLAYER_H
#define FFMPEG_PLAYER_H
#ifndef WIN32
#ifndef IOS
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

#include <SDL.h>
#include <SDL_thread.h>
#include <stdio.h>
#include <math.h>

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

typedef struct PacketQueue {
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
  SDL_mutex *mutex;
  SDL_cond *cond;
} PacketQueue;
typedef struct VideoPicture {
  SDL_Overlay *bmp;
  int width, height; /* source height & width */
  int allocated;
  double pts;
} VideoPicture;

typedef struct VideoState {
  AVFormatContext *pFormatCtx;
  int             videoStream, audioStream;

  int             av_sync_type;
  double          external_clock; /* external clock base */
  int64_t         external_clock_time;
  int             seek_req;
  int             seek_flags;
  int64_t         seek_pos;

  double          audio_clock;
  AVStream        *audio_st;
  PacketQueue     audioq;
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
  AVStream        *video_st;
  PacketQueue     videoq;
  VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
  int             pictq_size, pictq_rindex, pictq_windex;
  SDL_mutex       *pictq_mutex;
  SDL_cond        *pictq_cond;
  SDL_Thread      *parse_tid;
  SDL_Thread      *video_tid;

  char            filename[1024];
  int             quit;
} VideoState;

enum {
  AV_SYNC_AUDIO_MASTER,
  AV_SYNC_VIDEO_MASTER,
  AV_SYNC_EXTERNAL_MASTER,
};

void alloc_picture(void *userdata);
void video_refresh_timer(void *userdata);
class	cFFMpegPlayer
{
      //avoid couple object,
      static int    m_iNumPlay;
      VideoState      *m_pVideoState;
public:
	cFFMpegPlayer();
	virtual ~cFFMpegPlayer();
	bool	OpenFile(char*e_strFileName);
        //put this in the sdl event FF_ALLOC_EVENT::
        void    Update()
        {
            if(m_pVideoState)
                alloc_picture(m_pVideoState);
        }
        //put this in the sdl event case FF_REFRESH_EVENT:
        void    Render()
        {
          if( m_pVideoState )
            video_refresh_timer(m_pVideoState);
        }
        float    GetDuration();
};

//void GameApp::process_events()
//{
//    /* Our SDL event placeholder. */
//    SDL_Event event;
//
//    /* Grab all the events off the queue. */
//    while( SDL_PollEvent( &event ) )
//    {
//
//        switch( event.type )
//	{
//            case FF_ALLOC_EVENT:
//                g_pFFMpegPlayer->Update ();
//                break;
//            case FF_REFRESH_EVENT:
//                //g_pFFMpegPlayer->Render (&event);
//                //SDL_Flip(g_pMainSurface); //Refresh the screen
//                break;
//            case SDL_KEYDOWN:
//                    /* Handle key presses. */
//                done = true;
//                handle_key_down( &event.key.keysym );
//                    break;
//            case SDL_QUIT:
//            case FF_QUIT_EVENT:
//                    done = true;
//                    return;
//        }
//    }
//}



#endif//end LINUX
#endif//end win32
#endif