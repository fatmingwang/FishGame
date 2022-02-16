#include "stdafx.h"
#include "DiceTable.h"
#include "BluffingGameProbability.h"
//<DiceRule Min="1" Max="7" PercentForSpecialNumeral1="0.1" NumDice="5">

sDiceRule::sDiceRule(TiXmlElement*e_pelement)
{
	iNumDice = 5;
	iMin = 1;
	iMax = 6;
	fPercentForSpecialNumeral1 = 0.1f;
	iAllDiceCount = 10;
	PARSE_ELEMENT_START(e_pelement)
		COMPARE_NAME("Min")
		{
			iMin = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Max")
		{
			iMax = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("PercentForSpecialNumeral1")
		{
			fPercentForSpecialNumeral1 = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("NumDice")
		{
			iNumDice = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("AllDiceCount")
		{
			iAllDiceCount = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}

//<DiceTable PI="Image/DiceImage.pi" Dice1="" Dice2="" Dice3="" Dice4="" Dice5="" Dice6="" BlockDice=""/>

cDiceTable::cDiceTable(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule)
{
	m_vMyDiceImagePos = Vector3::Zero;
	m_fMyDiceImageGap = 50.f;
	m_iCurrentSelectDiceCount = -1;;
	m_iCurrentSelectDicePoint = -1;
	m_pDiceRule = e_pDiceRule;
	m_pBlockImage = 0;
	m_pPI = 0;
	m_pSelectedNumeralImage = 0;
	m_pSelectedDiceImage = 0;
	m_bReveal = false;
	ProcessImageData(e_pElement);
	m_NumeralImageButton.SetReclickable(true);
	m_DiceImageButtonContainer.SetReclickable(true);
}

cDiceTable::~cDiceTable()
{
	m_NumeralImageButton.Destroy();
	m_DiceImageButtonContainer.Destroy();
}
//<DiceTable PI="Image/DiceImage.pi" DiceCount="6" Dice1="" Dice2="" Dice3="" Dice4="" Dice5="" Dice6="" BlockDice="" NumeralImage="Num0,Num9"/>
//	<Data Name="AI" 	DicePos="900,100" DiceImageGap="30" NumeralPos="" NumeralImageGap="" />
//</DiceTable>
void		cDiceTable::ProcessImageData(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("PI")
		{
			assert(m_pPI == 0);
			this->m_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			if( !m_pPI )
			{
				UT::ErrorMsg(l_strValue,L"not exists!");
			}
		}
		else
		COMPARE_NAME("BlockDice")
		{
			if( m_pPI )
				this->m_pBlockImage = m_pPI->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("DiceCount")
		{
			int	l_iCount = VALUE_TO_INT;
			for( int i=0;i<l_iCount;++i )
				m_DiceImageButtonContainer.AddObjectNeglectExist(0);
		}
		else
		COMPARE_NAME("NumeralImage")
		{
			for( int i=1;i<m_pDiceRule->iAllDiceCount+1;++i )
			{
				std::wstring	l_strNumerialImage = l_strValue;
				cAssembleImageButton*l_pAssembleImageButton = new cAssembleImageButton();
				cAssembleImageButton*l_pFinishButton = new cAssembleImageButton();
				if( i<10 )
				{
					l_strNumerialImage += ValueToStringW(i);
					cPuzzleImageUnit*l_pPIUnit = m_pPI->GetObject(l_strNumerialImage.c_str());
					cPuzzleImageUnit*l_pClone = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
					cPuzzleImageUnit*l_pClone2 = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
					l_pAssembleImageButton->AddObjectNeglectExist(l_pClone);
					l_pFinishButton->AddObjectNeglectExist(l_pClone2);
				}
				else
				{
					float	l_fGap = 0.f;
					std::wstring	l_strNum = ValueToStringW(i);
					int	l_iNumSize = l_strNum.length();
					for( int j=0;j<l_iNumSize;++j )
					{
						l_strNumerialImage = l_strValue;
						l_strNumerialImage += l_strNum[j];
						cPuzzleImageUnit*l_pPIUnit = m_pPI->GetObject(l_strNumerialImage.c_str());
						cPuzzleImageUnit*l_pClone = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
						cPuzzleImageUnit*l_pClone2 = dynamic_cast<cPuzzleImageUnit*>(l_pPIUnit->Clone());
						l_pAssembleImageButton->AddObjectNeglectExist(l_pClone);
						l_pFinishButton->AddObjectNeglectExist(l_pClone2);
						l_pClone->SetPos(Vector3(l_fGap,0,0));
						l_pClone2->SetPos(Vector3(l_fGap,0,0));
						l_fGap += *l_pPIUnit->GetWidth();
					}
				}
				l_pFinishButton->SetColor(Vector4::HalfDark);;
				l_pFinishButton->SetName(l_strNumerialImage.c_str());
				l_pAssembleImageButton->SetFinishImage(l_pFinishButton);
				SAFE_DELETE(l_pFinishButton);
				m_NumeralImageButton.AddObjectNeglectExist(l_pAssembleImageButton);
				//cImageButton*l_pImageButton = new cImageButton(l_pPIUnit,Vector3::Zero);
				//l_pPIUnit->SetColor(Vector4::HalfDark);
				//l_pImageButton->SetFinishImage(l_pPIUnit);
				//l_pPIUnit->SetColor(Vector4::One);
				//m_NumeralImageButton.AddObjectNeglectExist(l_pImageButton);
			}
		}
		else
		{
			if( l_strName[0] == 'D' &&
				l_strName[1] == 'i' &&
				l_strName[2] == 'c' &&
				l_strName[3] == 'e')
			{
				const WCHAR*l_strDiceValue = &l_strName[4];
				int	l_iDiceNumeral = GetInt(l_strDiceValue);
				if( m_pPI )
				{
					cPuzzleImageUnit*l_pDiceImage = m_pPI->GetObject(l_strValue);
					if( l_pDiceImage )
					{
						cImageButton*l_pImageButton = new cImageButton(l_pDiceImage,Vector3::Zero);
						m_DiceImageButtonContainer.ReplaceObject(l_iDiceNumeral-1,l_pImageButton);
						l_pDiceImage->SetColor(Vector4::HalfDark);
						l_pImageButton->SetFinishImage(l_pDiceImage);
						l_pDiceImage->SetColor(Vector4::One);
					}
				}
			}
		}
	TO_NEXT_VALUE
	ProcessPosData(e_pElement->FirstChildElement());
}
//<Data DicePos="100,800" DiceImageGap="59" NumeralPos="100,840" NumeralImageGap="39" MyDiceImagePrefixName="Probability board_point" MyDiceImagePos="100,900,0" MyDiceImageGap="50" />
void		cDiceTable::ProcessPosData(TiXmlElement*e_pElement)
{
	if( !e_pElement )
		return;
	Vector3	l_vDicePos = Vector3::Zero;
	float	l_fDiceGap = 0.f;
	Vector3	l_vNumeralPos = Vector3::Zero;
	float	l_fNimeralGap = 0.f;
	PARSE_ELEMENT_START(e_pElement)
		//COMPARE_NAME("Name")
		//{
		//}
		//else
		COMPARE_NAME("DicePos")
		{
			l_vDicePos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("DiceImageGap")
		{
			l_fDiceGap = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("NumeralPos")
		{
			l_vNumeralPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("NumeralImageGap")
		{
			l_fNimeralGap = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("MyDiceImagePrefixName")
		{
			m_strMyDiceImagePrefixName = l_strValue;
		}
		else
		COMPARE_NAME("MyDiceImagePos")
		{
			m_vMyDiceImagePos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("MyDiceImageGap")
		{
			m_fMyDiceImageGap = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	if( m_DiceImageButtonContainer.GetObject(0) )
	{
		int	l_iDiceImageWidth = *m_DiceImageButtonContainer.GetObject(0)->GetWidth();
		Vector4	l_vDiceCollideRange(l_vDicePos.x,l_vDicePos.y,l_vDicePos.x,l_vDicePos.y+*m_DiceImageButtonContainer.GetObject(0)->GetHeight());
		for( int i=0;i<m_DiceImageButtonContainer.Count();++i )
		{
			l_vDicePos.x += l_fDiceGap;
			m_DiceImageButtonContainer.GetObject(i)->SetPos(l_vDicePos);
		}
		l_vDiceCollideRange.z = l_vDicePos.x+l_iDiceImageWidth;
		m_DiceImageButtonContainer.SetCollisionRange(l_vDiceCollideRange);
	}
	if( m_NumeralImageButton.Count() )
	{
		int	l_iNumeralImageWidth = *m_NumeralImageButton.GetObject(0)->GetWidth();
		Vector4	l_vNumeralCollideRange(l_vNumeralPos.x,l_vNumeralPos.y,l_vNumeralPos.x,l_vNumeralPos.y+*m_NumeralImageButton.GetObject(0)->GetHeight());
		for( int i=0;i<m_NumeralImageButton.Count();++i )
		{
			m_NumeralImageButton.GetObject(i)->SetPos(l_vNumeralPos);
			l_vNumeralPos.x += l_fNimeralGap;
			if( i>=10 )
				l_vNumeralPos.x += l_fNimeralGap;
			if( i>=9 && (i+1)%10 == 0 )
			{
				l_vNumeralCollideRange.z = l_vNumeralPos.x;
				l_vNumeralPos.x -= 9*l_fNimeralGap;
				l_vNumeralPos.y += l_fNimeralGap;
				l_vNumeralCollideRange.w = l_vNumeralPos.y+l_fNimeralGap;
			}
		}
		if( m_NumeralImageButton.Count() <=10 )			
			l_vNumeralCollideRange.z = l_vNumeralPos.x+l_iNumeralImageWidth;
		m_NumeralImageButton.SetCollisionRange(l_vNumeralCollideRange);
	}
	
}

void		cDiceTable::Init()
{
	m_iCurrentSelectDiceCount = -1;;
	m_iCurrentSelectDicePoint = -1;
	m_DiceImageButtonContainer.Init();
	m_NumeralImageButton.Init();
	m_MyDiceContent.clear();
	m_MyDiceContent = GenerateDiceTable(m_pDiceRule->iNumDice,m_pDiceRule->iMax,m_pDiceRule->fPercentForSpecialNumeral1);
	GenerateDiceContentImages();
}

void		cDiceTable::Update(float e_fElpaseTime,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iTotalPlayer)
{
	m_DiceImageButtonContainer.Update(e_fElpaseTime);
	m_NumeralImageButton.Update(e_fElpaseTime);
	if( m_DiceImageButtonContainer.IsSatisfiedCondition() )
	{
		m_iCurrentSelectDicePoint = m_DiceImageButtonContainer.GetCurrentWorkingObjectIndex();
	}
	if( m_NumeralImageButton.IsSatisfiedCondition() )
	{
		m_iCurrentSelectDiceCount = m_NumeralImageButton.GetCurrentWorkingObjectIndex()+1;
	}
	if( !IsCallConditionOk(e_iCurrentDicePoint,e_iCurrentDiceCount,m_iCurrentSelectDicePoint,m_iCurrentSelectDiceCount,e_iTotalPlayer) )
	{
		m_iCurrentSelectDicePoint = -1;
		m_iCurrentSelectDiceCount = -1;
	}
}

void		cDiceTable::Render()
{
	if( m_bReveal )
	{
		m_MyDiceContentImage.Render();
		m_DiceImageButtonContainer.Render();
	}
	else
	if( m_pBlockImage )
	{
		int	l_iCount = m_MyDiceContentImage.Count();
		for(int i=0;i<l_iCount;++i)		
		{
			m_pBlockImage->SetPos(m_MyDiceContentImage[i]->GetPos());
			m_pBlockImage->Render();
		}
	}
	m_NumeralImageButton.Render();
	//m_NumeralImageButton.DebugRender();
}

void		cDiceTable::DebugRender()
{
	m_DiceImageButtonContainer.DebugRender();
	m_NumeralImageButton.DebugRender();
}

void		cDiceTable::MouseDown(int e_iX,int e_iY)
{
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseDown(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseDown(e_iX,e_iY);
}

void		cDiceTable::MouseMove(int e_iX,int e_iY)
{
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseMove(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseMove(e_iX,e_iY);
}

void		cDiceTable::MouseUp(int e_iX,int e_iY)
{
	if( !m_bReveal )
		return;
	if( m_NumeralImageButton.Collide(e_iX,e_iY) )
		m_NumeralImageButton.MouseUp(e_iX,e_iY);
	if( m_DiceImageButtonContainer.Collide(e_iX,e_iY) )
		m_DiceImageButtonContainer.MouseUp(e_iX,e_iY);
}

int			cDiceTable::GetCurrentSelectNumeral()
{
	return m_iCurrentSelectDiceCount;
}

int			cDiceTable::GetCurrentSelectDice()
{
	return m_iCurrentSelectDicePoint;
}

bool		cDiceTable::IsCallConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iNewDicePoint,int e_iNewDiceCount,int e_iTotalPlayer)
{
	if( e_iNewDiceCount > e_iCurrentDiceCount && e_iNewDiceCount <= m_pDiceRule->iNumDice*e_iTotalPlayer )
	{
		return true;
	}
	else
	if( e_iNewDiceCount == e_iCurrentDiceCount )
	{
		if( e_iNewDicePoint > e_iCurrentDicePoint  )
			return true;
	}
	//count is samller is not allowable
	return false;

}

bool		cDiceTable::IsConditionOk()
{
	if( m_iCurrentSelectDicePoint != -1 && m_iCurrentSelectDiceCount != -1 )
		return true;
	return false;
}

void		cDiceTable::GenerateDiceContentImages()
{
	m_MyDiceContentImage.Destroy();
	Vector3	l_vStartPos = m_vMyDiceImagePos;
	for( size_t i=0;i<m_MyDiceContent.size();++i )
	{
		std::wstring	l_strImageName = m_strMyDiceImagePrefixName;
		l_strImageName += ValueToStringW(m_MyDiceContent[i]+1);
		cPuzzleImageUnit*l_pPIUnit = this->m_pPI->GetObject(l_strImageName.c_str());
		if( l_pPIUnit )
		{
			cBaseImage*l_pImage = new cBaseImage(l_pPIUnit);
			l_pImage->SetPos(l_vStartPos);
			m_MyDiceContentImage.AddObjectNeglectExist(l_pImage);
		}
		l_vStartPos.x += m_fMyDiceImageGap;
	}
}

void		cDiceTable::SetDiceContent(std::vector<int> e_DiceContent)
{
	m_MyDiceContent = e_DiceContent;
	GenerateDiceContentImages();
}