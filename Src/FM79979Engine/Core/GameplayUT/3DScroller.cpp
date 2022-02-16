#include "../stdafx.h"
#include "../Image/PuzzleImage.h"
#include "../Image/SimplePrimitive.h"
#include "3DScroller.h"

namespace FATMING_CORE
{
    sKeepObjectPositionAtCenter::sKeepObjectPositionAtCenter(float e_fTimeToCenter,Vector4 e_vRect,bool e_bDirectionIsHorizontal)
    {
        bDirectionIsHorizontal = e_bDirectionIsHorizontal;
        SetData(e_fTimeToCenter,e_vRect);
        bMovingToCenter = false;
        pvPos = 0;
    }
    void    sKeepObjectPositionAtCenter::SetData(float e_fTimeToCenter,Vector4 e_vRect)
    {
        vRect = e_vRect;
        GotoCenterTC.SetTargetTime(e_fTimeToCenter);
    }
    void    sKeepObjectPositionAtCenter::SetPosMoveToCenter(Vector3*e_pPos)
    {
        pvPos = e_pPos;
        //Vector3 l_vCenter((vRect.z-vRect.x)/2.f,(vRect.y-vRect.w)/2.f,0.f );
        //now ignore height
        Vector3 l_vStartpos(vRect.x,vRect.y,0.f );
        vDistanceToCenter = l_vStartpos-*pvPos;
        bMovingToCenter = true;
        GotoCenterTC.Start();
    }
    //if mouse up event is actived,and object is not at center,set the object back to center position
    void    sKeepObjectPositionAtCenter::Start()
    {
        Vector3 l_vCenter((vRect.z-vRect.x)/2.f,(vRect.y-vRect.w)/2.f,0.f );
        vDistanceToCenter = *pvPos-l_vCenter;
        if( vDistanceToCenter.Length()>2.f )
            bMovingToCenter = true;
        else
            bMovingToCenter = false;
        GotoCenterTC.Start();
    }
    //
    void    sKeepObjectPositionAtCenter::Update(float e_fElpaseTime)
    {
        if( bMovingToCenter )
        {
            GotoCenterTC.Update(e_fElpaseTime);
            if( GotoCenterTC.bTragetTimrReached )
            {
                vMovedDistance = Vector3(vRect.x-pvPos->x,vRect.y-pvPos->y,0.f);
                //*pvPos = Vector3(this->vRect.x,this->vRect.y,0.f);
                bMovingToCenter = false;
            }
            else
            {
                float   l_fScale = 1/GotoCenterTC.fTargetTime;
                vMovedDistance = vDistanceToCenter*e_fElpaseTime*l_fScale;
                //*pvPos += vMovedDistance;
            }
        }
    }

    //0 for not ye,1 for left 2 for right
    int sKeepObjectPositionAtCenter::IsOverCenterRange()
    {
        float   l_fOverRangeLength;
        if( bDirectionIsHorizontal )
        {
            l_fOverRangeLength = this->vRect.z-this->vRect.x;
            if( this->pvPos->x+l_fOverRangeLength*0.3<vRect.x )
                return 1;
            if( this->pvPos->x+l_fOverRangeLength*0.6>vRect.z )
                return 2;
        }
        else
        {
            l_fOverRangeLength = this->vRect.w-this->vRect.y;
            if( this->pvPos->y+l_fOverRangeLength*0.3<vRect.y )
                return 1;
            if( this->pvPos->y+l_fOverRangeLength*0.6>vRect.w )
                return 2;
        }
        return 0;
    }

    c3DScroller::c3DScroller(cPuzzleImage*e_pPI,bool e_bHorizontal)
    {
        m_bDirectionIsHorizontal = e_bHorizontal;
        m_KeepObjectPositionAtCenter.bDirectionIsHorizontal = m_bDirectionIsHorizontal;
        m_iCurrentFocusIndex = -1;
        m_fRotationAngle = 30.f;
        m_iViewAbleCount = 7;
        m_iLastSelectedIndex = -1;
        m_vTotalRect = Vector4(0,0,0,0);
        m_fImageScaleSize = 0.8f;
        this->AddImages(e_pPI);
    }
	c3DScroller::c3DScroller(c3DScroller*e_p3DScroller):cNamedTypedObjectVector<cBaseImage>(e_p3DScroller)
	{
        m_bDirectionIsHorizontal = e_p3DScroller->m_bDirectionIsHorizontal;
        m_KeepObjectPositionAtCenter.bDirectionIsHorizontal = m_bDirectionIsHorizontal;
        m_iCurrentFocusIndex = e_p3DScroller->m_iCurrentFocusIndex;
        m_fRotationAngle = e_p3DScroller->m_fRotationAngle;
        m_iViewAbleCount = e_p3DScroller->m_iViewAbleCount;
        m_iLastSelectedIndex = e_p3DScroller->m_iLastSelectedIndex;
        m_vTotalRect = e_p3DScroller->m_vTotalRect;
        m_fImageScaleSize = e_p3DScroller->m_fImageScaleSize;
	}
    //
    c3DScroller::~c3DScroller()
    {
    
    }
    //
    void    c3DScroller::Init()
    {
        cClickMouseBehavior::Init();
        int l_iCount = this->Count();
        if( l_iCount == 0 )
            return;
        if( m_iViewAbleCount>l_iCount )
            m_iViewAbleCount = l_iCount;
        //make it as odd numeral
        if( m_iViewAbleCount%2 == 0 )
        {
            m_iViewAbleCount -= 1;
        }
        m_iCurrentFocusIndex = 0;
        cBaseImage*l_pImage = this->GetObject(0);
        m_iLastSelectedIndex = -1;
        cBaseImage*l_pBaseImage = GetObject(0);
        POINT   l_Size = {*l_pBaseImage->GetWidth(),*l_pBaseImage->GetHeight()};
        if( m_bDirectionIsHorizontal )
            this->m_fEachImageDistance = l_Size.x/2.f;
        else
            this->m_fEachImageDistance = l_Size.y/2.f;
        Vector3 l_vPos = Vector3((m_vTotalRect.z-m_vTotalRect.x)/2.f-l_Size.x/2.f,(m_vTotalRect.w-m_vTotalRect.y)/2.f-l_Size.y/2.f,0.f);
        Vector4 l_vFocusRange(l_vPos.x,
                              l_vPos.y,
                              l_vPos.x+l_Size.x,
                              l_vPos.y+l_Size.y);
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                cBaseImage*l_pImage = GetObject(i);
                l_pImage->SetPos(l_vPos);
                if( m_bDirectionIsHorizontal )
                    l_vPos.x += m_fEachImageDistance;
                else
                    l_vPos.y += m_fEachImageDistance;
            }
        }
        this->m_KeepObjectPositionAtCenter.SetData(0.3f,l_vFocusRange);
		m_KeepObjectPositionAtCenter.pvPos = this->GetObject(this->m_iCurrentFocusIndex)->GetLocalPositionPointer();
        m_KeepObjectPositionAtCenter.bMovingToCenter = false;
    }    
    //
    void    c3DScroller::SetRotationAngle(float e_fRotationAngle)
    {
        m_fRotationAngle = e_fRotationAngle;
        while( m_fRotationAngle>90.f )
            m_fRotationAngle -= 90.f;
        if(this->Count() == 0)
            this->m_fEachImageDistance = 1.f;
        else
        {
            float   l_fScale = m_fRotationAngle/90.f;
            this->m_fEachImageDistance = *this->GetObject(0)->GetWidth()*l_fScale;
        }
        
    }
    //
    void    c3DScroller::SetTotalRect(Vector4 e_vTotalRect)
    {
        this->m_vTotalRect = e_vTotalRect;
        if( this->Count() )
            Init();
    }
    //
    void    c3DScroller::AddImages(cPuzzleImage*e_pPI)
    {
        this->Destroy();
        int l_iCount = e_pPI->Count();
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                cPuzzleImageUnit*l_pPuzzleImageUnit = new cPuzzleImageUnit(e_pPI->GetObject(i));
                this->AddObjectNeglectExist(l_pPuzzleImageUnit);
            }
#ifdef WIN32
            cBaseImage*l_pBaseImage = this->m_ObjectList[0];
            POINT   l_Size = {*l_pBaseImage->GetWidth(),*l_pBaseImage->GetHeight()};
            for( int i=0;i<l_iCount;++i )
            {
                cPuzzleImageUnit*l_pPuzzleImageUnit = e_pPI->GetObject(i);
                assert(l_Size.x == *l_pPuzzleImageUnit->GetWidth() );
                assert(l_Size.y == *l_pPuzzleImageUnit->GetHeight() );
            }
#endif
        }
        Init();
    }

    void    c3DScroller::InternalMouseMove(int e_iPosX,int e_iPosY)
    {
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
        this->m_MouseMoveing.x = e_iPosX-m_CurrentMousePos.x;
        this->m_MouseMoveing.y = e_iPosY-m_CurrentMousePos.y;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
        Vector3 l_vPos;
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            cBaseImage*l_pBaseImage = this->m_ObjectList[i];
			l_vPos = *l_pBaseImage->GetLocalPositionPointer();
            if(this->m_bDirectionIsHorizontal)
                l_vPos.x += this->m_MouseMoveing.x;
            else
                l_vPos.y += this->m_MouseMoveing.y;
            l_pBaseImage->SetPos(l_vPos);
        }
        //cBaseImage*l_pBaseImage = this->m_ObjectList[this->m_iCurrentFocusIndex];
        //l_vPos = *l_pBaseImage->GetPos();
        //int l_iWidth = *l_pBaseImage->GetWidth()
        
        
    }
    //first time mouse into
    void    c3DScroller::InternalMouseDown(int e_iPosX,int e_iPosY)
    {
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
    }
    //while mouse clicked is up
    //while current index collide with its image position and width height
    //while focus to left get current focus image position and collide with left one image's position,start to next start
    //while focus to right get current focus image's position with its width and get right image's position to collide,nd to next end
    void    c3DScroller::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
        if(this->m_MouseMoveData.DownUpDistance()>5.f)
        {
            m_iNewFocusIndex = m_iCurrentFocusIndex;
            this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(m_iCurrentFocusIndex)->GetLocalPositionPointer());
            return;
        }
        int l_iCount = this->Count();
        if( l_iCount )
        {
            if( e_iPosY<m_vTotalRect.y||e_iPosY>m_vTotalRect.w )
                return;
            //how many ite shoudl be left or right side.
            int l_iSideCount = (this->m_iViewAbleCount-1)/2;
            cBaseImage*l_pImage = this->GetObject(0);
            //collide for center image
            if( e_iPosX>=this->m_KeepObjectPositionAtCenter.vRect.x&&e_iPosY>=this->m_KeepObjectPositionAtCenter.vRect.y )
            {
                if( e_iPosX<=this->m_KeepObjectPositionAtCenter.vRect.z&&e_iPosY<=this->m_KeepObjectPositionAtCenter.vRect.w )
                {
                    this->m_iLastSelectedIndex = m_iCurrentFocusIndex;
                    return;
                }
            }
            int l_iLeftStartCollisionIndex = this->m_iCurrentFocusIndex-l_iSideCount;
            int l_iEndCount = this->m_iCurrentFocusIndex+l_iSideCount;
            if( this->m_iCurrentFocusIndex == 0 )
            {
                l_iEndCount = m_iViewAbleCount-1;
            }
            else
            if( this->m_iCurrentFocusIndex == this->Count()-1 )
            {
                l_iLeftStartCollisionIndex = m_iCurrentFocusIndex-m_iViewAbleCount+1;
            }
            if( l_iEndCount >= this->Count() )
                l_iEndCount = this->Count()-1;
            if( l_iLeftStartCollisionIndex < 0 )
                l_iLeftStartCollisionIndex = 0;
            //focus to left,up
            Vector2 l_vPreviousImagePos(this->m_bDirectionIsHorizontal?m_KeepObjectPositionAtCenter.vRect.x:m_KeepObjectPositionAtCenter.vRect.z,this->m_bDirectionIsHorizontal?m_KeepObjectPositionAtCenter.vRect.w:m_KeepObjectPositionAtCenter.vRect.y);
            Vector2 l_vCurrentImagePos;
            for( int i=m_iCurrentFocusIndex-1;i>l_iLeftStartCollisionIndex-1;--i )
            {
                l_pImage = this->GetObject(i);
                l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x,l_pImage->GetLocalPositionPointer()->y);
                if( e_iPosX>=l_vCurrentImagePos.x&&e_iPosY>=l_vCurrentImagePos.y )
                if( e_iPosX<=l_vPreviousImagePos.x&&e_iPosY<=l_vPreviousImagePos.y )
                {
                    m_iNewFocusIndex = i;
                    this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                    return;
                }
                l_vPreviousImagePos.x = this->m_bDirectionIsHorizontal?l_vCurrentImagePos.x:l_vPreviousImagePos.x;
                l_vPreviousImagePos.y = m_bDirectionIsHorizontal?l_vCurrentImagePos.y+*l_pImage->GetHeight():l_vCurrentImagePos.y;
            }
            //focus to right,down
            if( m_bDirectionIsHorizontal )
            {
                l_vPreviousImagePos = Vector2(m_KeepObjectPositionAtCenter.vRect.z,m_KeepObjectPositionAtCenter.vRect.y);
                for( int i=m_iCurrentFocusIndex+1;i<l_iEndCount+1;++i )
                {
                    l_pImage = this->GetObject(i);
                    //get last image's end position
                    l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x+*l_pImage->GetWidth(),l_pImage->GetLocalPositionPointer()->y+*l_pImage->GetHeight());
                    if( e_iPosX>=l_vPreviousImagePos.x&&e_iPosY>=l_vPreviousImagePos.y )
                    if( e_iPosX<=l_vCurrentImagePos.x&&e_iPosY<=l_vCurrentImagePos.y )
                    {
                        m_iNewFocusIndex = i;
                        this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                        return;
                    }
                    l_vPreviousImagePos.x = l_pImage->GetLocalPositionPointer()->x;
                    l_vPreviousImagePos.y = l_pImage->GetLocalPositionPointer()->y;
                }
            }
            else
            {
                l_vPreviousImagePos = Vector2(m_KeepObjectPositionAtCenter.vRect.x,m_KeepObjectPositionAtCenter.vRect.w);
                for( int i=m_iCurrentFocusIndex+1;i<l_iEndCount+1;++i )
                {
                    l_pImage = this->GetObject(i);
                    //get last image's end position
                    l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x+*l_pImage->GetWidth(),l_pImage->GetLocalPositionPointer()->y+*l_pImage->GetHeight());
                    if( e_iPosX>=l_vPreviousImagePos.x&&e_iPosY>=l_vPreviousImagePos.y )
                    if( e_iPosX<=l_vCurrentImagePos.x&&e_iPosY<=l_vCurrentImagePos.y )
                    {
                        m_iNewFocusIndex = i;
                        this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                        return;
                    }
                    l_vPreviousImagePos.x = l_pImage->GetLocalPositionPointer()->x;
                    l_vPreviousImagePos.y = l_vCurrentImagePos.y;
                }
            }
        }
    }

    bool    c3DScroller::InternalCollide(int e_iPosX,int e_iPosY)
    {
        if( this->m_eObjectMouseBehavior == eOMB_NONE|| this->m_eObjectMouseBehavior == eOMB_UP )
        {
            if(this->m_vTotalRect.x<=e_iPosX&&
                this->m_vTotalRect.y<=e_iPosY&&
                this->m_vTotalRect.z>=e_iPosX&&
                this->m_vTotalRect.w>=e_iPosY)
                return true;
             else
                return false;
        }
        return true;
    }

    void    c3DScroller::Update(float e_fElpaseTime)
    {
		int	l_iCount = this->Count();
        if(m_KeepObjectPositionAtCenter.bMovingToCenter)
        {
            m_KeepObjectPositionAtCenter.Update(e_fElpaseTime);
            for( int i=0;i<l_iCount;++i )
            {
                //if( i!= this->m_iCurrentFocusIndex )
                    this->m_ObjectList[i]->SetPos(*this->m_ObjectList[i]->GetLocalPositionPointer()+m_KeepObjectPositionAtCenter.vMovedDistance);
            }
            if(m_KeepObjectPositionAtCenter.bMovingToCenter )
            {
                this->m_iCurrentFocusIndex = m_iNewFocusIndex;
            }
            return;
        }
        //if current focus object is over the center set it to prior or next
        int l_iOverCentRange = m_KeepObjectPositionAtCenter.IsOverCenterRange();
        if( l_iOverCentRange == 1 )
        {//find prior
            if(this->m_iCurrentFocusIndex+1<l_iCount)
            {
                this->m_KeepObjectPositionAtCenter.pvPos = this->GetObject(m_iCurrentFocusIndex+1)->GetLocalPositionPointer();
                m_iCurrentFocusIndex +=1 ;
            }
        }
        else
        if( l_iOverCentRange == 2 )
        {//find next
            if(this->m_iCurrentFocusIndex>0)
            {
                this->m_KeepObjectPositionAtCenter.pvPos = this->GetObject(m_iCurrentFocusIndex-1)->GetLocalPositionPointer();
                m_iCurrentFocusIndex -=1 ;
            }
        }
    }
    
    void    c3DScroller::Render()
    {
		int	l_iCount = this->Count();
        int l_iSideCount = (this->m_iViewAbleCount-1)/2;
        int l_iLeftStartCollisionIndex = this->m_iCurrentFocusIndex-l_iSideCount;
        int l_iEndCount = this->m_iCurrentFocusIndex+l_iSideCount;
        if( this->m_iCurrentFocusIndex == 0 )
        {
            l_iEndCount = m_iViewAbleCount-1;
        }
        else
        if( this->m_iCurrentFocusIndex == l_iCount-1 )
        {
            l_iLeftStartCollisionIndex = m_iCurrentFocusIndex-m_iViewAbleCount+1;
        }
        if( l_iEndCount >= l_iCount )
            l_iEndCount = l_iCount-1;
        if( l_iLeftStartCollisionIndex < 0 )
            l_iLeftStartCollisionIndex = 0;
        //left to focus
        cBaseImage*l_pBaseImage = 0;
        Vector3 l_vPos;
        float   l_fXScale = 1.f;
        float   l_fYScale = 1.f;
        this->m_bDirectionIsHorizontal?l_fYScale=m_fImageScaleSize:l_fXScale=m_fImageScaleSize;
        for( int i=l_iLeftStartCollisionIndex;i<this->m_iCurrentFocusIndex;++i )
        {
            l_pBaseImage = this->m_ObjectList[i];
            l_vPos = *l_pBaseImage->GetLocalPositionPointer();
            l_vPos.x += *l_pBaseImage->GetWidth()/2.f;
            l_vPos.y += *l_pBaseImage->GetHeight()/2.f;
            //this->m_ObjectList[i]->Render(cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::YAxisRotationMatrix(D3DXToRadian(45))*cMatrix44::ScaleMatrix(Vector3(1,m_fImageScaleSize,0.8f)));
            this->m_ObjectList[i]->Render(cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::ScaleMatrix(Vector3(l_fXScale,l_fYScale,0.8f)));
        }
        //right to focus
        for( int i=l_iEndCount;i>m_iCurrentFocusIndex;--i )
        {
            l_pBaseImage = this->m_ObjectList[i];
            l_vPos = *l_pBaseImage->GetLocalPositionPointer();
            l_vPos.x += *l_pBaseImage->GetWidth()/2.f;
            l_vPos.y += *l_pBaseImage->GetHeight()/2.f;
            this->m_ObjectList[i]->Render(cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::ScaleMatrix(Vector3(l_fXScale,l_fYScale,0.8f)));
        }
        //render focus
        this->m_ObjectList[m_iCurrentFocusIndex]->Render();
    }
    
    void    c3DScroller::DebugRender()
    {
		int	l_iCount = this->Count();
        if(l_iCount)
        {
            cBaseImage*l_pBaseImage = GetObject(0);
            Vector2 l_vLinePos[4];
            POINT   l_Size = {*l_pBaseImage->GetWidth(),*l_pBaseImage->GetHeight()};    
            Vector3 l_vPos((m_vTotalRect.z-m_vTotalRect.x)/2.f-l_Size.x/2.f,(m_vTotalRect.w-m_vTotalRect.y)/2.f-l_Size.y/2.f,0.f);
            if(this->m_bDirectionIsHorizontal)
            {
                l_vLinePos[0] = Vector2(l_vPos.x,0.f);
                l_vLinePos[1] = Vector2(l_vPos.x,m_vTotalRect.w);
                l_vLinePos[2] = Vector2(l_vPos.x+l_Size.x,0.f);
                l_vLinePos[3] = Vector2(l_vPos.x+l_Size.x,m_vTotalRect.w);
            }
            else
            {
                l_vLinePos[0] = Vector2(0.f,l_vPos.y);
                l_vLinePos[1] = Vector2(m_vTotalRect.z,l_vPos.y);
                l_vLinePos[2] = Vector2(0.f,l_vPos.y+l_Size.y);
                l_vLinePos[3] = Vector2(m_vTotalRect.z,l_vPos.y+l_Size.y);
            }
            GLRender::RenderLine((float*)l_vLinePos,4,Vector4(1,1,0,1),2);
            GLRender::RenderLine((float*)l_vLinePos[2],4,Vector4(1,1,0,1),2);
        }
    }
//end namespace FATMING_CORE
}