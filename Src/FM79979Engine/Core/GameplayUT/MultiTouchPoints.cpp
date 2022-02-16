#include "../stdafx.h"
#include "MultiTouchPoints.h"
namespace FATMING_CORE
{
	eGestureEnum	GetGesture(const WCHAR*e_strGestureEnum)
	{	
		if( !wcscmp(e_strGestureEnum,L"ClickDown") )
			return eGE_CLICK_DOWN;
		if( !wcscmp(e_strGestureEnum,L"Horver") )
			return eGE_HORVER;
		if( !wcscmp(e_strGestureEnum,L"ClickUp") )
			return eGE_CLICK_UP;
		if( !wcscmp(e_strGestureEnum,L"DoubleClick") )
			return eGE_DOUBLE_CLICK;
		if( !wcscmp(e_strGestureEnum,L"SlideLeft") )
			return eGE_SLIDE_LEFT;
		if( !wcscmp(e_strGestureEnum,L"SlideUp") )
			return eGE_SLIDE_UP;
		if( !wcscmp(e_strGestureEnum,L"SlideRight") )
			return eGE_SLIDE_RIGHT;
		if( !wcscmp(e_strGestureEnum,L"SlideDown") )
			return eGE_SLIDE_DOWN;
		if( !wcscmp(e_strGestureEnum,L"ZoomIn") )
			return eGE_ZOOM_IN;
		if( !wcscmp(e_strGestureEnum,L"ZoomOut") )
			return eGE_ZOOM_OUT;
		assert(0&&"GestureEnum data wrong!!");
		return eGE_MAX;
	}

	const WCHAR*			ValueToStringW(eGestureEnum e_eGestureEnum)
	{
		if( e_eGestureEnum == eGE_CLICK_DOWN )
			return L"ClickDown";
		if( e_eGestureEnum == eGE_HORVER )
			return L"Horver";
		if( e_eGestureEnum == eGE_CLICK_UP )
			return L"ClickUp";
		if( e_eGestureEnum == eGE_DOUBLE_CLICK )
			return L"DoubleClick";
		if( e_eGestureEnum == eGE_SLIDE_LEFT )
			return L"SlideLeft";
		if( e_eGestureEnum == eGE_SLIDE_UP )
			return L"SlideUp";
		if( e_eGestureEnum == eGE_SLIDE_RIGHT )
			return L"SlideRight";
		if( e_eGestureEnum == eGE_SLIDE_DOWN )
			return L"SlideDown";
		if( e_eGestureEnum == eGE_ZOOM_IN )
			return L"ZoomIn";
		if( e_eGestureEnum == eGE_ZOOM_OUT )
			return L"ZoomOut";
		return L"unknow";
	}

	sMultiTouchPoints::sMultiTouchPoints()
	{
		iTouchPoints = 0;
		iLastFrameTouchPoints = 0;
		fTouchPointsDistance = 0;
		Gesture = eGE_MAX;
	}


	void	sMultiTouchPoints::MouseDown(int e_iPosX,int e_iPosY)
	{
		TouchPoints[iTouchPoints].MouseDown(e_iPosX,e_iPosY);
		bTouched[iTouchPoints] = true;
		iTouchPoints++;
	}
	void	sMultiTouchPoints::MouseMove(int e_iPosX,int e_iPosY)
	{
		TouchPoints[iTouchPoints].MouseMove(e_iPosX,e_iPosY);
		bTouched[iTouchPoints] = true;
		iTouchPoints++;
	}
	void	sMultiTouchPoints::MouseUp(int e_iPosX,int e_iPosY)
	{
		TouchPoints[iTouchPoints].MouseUp(e_iPosX,e_iPosY);
		bTouched[iTouchPoints] = false;
		iTouchPoints++;
	}

	void	sMultiTouchPoints::Update(float e_fElpaseTime)
	{
		if( iLastFrameTouchPoints >= 2&& iTouchPoints == 1 )
		{//from 2 points becom 1 touch points,change mouse down position avoid huge move.
			for( int i=0;i<iTouchPoints;++i )
			{
				POINT	l_Pos = TouchPoints[i].DownPos;
				TouchPoints[i].MouseDown(l_Pos.x,l_Pos.y);
			}
		}
		//get gesture
		//if( iTouchPoints > 1 )
		//{
		//
		//}
		iLastFrameTouchPoints = iTouchPoints;
		memset(bTouched,0,sizeof(bool)*MAX_TOUCH_POINTS);
		iTouchPoints = 0;
	}

	sMouseState	sMultiTouchPoints::GetCameraMouseState()
	{
		sMouseState	l_sMouseState;
		memset(&l_sMouseState,0,sizeof(sMouseState));
		if( iTouchPoints == 1 )
		{
			memset(&l_sMouseState,0,sizeof(sMouseState));
			l_sMouseState.bLefButton = true;
			sMouseMoveData	*l_pMouseMoveData = &TouchPoints[0];
			l_sMouseState.iMoveX = l_pMouseMoveData->FrameMovePos.x;
			l_sMouseState.iMoveY = l_pMouseMoveData->FrameMovePos.y;
			l_sMouseState.iCurrentX = l_pMouseMoveData->MovePos.x;
			l_sMouseState.iCurrentY = l_pMouseMoveData->MovePos.y;
			fTouchPointsDistance = 0.f;
			assert(0&&"lazy to do this now");
			//Gesture = eFG_ONE_FINGER;
		}
		else
		if( iTouchPoints == 2 )
		{
			sMouseMoveData	*l_pMouseMoveData1 = &TouchPoints[0];
			sMouseMoveData	*l_pMouseMoveData2 = &TouchPoints[1];
			float	l_fLastTouchPointDis = this->fTouchPointsDistance;
			Vector2	l_vTouchMoveDirection = Vector2((float)l_pMouseMoveData1->MovePos.x-l_pMouseMoveData2->MovePos.x,(float)l_pMouseMoveData1->MovePos.y-l_pMouseMoveData2->MovePos.y);
			fTouchPointsDistance = l_vTouchMoveDirection.Length();
			if( l_fLastTouchPointDis == 0.f )
			{
				//Gesture = eFG_NONE;
				assert(0&&"lazy to do this now");
				return l_sMouseState;
			}

			if( fabs(fTouchPointsDistance - l_fLastTouchPointDis)<=MIN_TWO_FINGERS_MOVE_DISTANCE )
			{
				//Gesture = eFG_MOVE;
				assert(0&&"lazy to do this now");
				l_sMouseState.bRigButton = true;
				l_sMouseState.iMoveX = (int)l_vTouchMoveDirection.x;
				l_sMouseState.iMoveY = (int)l_vTouchMoveDirection.y;
				return l_sMouseState;
			}
			l_sMouseState.bMidButton = true;
			if( l_fLastTouchPointDis<fTouchPointsDistance )
			{
				//Gesture = eFG_ZOOM_IN;
				assert(0&&"lazy to do this now");
				l_sMouseState.iMoveZ = 1;
			}
			else
			{
				//Gesture = eFG_ZOOM_OUT;
				assert(0&&"lazy to do this now");
				l_sMouseState.iMoveZ = -1;
			}
		}
		return l_sMouseState;
	}
//end namespace
}