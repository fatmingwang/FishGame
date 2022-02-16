#ifndef _MULTI_TOUCH_POINTS_
#define _MULTI_TOUCH_POINTS_

#include "Mouse_Touch.h"
#include "../Camera/MouseCamera.h"
namespace FATMING_CORE
{
	//here should get 2 points to decide its gesture
	//ex:pan(2 fingers move),zoom,
	//if its pan 2 fingers move direction could be dectec by direction's angle
	//if angle is bigger than 90(or specific one) is should be zoom,but it's possible
	//one finger move andother is stay at same position.
	//if zoom gesture its mean 2 finger's direction's angle is bigger than 90.
	//current it only support 2 touch points.

	//mouse left button for two fingers pan
	//mouse mid button for 2 fingers zoom
	#define	MAX_TOUCH_POINTS		4//although its four but it only works for 2 points
	enum	eGestureEnum
	{
		eGE_CLICK_DOWN = 0,	//click with collision range
		eGE_HORVER,			//stay at same position for a while
		eGE_CLICK_UP,		//click up
		eGE_DOUBLE_CLICK,	//click twice with collision range at a short time//DOUBLE_CLICK_TIME_SPAN
		eGE_SLIDE_LEFT,		//
		eGE_SLIDE_UP,		//
		eGE_SLIDE_RIGHT,	//
		eGE_SLIDE_DOWN,		//
		eGE_ZOOM_IN,		//
		eGE_ZOOM_OUT,		//
		eGE_MAX,
	};

	eGestureEnum	GetGesture(const WCHAR*e_strGestureEnum);
	const WCHAR*	ValueToStringW(eGestureEnum e_eGestureEnum);
	struct	sMultiTouchPoints
	{
		sMultiTouchPoints();
		sMouseMoveData	TouchPoints[MAX_TOUCH_POINTS];
		//how much touch point has been touched
		bool			bTouched[MAX_TOUCH_POINTS];
		//current touch points count
		int				iTouchPoints;
		//prior touch points count
		int				iLastFrameTouchPoints;
		//current support for 2 touch points
		float			fTouchPointsDistance;
		Vector2			vTouchMoveDirection;
		//
		eGestureEnum	Gesture;
		//I am assume all points only call mouse event one time
		void			MouseDown(int e_iPosX,int e_iPosY);
		void			MouseMove(int e_iPosX,int e_iPosY);
		void			MouseUp(int e_iPosX,int e_iPosY);
		void			Update(float e_fElpaseTime);
		sMouseState		GetCameraMouseState();
	};
//end namespace FATMING_CORE
}
#endif