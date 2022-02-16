#include "../stdafx.h"
#include "SimplePrimitive.h"
//#include "AnimationParser.h"
#include "SubMPDI.h"
#include "MPDI.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
#include "../Sound/SoundManager.h"
namespace FATMING_CORE
{
	const wchar_t*     cCueToStartCurveWithTime::TypeID( L"cCueToStartCurveWithTime" );
	cCueToStartCurveWithTime::cCueToStartCurveWithTime(cMulti_PI_Image*e_pMulti_PI_Image):cMulti_PI_Image(e_pMulti_PI_Image)
	{
		m_eAnchorType = eIAT_CENTER_CENTER;
		m_iCurrentHintPointDataIndex = -1;
		m_vRotationAnglePosOffset = Vector3::Zero;
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		m_fScale = 1.f;
		m_bBehaviorUpdateActive = false;
		m_fCurrentTime = 0.f;
		m_bColorBlending = false;
		m_SrcBlendingMode = GL_SRC_ALPHA;
		m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		m_bHintPoint = false;
		m_vAngle = Vector3::Zero;
	}

	cCueToStartCurveWithTime::cCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime)
		:cMulti_PI_Image(e_pCueToStartCurvesWithTime),cCurveWithTime(e_pCueToStartCurvesWithTime)
	{
		if(e_pCueToStartCurvesWithTime->IsStayAtLastFrame())
		{
			int a=0;
		}
		m_eAnchorType = e_pCueToStartCurvesWithTime->m_eAnchorType;

		m_vRotationAnglePosOffset = e_pCueToStartCurvesWithTime->m_vRotationAnglePosOffset;
		//m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData(e_pCueToStartCurvesWithTime->m_pCurrentPointData);
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		size_t l_iSize = e_pCueToStartCurvesWithTime->m_PointDataList.size();
		m_PointDataList.reserve(l_iSize);
		for(size_t i=0;i<l_iSize;++i)
		{
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_pCueToStartCurvesWithTime->m_PointDataList[i]);
			m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		}
		m_fScale = e_pCueToStartCurvesWithTime->m_fScale;
		m_bBehaviorUpdateActive = e_pCueToStartCurvesWithTime->m_bBehaviorUpdateActive;
		m_fCurrentTime = e_pCueToStartCurvesWithTime->m_fCurrentTime;
		m_bColorBlending = e_pCueToStartCurvesWithTime->m_bColorBlending;
		m_SrcBlendingMode = e_pCueToStartCurvesWithTime->m_SrcBlendingMode;
		m_DestBlendingMode = e_pCueToStartCurvesWithTime->m_DestBlendingMode;
		m_bHintPoint = e_pCueToStartCurvesWithTime->m_bHintPoint;
		m_iCurrentHintPointDataIndex = e_pCueToStartCurvesWithTime->m_iCurrentHintPointDataIndex;
		m_HintPointVector = e_pCueToStartCurvesWithTime->m_HintPointVector;
		m_vAngle = e_pCueToStartCurvesWithTime->m_vAngle;
	}

	cCueToStartCurveWithTime::cCueToStartCurveWithTime()
	{
		m_eAnchorType = eIAT_CENTER_CENTER;
		m_iCurrentHintPointDataIndex = -1;
		m_vRotationAnglePosOffset = Vector3::Zero;
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		m_fScale = 1.f;
		m_bBehaviorUpdateActive = false;
		m_fCurrentTime = 0.f;
		m_bColorBlending = false;
		m_SrcBlendingMode = GL_SRC_ALPHA;
		m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		m_bHintPoint = false;
		m_vAngle = Vector3::Zero;	
	}

	cCueToStartCurveWithTime::~cCueToStartCurveWithTime()
	{
		DELETE_VECTOR(m_PointDataList,sTexBehaviorDataWithImageIndexData*);
		SAFE_DELETE(m_pCurrentPointData);
	}

	void	cCueToStartCurveWithTime::UpdateData()//to detec current point index and image index
	{
		if( !m_PointDataList.size() )
			return;
		int	l_iCurrentOriginalPointIndex = (int)(m_PointDataList.size()-1);
		if(!this->m_bAnimationDone)
		{
			l_iCurrentOriginalPointIndex = CalculateCurrentPointIndex();
#ifdef DEBUG
			if( l_iCurrentOriginalPointIndex <0 )
			{
				int a=0;
			}
#endif
			float l_fEndTime = this->GetEndTime();
			//if(l_iCurrentOriginalPointIndex+1<(int)m_PointDataList.size() &&
			//	(this->m_fPastTime<l_fEndTime||this->m_bAnimationLoop) )
			if( this->m_fPastTime<this->GetEndTime() || this->m_bAnimationLoop )
			{
				if(m_bBehaviorUpdateActive && l_iCurrentOriginalPointIndex+1<(int)m_PointDataList.size() )
				{
					m_PointDataList[l_iCurrentOriginalPointIndex]->ToLerp(m_PointDataList[l_iCurrentOriginalPointIndex+1],this->m_fCurrentLERPTime,m_pCurrentPointData);
				}
				else
				//because fucking dot net is stupid about momory stack,it will get old memory,and I dnno why,so I have to recheck again here...
				if( (int)m_PointDataList.size() > l_iCurrentOriginalPointIndex )
				{
					m_PointDataList[l_iCurrentOriginalPointIndex]->DumpData(m_pCurrentPointData);
				}
			}
			else
			{
				if(!this->m_bCurveLoop)
					m_bAnimationDone = true;
				m_PointDataList[l_iCurrentOriginalPointIndex]->DumpData(m_pCurrentPointData);
			}
		}
		else
		{
			if( l_iCurrentOriginalPointIndex>=(int)m_PointDataList.size() )
				l_iCurrentOriginalPointIndex = m_PointDataList.size()-1;
			*m_pCurrentPointData = *this->m_PointDataList[l_iCurrentOriginalPointIndex];
		}
		//update to latest pi.
		m_pPuzzleImage = m_pCurrentPointData->pPI;
#ifdef	DEBUG 
		if( m_pCurrentPointData->iImageIndex == -1 )
		{
			std::wstring	l_str = UT::ComposeMsgByFormat(L"image error,Name:",this->GetName());
			cGameApp::OutputDebugInfoString(l_str.c_str());
			//UT::ErrorMsg(this->GetName(),L"image error");
		}
#endif
		//hint point detect
		UINT	l_iNumHintPoint = (UINT)m_HintPointVector.size();
		if( l_iNumHintPoint )
		{
			sHintPoint*l_pHintPoint = &m_HintPointVector[0];
			for( UINT i=0;i<l_iNumHintPoint;++i )
			{
				if( l_pHintPoint[i].iPointIndex == l_iCurrentOriginalPointIndex )
				{
					if( m_iCurrentHintPointDataIndex != i )
					{
						m_bHintPoint = true;
						m_iCurrentHintPointDataIndex = i;
						if( l_pHintPoint->AnimationEvent == eAE_SOUND )
						{
							cBasicSound*l_pSound = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath(l_pHintPoint->strEventName));
							if( l_pSound  )
								l_pSound->Play(true);
						}
						//else
						//{
						//cPrtEmitter*l_pPrtEmitter = cGameApp::m_spPaticleManager->GetObject(l_pHintPoint->strEventName);
						//if( l_pPrtEmitter )
						//	l_pPrtEmitter->
						//}
						return;
					}
				}
			}
			m_bHintPoint = false;
		}
	}

	void	cCueToStartCurveWithTime::AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize)
	{
		cCurveWithTime::AddPoint(e_vPos,e_fTime);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_Size,e_vAngle,e_vColor,e_iImageIndex,e_bMirror,e_pPI);
		m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		if(e_bCheckOptmize&&this->GetOwner())
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = dynamic_cast<cMultiPathDynamicImage*>(this->GetOwner());
			l_pMultiPathDynamicImage->CheckRenderOptmize();
		}
	}

	void	cCueToStartCurveWithTime::AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,float e_fAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize)
	{
		cCurveWithTime::AddPoint(e_vPos,e_fTime);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_Size,Vector3(0,0,e_fAngle),e_vColor,e_iImageIndex,e_bMirror,e_pPI);
		m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		if(e_bCheckOptmize&&this->GetOwner())
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = dynamic_cast<cMultiPathDynamicImage*>(this->GetOwner());
			l_pMultiPathDynamicImage->CheckRenderOptmize();
		}
	}
	//
	void	cCueToStartCurveWithTime::DelPoint(int e_iIndex)
	{
		cCurveWithTime::DelPoint(e_iIndex);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_PointDataList[e_iIndex];
		delete l_pTexBehaviorDataWithImageIndexData;
		m_PointDataList.erase(m_PointDataList.begin()+e_iIndex);
	}

	void	cCueToStartCurveWithTime::ClearPoint()
	{
		while(m_PointDataList.size())
		{
			DelPoint(0);
		}
	}

	sTexBehaviorDataWithImageIndexData*cCueToStartCurveWithTime::GetPointData(int e_iIndex)
	{
		if( (int)m_PointDataList.size() <= e_iIndex )
			return 0;
		return m_PointDataList[e_iIndex];
	}

	void	cCueToStartCurveWithTime::InsertPoint(Vector3 e_Pos,float e_fTime,sTexBehaviorDataWithImageIndexData* e_pTexBehaviorDataWithImageIndexData,int e_iIndex)
	{
		cCurveWithTime::InsertPoint(e_Pos,e_fTime,e_iIndex);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_pTexBehaviorDataWithImageIndexData);
		this->m_PointDataList.insert(m_PointDataList.begin()+e_iIndex,l_pTexBehaviorDataWithImageIndexData);
	}

	//void cCueToStartCurveWithTime::ReplacePointsByCount(int e_iNumPoint)
	//{
	//	cCurveWithTime::ReplacePointsByCount(e_iNumPoint);
	//	if( !e_iNumPoint )
	//		return;
	//	std::vector<sTexBehaviorDataWithImageIndexData>l_PointDataList = m_PointDataList;
	//	int	l_iNum = l_PointDataList.size();
	//	float	l_fStep = (float)l_iNum/e_iNumPoint;
	//	m_PointDataList.clear();
	//	for( int i=0;i<e_iNumPoint;++i )
	//	{
	//		m_PointDataList.push_back(l_PointDataList[(int)(l_fStep*i)]);
	//	}
	//}

	void cCueToStartCurveWithTime::SetOriginalToFinal()
	{
		std::vector<sTexBehaviorDataWithImageIndexData*> l_PointDataList;
		UINT	l_uiSize = (UINT)this->m_FinallyPointList.size();
		UINT	l_iStepSize = l_uiSize/this->m_OriginalPointList.size();
		//	int	l_iLod = 1;	
		for( UINT i=0;i<l_uiSize;++i )
		{
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(m_PointDataList[i/l_iStepSize]);
			l_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		}
		DELETE_VECTOR(m_PointDataList,sTexBehaviorDataWithImageIndexData*);
		m_PointDataList.reserve(l_uiSize);
		for( UINT i=0;i<l_uiSize;++i )
		{
			m_PointDataList.push_back(l_PointDataList[i]);
		}
		l_PointDataList.clear();
		cCurveWithTime::SetOriginalToFinal();
		assert(m_PointDataList.size() == this->m_OriginalPointList.size());
	}

	void	cCueToStartCurveWithTime::RenderPuzzleData(sPuzzleData*e_pPuzzleData,Vector3 e_vPos,Vector3 e_vAngle)
	{
		if( m_pCurrentPointData->vColor.a == 0 )//totaly transparent
			return;
		GLenum	l_OriginalSrc,l_OriginalDest;
		if(this->m_bColorBlending)
		{
			glGetIntegerv(GL_BLEND_SRC,(GLint*)&l_OriginalSrc);
			glGetIntegerv(GL_BLEND_DST,(GLint*)&l_OriginalDest);
			glBlendFunc(m_SrcBlendingMode,m_DestBlendingMode);
		}
		float	l_Vertices[8];
		float   l_pfTextureUV[8];
		m_matRenderMatrix = GetRenderPuzzleDataAndMatrix(m_pCurrentPointData,2,l_Vertices,l_pfTextureUV,e_pPuzzleData,e_vPos,e_vAngle);
		DrawQuadWithMatrix(l_Vertices,l_pfTextureUV,m_pCurrentPointData->vColor,(float*)&m_matRenderMatrix.m[0][0]);
		if(this->m_bColorBlending)
		{
			glBlendFunc(l_OriginalSrc,l_OriginalDest);
		}
	}


	void	cCueToStartCurveWithTime::SetColor(Vector4 e_vColor)
	{
		UINT	l_uiSize = (UINT)this->m_FinallyPointList.size();
		for( UINT i=0;i<l_uiSize;++i )
		{
			m_PointDataList[i]->vColor = e_vColor;
		}
	}

	void	cCueToStartCurveWithTime::SetColorScale(Vector4 e_vColor)
	{
        UINT	l_uiSize = (UINT)this->m_FinallyPointList.size();
		for( UINT i=0;i<l_uiSize;++i )
		{
			Vector4	l_vColor = m_PointDataList[i]->vColor;
			l_vColor.x *= e_vColor.x;
			l_vColor.y *= e_vColor.y;
			l_vColor.z *= e_vColor.z;
			l_vColor.w *= e_vColor.w;
			m_PointDataList[i]->vColor = l_vColor;
		}
	}

	void	cCueToStartCurveWithTime::SetCurrentColor(Vector4 e_vColor)
	{
		this->m_pCurrentPointData->vColor = e_vColor;
	}


	cMatrix44  cCueToStartCurveWithTime::GetRenderPuzzleDataAndMatrix(sTexBehaviorDataWithImageIndexData*e_pImageData,int e_iStride,float*e_pOutVertices,float*e_pOutTextureUV,sPuzzleData*e_pPuzzleData,Vector3 e_vPos,Vector3 e_vAngle)
	{
		sPuzzleData*l_pPuzzleData = e_pPuzzleData;
#ifdef DEBUG
		if( !e_pImageData || !l_pPuzzleData || l_pPuzzleData->Size.x==0 || l_pPuzzleData->Size.y == 0 )
		{//something wrong here.
			int a=0;
		}
#endif
		Vector2	l_vScale(e_pImageData->Size.x/l_pPuzzleData->Size.x,e_pImageData->Size.y/l_pPuzzleData->Size.y);
		l_vScale *= m_fScale;
		Vector2	l_vImageSize(e_pImageData->Size.x*this->m_fScale,e_pImageData->Size.y*this->m_fScale);
		Vector2	l_vImageHalfSize = l_vImageSize/2.f;
		Vector2	l_vOffsetPos((float)l_pPuzzleData->OffsetPos.x,(float)l_pPuzzleData->OffsetPos.y);
		Vector2	l_vOriginalSize((float)l_pPuzzleData->OriginalSize.x,(float)l_pPuzzleData->OriginalSize.y);
		Vector2	l_vHalfOriginalSize = l_vOriginalSize/2.f;
		Vector2	l_vCenterToOffset = l_vOffsetPos-l_vHalfOriginalSize;

		Vector2	l_vFinalOffsetPos = l_vOriginalSize/2;
		//although this one is not correct but work for some function.
		Vector4	l_vEntendImagePoint = GetExtendImagePoint(m_eAnchorType,l_vImageHalfSize,l_vImageSize,1.f);
		Vector4	l_vDrawRect = l_vEntendImagePoint;
		if( m_eAnchorType == eIAT_CENTER_CENTER )
		{
			l_vCenterToOffset.x *= l_vScale.x;
			l_vCenterToOffset.y *= l_vScale.y;
			l_vDrawRect = Vector4(0,0,l_vImageSize.x,l_vImageSize.y);
		}
		//set vertex
		if( e_pOutVertices )
		{
			if( e_iStride == 2 )
			{
				//e_pOutVertices[0] = 0;				 e_pOutVertices[1] = 0;
				//e_pOutVertices[2] = l_vImageSize.x;  e_pOutVertices[3] = 0;
				//e_pOutVertices[4] = 0;				 e_pOutVertices[5] = l_vImageSize.y;
				//e_pOutVertices[6] = l_vImageSize.x;  e_pOutVertices[7] = l_vImageSize.y;
				e_pOutVertices[0] = l_vDrawRect.x;  e_pOutVertices[1] = l_vDrawRect.y;
				e_pOutVertices[2] = l_vDrawRect.z;  e_pOutVertices[3] = l_vDrawRect.y;
				e_pOutVertices[4] = l_vDrawRect.x;  e_pOutVertices[5] = l_vDrawRect.w;
				e_pOutVertices[6] = l_vDrawRect.z;  e_pOutVertices[7] = l_vDrawRect.w;
			}
			else
			if( e_iStride == 3 )
			{
				e_pOutVertices[0] = 0;				e_pOutVertices[1] = 0;				e_pOutVertices[2] = 0.f;
				e_pOutVertices[3] = l_vImageSize.x;	e_pOutVertices[4] =	0;				e_pOutVertices[5] = 0.f;
				e_pOutVertices[6] = 0;				e_pOutVertices[7] = l_vImageSize.y; e_pOutVertices[8] = 0.f;
				e_pOutVertices[9] = l_vImageSize.x;	e_pOutVertices[10]= l_vImageSize.y; e_pOutVertices[11] = 0.f;
			}
		}
		if( e_pOutTextureUV )
			AssignUVDataToTriangleStrip(l_pPuzzleData->fUV,e_pOutTextureUV,e_pImageData->bMirror);
		cMatrix44  l_mat2DTransform;
		//not quaterion is because quaterion has more complex calculation.
		Vector3	l_vPos = e_vPos;
		l_vPos.x += l_vFinalOffsetPos.x;
		l_vPos.y += l_vFinalOffsetPos.y;

		l_mat2DTransform = cMatrix44::TranslationMatrix(l_vPos+m_vCurrentPosition);
		l_mat2DTransform *= cMatrix44::RotationMatrix(e_vAngle+this->m_pCurrentPointData->vAngle);
		l_mat2DTransform *= cMatrix44::TranslationMatrix(m_vRotationAnglePosOffset+l_vCenterToOffset);
		if( this->GetParent() )
		{
			cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(this->GetParent());
			l_pMPDI->GetRotationCenter();
			cMatrix44	l_Parentmat = this->GetParent()->GetWorldTransform();
			l_mat2DTransform = l_Parentmat*l_mat2DTransform;
		}
		return l_mat2DTransform;
	}
	//0    14
	//23	5
	bool	cCueToStartCurveWithTime::GetRenderData(float*e_pfVertices,float*e_pfUV,float*e_pfColor,bool e_bForceToFetch)
	{
		if( !e_bForceToFetch )
		{
			if( m_bAnimationDone )
			{
				if( this->m_bCurveMoveDone && !this->m_bAnimationLoop )
					return  false;
			}
		}
		if(m_pCurrentPointData->iImageIndex == -1 )
			return false;
		if( !m_bStart || m_pCurrentPointData->vColor.a == 0 )
			return false;

		sPuzzleData*l_pPuzzleData = this->m_pCurrentPointData->pPI->GetPuzzleData()[this->m_pCurrentPointData->iImageIndex];
		if( e_pfUV )
			AssignUVDataTo2Triangles(l_pPuzzleData->fUV,e_pfUV,m_pCurrentPointData->bMirror);
		if( e_pfColor )
		{
			for( int i=0;i<6;++i )
				memcpy(&e_pfColor[i*4],&m_pCurrentPointData->vColor,sizeof(Vector4));
		}
		Vector3	l_Vertices[4];
		float   l_pfTextureUV[8];
		Vector3	l_vPos = this->GetPos();
		cMatrix44  l_mat2DTransform = GetRenderPuzzleDataAndMatrix(m_pCurrentPointData,3,(float*)l_Vertices,l_pfTextureUV,l_pPuzzleData,l_vPos,m_vAngle);
		//left down
		if( e_pfVertices )
		{
			for( int i=0;i<4;++i )
				l_Vertices[i] = l_mat2DTransform.TransformCoordinate(l_Vertices[i]);
			e_pfVertices[0] = l_Vertices[2].x;			//left down
			e_pfVertices[1] = l_Vertices[2].y;
			e_pfVertices[2] = l_Vertices[2].z;

			e_pfVertices[3] = l_Vertices[3].x;			//right down
			e_pfVertices[4] = l_Vertices[3].y;
			e_pfVertices[5] = l_Vertices[3].z;

			e_pfVertices[6] = l_Vertices[0].x;			//left up
			e_pfVertices[7] = l_Vertices[0].y;
			e_pfVertices[8] = l_Vertices[0].z;

			e_pfVertices[9] = l_Vertices[0].x;			//left up
			e_pfVertices[10] = l_Vertices[0].y;
			e_pfVertices[11] = l_Vertices[0].z;

			e_pfVertices[12] = l_Vertices[3].x;			//right down
			e_pfVertices[13] = l_Vertices[3].y;
			e_pfVertices[14] = l_Vertices[3].z;

			e_pfVertices[15] = l_Vertices[1].x;			//right up
			e_pfVertices[16] = l_Vertices[1].y;
			e_pfVertices[17] = l_Vertices[1].z;
		}
		return true;
	}

	bool	cCueToStartCurveWithTime::IsUsingPuzzleImage(cPuzzleImage*e_pPI)
	{
		size_t	l_uiSize = this->m_PointDataList.size();
		for(size_t i=0;i<l_uiSize;++i)
		{
			if(e_pPI == m_PointDataList[i]->pPI)
				return true;
		}
		return false;
	}

	void cCueToStartCurveWithTime::Render(sPuzzleData*e_pPuzzleData,int e_iPosX,int e_iPosY)
	{
		if( !this->m_bAnimationDone )
		{
			if( !m_bStart )
				return;
			this->m_pCurrentPointData->pPI->ApplyImage();
			Vector3	l_vPos = this->GetPos();
			l_vPos.x += e_iPosX;
			l_vPos.y += e_iPosY;
			RenderPuzzleData(e_pPuzzleData,l_vPos,m_vAngle);
		}
	}

	int	cCueToStartCurveWithTime::CalculateCurrentPointIndex()
	{
		if( m_iCurrentPointIndex == -1 )//time has changed,it should't happen but for edit is fine
			m_iCurrentPointIndex = 0;
		if( this->m_iLOD==1 )
			return m_iCurrentPointIndex;
		float	l_fCurrentStep = m_iCurrentPointIndex/(float)this->m_FinalTimeList.size();
		l_fCurrentStep *= this->m_OriginalTimeList.size();
#ifdef DEBUG
		if( (unsigned int)l_fCurrentStep>=m_PointDataList.size() )
		{
			assert(0&&"call fatming - MDPI.h");
		}
#endif
		return (int)l_fCurrentStep;
	}

	void	cCueToStartCurveWithTime::InternalInit()
	{
		cCurveWithTime::InternalInit();
		this->m_bAnimationDone = false;
		m_bHintPoint = false;
		m_iCurrentHintPointDataIndex = -1;
		this->Update(EPSIONAL);
	}
	void	cCueToStartCurveWithTime::InternalUpdate(float e_fElpaseTime)
	{
		if( this->m_bCurveMoveDone )
			return ;
		cCurveWithTime::InternalUpdate(e_fElpaseTime);
		UpdateData();
	}

	void cCueToStartCurveWithTime::InternalRender()
	{
		if( this->m_bCurveMoveDone )
		{
			if( m_bStayAtLastFrame )
			{
				this->RenderLastFrame();
			}
			return ;
		}
		if( !m_pCurrentPointData->pPI )
			return;
		m_pCurrentPointData->pPI->ApplyImage();

		if( m_pCurrentPointData->iImageIndex == -1 )
		{
#ifdef DEBUG
			std::wstring	l_str = UT::ComposeMsgByFormat(L"image error,Name:",this->GetName());
			cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
			return;
		}
		sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[this->m_pCurrentPointData->iImageIndex];
		Vector3	l_vPos = this->GetPos();
		RenderPuzzleData(l_pPuzzleData,l_vPos,m_vAngle);
	}

	void	cCueToStartCurveWithTime::InvertOrder()
	{
		cCurveWithTime::InvertOrder();
		InvertVectorDateOrder(&m_PointDataList);
	}

	void	cCueToStartCurveWithTime::RenderLastFrame()
	{
		if( m_PointDataList.size() == 0 || !m_pCurrentPointData->pPI )
			return;
		float	l_fLastTime = this->GetEndTime()-EPSIONAL;
		if( fabs(l_fLastTime - m_fPastTime) > 0.f )
		{
			GetPositionByTime(l_fLastTime-EPSIONAL);
		}
		*m_pCurrentPointData = *this->m_PointDataList[m_PointDataList.size()-1];
		int l_iLastFrameImageIndex = this->m_PointDataList[m_PointDataList.size()-1]->iImageIndex;
		if( l_iLastFrameImageIndex == -1 )
		{
#ifdef DEBUG
			std::wstring	l_str = UT::ComposeMsgByFormat(L"image error,Name:",this->GetName());
			UT::ErrorMsg(l_str.c_str(),L"Error RenderLastFrame");
			//cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
			return;
		}
		sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[l_iLastFrameImageIndex];
		Vector3	l_vPos = this->GetPos();
		m_pCurrentPointData->pPI->ApplyImage();
		RenderPuzzleData(l_pPuzzleData,l_vPos,m_vAngle);
	}

	void	cCueToStartCurveWithTime::SetAnimationLoop(bool e_bLoop)
	{
		m_bAnimationLoop = e_bLoop;
		this->m_bCurveLoop = e_bLoop;
	}

	void	cCueToStartCurveWithTime::RenderByGlobalTime(float e_fTime)
	{
		this->SetCurrentTime(e_fTime);
		int	l_iLastImageIndex = -1;
		UINT	l_uiSize = (UINT)this->GetTimeList().size();
		if( !l_uiSize )
			return;
		float	l_fEndTime = this->GetTimeList()[l_uiSize-1];
		//ensure start time
		if(this->GetStartTime()>m_fCurrentTime)
			return;
		//it's over start time and end time,it's possible using in the editor ,so it is possible not set end time yet,so do safe way to get right time
		float	l_fStartAndEndTime = this->GetStartTime()+l_fEndTime;
		float	l_fTargetTime = m_fCurrentTime;
		if( l_fStartAndEndTime<=m_fCurrentTime )
		{//it's loop so we have to loop it,current time minus it's own play time and play it
			if( !this->IsAnimationLoop() )
			{
				return;
			}
			else
			{
				l_fTargetTime = UT::GetFloatModulus(e_fTime-GetStartTime(),l_fEndTime);
				l_fTargetTime += GetStartTime();
			}
		}
		//render by global time so animation won't be done
		this->m_bAnimationDone = false;
		GetPositionByTime(l_fTargetTime);
		//l_iLastImageIndex = this->GetCurrentImageIndex();
		l_iLastImageIndex = this->m_pCurrentPointData->iImageIndex;
		if( l_iLastImageIndex != -1 )
		{
			if( m_pCurrentPointData->pPI )
			{
				m_pCurrentPointData->pPI->ApplyImage();
				sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[m_pCurrentPointData->iImageIndex];
				Vector3	l_vPos = +this->GetPos();
				RenderPuzzleData(l_pPuzzleData,l_vPos,m_vAngle);
			}
		}	
	}

	sHintPoint*	cCueToStartCurveWithTime::GetCurrentHintPointData(int *e_piNumRelated)//it could be none if it doesn't occur any hitpoint
	{
		if( m_bHintPoint )
		{
			if( e_piNumRelated )
			{
				*e_piNumRelated = 1;
				int	l_iHintPointIndex = m_HintPointVector[m_iCurrentHintPointDataIndex].iPointIndex;
				for( UINT i=m_iCurrentHintPointDataIndex+1;i<m_HintPointVector.size();++i )
				{
					if( l_iHintPointIndex == m_HintPointVector[i].iPointIndex )
						*e_piNumRelated = *e_piNumRelated+1;
				}
			}
			return &m_HintPointVector[m_iCurrentHintPointDataIndex];
		}
		return 0;
	}

	void	cCueToStartCurveWithTime::ChangeAllImageByImageIndex(int e_iIndex,bool e_bScalingImage)
	{
		for(size_t i =0;i<m_PointDataList.size();++i)
		{
			if( e_bScalingImage )
			{
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_PointDataList[i];
				cPuzzleImageUnit*l_pPIUnuit = (*l_pTexBehaviorDataWithImageIndexData->pPI)[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
				if( l_pPIUnuit )
				{
					float	l_fScaleX = l_pTexBehaviorDataWithImageIndexData->Size.x/(*l_pPIUnuit->GetHeight());
					float	l_fScaleY = l_pTexBehaviorDataWithImageIndexData->Size.y/(*l_pPIUnuit->GetHeight());
					l_pPIUnuit = (*l_pTexBehaviorDataWithImageIndexData->pPI)[e_iIndex];
					m_PointDataList[i]->Size.x = *l_pPIUnuit->GetWidth()*l_fScaleX;
					m_PointDataList[i]->Size.y = *l_pPIUnuit->GetHeight()*l_fScaleY;
				}
			}
			m_PointDataList[i]->iImageIndex = e_iIndex;
		}
	}

	void	cCueToStartCurveWithTime::ImageRotation(Vector3 e_vAngle)
	{
		if( m_PointDataList.size() )
		{
			//float	l_fOffsetAngle = e_fAngle-m_PointDataList[0]->fAngle;
			for(size_t i =0;i<m_PointDataList.size();++i)
			{
				//m_PointDataList[i]->fAngle += l_fOffsetAngle;
				m_PointDataList[i]->vAngle.x = GetFloatModulus(m_PointDataList[i]->vAngle.x+e_vAngle.x,360);
				m_PointDataList[i]->vAngle.y = GetFloatModulus(m_PointDataList[i]->vAngle.y+e_vAngle.y,360);
				m_PointDataList[i]->vAngle.z = GetFloatModulus(m_PointDataList[i]->vAngle.z+e_vAngle.z,360);
			}
		}
	}

	void	cCueToStartCurveWithTime::ImageRelativeRotation(Vector3 e_vAngle)
	{
		if( m_PointDataList.size() )
		{
			for(size_t i =0;i<m_PointDataList.size();++i)
			{
				m_PointDataList[i]->vAngle += e_vAngle;
			}
		}	
	}

	void	cCueToStartCurveWithTime::RotateCurveWithCurveCenter(Quaternion e_Quaternion,bool e_bRotateImage)
	{
		cCurveWithTime::RotateCurveWithCurveCenter(e_Quaternion.ToMatrix());
		if( e_bRotateImage )
		{
			Vector3 l_vAngle = e_Quaternion.ToEuler();
			Vector3	l_vDegreeAngle(D3DXToDegree(l_vAngle.x),D3DXToDegree(l_vAngle.y),D3DXToDegree(l_vAngle.z));
			ImageRelativeRotation(l_vDegreeAngle);
		}
	}

	void	cCueToStartCurveWithTime::RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion,bool e_bRotateImage)
	{
		cCurveWithTime::RotateCurveWithSpecificCenterPosition(e_vCenterPos,e_Quaternion);
		Vector3 l_vAngle = e_Quaternion.ToEuler();
		if( e_bRotateImage )
		{
			Vector3	l_vDegreeAngle(D3DXToDegree(l_vAngle.x),D3DXToDegree(l_vAngle.y),D3DXToDegree(l_vAngle.z));
			ImageRotation(l_vDegreeAngle);
		}
	}


	void	cCueToStartCurveWithTime::SetScaleForPointData(float e_fScale)
	{
		if( m_PointDataList.size() )
		{
			for(size_t i =0;i<m_PointDataList.size();++i)
			{
				m_PointDataList[i]->Size *= e_fScale;
			}
		}    
	}

	bool    cCueToStartCurveWithTime::Collide(int e_iPosX,int e_iPosY)
	{
		int l_iTargetIndex = CalculateCurrentPointIndex();
		Vector2 l_vSize = this->m_PointDataList[l_iTargetIndex]->Size;
		Vector3	l_vPos = GetPos();
		float	l_fPosX = this->m_vCurrentPosition.x+l_vPos.x;
		float	l_fPosY = this->m_vCurrentPosition.y+l_vPos.y;
		return CollideRectWithTransform(e_iPosX,e_iPosY,
			(int)(l_fPosX),
			(int)(l_fPosY),
			this->m_PointDataList[l_iTargetIndex]->vAngle,
			(int)l_vSize.x,(int)l_vSize.y);
	}

	void    cCueToStartCurveWithTime::RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor)
	{
		int	l_iLastImageIndex = -1;
		UINT	l_uiSize = (UINT)this->GetTimeList().size();
		if( !l_uiSize )
			return;
		float	l_fEndTime = this->GetTimeList()[l_uiSize-1];
		//ensure start time
		if(this->GetStartTime()>0.000001f)
			return;
		//it's over start time and end time
		if( this->GetStartTime()+l_fEndTime<=0.0000001f )
		{//it's loop so we have to loop it,current time minus it's own play time and play it
			if( !this->IsAnimationLoop() )
				return;
		}
		GetPositionByTime(0.00000001f);
		Vector4 l_vOriginalColor = m_pCurrentPointData->vColor;
		l_iLastImageIndex = m_pCurrentPointData->iImageIndex;
		if( l_iLastImageIndex != -1 )
		{
			if( e_bBlending )
				m_pCurrentPointData->vColor = e_vColor;
			m_pCurrentPointData->pPI->ApplyImage();
			//sPuzzleData*l_pPuzzleData = this->m_ppPuzzleData[this->GetCurrentImageIndex()];
			sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[l_iLastImageIndex];
			Vector3	l_vPos = this->GetPos();
			RenderPuzzleData(l_pPuzzleData,l_vPos,m_vAngle);
			if( e_bBlending )
				m_pCurrentPointData->vColor = l_vOriginalColor;
		}	
	}

	Vector3 cCueToStartCurveWithTime::GetPositionByTime(float e_fGlobalTime)
	{
		if( m_fPastTime != e_fGlobalTime )
		{
			m_fPastTime = e_fGlobalTime;
			float	l_fPastTime = e_fGlobalTime-m_fStartTime;
			if( l_fPastTime >= 0.f )
			{
				this->m_vCurrentPosition = cCurveWithTime::GetPositionByTime(l_fPastTime);
				CalculateCurrentPointIndex();
				UpdateData();
			}
		}
		return m_vCurrentPosition;
	}

	void    cCueToStartCurveWithTime::RenderCollide()
	{
		if( !m_pCurrentPointData || m_pCurrentPointData->iImageIndex == -1 )
			return;
		//l_vPos.x += this->GetPos().x+m_pCurrentPointData->Size.x/2.f;
		//l_vPos.y += this->GetPos().y+m_pCurrentPointData->Size.y/2.f;
		//cMatrix44	l_mat = cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::RotationMatrix(m_pCurrentPointData->vAngle)*cMatrix44::ScaleMatrix(Vector3(m_pCurrentPointData->Size.x,m_pCurrentPointData->Size.y,1.f));
		//RenderRectangle(1,1,l_mat,m_pCurrentPointData->vColor);
		sPuzzleData*l_p = m_pCurrentPointData->pPI->GetPuzzleData(m_pCurrentPointData->iImageIndex);
		Vector3	l_vAngle = m_pCurrentPointData->vAngle+this->m_vAngle;
		GLRender::RenderRectangle(this->m_vCurrentPosition+GetPos()+this->m_vRotationAnglePosOffset,
			(float)l_p->OriginalSize.x,
			(float)l_p->OriginalSize.y,
			m_pCurrentPointData->vColor,
			l_vAngle);
	}
	//<SubMPDI MPDIList="" MPDI="" SubMPDI=""/>
	cCueToStartCurveWithTime*		cCueToStartCurveWithTime::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"MPDI");
		cSubMPDI*l_pSubMPDI = 0;
		cSubMPDI*l_pCloneSubMPDI = 0;
		cMPDI*l_pMPDI = 0;
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
			else
			COMPARE_NAME("SubMPDI")
			{
				if( l_pMPDI )
					l_pSubMPDI = l_pMPDI->GetObject(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if(l_pSubMPDI)
			l_pCloneSubMPDI = dynamic_cast<cSubMPDI*>(l_pSubMPDI->Clone());
		return l_pCloneSubMPDI;
	}
	//end namespace FATMING_CORE
}