#include "../stdafx.h"
#include "../Image/PuzzleImage.h"
#include "Scroller.h"
#include "ImageButton.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	cScroller::cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,bool e_bHorizontal,cPuzzleImageUnit*e_pIcon,Vector3* e_pvIconPos)
	:cTabPage(e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
    {
		if( e_pPI )
			this->SetName(e_pPI->GetName());
		m_CurrentMousePos.x = m_CurrentMousePos.y = m_MouseMoveing.x = m_MouseMoveing.y = 0;
        m_bDirectionIsHorizontal = e_bHorizontal;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
		m_fObjectGap = 0.f;
        this->AddObject(e_pPI);
    }

	cScroller::cScroller(std::vector<cBaseImage*>*e_pImageList,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,bool e_bHorizontal,cPuzzleImageUnit*e_pIcon,Vector3* e_pvIconPos)
	:cTabPage(e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
	{
		m_fObjectGap = 0.f;
		m_CurrentMousePos.x = m_CurrentMousePos.y = m_MouseMoveing.x = m_MouseMoveing.y = 0;
        m_bDirectionIsHorizontal = e_bHorizontal;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
        this->AddObject(e_pImageList);	
	}
    //
    cScroller::~cScroller()
    {
    
    }
	//
	bool	cScroller::CollideObjectIndex(cClickMouseBehavior*e_pClickMouseBehavior,int e_iPosX,int e_iPosY)
	{
		return e_pClickMouseBehavior->Collide(e_iPosX,e_iPosY);
		bool	l_bCollided = false;
		if( m_bDirectionIsHorizontal )
		{
			if(e_iPosX>e_pClickMouseBehavior->GetLocalPositionPointer()->x)
			{
				if( e_iPosX<e_pClickMouseBehavior->GetLocalPositionPointer()->x+e_pClickMouseBehavior->GetCollisionRange().Width() )
					l_bCollided = true;
			}
		}
		else
		{
			if(e_iPosY>e_pClickMouseBehavior->GetLocalPositionPointer()->y)
			{
				if( e_iPosY<e_pClickMouseBehavior->GetLocalPositionPointer()->y+e_pClickMouseBehavior->GetCollisionRange().Height() )
					l_bCollided = true;
			}					
		}
		return l_bCollided;
	}

    //
    void    cScroller::Init()
    {
        cMouse_TouchWithCancelButton::Init();
		m_bRollBack = false;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
		if( this->m_pBG )
			m_pBG->Init();
        int l_iCOunt = this->Count();
        if( l_iCOunt == 0 )
            return;
		cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(0);
        //POINT   l_Size = {*l_pClickMouseBehavior->GetWidth(),*l_pClickMouseBehavior->GetHeight()};
		POINT   l_Size = {(int)l_pClickMouseBehavior->GetCollisionRange().Width(),(int)l_pClickMouseBehavior->GetCollisionRange().Height()};
        Vector3 l_vPos = Vector3(m_vCollisionRange.x,m_vCollisionRange.y,0.f);
        int l_iCount = Count();
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                l_pClickMouseBehavior = GetObject(i);
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
                if( m_bDirectionIsHorizontal )
				{
					m_fTotalLength += l_pClickMouseBehavior->GetCollisionRange().Width();
					l_vPos.x += l_pClickMouseBehavior->GetCollisionRange().Width()+m_fObjectGap;
				}
                else
				{
					m_fTotalLength += l_pClickMouseBehavior->GetCollisionRange().Height();
					l_vPos.y += l_pClickMouseBehavior->GetCollisionRange().Height()+m_fObjectGap;
				}
            }
        }
    }

	bool	cScroller::IsScrollable()
	{
		int l_iCount = this->Count();
		if( l_iCount<2 )
			return false;
		//cImageButton*l_pClickMouseBehavior = (cImageButton*)this->GetObject(0);
		//float	l_fImageLength = 0.f;
		////check last if it's not at last pos
		//if( m_bDirectionIsHorizontal )
		//{
		//	l_fImageLength = (float)*l_pClickMouseBehavior->GetWidth();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.z-m_vCollisionRange.x)
		//	{//it less than total viewable length so do not scroller
		//		return false;
		//	}
		//}
		//else
		//{
		//	l_fImageLength = (float)*l_pClickMouseBehavior->GetHeight();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.w-m_vCollisionRange.y)
		//	{
		//		return false;
		//	}
		//}
		return true;
	}
    //first time mouse into
    void    cScroller::MouseDown(int e_iPosX,int e_iPosY)
    {
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseDown(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
		//m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
		//if( this->m_bOutOfWorkingZone )
		//	return;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
		m_bRollBack = false;
        int l_iCount = this->Count();
        if( l_iCount )
        {
			//if( this->m_MouseMoveData.DownUpDistance()<5.f )
			{
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(i);
					//fuck offset problem so I have to collide with fucking weired way
					//if(l_pClickMouseBehavior->CollideTexture(e_iPosX,e_iPosY,false))
					if(CollideObjectIndex(l_pClickMouseBehavior,e_iPosX,e_iPosY))
					{
						m_iSelectedIndex = i;
						m_pCurrentMouseDownCollidedObject = l_pClickMouseBehavior;
						return;
					}
				}
			}
		}
    }
	//
    void    cScroller::MouseMove(int e_iPosX,int e_iPosY)
    {
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseMove(e_iPosX,e_iPosY);
		if(!cClickMouseBehavior::m_bCollided)
			return;
		//cClickMouseBehavior::MouseMove(e_iPosX,e_iPosY);
		//if( this->m_eObjectMouseBehavior == eOMB_NONE )
		//	return;
		if( this->m_bOutOfWorkingZone )
			return;
        this->m_MouseMoveing.x = e_iPosX-m_CurrentMousePos.x;
        this->m_MouseMoveing.y = e_iPosY-m_CurrentMousePos.y;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
        Vector3 l_vPos;
        int l_iCount = this->Count();
		cClickMouseBehavior*l_pClickMouseBehavior = 0;
        for( int i=0;i<l_iCount;++i )
        {
			l_pClickMouseBehavior = this->m_ObjectList[i];
			l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
			if(this->m_bDirectionIsHorizontal)
				l_vPos.x += this->m_MouseMoveing.x;
			else
				l_vPos.y += this->m_MouseMoveing.y;
			l_pClickMouseBehavior->SetLocalPosition(l_vPos);
        }        
    }
    //while mouse clicked is up
    //while current index collide with its image position and width height
    //while focus to left get current focus image position and collide with left one image's position,start to next start
    //while focus to right get current focus image's position with its width and get right image's position to collide,nd to next end
    void    cScroller::MouseUp(int e_iPosX,int e_iPosY)
    {
//		m_eObjectMouseBehavior = eOMB_NONE;
		if( m_bRollBack )
			return;
		//if out of working zone do nothing.
		bool	l_bOutOfWorkingZone = m_bOutOfWorkingZone;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseUp(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
		m_eObjectMouseBehavior = eOMB_NONE;
		if( l_bOutOfWorkingZone )
			return;
        int l_iCount = this->Count();
        if( l_iCount )
        {
			if( this->m_MouseMoveData.DownUpDistance()<15.f )
			{
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(i);
					//if(l_pClickMouseBehavior->CollideTexture(e_iPosX,e_iPosY,false))
					//fuck offset problem so I have to collide with fucking weired way
					if(CollideObjectIndex(l_pClickMouseBehavior,e_iPosX,e_iPosY))
					{
						if(m_iSelectedIndex == i )
						{
							this->m_bClickMouseBehaviorSatisfied = true;
							this->m_pCurrentWorkingObject = l_pClickMouseBehavior;
							return;
						}
						else
							m_iSelectedIndex = -1;
						return;
					}
				}
				return;
			}
			m_iSelectedIndex = -1;
			if(!IsScrollable())
				return;
			m_vAddedDis = Vector3::Zero;
			//check first if it over range
			cClickMouseBehavior*l_pFirstImageButton = this->GetObject(0);
			//assert(cImageButton::TypeID == l_pClickMouseBehavior->Type() );
			Vector3	l_vPos = *l_pFirstImageButton->GetLocalPositionPointer();
			float	l_fImageLength = 0.f;
			float	l_fDidOfFirstToStart = 0.f;
			float	l_fDisOfLastToEnd = 0.f;
			if( this->m_bDirectionIsHorizontal )
			{
				l_fImageLength = l_pFirstImageButton->GetCollisionRange().Width();
				l_fDidOfFirstToStart = l_vPos.x-this->m_vCollisionRange.x;
				m_vDisToRightPos = Vector3(-l_fDidOfFirstToStart,0.f,0.f);
			}
			else
			{
				l_fImageLength = l_pFirstImageButton->GetCollisionRange().Height();
				l_fDidOfFirstToStart = l_vPos.y-this->m_vCollisionRange.y;
				m_vDisToRightPos = Vector3(0.f,-l_fDidOfFirstToStart,0.f);
			}
			if( l_fDidOfFirstToStart>3.f )
			{
				this->m_bRollBack = true;
				this->m_RollBackTC.Start();
				return;
			}
			//if total length is not over slide range,check scroll to first or last
			//check last if it's not at last pos
			cClickMouseBehavior*l_pLasttImageButton = this->GetObject(this->Count()-1);
			l_vPos = *l_pLasttImageButton->GetLocalPositionPointer();
			if( m_bDirectionIsHorizontal )
			{
				if(this->m_vCollisionRange.z>(l_vPos.x-l_pFirstImageButton->GetLocalPositionPointer()->x)+l_pLasttImageButton->GetCollisionRange().Width())
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;			
				}
			}
			else
			{
				if(this->m_vCollisionRange.w>(l_vPos.y-l_pFirstImageButton->GetLocalPositionPointer()->y)+l_pLasttImageButton->GetCollisionRange().Height())
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;			
				}			
			}

			if( m_bDirectionIsHorizontal )
			{
				l_fDisOfLastToEnd = l_vPos.x-this->m_vCollisionRange.z+l_pLasttImageButton->GetCollisionRange().Width();
				this->m_vDisToRightPos = Vector3(-l_fDisOfLastToEnd,0.f,0.f);
			}
			else
			{
				l_fDisOfLastToEnd = l_vPos.y-this->m_vCollisionRange.w+l_pLasttImageButton->GetCollisionRange().Height();
				this->m_vDisToRightPos = Vector3(0.f,-l_fDisOfLastToEnd,0.f);
			}
			if( l_fDisOfLastToEnd < 3.f)
			{
				this->m_bRollBack = true;
				this->m_RollBackTC.Start();
				return;
			}
			//if down and up is smaller than 0.2f do a smooth scroller
			const float	l_fMinTime = 0.2f;
			const float	l_fMinPencentage = 0.0f;
			if( this->m_MouseMoveData.fDownToUpElpaseTime <l_fMinTime )
			{
				float	l_fViewRectLength;
				if(this->m_bDirectionIsHorizontal)
					l_fViewRectLength = this->m_vCollisionRange.z-this->m_vCollisionRange.x;
				else
					l_fViewRectLength = this->m_vCollisionRange.w-this->m_vCollisionRange.y;
				Vector2	l_vDownUpDirection;
				float	l_fDistance = this->m_MouseMoveData.DownUpDistance(&l_vDownUpDirection);
				float	l_fPercentage = l_fDistance/l_fViewRectLength;
				if( l_fPercentage>1.f )
					l_fPercentage = 1.f;
				float	l_fLengthToScroll = m_fTotalLength * l_fPercentage;
				if( l_fPercentage>l_fMinPencentage )
				{
					if( this->m_bDirectionIsHorizontal )
					{
						if(l_vDownUpDirection.x<0.f)//to right
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(-l_fLengthToScroll,0.f,0.f);
						}
						else//to left
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(l_fLengthToScroll,0.f,0.f);
						}
					}
					else
					{
						if(l_vDownUpDirection.y<0.f)//to down
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(0.f,-l_fLengthToScroll,0.f);
						}
						else//to top
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(0.f,l_fLengthToScroll,0.f);
						}

					}
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
				}
			}
        }
    }

    void    cScroller::Update(float e_fElpaseTime)
    {
		cMouse_TouchWithCancelButton::Update(e_fElpaseTime);
		if(this->m_pBG)
		{
			m_pBG->Update(e_fElpaseTime);
		}
		if(this->m_bRollBack)
        {
			m_RollBackTC.Update(e_fElpaseTime);
			Vector3	l_vDisToMove;
			if( m_RollBackTC.bTragetTimrReached )
			{
				
				l_vDisToMove = m_vDisToRightPos-m_vAddedDis;
				m_bRollBack = false;				
			}
			else
			{
				l_vDisToMove = this->m_vDisToRightPos*(e_fElpaseTime/m_RollBackTC.fTargetTime);
			}
			m_vAddedDis += l_vDisToMove;
			int	l_iCount = this->Count();
			for( int i=0;i<l_iCount;++i )
			{
				
				cClickMouseBehavior*l_pClickMouseBehavior = this->m_ObjectList[i];
				l_pClickMouseBehavior->SetLocalPosition(*l_pClickMouseBehavior->GetLocalPositionPointer()+l_vDisToMove);
			}
            return;
        }
		if( m_pCurrentWorkingObject )
			m_pCurrentWorkingObject->Update(e_fElpaseTime);
    }
    
    void    cScroller::Render()
    {
		int	l_iCount = this->Count();
		if( l_iCount == 0 )
			return;
		cClickMouseBehavior*l_pClickMouseBehavior = 0;
		Vector3	l_vPos;
		Vector4	l_vOriginalColor;
		if( -1 != m_iSelectedIndex )
		{
			l_pClickMouseBehavior = this->GetObject(m_iSelectedIndex);
			//l_vOriginalColor = *l_pClickMouseBehavior->GetColor();
			//l_pClickMouseBehavior->SetColor(g_vImageButtonClickedColor);
		}
		if(this->m_pBG)
		{
			m_pBG->Render();
		}
		glEnable(GL_SCISSOR_TEST);
		Vector4	l_vViewRect = ViewRectToOpenGLScissor(m_vCollisionRange);
		glScissor((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
		if(this->m_bDirectionIsHorizontal)
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pClickMouseBehavior = this->m_ObjectList[i];
				l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
				if(l_vPos.x+l_pClickMouseBehavior->GetCollisionRange().Width()>=this->m_vCollisionRange.x)
				{
					if( l_vPos.x>this->m_vCollisionRange.z )
					{
						break;
					}
					l_pClickMouseBehavior->Render();
					if( m_iSelectedIndex == i )
					{
						GLRender::RenderRectangle(*l_pClickMouseBehavior->GetLocalPositionPointer(),l_pClickMouseBehavior->GetCollisionRange().Width(),l_pClickMouseBehavior->GetCollisionRange().Height(),Vector4(1,1,0,1));
					}
				}
			}
		}
		else
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pClickMouseBehavior = this->m_ObjectList[i];
				l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
				float	l_fPosY = l_vPos.y+l_pClickMouseBehavior->GetCollisionRange().Height();
				if(l_fPosY >= this->m_vCollisionRange.y)
				{
					if( l_vPos.y>this->m_vCollisionRange.w )
					{
						break;
					}
					l_pClickMouseBehavior->Render();
					if( m_iSelectedIndex == i )
					{
						GLRender::RenderRectangle(*l_pClickMouseBehavior->GetLocalPositionPointer(),l_pClickMouseBehavior->GetCollisionRange().Width(),l_pClickMouseBehavior->GetCollisionRange().Height(),Vector4(1,1,0,1));
					}
				}
			}
		}
		if( -1 != m_iSelectedIndex )
		{
			l_pClickMouseBehavior = this->GetObject(m_iSelectedIndex);
			//l_pClickMouseBehavior->SetColor(l_vOriginalColor);
		}
		glDisable(GL_SCISSOR_TEST);
		if( this->m_pCancelButton )
			m_pCancelButton->Render();
    }
    
    void    cScroller::DebugRender()
    {
        if(this->Count())
        {
            Vector2 l_vLinePos[4];
			if( this->m_bDirectionIsHorizontal )
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);
			}
			else
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);			
			}
			GLRender::RenderLine((float*)l_vLinePos,4,Vector4(1,1,0,1),2,cMatrix44::Identity,5);
			GLRender::RenderLine((float*)l_vLinePos[2],4,Vector4(1,1,0,1),2,cMatrix44::Identity,5);
        }
    }

	const WCHAR*	cScroller::GetSelectedIndexName()
	{
		cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(this->m_iSelectedIndex);
		if( !l_pClickMouseBehavior )
			return 0;
		return l_pClickMouseBehavior->GetName();
	}
//end namespace FATMING_CORE

//end namespace
};