#include "../stdafx.h"
#include "SimplePrimitive.h"
//#include "AnimationParser.h"
#include "MPDI.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
#include "../Sound/SoundManager.h"
//#include "../Sound/SoundShell.h"
//#include "../Particle/ParticleBase.h"
//#include "../Particle/ParticleManager.h"
//math
#ifndef D3DXToRadian
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#endif
#ifndef D3DXToDegree
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
#endif
namespace FATMING_CORE
{
	char	g_strForMPDIString[MAX_PATH];
	WCHAR	g_strForMPDIStringW[MAX_PATH];
	const wchar_t*     cMultiPathDynamicImage::TypeID( L"cMPDI" );
	const wchar_t*     cMPDIList::TypeID( L"cMPDIList" );
	bool	cMPDIList::m_b3DCameraView = false;
	cMultiPathDynamicImage::cMultiPathDynamicImage(cMulti_PI_Image*e_pMulti_PI_Image):cMulti_PI_Image(e_pMulti_PI_Image)
	{
		m_pViewPort = 0;
		m_bHitHintPoint = false;
		m_bStopAtLastFrame = false;
		m_vPos = Vector3::Zero;
		m_fScale = 1.f;
		m_vAngle = Vector3::Zero;
		m_fPastTime = 0.f;
		m_iPlayCount = 1;
		m_fTotalPlayTime = 0.f;
		m_fCurrentProgress = 0.f;
		m_iCurrentPlayCount = 0;
		m_bRenderOptmize = false;
		m_vDrawSize = Vector2::Zero;
	}

	cMultiPathDynamicImage::cMultiPathDynamicImage(cMultiPathDynamicImage*e_pMultiPathDynamicImage)
		:cFatmingGroupBehaviorList<cCueToStartCurveWithTime>(e_pMultiPathDynamicImage),cMulti_PI_Image(e_pMultiPathDynamicImage)
	{
		m_pViewPort = 0;
		if(  e_pMultiPathDynamicImage->m_pViewPort )
		{
			m_pViewPort = new Vector4();
			*m_pViewPort = *e_pMultiPathDynamicImage->m_pViewPort;
		}
		m_bHitHintPoint = false;
		m_bStopAtLastFrame = e_pMultiPathDynamicImage->m_bStopAtLastFrame;
		m_bRenderOptmize = e_pMultiPathDynamicImage->m_bRenderOptmize;
		m_fScale = e_pMultiPathDynamicImage->m_fScale;
		m_vAngle = e_pMultiPathDynamicImage->GetAngle();
		m_vDrawSize = e_pMultiPathDynamicImage->GetDrawSize();
		this->SetPos(e_pMultiPathDynamicImage->m_vPos);
	}

	cMultiPathDynamicImage::~cMultiPathDynamicImage()
	{
		SAFE_DELETE(m_pViewPort);
	}

	void	cMultiPathDynamicImage::FindoutAllPointsCenter()
	{
		int	l_iNum = Count();
		if( Count() && (*this)[0]->GetOriginalPointList().size() )
		{
			Vector3	l_veLeftUp = (*this)[0]->GetOriginalPointList()[0];
			Vector3	l_veRightDown = l_veLeftUp;
			for(int i=0;i<Count();++i)
			{
				std::vector<Vector3>	l_PosList = (*this)[i]->GetOriginalPointList();
				for( UINT i=0;i<l_PosList.size();++i )
				{
					Vector3	*l_vPos = &l_PosList[i];
					if( l_veLeftUp.x>l_vPos->x )
						l_veLeftUp.x = l_vPos->x;
					if( l_veLeftUp.y>l_vPos->y )//2d so y up is getting small and small
						l_veLeftUp.y = l_vPos->y;
					if( l_veRightDown.x<l_vPos->x )
						l_veRightDown.x = l_vPos->x;
					if( l_veRightDown.y<l_vPos->y )
						l_veRightDown.y = l_vPos->y;
				}
			}
			m_vRotationCenter = Vector3((l_veLeftUp.x+l_veRightDown.x)/2.f,(l_veLeftUp.y+l_veRightDown.y)/2.f,0.f);
		}
	}
//=======================================
	void	cMultiPathDynamicImage::RefreshData()
	{
		//find out draw rect
		int	l_iCount = this->Count();
		Vector4	l_vDrawRect(FLT_MAX,FLT_MAX,FLT_MIN,FLT_MIN);
		for( int i=0;i<l_iCount;++i )
		{
			cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = GetObject(i);
			std::vector<Vector3>l_vPointPos = l_pCueToStartCurveWithTime->GetOriginalPointList();
			std::vector<sTexBehaviorDataWithImageIndexData*>*l_pDataPointList = l_pCueToStartCurveWithTime->GetPointDataList();
			int	l_iSize = (int)l_vPointPos.size();
			for( int i=0;i<l_iSize;++i )
			{
				Vector3	l_vPos = l_vPointPos[i];
				if( l_vDrawRect.x >l_vPos.x )
					l_vDrawRect.x = l_vPos.x;
				if( l_vDrawRect.y >l_vPos.y )
					l_vDrawRect.y = l_vPos.y;
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = l_pCueToStartCurveWithTime->GetPointData(i);
				Vector2	l_vRightDownPos(l_vPos.x+l_pTexBehaviorDataWithImageIndexData->Size.x,l_vPos.y+l_pTexBehaviorDataWithImageIndexData->Size.y);
				if( l_vDrawRect.z <l_vRightDownPos.x )
					l_vDrawRect.z = l_vRightDownPos.x;
				if( l_vDrawRect.w <l_vRightDownPos.y )
					l_vDrawRect.w = l_vRightDownPos.y;
			}
		}
		SetDrawSize(Vector2(l_vDrawRect.Width(),l_vDrawRect.Height()));	
		CheckRenderOptmize();
		FindoutAllPointsCenter();
		RefreshTotalPlayTime();
	}

	void	cMultiPathDynamicImage::Merge(cMultiPathDynamicImage*e_pMultiPathDynamicImage)
	{
		if( !e_pMultiPathDynamicImage && e_pMultiPathDynamicImage == this )
			return;
		cMulti_PI_Image::Merge(e_pMultiPathDynamicImage);
		cFatmingGroupBehaviorList<cCueToStartCurveWithTime>::Merge(e_pMultiPathDynamicImage);
	}

	void	cMultiPathDynamicImage::CheckRenderOptmize()
	{
		m_bRenderOptmize = false;
		int	l_iCount = this->Count();
		//only one path do not optmize.
		if( l_iCount < 2 )
			return;
		cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = this->m_ObjectList[0];
		if( l_pCueToStartCurveWithTime->GetPointDataList()->size()==0||!l_pCueToStartCurveWithTime->GetPointData(0)->pPI )
			return;
		GLenum	l_SrcBlendingMode = GL_SRC_ALPHA;
		GLenum	l_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		GLuint	l_uiTextureID = l_pCueToStartCurveWithTime->GetPointData(0)->pPI->GetTexture()->GetImageIndex();
		for( int i=0;i<l_iCount;++i )
		{
			l_pCueToStartCurveWithTime = this->m_ObjectList[i];
			if( l_pCueToStartCurveWithTime->Type() != cCueToStartCurveWithTime::TypeID )
				return;
			if(l_SrcBlendingMode != l_pCueToStartCurveWithTime->GetSrcBlendingMode() || l_pCueToStartCurveWithTime->GetDestBlendingMode() != l_DestBlendingMode )
			{
				return;
			}
			//only 1 pi
			if( this->m_pPIList->Count() == 1 )
				continue;
			int	l_iPointDataCount = (int)l_pCueToStartCurveWithTime->GetPointDataList()->size();
			for( int j=0;j<l_iPointDataCount;++j )
			{
				if( !l_pCueToStartCurveWithTime->GetPointData(j)->pPI )
				{
					UT::ErrorMsg(L"no PI!!",l_pCueToStartCurveWithTime->GetName());
					return;
				}
				if( l_uiTextureID != l_pCueToStartCurveWithTime->GetPointData(j)->pPI->GetTexture()->GetImageIndex() )
					return;
			}
		}
		m_bRenderOptmize = true;
	}

	void	cMultiPathDynamicImage::InternalUpdate(float e_fElpaseTime)
	{
		cFatmingGroupBehaviorList<cCueToStartCurveWithTime>::InternalUpdate(e_fElpaseTime);
		int	l_iCount = this->Count();
		if( this->IsAnimationDone() )
		{
			if( !this->m_bStopAtLastFrame )
			{
				for( int i=0;i<l_iCount;++i )
				{
					if(this->m_ObjectList[i]->IsAnimationLoop())
					{
						m_ObjectList[i]->Update(e_fElpaseTime);
					}
				}
			}
			else
			{
				for( int i=0;i<l_iCount;++i )
				{
					if(this->m_ObjectList[i]->IsAnimationLoop())
					{
						m_ObjectList[i]->Update(e_fElpaseTime);
					}
					else
					{
						float	l_fTime = this->m_ObjectList[i]->GetEndTime();
						this->m_ObjectList[i]->GetPositionByTime(l_fTime);
					}
				}
			}
		}

		m_bHitHintPoint = false;
		for( int i=0;i<l_iCount;++i )
		{
			if(this->m_ObjectList[i]->m_bHintPoint)
			{
				m_bHitHintPoint = true;
				return;
			}
		}
	}

	void	cMultiPathDynamicImage::RenderByGlobalTime()
	{
		cMatrix44	l_OriginalCameraView;
		Vector3	l_vOriginal = this->m_vPos;
		cFatmingGroupBehaviorList<cCueToStartCurveWithTime>::RenderByGlobalTime();
	}

	void	cMultiPathDynamicImage::InternalRender()
	{
		if(this->m_pViewPort)
		{
			Vector4 l_ResultViewPort = ViewRectToOpenGLScissor(*m_pViewPort);
			glEnable(GL_SCISSOR_TEST);
			glScissor((int)l_ResultViewPort.x,(int)l_ResultViewPort.y,(int)l_ResultViewPort.z,(int)l_ResultViewPort.w);		
		}
		cMatrix44	l_OriginalCameraView;
		Vector3	l_vOriginal = this->m_vPos;
		int	l_iIndex = 0;
		if(!this->m_bRenderOptmize)
		{
			if( this->m_bStopAtLastFrame && this->m_fCurrentProgress >= 1.f )
			{
				size_t	l_iCount = this->m_ObjectList.size();
				for( size_t i=0;i<l_iCount;++i )
				{
					if( m_ObjectList[i]->IsAnimationLoop() )
						m_ObjectList[i]->Render();
					else
					if( m_ObjectList[i]->GetEndTime()>=this->m_fTotalPlayTime )
						m_ObjectList[i]->RenderLastFrame();
				}
			}
			else
				cFatmingGroupBehaviorList<cCueToStartCurveWithTime>::InternalRender();
			goto EXIT;
		}

	    if( !m_bStopAtLastFrame&&m_fCurrentProgress >= 1.f && !WaitUntilAllAnimationIsDone() )
		    goto EXIT;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    cCueToStartCurveWithTime* l_pTYPE = this->m_ObjectList[i];
			if( l_pTYPE->IsAnimationDone()&&!m_bStopAtLastFrame&&!l_pTYPE->IsAnimationLoop() )
			    continue;
			if(l_pTYPE->GetRenderData(&g_fMPDIOptmizeRenderVertices[l_iIndex*3*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],
				&g_fMPDIOptmizeRenderUV[l_iIndex*2*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],
				&g_fMPDIOptmizeRenderColor[l_iIndex*4*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],this->m_bStopAtLastFrame))
			{
				++l_iIndex;
			}
	    }
		if( l_iIndex >0 )
		{
			cBaseShader*l_p2DShader = GetCurrentShader();
			UseShaderProgram(DEFAULT_SHADER);
			this->m_ObjectList[0]->GetPointData(0)->pPI->ApplyImage();
			SetupShaderWorldMatrix(cMatrix44::Identity);
			myGlVertexPointer(3,g_fMPDIOptmizeRenderVertices);
			myGlUVPointer(2,g_fMPDIOptmizeRenderUV);
			myGlColorPointer(4,g_fMPDIOptmizeRenderColor);
			MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, l_iIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
			UseShaderProgram(l_p2DShader);
		}
EXIT:
		if(this->m_pViewPort)
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}

	void	cMultiPathDynamicImage::RenderLastFrameImage()
	{
		float	l_fOriginalCurrentTime = m_fPastTime;
		float	l_fOriginalProgress = m_fCurrentProgress;
		m_fPastTime = m_fTotalPlayTime-0.00001f;
		m_fCurrentProgress = 1.f-0.000001f;
		RenderByGlobalTime();
		m_fPastTime = l_fOriginalCurrentTime;
		m_fCurrentProgress = l_fOriginalProgress;
	}
	
	void	cMultiPathDynamicImage::RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor)
	{
	    int l_iCount = this->Count();
	    for( int i=0;i<l_iCount;++i )
	    {
	        this->m_ObjectList[i]->RenderFirsttFrameImage(e_bBlending,e_vColor);
	    }
	}

	Vector3	cMultiPathDynamicImage::GetLastFramePos(int e_iIndex)
	{
		cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_ObjectList[e_iIndex];
		return l_pCueToStartCurvesWithTime->GetPositionByTime(m_fTotalPlayTime);
	}

	Vector3	cMultiPathDynamicImage::GetPathCurrentPos(int e_iIndex)
	{
		if( m_ObjectList.size()>0 )
		{
			return Vector3(m_ObjectList[e_iIndex]->GetCurrentPosition());
		}
		assert(0&&"there is no any content in the vector");
		return Vector3(0.f,0.f,0.f);;
	}

	Vector3	cMultiPathDynamicImage::GetPathCurrentPosWithPos(int e_iIndex)
	{
		return GetPathCurrentPos(e_iIndex)+this->m_vPos;
	}

	void	cMultiPathDynamicImage::SetScale(float e_fValue)
	{
		if( e_fValue != m_fScale )
		{
			this->m_fScale = e_fValue;
			for( int i=0;i<Count();++i )
				this->m_ObjectList[i]->SetScale(e_fValue);
		}
	}
//=======================================
	void	cMultiPathDynamicImage::SetPos(Vector3 e_vPos)
	{
		this->m_vPos = e_vPos;
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetPos(m_vPos);
		}
	}
//=======================================
	void	cMultiPathDynamicImage::SetTransform(cMatrix44 e_mat)
	{
		Quaternion	l_Quaternion(e_mat);
		this->m_vAngle = l_Quaternion.ToEulerWithDegree();
		this->SetPos(e_mat.GetTranslation());
	}

	void	cMultiPathDynamicImage::SetTransformByImageCenter(cMatrix44 e_mat)
	{
		Quaternion	l_Quaternion(e_mat);
		this->m_vAngle = l_Quaternion.ToEuler();
		this->SetPosByImageCenter(e_mat.GetTranslation());
	}

	cMatrix44		cMultiPathDynamicImage::GetTransformByImageCenter()
	{
		Vector3	l_vPos(m_vPos.x + this->m_vDrawSize.x/2.f,m_vPos.y + this->m_vDrawSize.y/2.f,0.f);
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::RotationMatrix(this->m_vAngle);
		if( this->m_fScale != 1.f )
		{
			l_mat *= cMatrix44::ScaleMatrix(Vector3(m_fScale,m_fScale,m_fScale));
		}
		if( this->Count() > 0 )
		{
			l_mat *= cMatrix44::TranslationMatrix(this->GetObject(0)->m_vRotationAnglePosOffset);
		}
		return l_mat;
	}

	cMatrix44		cMultiPathDynamicImage::GetTransform()
	{
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(m_vPos)*cMatrix44::RotationMatrix(this->m_vAngle);
		if( this->m_fScale != 1.f )
		{
			l_mat *= cMatrix44::ScaleMatrix(Vector3(m_fScale,m_fScale,m_fScale));
		}
		if( this->Count() > 0 )
		{
			l_mat *= cMatrix44::TranslationMatrix(this->GetObject(0)->m_vRotationAnglePosOffset);
		}
		return l_mat;
	}
//=======================================
	Vector3	cMultiPathDynamicImage::GetPosByImageCenter()
	{
		Vector3	l_vPos(m_vPos.x + this->m_vDrawSize.x*this->m_fScale/2.f,
		m_vPos.y + this->m_vDrawSize.y*this->m_fScale/2.f,0.f);
		return l_vPos;
	}

	void	cMultiPathDynamicImage::SetPosByImageCenter(Vector3 e_vPos)
	{
		this->m_vPos = e_vPos;
		m_vPos.x -= this->m_vDrawSize.x*this->m_fScale/2.f;
		m_vPos.y -= this->m_vDrawSize.y*this->m_fScale/2.f;
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetPos(m_vPos);
		}
	}

	//add a specific relative position to all child.
	void	cMultiPathDynamicImage::SetRelativePosToChild(Vector3 e_vPos)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			(*this)[i]->AddOffsetToAllPoint(e_vPos);
		}
	}
	//set a offset position by center position
	void	cMultiPathDynamicImage::SetCenterPosToChild(Vector3 e_vPos)
	{
		this->FindoutAllPointsCenter();
		 Vector3	l_vPos = e_vPos-this->m_vRotationCenter;
		 SetRelativePosToChild(l_vPos);
	}

	void	cMultiPathDynamicImage::SetAngle(float e_fZangle)
	{
		cMultiPathDynamicImage::SetAngle(Vector3(0,0,e_fZangle));
	}

	void	cMultiPathDynamicImage::SetAngle(Vector3 e_vAngle)
	{
		this->m_vAngle = e_vAngle;
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetAngle(m_vAngle);
		}
	}

	cMatrix44		cMultiPathDynamicImage::GetMatrix()
	{
		if( m_fScale != 1.f )
			return cMatrix44::TranslationMatrix(this->m_vPos)*cMatrix44::RotationMatrix(this->m_vAngle)*cMatrix44::ScaleMatrix(Vector3(this->m_fScale,this->m_fScale,1.f));
		return cMatrix44::TranslationMatrix(this->m_vPos)*cMatrix44::RotationMatrix(this->m_vAngle);
	}
	
	void    cMultiPathDynamicImage::SetCurrentColor(Vector4 e_vColor)
	{
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetCurrentColor(e_vColor);
		}	
	}

    void    cMultiPathDynamicImage::SetColor(Vector4 e_vColor)
    {
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetColor(e_vColor);
		}
    }

	void    cMultiPathDynamicImage::SetColorScale(Vector4 e_vColor)
	{
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetColorScale(e_vColor);
		}	
	}

	void	cMultiPathDynamicImage::SetSelfRotationToChild(Quaternion e_Quaternion,bool e_bRotateImage)
	{
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->RotateCurveWithCurveCenter(e_Quaternion,e_bRotateImage);
		}
	}

	void	cMultiPathDynamicImage::SetCenterRotationToChild(Quaternion e_Quaternion,bool e_bRotateImage)
	{
		this->FindoutAllPointsCenter();
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->RotateCurveWithSpecificCenterPosition(this->m_vRotationCenter,e_Quaternion,e_bRotateImage);
		}
	}
	
    void	cMultiPathDynamicImage::SetScaleForAllChildPointData(float e_fValue)
    {
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->SetScaleForPointData(e_fValue);
		}
    }
    
	void    cMultiPathDynamicImage::RenderCollide()
	{
		for( size_t i=0;i<this->m_ObjectList.size();++i )
		{
			m_ObjectList[i]->RenderCollide();
		}	
	}

    bool    cMultiPathDynamicImage::Collide(int e_iPosX,int e_iPosY,int e_iIndex)
    {
        return m_ObjectList[e_iIndex]->Collide(e_iPosX,e_iPosY);
    }
    
    bool    cMultiPathDynamicImage::Collide(int e_iPosX,int e_iPosY)
    {
        int l_iCOunt = this->Count();
        for( int i=0;i<l_iCOunt;++i )
        {
            if(m_ObjectList[i]->Collide(e_iPosX,e_iPosY))
                return true;
        }
        return false;
    }

	bool	cMultiPathDynamicImage::Collide(cMultiPathDynamicImage*e_pMPDI)
	{
		Vector3	l_vPos = this->GetPos();
		Vector2	l_vSize = this->GetDrawSize();
		Vector4	l_vRect1(l_vPos.x,l_vPos.y,l_vPos.x+l_vSize.x,l_vPos.y+l_vSize.y);
		l_vPos = e_pMPDI->GetPos();
		l_vSize = e_pMPDI->GetDrawSize();
		Vector4	l_vRect2(l_vPos.x,l_vPos.y,l_vPos.x+l_vSize.x,l_vPos.y+l_vSize.y);
		return l_vRect1.Collide(l_vRect2);
	}

	void	cMultiPathDynamicImage::SetViewPort(Vector4 e_vViewPort)
	{
		if(!this->m_pViewPort)
			m_pViewPort = new Vector4();
		*m_pViewPort = e_vViewPort;
	}

	void	cMultiPathDynamicImage::RemoveViewPort()
	{
		SAFE_DELETE(m_pViewPort);
	}

	bool	cMultiPathDynamicImage::IsUsingPI(cPuzzleImage*e_pPI,bool e_bRemoveReferenceIfNotInUsing)
	{
		if( !e_pPI )
			return false;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(this->GetObject(i)->IsUsingPuzzleImage(e_pPI))
				return true;
		}
		if( e_bRemoveReferenceIfNotInUsing )
		{
			if(this->GetPuzzleImage(e_pPI->GetName()))
			{
				RemovePIReference(e_pPI);
				for( int i=0;i<l_iCount;++i )
				{
					this->GetObject(i)->RemovePIReference(e_pPI);
				}
			}
		}
		return false;
	}
	//<MPDI MPDIList="" MPDI="" />
	cMultiPathDynamicImage*		cMultiPathDynamicImage::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cMultiPathDynamicImage::TypeID);
		cMultiPathDynamicImage*l_pMPDI = 0;
		cMultiPathDynamicImage*l_pCloneMPDI = 0;
		cMPDIList*l_pMPDIList = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("MPDIList")
			{
				l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
			}
			else
			COMPARE_NAME("MPDI")
			{
				if(l_pMPDIList)
					l_pMPDI = l_pMPDIList->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if( l_pMPDI )
			l_pCloneMPDI = dynamic_cast<cMPDI*>(l_pMPDI->Clone());
		return l_pCloneMPDI;
	}

	//===============================
	//
	//===============================

	cMPDIList::cMPDIList(cMPDIList*e_pcMPDIList):cMulti_PI_Image(e_pcMPDIList)
	{
		this->m_vPos = e_pcMPDIList->m_vPos;
		m_iCurrentPlayAnimationIndex = -1;
		m_pCurrentMultiPathDynamicImage = 0;
		for( int i=0;i<e_pcMPDIList->Count();++i )
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = new cMultiPathDynamicImage(e_pcMPDIList->GetObject(i));
			bool	l_b = this->AddObject(l_pMultiPathDynamicImage);
			assert(l_b&&"forgot to set name??"); 
		}
		
	}

	cMPDIList::cMPDIList():cMulti_PI_Image()
	{
		m_iCurrentPlayAnimationIndex = -1;
		m_pCurrentMultiPathDynamicImage = 0;
		this->m_vPos = Vector3::Zero;
	}
	
	bool    cMPDIList::RemovePIReference(cPuzzleImage*e_pPuzzleImage,std::wstring*e_pInUsingDataInfo)
	{
		bool	l_bInUse = false;
	    int l_iCount = this->Count();
	    for( int i=0;i<l_iCount;++i )
	    {
	        cMultiPathDynamicImage*l_pMultiPathDynamicImage = this->m_ObjectList[i];
	        int l_iCount2 = l_pMultiPathDynamicImage->Count();
	        for( int j=0;j<l_iCount2;++j )
	        {
	            cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
	            size_t  l_iCount3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
	            for( size_t k=0;k<l_iCount3;++k )
	            {
	                if((*l_pCueToStartCurveWithTime->GetPointDataList())[k]->pPI == e_pPuzzleImage)
					{
						if( e_pInUsingDataInfo )
						{
							*e_pInUsingDataInfo += l_pMultiPathDynamicImage->GetName();
							*e_pInUsingDataInfo += L",";
							*e_pInUsingDataInfo += l_pCueToStartCurveWithTime->GetName();
							*e_pInUsingDataInfo += L",PointIndex:";
							*e_pInUsingDataInfo += ValueToStringW(k);
							*e_pInUsingDataInfo += L"\n";
						}
	                    l_bInUse = true;
					}
	            }
	        }
	    }
		if( l_bInUse )
			return false;
		const WCHAR*l_strPIName = e_pPuzzleImage->GetName();
		ForceRemovePIReference(e_pPuzzleImage);
		if( cGameApp::m_spAnimationParser )
		{
			int	l_iNumUsing = cGameApp::m_spAnimationParser->NumObjectUsingPI(e_pPuzzleImage);
			if( l_iNumUsing >1 )
				cGameApp::m_spAnimationParser->GetAllBaseImageList()->RemoveObject(l_strPIName);
		}
	    return true;
	}

    void    cMPDIList::ForceRemovePIReference(cPuzzleImage*e_pPuzzleImage)
    {
 	    int l_iCount = this->Count();
	    for( int i=0;i<l_iCount;++i )
	    {
	        cMultiPathDynamicImage*l_pMultiPathDynamicImage = this->m_ObjectList[i];
            l_pMultiPathDynamicImage->RemovePIReference(e_pPuzzleImage);
	        int l_iCount2 = l_pMultiPathDynamicImage->Count();
	        for( int j=0;j<l_iCount2;++j )
	        {
	            cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
	            size_t  l_iCount3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
	            l_pCueToStartCurveWithTime->RemovePIReference(e_pPuzzleImage);
	            for( size_t k=0;k<l_iCount3;++k )
	            {
	                if((*l_pCueToStartCurveWithTime->GetPointDataList())[k]->pPI == e_pPuzzleImage)
	                {
	                    (*l_pCueToStartCurveWithTime->GetPointDataList())[k]->pPI = 0;
	                    (*l_pCueToStartCurveWithTime->GetPointDataList())[k]->iImageIndex = -1;
	                }
	            }
	        }
	    }
		cMulti_PI_Image::RemovePIReference(e_pPuzzleImage);
    }

	void	cMPDIList::Merge(cMPDIList*e_pMPDIList)
	{
		if( e_pMPDIList == this )
			return;
		cMulti_PI_Image::Merge(e_pMPDIList);
		int	l_iNum = e_pMPDIList->Count();
		char	l_strName[MAX_PATH];
		for(int i =0;i<l_iNum;++i)
		{
			cMultiPathDynamicImage* l_pOriginal = (*e_pMPDIList)[i];
			cMultiPathDynamicImage* l_pNewOne = dynamic_cast<cMultiPathDynamicImage*>(l_pOriginal->Clone());
			while(!this->AddObject(l_pNewOne))
			{
				static int	l_isSeriesIndex = 0;
				++l_isSeriesIndex;
				sprintf(l_strName,"%ls%d",UT::WcharToChar(l_pOriginal->GetName()).c_str(),l_isSeriesIndex);
				l_pNewOne->SetName(UT::CharToWchar(l_strName));
			}
		}
	}

	void	cMPDIList::ReleaseHintpointSoundData()
	{
		int	l_iNumMPDI = Count();
		for( int i=0;i<l_iNumMPDI;++i )
		{
			cMPDI*l_pMPDI = GetObject(i);
			int	l_iNumSubMPDI = l_pMPDI->Count();
			for( int j=0;j<l_iNumSubMPDI;++j )
			{
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				std::vector<sHintPoint>*l_pHintPointList = l_pSubMPDI->GetHintPointVector();
				if(l_pHintPointList->size())
				{
					for(size_t k=0;k<l_pHintPointList->size();++k)
					{
						sHintPoint*l_pHintPoint = &(*l_pHintPointList)[k];
						if(l_pHintPoint->AnimationEvent == eAE_SOUND)
						{
							if(cGameApp::m_spSoundParser)
							{
								cBasicSound*l_pNamedTypedObject = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath(l_pHintPoint->strEventName));
								if( l_pNamedTypedObject )
								{
									std::wstring	l_strName = l_pNamedTypedObject->GetName();
									if( l_pNamedTypedObject )//it's possible sound manager delete before animation manager.
									{
										int	l_iReferenceCount = l_pNamedTypedObject->Release(l_pSubMPDI);
										if( l_iReferenceCount == 0 )
										{
											cGameApp::m_spSoundParser->RemoveObjectWithoutDelete(l_strName.c_str());
										}
									}
								}
							}
						}
					}
				}
			}
		}	
	}


    char*  cMPDIList::GetFileName(const WCHAR*e_strObjectName)
    {
		sprintf(g_strForMPDIString,"%s\0",UT::WcharToChar(e_strObjectName).c_str());
        int l_iIndex = UT::FindChacterIndexFromBackBuffer(e_strObjectName,L'_');
		if( l_iIndex == -1 )
		{
			sprintf(g_strForMPDIString,"%s.mpdi\0",UT::WcharToChar(e_strObjectName).c_str());
		}
		else
		{
			g_strForMPDIString[l_iIndex] = L'.';
		}
        return g_strForMPDIString;
    }

#ifdef ANDROID
    std::wstring cMPDIList::FileToMPDIListName(const WCHAR*e_strFileName)
    {
		std::wstring	l_wstring;
		char	l_str[MAX_PATH];
		sprintf(l_str,"%s_mpdi\0",UT::GetFileNameWithoutFullPath(UT::WcharToChar(e_strFileName)));
		WCHAR	l_temp[MAX_PATH];
		CHAR_TO_WCHAR(l_str,l_temp);
		l_wstring = l_temp;
        return l_wstring;
    }

	std::wstring cMPDIList::FileToMPDIListName(const char*e_strFileName)
	{
		std::wstring	l_wstring;
		char	l_str[MAX_PATH];
		sprintf(l_str,"%s_mpdi\0",UT::GetFileNameWithoutFullPath(e_strFileName));
		WCHAR	l_temp[MAX_PATH];
		CHAR_TO_WCHAR(l_str,l_temp);
		l_wstring = l_temp;
        return l_wstring;
	}
#else
	std::wstring cMPDIList::FileToMPDIListName(const char*e_strFileName)
	{
		std::wstring	l_str;
		WCHAR l_strTemp[TEMP_SIZE];
		swprintf(l_strTemp,MAX_PATH,L"%ls_mpdi\0",UT::GetFileNameWithoutFullPath(UT::CharToWchar(e_strFileName).c_str()));
		l_str = l_strTemp;
        return l_str;
	}
    std::wstring cMPDIList::FileToMPDIListName(const WCHAR*e_strObjectName)
    {
		std::wstring	l_str;
		WCHAR l_strTemp[TEMP_SIZE];
        swprintf(l_strTemp,MAX_PATH,L"%ls_mpdi\0",UT::GetFileNameWithoutFullPath((WCHAR*)e_strObjectName));
		l_str = l_strTemp;
        return l_str;
    }
#endif
//end namespace FATMING_CORE
}