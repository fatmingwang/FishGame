#include "stdafx.h"
#include "GameEffectBase.h"
#include "GameEffectScoreWithCoin.h"

cGameEffectScoreWithCoin::sCoinSpinAnimationData::sCoinSpinAnimationData()
{
	iPuzzleImageUnitIndex = -1;
	iCurrentIndex = 0;
	fCurrentTime = 0.f;
	bDone = false;
}

cGameEffectScoreWithCoin::sCoinSpinAnimationData::~sCoinSpinAnimationData()
{

}

void	cGameEffectScoreWithCoin::sCoinSpinAnimationData::SetData(Vector3 e_vStartPos,Vector3 e_vEndPos,float e_fSpeed,float e_fAngle)
{
	vCurrentPos = e_vStartPos;
	vMoveDirection = (e_vStartPos-e_vEndPos).Normalize();
	vCurrntPos = e_vStartPos;
	vEndPos = e_vEndPos;
	fSpeed = e_fSpeed;
	fAngle = e_fAngle;
	bDone = false;
}

void	cGameEffectScoreWithCoin::sCoinSpinAnimationData::Update(float e_fElpaseTime,std::vector<cImageIndexOfAnimation::sImageIndexAndTimeGap>*e_pAnimationData)
{
	float	l_fBeforeMoveDistanceLength = (vEndPos-vCurrntPos).Length();
	vCurrntPos += vMoveDirection*e_fElpaseTime*fSpeed;
	float	l_fAfterMoveDistanceLength = (vEndPos-vCurrntPos).Length();
	if( l_fAfterMoveDistanceLength <= l_fBeforeMoveDistanceLength )
	{
		this->bDone = true;
		return;
	}
	fCurrentTime += e_fElpaseTime;
	float	l_fTimeGap = (*e_pAnimationData)[iCurrentIndex].fTimeGap;
	if( fCurrentTime >= l_fTimeGap )
	{
		fCurrentTime -= l_fTimeGap;
		++iCurrentIndex;
		int	l_iSize = e_pAnimationData->size();
		if( iCurrentIndex >= l_iSize )
			iCurrentIndex = 0;
	}
	iPuzzleImageUnitIndex = (*e_pAnimationData)[iCurrentIndex].iIndex;
	
}

void	cGameEffectScoreWithCoin::sCoinSpinAnimationData::Render(cPuzzleImage*ePI)
{
	cPuzzleImageUnit*l_pPIUnit = ePI->GetObject(iPuzzleImageUnitIndex);
	if( l_pPIUnit )
	{
		//cMatrix44
		l_pPIUnit->SetAngle(fAngle);
		l_pPIUnit->SetPos(vCurrntPos);
		l_pPIUnit->Render();
	}
}

cGameEffectScoreWithCoin::cGameEffectScoreWithCoin(cNumeralImage*e_pNumerialImage,float e_fSpeedToDestination)
{
	m_fSpeedToDestination = e_fSpeedToDestination;
	this->m_pNumerialImage = e_pNumerialImage;
}

cGameEffectScoreWithCoin::~cGameEffectScoreWithCoin()
{
	SAFE_DELETE(m_pNumerialImage);
}
//Fish Name,player ID,Matrix
void				cGameEffectScoreWithCoin::SetData(void*e_pPlayerBehaviorBase)
{
	sGameEffectInputData*l_pGameEffectInputData = (sGameEffectInputData*)e_pPlayerBehaviorBase;
	//l_pGameEffectInputData->iPlayerID;
	//l_pGameEffectInputData->iValue;
	//l_pGameEffectInputData->DestinationMatrix;
	//l_pGameEffectInputData->strMonstrName;
	Vector3 l_vScale;
	Vector3 l_vRotation;
	Vector3 l_vTranslation;
	float inverted;
	l_pGameEffectInputData->ShowMatrix.Decompose(l_vScale,l_vRotation,l_vTranslation,inverted);
	sCoinSpinAnimationData	l_CoinSpinAnimationData;
	l_CoinSpinAnimationData.SetData(l_pGameEffectInputData->ShowMatrix.GetTranslation(),l_pGameEffectInputData->DestinationMatrix.GetTranslation(),m_fSpeedToDestination,D3DXToDegree(l_vRotation.z));
	m_CoinSpinAnimationDataList.push_back(l_CoinSpinAnimationData);
}

void				cGameEffectScoreWithCoin::Update(float e_fElpaseTime)
{

}