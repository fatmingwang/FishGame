#ifndef	_TIME_UT_H_
#define	_TIME_UT_H_
namespace UT
{
	struct sTimeAndFPS
	{
#ifdef IOS
		static double dbConversion;
#endif
		//calculate	elpase time
		UINT					uiPreviousTime;		//previously frame time ticks
		UINT					uiCurrentTime;		//current frame time ticks
		UINT					uiElpaseTime;		//current frame substract previously frame = elpase time
		float					fElpaseTime;		//uiElpaseTime/1000.f
		//for frame rate
		UINT					uiNumFrame;			//per second frame rate
		float					fTimeFor1Sec;		//detect is time reached 1 sec

		char					strFrameRate[16];

		sTimeAndFPS();
		void					Update();
		char* 					GetFPS();
		char* 					GetDynamicFPS();
		static	float			GetSecondDiff(sTimeAndFPS*e_pT1,sTimeAndFPS*e_pT2);
	};
	//to calculate the destination time is reached or not,
	struct	sTimeCounter
	{
		bool					bLoop;//do not set this directly,instead by SetLoop
		float					fRestTime;
		float					fTargetTime;
		//while the target time is reached set to true,else false
		bool					bTragetTimrReached;
		sTimeCounter(){ bTragetTimrReached = true; fRestTime = fTargetTime = 1.f; bLoop = false; }
		sTimeCounter(float e_fTargetTime);
		void					Start(float e_fElpaseTime = 0.f);//it could be start by specific time that it have had past
		void					Update(float e_fElpaseTime);
		void					SetTargetTime(float e_fTargetTime);
		void					SetLoop(bool e_bLoop);
		//0 to 1.
		float					GetLERP();
	};
	//
	//=============
	//only active once,if it has been actived,it won't be active until start is called again.
	//=============
	struct	sKeyTimeActive
	{
		bool				bTriggered;
		float				fTargetTime;
		//u can call this to reset active time
		void				Start(float e_fTargetTime){ fTargetTime = e_fTargetTime; bTriggered = false; }
		//ensure u have set dest time already
		void				Start(){ bTriggered = false; }
		bool				IsActive(float e_fCurrentTime);
	};
//end namespace UT
}

#endif