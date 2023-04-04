#include "stdafx.h"
#include "FishGroup.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"
#include "../ProbabilityFish/FishProbability.h"
//
namespace FATMING_CORE
{
	extern bool	g_bSkipImageLoad;
}

cFishGroup::cFishGroup(  )
{
	//moving firection offset
	m_fGroupShowOffsetTime = 1.f;
	m_fDirectionOffsetX = Vector2(-0.5f,0.5f);
	m_fDirectionOffsetY = Vector2(-0.5f,0.5f);
}


cFishGroup::cFishGroup( int e_nId, const char* e_strFishGroupMPDI )
{
	//moving firection offset
	m_fGroupShowOffsetTime = 1.f;
	m_fDirectionOffsetX = Vector2(-0.5f,0.5f);
	m_fDirectionOffsetY = Vector2(-0.5f,0.5f);
	if( !cFishApp::m_spMonsterManager )
		return;
	g_bSkipImageLoad = true;
	cAnimationParser	l_AnimationParser;
	if(l_AnimationParser.Parse(e_strFishGroupMPDI))
	{
		cMPDIList*l_pMPDIlist = l_AnimationParser.GetMPDIList(0);
	
		//path, Winning02.mpdi -> index = 0,1, 2
		for( int i=0; i<l_pMPDIlist->Count(); ++i )
		{
			cMPDI*l_pMPDI = l_pMPDIlist->GetObject(i);
			cFishGroupPointData*l_pFishGroupPointData = new cFishGroupPointData();
			int l_nCount2 = l_pMPDI->Count();
			for( int j=0;j<l_nCount2;++j )
			{

				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = l_pSubMPDI->GetPointDataList();
				if( l_pPointList->size() )
				{
					//const WCHAR*l_strFishName = (*l_pPointList)[0]->pPI->GetObject((*l_pPointList)[0]->iImageIndex)->GetName();
					const WCHAR*l_strFishName = (*l_pPointList)[0]->pPI->GetObject((*l_pPointList)[0]->iImageIndex)->GetName();
					int	l_iFishIndex = cFishApp::m_spMonsterManager->m_AllFish.GetObjectIndexByName(l_strFishName);
					if( l_iFishIndex == -1 )
					{
						//UT::ErrorMsg(l_strFishName,L"there is no such fish name!");
						l_iFishIndex = 0;
					}
					cFishGroupPointData::sFishPosList l_FishPos;
					l_FishPos.iFishIndex = l_iFishIndex;
					l_pFishGroupPointData->m_FishGroupData.push_back(l_FishPos);
				}
				const WCHAR*l_str = l_pSubMPDI->GetName();

				//cCurveWithTime	*l_pCurveWithTime = new cCurveWithTime(l_pSubMPDI);
				//l_pCurveWithTime->SetCurveLoop(false);
				//l_pFishGroupPointData->vCurveList.push_back( *l_pCurveWithTime );
				//SAFE_DELETE(l_pCurveWithTime);
				cCurveWithTime	l_CurveWithTime(l_pSubMPDI);
				l_CurveWithTime.SetCurveLoop(false);
				l_pFishGroupPointData->vCurveList.push_back( l_CurveWithTime );
			}
//int abc = l_pFishGroupPointData->vCurveList.size();
			m_FishGroup.AddObjectNeglectExist( l_pFishGroupPointData );
		}
		
	}
	else
	{
		UT::ErrorMsg(e_strFishGroupMPDI,"file parse failed");
	}
	//
	l_AnimationParser.Destroy();
	g_bSkipImageLoad = false;
}

void	cFishGroup::ProcessData( const char* e_strFishGroupMPDI )
{
	g_bSkipImageLoad = true;
	cAnimationParser	l_AnimationParser;
	if(l_AnimationParser.Parse(e_strFishGroupMPDI))
	{
		cMPDIList*l_pMPDIlist = l_AnimationParser.GetMPDIList(0);
		for( int i=0;i<l_pMPDIlist->Count();++i )
		{
			cMPDI*l_pMPDI = l_pMPDIlist->GetObject(i);
			cFishGroupPointData*l_pFishGroupPointData = new cFishGroupPointData();
			l_pFishGroupPointData->m_vDrawSize = l_pMPDI->GetDrawSize();
			for( int j=0;j<l_pMPDI->Count();++j )
			{
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				//
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = l_pSubMPDI->GetPointDataList();
				if( l_pPointList->size() )
				{
					const WCHAR*l_strFishName = (*l_pPointList)[0]->pPI->GetObject((*l_pPointList)[0]->iImageIndex)->GetName();
					int	l_iFishIndex = cFishApp::m_spMonsterManager->GetObjectIndexByName(l_strFishName);
					if( l_iFishIndex == -1 )
					{
						UT::ErrorMsg(l_strFishName,L"there is no such fish name!");
					}
					cFishGroupPointData::sFishPosList l_FishPos;
					l_FishPos.iFishIndex = l_iFishIndex;
					std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointDataList = l_pSubMPDI->GetPointDataList();
					l_FishPos.vPosList = l_pSubMPDI->GetOriginalPointList();
					for(size_t i = 0;i<l_pPointDataList->size();++i)
					{
						Vector2	l_vSize = (*l_pPointDataList)[i]->Size/2.f;
						l_FishPos.vPosList[i].x += l_vSize.x;
						l_FishPos.vPosList[i].y += l_vSize.y;
					}
					l_pFishGroupPointData->m_FishGroupData.push_back(l_FishPos);
				}
			}
			m_FishGroup.AddObjectNeglectExist(l_pFishGroupPointData);
		}
	}
	else
	{
		UT::ErrorMsg(e_strFishGroupMPDI,"file parse failed");
	}
	l_AnimationParser.Destroy();
	g_bSkipImageLoad = false;
}








cFishGroup::~cFishGroup()
{
}








void	cFishGroup::FishGroupGoUpToDown(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(0.f,-cGameApp::m_spOpenGLRender->m_vGameResolution.y,0.f);
	Vector3	l_vEndPos = Vector3(0.f,cGameApp::m_spOpenGLRender->m_vGameResolution.y,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}

void	cFishGroup::FishGroupGoDownToUp(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(0.f,cGameApp::m_spOpenGLRender->m_vGameResolution.y,0.f);
	Vector3	l_vEndPos = Vector3(0.f,-cGameApp::m_spOpenGLRender->m_vGameResolution.y,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}

void	cFishGroup::FishGroupGoLeftToRight(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(-cGameApp::m_spOpenGLRender->m_vGameResolution.x,0.f,0.f);
	Vector3	l_vEndPos = Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x,0.f,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}








void	cFishGroup::FishGroupGRightTooLeft(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x,0.f,0.f);
	Vector3	l_vEndPos = Vector3(-cGameApp::m_spOpenGLRender->m_vGameResolution.x,0.f,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}

int		cFishGroup::FishGroupGoFishesCount(int e_iIndex)
{
	cFishGroup::cFishGroupPointData*l_pFishGroupPointData = this->m_FishGroup.GetObject(e_iIndex);
	int	l_iNumMonsterGo = 0;
	if( l_pFishGroupPointData )
	{
		size_t	l_uiSize =  l_pFishGroupPointData->m_FishGroupData.size();
		for( size_t	i=0;i<l_uiSize;++i )
		{
			cFishGroup::cFishGroupPointData::sFishPosList*l_pFishPosList = &l_pFishGroupPointData->m_FishGroupData[i];
			size_t	l_uiSize2 = l_pFishPosList->vPosList.size();
			l_iNumMonsterGo += (int)l_uiSize2;
		}
	}
	return l_iNumMonsterGo;
}







int	cFishGroup::FishGroupGo(int e_iIndex,cCurveWithTime e_TargetPath,bool e_bForceFetch)
{
	cFishGroup::cFishGroupPointData*l_pFishGroupPointData = this->m_FishGroup.GetObject(e_iIndex);
	Vector3	l_vStartPoint = e_TargetPath.GetOriginalPointList()[0];
	Vector3	l_vEndPoint = e_TargetPath.GetOriginalPointList()[1];
	float	l_fEndTime = e_TargetPath.GetOriginalTimeList()[1];
	int	l_iNumMonsterGo = 0;
	if( l_pFishGroupPointData )
	{
		size_t	l_uiSize =  l_pFishGroupPointData->m_FishGroupData.size();
		for( size_t	i=0;i<l_uiSize;++i )
		{
			cFishGroup::cFishGroupPointData::sFishPosList*l_pFishPosList = &l_pFishGroupPointData->m_FishGroupData[i];
			int	l_iFishId = l_pFishPosList->iFishIndex;
			size_t	l_uiSize2 = l_pFishPosList->vPosList.size();
			for( size_t	j=0;j<l_uiSize2;++j )
			{
				cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(l_iFishId,e_bForceFetch);

				if( !l_pMonster )
				{
					break;
				}
				cCurveWithTime	l_TranslatePath;
				Vector3	l_vPos = l_pFishPosList->vPosList[j];
				l_vPos = l_pFishPosList->vPosList[j]+l_vStartPoint;
				l_TranslatePath.AddPoint(l_vPos,0.f);
				l_vPos = l_pFishPosList->vPosList[j]+l_vEndPoint;
				l_TranslatePath.AddPoint(l_vPos,l_fEndTime);
				l_pMonster->SetPos(l_vPos);
				l_pMonster->Init();
				l_TranslatePath.SetCalAngle(true);
				l_pMonster->SetPath(&l_TranslatePath);
				++l_iNumMonsterGo;
			}
		}
	}
	return l_iNumMonsterGo;
}








int	cFishGroup::FishGroupGo(int e_iIndex,bool e_bForceFetch)
{
	cCurveWithTime	l_TargetPath;
	cFishGroup::cFishGroupPointData*l_pFishGroupPointData = this->m_FishGroup.GetObject(e_iIndex);
	Vector3	l_vFirstFishPos;
	Vector3	l_vFirstPointPos;
	int	l_iNumMonsterGo = 0;
	float	l_fTimeOffset = m_fGroupShowOffsetTime;
	if( l_pFishGroupPointData )
	{
		size_t	l_uiSize =  l_pFishGroupPointData->m_FishGroupData.size();
		for( size_t	i=0;i<l_uiSize;++i )
		{
			cFishGroup::cFishGroupPointData::sFishPosList*l_pFishPosList = &l_pFishGroupPointData->m_FishGroupData[i];
			int	l_iFishId = l_pFishPosList->iFishIndex;
			size_t	l_uiSize2 = l_pFishPosList->vPosList.size();
			for( size_t	j=0;j<l_uiSize2;++j )
			{
				cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(l_iFishId,e_bForceFetch);
				if( !l_pMonster )
					break;
				if( j == 0 && i==0 )
				{
					l_TargetPath = *l_pMonster->GetPath();
					l_vFirstFishPos = cFishApp::m_spProbabilityFish->GetRandomPos(l_pFishGroupPointData->m_vDrawSize.Length());
					l_vFirstPointPos = l_pFishPosList->vPosList[0];
				}
				cCurveWithTime	l_TranslatePath;
				l_TranslatePath = l_TargetPath.GetOffsetCurve(l_fTimeOffset*j+l_fTimeOffset,m_fDirectionOffsetX,m_fDirectionOffsetY,5);
				l_TranslatePath.SetCalAngle(true);
				Vector3	l_vPos = l_pFishPosList->vPosList[j];
				Vector3	l_vPosOffset = 	l_vPos-l_vFirstPointPos;
				l_vPos = l_vFirstFishPos+l_vPosOffset;
				//Vector3	l_vPos = l_pFishPosList->vPosList[j];
				l_TranslatePath.Translate(l_vPosOffset);
				l_TranslatePath.FixPoint(0,l_vPos,0.f);
				l_pMonster->SetPos(l_vPos);
				l_pMonster->Init();



				l_pMonster->SetPath(&l_TranslatePath);
				++l_iNumMonsterGo;
			}
		}
	}
	return l_iNumMonsterGo;
}

void	cFishGroup::f_PrizeII_Call( float e_fTime )
{
	Vector3	l_vStartPos = Vector3( cGameApp::m_spOpenGLRender->m_vGameResolution.x/3,	0.f,	0.f );
	Vector3	l_vEndPos	= Vector3( cGameApp::m_spOpenGLRender->m_vGameResolution.x/3,	0.f,	0.f );
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	//
	f_PrizeII_Go( l_TargetPath );
}

void	cFishGroup::f_PrizeII_Go( cCurveWithTime e_TargetPath )
{
	cCurveWithTime	l_TargetPath;
	Vector3	l_vFirstFishPos;
	Vector3	l_vFirstPointPos;

	for( size_t	i=0; i<(size_t)m_FishGroup.Count(); ++i ) //how many index in mpdi
	{

		cFishGroup::cFishGroupPointData* l_pFishGroupPointData1 = this->m_FishGroup.GetObject((int)i);
		size_t	l_uiSizeSubMPDI1 =  l_pFishGroupPointData1->vCurveList.size();

		size_t	l_uiSize2 = 0;
		int	l_iFishId = 0;
		l_uiSize2 = l_uiSizeSubMPDI1;

		for( size_t	j=0; j<l_uiSize2; ++j )
		{
			cFishGroup::cFishGroupPointData::sFishPosList* l_pFishPosList = 0;
			l_pFishPosList = &l_pFishGroupPointData1->m_FishGroupData[j];
			l_iFishId = l_pFishPosList->iFishIndex;
			cMonster* l_pMonster = cFishApp::m_spMonsterManager->Require( l_iFishId,true );
			if( !l_pMonster )
			{
				break;
			}
			//
			cCurveWithTime	l_TranslatePath;
			l_TranslatePath = l_pFishGroupPointData1->vCurveList[j];

//			l_pMonster->SetPos(l_vPos);
			l_pMonster->Init();
			l_TranslatePath.SetLOD(5);
			l_TranslatePath.SetCalAngle( true );
			l_pMonster->SetPath( &l_TranslatePath );
		}
	}
}