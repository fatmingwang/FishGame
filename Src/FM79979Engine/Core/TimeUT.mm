#include "stdafx.h"
#include "TimeUT.h"
#include <time.h>
#ifdef WIN32
#include <direct.h>
#endif
#if defined(IOS)
#include <mach/mach_time.h>
#include <sys/time.h>
double sTimeAndFPS::dbConversion = 0.0;
#elif defined(WIN32)
#include "strsafe.h"
#pragma warning( disable : 4793 )
#pragma warning( disable : 4995 )
#include "../../Include/wglext.h"
#elif defined(LINUX)
#include "SDL.h"
#elif defined(ANDROID)
#include "jni.h"
#endif
namespace UT
{
	sTimeAndFPS::sTimeAndFPS()
	{
		uiElpaseTime = 0;
		uiPreviousTime = 0;
		uiCurrentTime = 0;

		uiNumFrame = 0;
		fTimeFor1Sec = 0.f;
		memset(strFrameRate,0,sizeof(strFrameRate));
		sprintf(strFrameRate,"%s fps","0");
#ifdef IOS
		if( dbConversion == 0 )
		{
			dbConversion = 1.0;//avoid enter again
			mach_timebase_info_data_t l_Info;
			kern_return_t err = mach_timebase_info(&l_Info);
			if( err == 0 )
			    dbConversion = 1e-9*(double)l_Info.numer/(double)l_Info.denom;
				//dbConversion = 1e-9;//*(double)l_Info.numer/(double)l_Info.denom;
		}
#endif
	}
	void	sTimeAndFPS::Update()//total game running time
	{
		uiPreviousTime = uiCurrentTime;
#if defined(WIN32)
		uiCurrentTime = GetTickCount();
#elif defined(IOS)
		uiCurrentTime = mach_absolute_time();
#else//android or linux and else
		struct timeval current;
		gettimeofday(&current, NULL);
		uiCurrentTime = current.tv_sec * 1000 + current.tv_usec/1000;
#endif

		uiElpaseTime = uiCurrentTime-uiPreviousTime;
#ifndef IOS		
		fElpaseTime = (float)uiElpaseTime/1000.f;
#else
		fElpaseTime = (float)((double)uiElpaseTime*dbConversion);
#endif
		uiNumFrame++;
		fTimeFor1Sec += fElpaseTime;
		if(fTimeFor1Sec>=1.f)
		{
			fTimeFor1Sec-=(int)fTimeFor1Sec;
			sprintf(strFrameRate,"%i fps",uiNumFrame);
			uiNumFrame = 0;
		}
	}

	float	sTimeAndFPS::GetSecondDiff(sTimeAndFPS*e_pT1,sTimeAndFPS*e_pT2)
	{
		UINT	l_uiResult = e_pT1->uiCurrentTime - e_pT2->uiCurrentTime;
		float	l_fElpaseTime = (float)(double)l_uiResult/1000.f;
		return l_fElpaseTime;
	}

	char* sTimeAndFPS::GetFPS()
	{
		return strFrameRate;
	}

	sTimeCounter::sTimeCounter(float e_fTargetTime)
	{
		SetTargetTime(e_fTargetTime);
		bLoop = false;
	}

	void	sTimeCounter::SetTargetTime(float e_fTargetTime)
	{
		bTragetTimrReached = false;
		fRestTime = e_fTargetTime;
		this->fTargetTime = e_fTargetTime;	
	}

	float	sTimeCounter::GetLERP()
	{
		float	l_fLERP = (1-fRestTime/fTargetTime);
		return l_fLERP>1?1:l_fLERP;
	}

	void	sTimeCounter::SetLoop(bool e_bLoop)
	{
		this->bLoop = e_bLoop;
		bTragetTimrReached = false;
	}

	//m_TimToFire.Start(m_TimToFire.fRestTime*-1);//start with past time set test as nagetive
	void	sTimeCounter::Start(float e_fElpaseTime)
	{
		bTragetTimrReached = false;
		fRestTime = fTargetTime-e_fElpaseTime;
	}

	void	sTimeCounter::Update(float e_fElpaseTime)
	{
		if( bTragetTimrReached )
		{
			if( bLoop )
				bTragetTimrReached = false;
			else
				return;
		}
		fRestTime-=e_fElpaseTime;
		if( fRestTime<=0.f )
		{
			bTragetTimrReached = true;
			if( bLoop )
			{
				fRestTime = fTargetTime+fRestTime;
				//if( -fRestTime>fTargetTime )
				//{
				//	fRestTime = fTargetTime;
				//}
				//else
				//	fRestTime = fTargetTime+fRestTime;
			}
		}
	}
	bool	sKeyTimeActive::IsActive(float e_fCurrentTime)
	{
		if( !bTriggered )
		{
			if( e_fCurrentTime>=fTargetTime ) 
			{
				bTriggered = true;
				return true;
			}
		}
		return false;
	}
//end namespace UT
}