#include "stdafx.h"
#include "BluffingDiceGame.h"
#include "DiceTable.h"
#include "BluffingDiceAI.h"
#include "SpecialDiceRule.h"

int	g_iSpecialDiceRulePoint = 0;

cBluffingDiceGame::cBluffingDiceGame(char*e_strGameSetupFileName)
{
	m_eCatchType = eCT_NO_CATCH;
	m_iWinIndex = -1;
	m_strFileName = e_strGameSetupFileName;
	m_iCallDicePoint = 0;
	m_iCallDiceCountIndex = 0;
	m_pDiceRule = 0;
	m_pBluffingDiceAI = 0;
	m_iTurnToCallIndex = 0;
	m_pCallImageButton = 0;
	m_pCatchImageButton = 0;
	m_pPlayerDiceTable = 0;
	m_pDiceImagePI = 0;
	m_pCallClickLegalSound = 0;
	m_pCallClickInlegalSound = 0;
	m_pPlayerCallSubMPDI = 0;
	m_pAICallSubMPDI = 0;
	m_pAICatchSubMPDI = 0;
	m_pPlayerCatchSubMPDI = 0;
	m_pPlayerWin = 0;
	m_pAIWin = 0;
	m_pSpecialDiceRule = 0;
	m_pFailedMenuButtonList = 0;
}
cBluffingDiceGame::~cBluffingDiceGame()
{
	Destroy();
}

void	cBluffingDiceGame::Destroy()
{
	SAFE_DELETE(m_CallDiceCountBehavior.pImage);
	SAFE_DELETE(m_pFailedMenuButtonList);
	SAFE_DELETE(m_pCallImageButton);
	SAFE_DELETE(m_pCatchImageButton);
	SAFE_DELETE(m_pBluffingDiceAI);
	SAFE_DELETE(m_pDiceRule);
	SAFE_DELETE(m_pPlayerDiceTable);
	SAFE_DELETE(m_pSpecialDiceRule);
	DELETE_VECTOR(m_AIDiceTableCallResultVector,cAIDiceTableCallResult*);
	m_ResourceStamp.ReleaseNewResource();
}
//<Root>
//	<DiceRule Min="1" Max="7" OneDiceRule="1" PercentForSpecialNumeral1="0.1" NumDice="5"/>
//	<DiceTableData FileName="">
//	<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
//</Root>
void	cBluffingDiceGame::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("DiceTableData")
	{
		ProcessDiceTableData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("AIData")
	{
		ProcessDiceAIData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("DiceRule")
	{
		ProcessDiceRule(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ImageData")
	{
		ProcessImageData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SoundData")
	{
		ProcessSoundData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialDiceRule")
	{
		m_pSpecialDiceRule = new cSpecialDiceRule(e_pTiXmlElement);
		g_iSpecialDiceRulePoint = m_pSpecialDiceRule->m_iSpecialDicePoint;
	}
}

void	cBluffingDiceGame::ProcessDiceRule(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pDiceRule == 0);
	m_pDiceRule = new sDiceRule(e_pTiXmlElement);
}
//<Root>
//	<DiceTableData Type="Player">
//		<DiceTable PI="Image/DiceImage.pi" Dice1="last_result 1" Dice2="last_result 2" Dice3="last_result 3" Dice4="last_result 4" Dice5="last_result 5" Dice6="last_result 5" BlockDice="Black" NumeralImage="Probability_0,Probability_9">
//			<Data DicePos="100,800" DiceImageGap="39" NumeralPos="100,840" NumeralImageGap="39" />
//		</DiceTable>
//	</DiceTableData>
//	<DiceTableData Type="AI">
//		<DiceTable PI="Image/DiceImage.pi" Dice1="last_result 1" Dice2="last_result 2" Dice3="last_result 3" Dice4="last_result 4" Dice5="last_result 5" Dice6="last_result 5" BlockDice="Black" >
//			<Data 	DicePos="100,400" DiceImageGap="39" NumeralPos="100,840" NumeralImageGap="39" />
//		</DiceTable>
//	</DiceTableData>
//</Root>
void	cBluffingDiceGame::ProcessDiceTableData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pBluffingDiceAI);
	//<DiceTableData FileName="">
	const WCHAR*l_strFileName = e_pTiXmlElement->Attribute(L"FileName");
	if( l_strFileName )
	{
		cNodeISAX	l_NodeISAX;
		if(l_NodeISAX.ParseDataIntoXMLNode(l_strFileName))
		{
			assert(m_pDiceRule && "dice rule please put at first child element");
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			if(l_pTiXmlElement)
			{
				l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
				int	l_iDiceTableIndex = 0;
				while( l_pTiXmlElement )
				{
					if( !wcscmp(l_pTiXmlElement->Value(),L"DiceTableData") )
					{
						const WCHAR*l_strType = l_pTiXmlElement->Attribute(L"Type");
						if( l_strType &&!wcscmp(l_strType,L"Player") )
						{
							assert( m_pPlayerDiceTable == 0 &&"player data exists");
							m_pPlayerDiceTable = new cDiceTable(l_pTiXmlElement->FirstChildElement(),m_pDiceRule);
							m_pPlayerDiceTable->SetReveal(true);
						}
						else
						{
							cAIDiceTableCallResult*l_pAIDiceTableCallResult = new cAIDiceTableCallResult(this->m_pBluffingDiceAI,l_pTiXmlElement->FirstChildElement(),l_iDiceTableIndex);
							//l_pAIDiceTableCallResult->GetDiceTable()->SetReveal(true);
							m_AIDiceTableCallResultVector.push_back(l_pAIDiceTableCallResult);
						}
						++l_iDiceTableIndex;
					}
					l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				}
				std::vector<cDiceTable*>	l_AllDiceTable;
				l_AllDiceTable.push_back(m_pPlayerDiceTable);
				for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
					l_AllDiceTable.push_back(m_AIDiceTableCallResultVector[i]->GetDiceTable());
				m_pBluffingDiceAI->AssignTableData(&l_AllDiceTable);
			}
		}
	}
	else
	{
		UT::ErrorMsg(L"ProcessDiceTableData error",L"no FileName attribute");
	}
}

void	cBluffingDiceGame::ProcessDiceAIData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pBluffingDiceAI==0);
	assert(m_pDiceRule);
	m_pBluffingDiceAI = new cBluffingDiceAI(e_pTiXmlElement,m_pDiceRule);
}
//CallLegalSound="CallClick.wav" CallInlegalSound="CallClickInlegal.wav"
void	cBluffingDiceGame::ProcessSoundData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("CallLegalSound")
		{
			m_pCallClickLegalSound = cGameApp::GetSoundByFileName(l_strValue);
			
		}
		else
		COMPARE_NAME("CallInlegalSound")
		{
			m_pCallClickInlegalSound =  cGameApp::GetSoundByFileName(l_strValue);;
		}
	PARSE_NAME_VALUE_END
}
//<ImageData PI="Image/Dice/Dice.pi" CallDicePointImagePrefixName="Probability board_point" CallDiceCountImagePrefixName="congratulation_"/>
void	cBluffingDiceGame::ProcessImageData(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		if(!wcscmp(e_pTiXmlElement->Value(),L"DiceResult"))
		{
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PI")
				{
					m_pDiceImagePI = cGameApp::GetPuzzleImageByFileName(l_strValue);
				}
				else
				COMPARE_NAME("CallDicePointImagePrefixName")
				{
					m_CallDicePointBehavior.m_strImagePrefixName = l_strValue;
				}
				else
				COMPARE_NAME("CallDiceCountImagePrefixName")
				{
					m_CallDiceCountBehavior.m_strImagePrefixName = l_strValue;
					std::wstring	l_strNumeralImage0 = l_strValue;l_strNumeralImage0 += ValueToStringW(0);
					std::wstring	l_strNumeralImage9 = l_strValue;l_strNumeralImage9 += ValueToStringW(9);
					cPuzzleImageUnit*l_pPIUnit0 = m_pDiceImagePI->GetObject(l_strNumeralImage0);
					cPuzzleImageUnit*l_pPIUnit9 = m_pDiceImagePI->GetObject(l_strNumeralImage9);
					m_CallDiceCountBehavior.pImage = new cNumeralImage(l_pPIUnit0,l_pPIUnit9);
				}
				else
				COMPARE_NAME("CallDicePointImagePos")
				{
					m_CallDicePointBehavior.vImagePos = VALUE_TO_VECTOR3;
				}
				else
				COMPARE_NAME("CallDiceCountImagePos")
				{
					m_CallDiceCountBehavior.vImagePos = VALUE_TO_VECTOR3;
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"CallHint"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					l_pMPDI = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("PlayerCall")
				{
					m_pPlayerCallSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pPlayerCallSubMPDI )
						m_pPlayerCallSubMPDI->SetAnimationDone(true);
				}
				else
				COMPARE_NAME("AICall")
				{
					m_pAICallSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"CatchHint"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					l_pMPDI = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("PlayerCatch")
				{
					m_pPlayerCatchSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pPlayerCatchSubMPDI )
						m_pPlayerCatchSubMPDI->SetAnimationDone(true);
				}
				else
				COMPARE_NAME("AICatch")
				{
					m_pAICatchSubMPDI = l_pMPDI->GetObject(l_strValue);
					if( m_pAICatchSubMPDI )
						m_pAICatchSubMPDI->SetAnimationDone(true);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"CatchResult"))
		{
			cMPDIList*l_pMPDILIst = 0;
			cMPDI*l_pMPDI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MPDIList")
				{
					l_pMPDILIst  =cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("WinMPDI")
				{
					m_pPlayerWin = l_pMPDILIst->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("LoseMPDI")
				{
					m_pAIWin = l_pMPDILIst->GetObject(l_strValue);
				}
			PARSE_NAME_VALUE_END
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"FailedMenu"))
		{
			m_pFailedMenuButtonList = GetClickMouseBehaviorList(e_pTiXmlElement->FirstChildElement());
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"cImageButton"))
		{//<ImageButton PI="Image/Dice/Dice.pi" CallButton="Call" CallPos="500,500" CatchButton="Open" CatchPos="500,550" />
			cImageButton*l_pImageButton = cImageButton::GetMe(e_pTiXmlElement);
			if( !l_pImageButton )
				return;
			if( l_pImageButton->IsSameName(L"Open") )
				m_pCatchImageButton = l_pImageButton;
			else
			if( l_pImageButton->IsSameName(L"Call") )
				m_pCallImageButton = l_pImageButton;
			else
			{
				UT::ErrorMsg(L"this button is not using",l_pImageButton->GetName());
				SAFE_DELETE(l_pImageButton);
			}
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}


void	cBluffingDiceGame::Init()
{
	this->Destroy();
	m_ResourceStamp.StampResource1();
	//
	m_CallDicePointBehavior.Init();
	m_CallDiceCountBehavior.Init();
	m_iCallDicePoint = 0;
	m_iCallDiceCountIndex = 0;
	m_eCatchType = eCT_NO_CATCH;
	if(!this->Parse(m_strFileName.c_str()))
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!");
	m_pPlayerDiceTable->Init();
	//
	std::vector<int> l_test;
	for( int i=0;i<5;++i )
		l_test.push_back(0);
	//m_pPlayerDiceTable->SetDiceContent(l_test);
	//
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
		m_AIDiceTableCallResultVector[i]->Init();
	if( m_pCallImageButton )
		m_pCallImageButton->Init();
	if( m_pCatchImageButton )
		m_pCatchImageButton->Init();
	if( m_pPlayerWin )
		m_pPlayerWin->Init();
	if( m_pAIWin )
		m_pAIWin->Init();
	if( m_pFailedMenuButtonList )
		m_pFailedMenuButtonList->Init();
	//
	m_ResourceStamp.StampResource2();
}

void	cBluffingDiceGame::DiceCallUpdate(float e_fElpaseTime)
{
	if( m_iTurnToCallIndex == 0 )
	{
		if( m_pCallImageButton )
		{
			m_pCallImageButton->Update(e_fElpaseTime);
			if( m_pCallImageButton->IsSatisfiedCondition() )
			{
				m_pCallImageButton->Init();
				if( m_pPlayerDiceTable->IsConditionOk())
				{
					if( m_pPlayerCallSubMPDI )
					{
						m_pPlayerCallSubMPDI->Init();
						m_pPlayerCallSubMPDI->Update(0.001f);
					}
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
					if( m_pCallClickLegalSound )
						m_pCallClickLegalSound->Play(true);
					++m_iTurnToCallIndex;
					if( m_pPlayerDiceTable->GetCurrentSelectDice() == m_pSpecialDiceRule->m_iSpecialDicePoint )
						this->m_pSpecialDiceRule->m_bWorking = false;
					CallDiceImageUpdate(m_pPlayerDiceTable->GetCurrentSelectNumeral(),m_pPlayerDiceTable->GetCurrentSelectDice());
				}
				else
				{
					if( m_pCallClickInlegalSound )
						m_pCallClickInlegalSound->Play(true);
				}
			}
		}
		if( m_pCatchImageButton )
		{
			m_pCatchImageButton->Update(e_fElpaseTime);
			if( m_pCatchImageButton->IsSatisfiedCondition() )
			{
				m_pCatchImageButton->Init();
				if( m_iCallDiceCountIndex != 0 )
				{
					if( m_pCallClickLegalSound )
						m_pCallClickLegalSound->Play(true);
					if( m_pAICallSubMPDI )
						m_pAICallSubMPDI->SetAnimationDone(true);
					if( m_pPlayerCallSubMPDI )
						m_pPlayerCallSubMPDI->SetAnimationDone(true);
					m_eCatchType = eCT_PLAYER_CATCH;
					m_pPlayerCatchSubMPDI->Init();
					m_pPlayerCatchSubMPDI->Update(0.001f);
					if( IsCatchWin() )
					{
						m_iWinIndex = 0;
					}
					else
					{
						m_iWinIndex = 1;
					}
				}
				else
				{
					if( m_pCallClickInlegalSound )
						m_pCallClickInlegalSound->Play(true);
				}
			}
		}
	}
	else
	{
		cAIDiceTableCallResult*l_pAIDiceTableCallResult = m_AIDiceTableCallResultVector[m_iTurnToCallIndex-1];
		l_pAIDiceTableCallResult->AIUpdate(m_iCallDiceCountIndex,m_iCallDicePoint,m_pSpecialDiceRule->m_bWorking);
		if(!l_pAIDiceTableCallResult->IsWaitForAI())
		{
			if(l_pAIDiceTableCallResult->IsCatch())
			{
				m_pAICatchSubMPDI->Init();
				m_pAICatchSubMPDI->Update(0.001f);
				if( m_pAICallSubMPDI )
					m_pAICallSubMPDI->SetAnimationDone(true);
				if( m_pPlayerCallSubMPDI )
					m_pPlayerCallSubMPDI->SetAnimationDone(true);
				m_eCatchType = eCT_AI_CATCH;
				if( IsCatchWin() )
					m_iWinIndex = m_iTurnToCallIndex;
				else
					m_iWinIndex = 0;
			}
			else
			{
				if( m_pAICallSubMPDI )
				{
					m_pAICallSubMPDI->Init();
					m_pAICallSubMPDI->Update(0.001f);
				}
				if( m_pPlayerCallSubMPDI )
					m_pPlayerCallSubMPDI->SetAnimationDone(true);
				CallDiceImageUpdate(m_pBluffingDiceAI->m_iDiceCountCallResult,m_pBluffingDiceAI->m_iDicePointCallResult);
				if( m_pBluffingDiceAI->m_iDicePointCallResult == m_pSpecialDiceRule->m_iSpecialDicePoint )
						this->m_pSpecialDiceRule->m_bWorking = false;
			}
			++m_iTurnToCallIndex;
			//now only 1 AI to avoid AI catch AI.
			//if( m_iTurnToCallIndex > (int)m_AIDiceTableCallResultVector.size() )
			{
				m_iTurnToCallIndex = 0;
			}
			l_pAIDiceTableCallResult->IsCall();
		}
	}
}

void	cBluffingDiceGame::CallDiceImageUpdate(int e_iCount,int e_iPoint)
{
	if( m_pDiceImagePI )
	{
		std::wstring	l_strPointImageName = m_CallDicePointBehavior.m_strImagePrefixName;
		std::wstring	l_strCountImageName = m_CallDiceCountBehavior.m_strImagePrefixName;
		m_iCallDiceCountIndex = e_iCount;
		m_iCallDicePoint = e_iPoint;
		l_strPointImageName += ValueToStringW(m_iCallDicePoint+1);
		l_strCountImageName += ValueToStringW(m_iCallDiceCountIndex);
		m_CallDicePointBehavior.pImage = m_pDiceImagePI->GetObject(l_strPointImageName.c_str());
		cNumeralImage*l_pNumeralImage = (cNumeralImage*)m_CallDiceCountBehavior.pImage;
		l_pNumeralImage->SetValue(m_iCallDiceCountIndex);
//		m_CallDiceCountBehavior.pImage = m_pDiceImagePI->GetObject(l_strCountImageName.c_str());
		m_CallDicePointBehavior.pImage->SetPos(m_CallDicePointBehavior.vImagePos);
		m_CallDiceCountBehavior.pImage->SetPos(m_CallDiceCountBehavior.vImagePos);
	}
}

void	cBluffingDiceGame::DiceCatchUpdate(float e_fElpaseTime)
{
	bool	l_bWaitCatchHintDone = false;
	if( m_eCatchType == eCT_AI_CATCH )
	{//play video phase
		if( !this->m_pAICatchSubMPDI->IsAnimationDone() )
			this->m_pAICatchSubMPDI->Update(e_fElpaseTime);
		else
		{
			l_bWaitCatchHintDone  =true;
		}
	}
	else
	if( m_eCatchType == eCT_PLAYER_CATCH )
	{//retry again menu or leave
		if( m_pPlayerCatchSubMPDI->IsAnimationDone() )
			m_pPlayerCatchSubMPDI->Update(e_fElpaseTime);
		else
		{
			l_bWaitCatchHintDone  =true;
		}
	}
	if( l_bWaitCatchHintDone )
	{
		if( m_iWinIndex == 0 )
		{//player
			if( m_pPlayerWin && !m_pPlayerWin->IsAnimationDone() )
				m_pPlayerWin->Update(e_fElpaseTime);
		}
		else
		{
			if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
				m_pAIWin->Update(e_fElpaseTime);
			else
			{
				if( m_pFailedMenuButtonList )
				{
					m_pFailedMenuButtonList->Update(e_fElpaseTime);
					if( m_pFailedMenuButtonList->IsSatisfiedCondition() )
					{
					
					}
				}
			}
		}
	}
}

void	cBluffingDiceGame::Update(float e_fElpaseTime)
{
	m_pPlayerDiceTable->Update(e_fElpaseTime,m_iCallDicePoint,m_iCallDiceCountIndex,1+this->m_AIDiceTableCallResultVector.size());
	if( m_eCatchType == eCT_NO_CATCH )
		DiceCallUpdate(e_fElpaseTime);
	else
		DiceCatchUpdate(e_fElpaseTime);
	static bool l_bTest = false;
	if( !l_bTest && cGameApp::m_sucKeyData[17] )
	{
		l_bTest = true;
	}
	if( l_bTest && !cGameApp::m_sucKeyData[17] )
	{
		this->Init();
		l_bTest = false;
	}

								//l_pAIDiceTableCallResult->GetDiceTable()->SetReveal(true);
}

void	cBluffingDiceGame::Render()
{
	m_pPlayerDiceTable->Render();
	if( m_pCallImageButton )
		m_pCallImageButton->Render();
	if( m_pCatchImageButton )
		m_pCatchImageButton->Render();
	if( m_CallDicePointBehavior.pImage )
		m_CallDicePointBehavior.pImage->Render();
	if( m_CallDiceCountBehavior.pImage )
	{
		cNumeralImage*l_pNumeralImage = (cNumeralImage*)m_CallDiceCountBehavior.pImage;
		if( l_pNumeralImage->GetValue() > 0 )
			m_CallDiceCountBehavior.pImage->Render();
	}
	if( m_pAICallSubMPDI )
		m_pAICallSubMPDI->Render();
	if( m_pPlayerCallSubMPDI )
		m_pPlayerCallSubMPDI->Render();
	if( m_pAICatchSubMPDI )
		m_pAICatchSubMPDI->Render();
	if( m_pPlayerCatchSubMPDI )
		m_pPlayerCatchSubMPDI->Render();
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		m_AIDiceTableCallResultVector[i]->Render();
	}
	if( m_pSpecialDiceRule )
		m_pSpecialDiceRule->Render();
	if( m_iWinIndex == 0 )
	{//player
		m_pPlayerWin->Render();
	}
	else
	if( m_iWinIndex == 1 )
	{
		if( m_pAIWin && !m_pAIWin->IsAnimationDone() )
			m_pAIWin->Render();
		else
		{
			if( m_pFailedMenuButtonList )
			{
				m_pFailedMenuButtonList->Render();
				m_pFailedMenuButtonList->DebugRender();
			}
		}
	}
}

void	cBluffingDiceGame::DebugRender()
{

}
void	cBluffingDiceGame::MouseDown(int e_iX,int e_iY)
{
	if( m_eCatchType == eCT_NO_CATCH )
	{
		if( m_iTurnToCallIndex == 0 )
		{
			m_pPlayerDiceTable->MouseDown(e_iX,e_iY);
		}
		if(m_pCatchImageButton)
			m_pCatchImageButton->MouseDown(e_iX,e_iY);
		if( m_pCallImageButton )
			m_pCallImageButton->MouseDown(e_iX,e_iY);
	}
	else
	{
		if( m_iWinIndex == 1 )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseDown(e_iX,e_iY);
			}
		}
	}
}

void	cBluffingDiceGame::MouseMove(int e_iX,int e_iY)
{
	if( m_eCatchType == eCT_NO_CATCH )
	{
		if( m_iTurnToCallIndex == 0 )
		{
			m_pPlayerDiceTable->MouseMove(e_iX,e_iY);
		}
		if(m_pCatchImageButton)
			m_pCatchImageButton->MouseMove(e_iX,e_iY);
		if( m_pCallImageButton )
			m_pCallImageButton->MouseMove(e_iX,e_iY);
	}
	else
	{
		if( m_iWinIndex == 1 )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseMove(e_iX,e_iY);
			}
		}	
	}
}

void	cBluffingDiceGame::MouseUp(int e_iX,int e_iY)
{
	if( m_eCatchType == eCT_NO_CATCH )
	{
		if( m_iTurnToCallIndex == 0 )
		{
			m_pPlayerDiceTable->MouseUp(e_iX,e_iY);
		}
		if(m_pCatchImageButton)
			m_pCatchImageButton->MouseUp(e_iX,e_iY);
		if( m_pCallImageButton )
			m_pCallImageButton->MouseUp(e_iX,e_iY);
	}
	else
	{
		if( m_iWinIndex == 1 )
		{
			if( m_pAIWin && m_pAIWin->IsAnimationDone() )
			{
				if( m_pFailedMenuButtonList )
					m_pFailedMenuButtonList->MouseUp(e_iX,e_iY);
			}
		}	
	}
}

const   WCHAR*	cBluffingDiceGame::GetNextPhaseName()
{
	return 0;
}

bool	cBluffingDiceGame::IsCatchWin()
{
	for(size_t i=0;i<m_AIDiceTableCallResultVector.size();++i)
	{
		m_AIDiceTableCallResultVector[i]->GetDiceTable()->SetReveal(true);
	}
	return this->m_pBluffingDiceAI->IsCatchWin(m_iCallDiceCountIndex,m_iCallDicePoint,this->m_pSpecialDiceRule->m_bWorking);
}