#include "stdafx.h"
#include "SlotProbabilityStatsticData.h"
#include "AllPhaseDefine.h"
#include "SlotBetTableResult.h"
//#include "SlotBetResult.h"
//#include "SlotGamePhase.h"
#//include "SlotObject.h"
#include "SpinObjectPrizeData.h"
#include "SlotRule.h"
cSlotProbabilityStatsticData::cSlotProbabilityStatsticData(const WCHAR*e_strStageName,cSpinObjectPrizeDataManager*e_pSpinObjectDataManager,cSlotRuleData*e_pSlotRuleData)
{
	m_pSlotRuleData = e_pSlotRuleData;
	m_pEventVariableManager = 0;
	std::wstring	l_strDirecory = cGameApp::m_sstrGameAppName;
	l_strDirecory += L"/Log/";
	l_strDirecory += e_strStageName;
	UT::MyCreateDirectory(UT::WcharToChar(l_strDirecory.c_str()).c_str());
	std::wstring	l_strProbabilityFileName =	l_strDirecory +L"/Probability_"	+UT::GetSystemTimeForFile(true).c_str()+L".txt";
	std::wstring	l_strBetResultFileName =	l_strDirecory +L"/BetResult_"		+UT::GetSystemTimeForFile(true).c_str()+L".txt";
	std::wstring	l_strVariableFileName =	l_strDirecory +L"/Variable.xml";
	m_ProbabilityFile.Writefile(UT::WcharToChar(l_strProbabilityFileName.c_str()).c_str(),true,false);
	m_BetResultFile.Writefile(UT::WcharToChar(l_strBetResultFileName.c_str()).c_str(),false,false);

	m_pi64InMoney = 0;
	m_pi64OutMoney = 0;
	m_iColumn = e_pSlotRuleData->m_TableCount.x;
	m_ppiConsecutiveScore = new int*[m_iColumn];
	m_ppiConsecutiveCount = new int*[m_iColumn];
	m_piPlayedRound = 0;
	m_i64StartIncome = 0;
	m_i64StartOut = 0;

	SetupVariable(UT::WcharToChar(l_strVariableFileName).c_str());

	if( e_pSpinObjectDataManager )
	{
		
		int	l_iSpinObjectMaximum = e_pSpinObjectDataManager->Count();
		m_AllSpinObjectResultShowCount.resize(l_iSpinObjectMaximum);
		for( int i=0;i<l_iSpinObjectMaximum;++i )
		{
			std::map<int,int>l_EmptyCosecutiveCount;
			for( int j=0;j<e_pSlotRuleData->m_TableCount.x;++j )
			{
				l_EmptyCosecutiveCount[j] = 0;
			}
			m_WinMoneySpinObjectMatchCount.push_back(l_EmptyCosecutiveCount);
		}
		m_LinesWinMoneyCount.resize(e_pSlotRuleData->m_LineRuleList.size());
	}

}

cSlotProbabilityStatsticData::~cSlotProbabilityStatsticData()
{
	SAFE_DELETE_ARRAY(m_ppiConsecutiveCount);
	SAFE_DELETE_ARRAY(m_ppiConsecutiveScore);
	SAFE_DELETE(m_pEventVariableManager);
}

void	cSlotProbabilityStatsticData::SetupVariable(const char*e_strEventVariableFileName)
{
	SAFE_DELETE(m_pEventVariableManager);
	m_pEventVariableManager = new cEventVariableManager();
	bool	l_bParseResult = m_pEventVariableManager->Parse(e_strEventVariableFileName);
	m_strEventVariableManagerFileName = e_strEventVariableFileName;
	const WCHAR*	l_strShowCount = L"ComboShowCount";
	const WCHAR*	l_strWinMoney = L"ComboWinMoney";
	if( !l_bParseResult )
	{
		//WCHAR*e_strName,eDataType e_eDataType,const WCHAR*e_strValue
		for( int i=0;i<m_iColumn;++i )
		{
			std::wstring	l_strComboWinMoneyName	= ValueToStringW(i+1);
			l_strComboWinMoneyName	+= l_strWinMoney;
			std::wstring	l_strComboShowCount		= ValueToStringW(i+1);
			l_strComboShowCount		+= l_strShowCount;

			cEventVariable*l_pComboWinMoney =		new cEventVariable(l_strComboWinMoneyName.c_str(),eDT_INT,L"0");	m_pEventVariableManager->AddObjectNeglectExist(l_pComboWinMoney);
			cEventVariable*l_pComboShowCount =		new cEventVariable(l_strComboShowCount.c_str(),eDT_INT,L"0");		m_pEventVariableManager->AddObjectNeglectExist(l_pComboShowCount);
		}
		cEventVariable*l_pIncome =				new cEventVariable(L"Income",eDT_INT64,L"0");			m_pEventVariableManager->AddObjectNeglectExist(l_pIncome);
		cEventVariable*l_pPay =					new cEventVariable(L"Pay",eDT_INT64,L"0");				m_pEventVariableManager->AddObjectNeglectExist(l_pPay);
		cEventVariable*l_pRound =				new cEventVariable(L"Round",eDT_INT,L"0");				m_pEventVariableManager->AddObjectNeglectExist(l_pRound);	
		m_pEventVariableManager->Export(m_strEventVariableManagerFileName.c_str());
	}
	for( int i=0;i<m_iColumn;++i )
	{
		std::wstring	l_strComboWinMoneyName	= ValueToStringW(i+1);
		l_strComboWinMoneyName	+= l_strWinMoney;
		std::wstring	l_strComboShowCount		= ValueToStringW(i+1);
		l_strComboShowCount		+= l_strShowCount;
		cEventVariable*l_pComboWinMoney =		m_pEventVariableManager->GetObject(l_strComboWinMoneyName.c_str());
		m_ppiConsecutiveScore[i] = l_pComboWinMoney->GetInt();
		cEventVariable*l_pComboShowCount =		m_pEventVariableManager->GetObject(l_strComboShowCount.c_str());
		m_ppiConsecutiveCount[i] = l_pComboShowCount->GetInt();
	}
	cEventVariable*l_pIncome =				m_pEventVariableManager->GetObject(L"Income");
	cEventVariable*l_pPay =					m_pEventVariableManager->GetObject(L"Pay");
	cEventVariable*l_pRound =				m_pEventVariableManager->GetObject(L"Round");

	m_pi64InMoney =				l_pIncome->GetInt64();
	m_pi64OutMoney =			l_pPay->GetInt64();
	m_i64StartIncome =			*m_pi64InMoney;
	m_i64StartOut =				*m_pi64OutMoney;
	m_piPlayedRound = l_pRound->GetInt();
}

void	cSlotProbabilityStatsticData::UpdateData(std::vector<sSlotBetTableResult*>*e_TableResultList,int e_iBetMoney,int e_iPlayerMoney)
{
	if( !m_pEventVariableManager )
		return;
	int	l_iCurrentRoundOutMoney = 0;
	int	l_iCurrentRoundBetMoney = e_iBetMoney;
	size_t	l_uiSize = e_TableResultList->size();
	//int	l_Test[5] = {0,0,0,0,0};
	for( size_t i=0;i<l_uiSize;++i )
	{
		sSlotBetTableResult*l_pTableResult = (*e_TableResultList)[i];
		size_t	l_uiSpinObjectSize = l_pTableResult->SpinObjectResultList.size();
		for( size_t j=0;j<l_uiSpinObjectSize;++j )
		{
			int	l_iIndex = l_pTableResult->SpinObjectResultList[j];
			m_AllSpinObjectResultShowCount[l_iIndex] = m_AllSpinObjectResultShowCount[l_iIndex]+1;
		}
		for(size_t j = 0;j<l_pTableResult->WinMoneyLineIndexList.size();++j)
		{
			++m_LinesWinMoneyCount[l_pTableResult->WinMoneyLineIndexList[j]];
		}
		for( int i=0;i<m_iColumn;++i )
		{
			*m_ppiConsecutiveScore[i] += l_pTableResult->piConsecutiveScore[i];
			*m_ppiConsecutiveCount[i] += l_pTableResult->piConsecutiveCount[i];
		}
		//l_Test[2] += l_pTableResult->i3ConsecutiveCount;
		//l_Test[3] += l_pTableResult->i4ConsecutiveCount;
		//l_Test[4] += l_pTableResult->i5ConsecutiveCount;
		l_iCurrentRoundOutMoney += l_pTableResult->iWinMoney;
		size_t	l_uiWinLineSize = l_pTableResult->WinMoneyLineList.size();
		for( size_t j=0;j<l_uiWinLineSize;++j )
		{
			int	l_iMatchCount = l_pTableResult->WinMoneyLineList[j].iMatchCount;
			int	l_iWinMoneySpinObjectID = l_pTableResult->WinMoneyLineList[j].iWinMoneySpinObjectID;
			//-1 for 0 start
			++m_WinMoneySpinObjectMatchCount[l_iWinMoneySpinObjectID][l_iMatchCount-1];
		}
	}
	int	l_iRevenue = l_iCurrentRoundBetMoney-l_iCurrentRoundOutMoney;

	*m_pi64InMoney +=				l_iCurrentRoundBetMoney;
	*m_pi64OutMoney +=				l_iCurrentRoundOutMoney;

	WriteProbabilityData(e_iPlayerMoney);
	m_pEventVariableManager->Export(m_strEventVariableManagerFileName.c_str());
}

void	cSlotProbabilityStatsticData::WriteProbabilityData(int e_iPlayerMoney)
{
	int	l_iNumLine = 0;
	std::string	l_str = "\n";
	for( size_t i=0;i<m_WinMoneySpinObjectMatchCount.size();++i )
	{
		int	l_iSpinObjectID = i;
		std::map<int,int>l_EmptyCosecutiveCount;
		l_str += "ID:";
		l_str += ValueToString(l_iSpinObjectID);
		l_str += "\t";
		for( int j=0;j<m_pSlotRuleData->m_TableCount.x;++j )
		{
			int	l_iConsecutive = j+1;
			int	l_iCount = m_WinMoneySpinObjectMatchCount[i][j];
			l_str += "Combo";
			l_str += ValueToString(l_iConsecutive);
			l_str += ":";
			l_str += ValueToString(l_iCount);
			l_str += "\t\t";
		}
		l_str += "\n";
		++l_iNumLine;
	}
	l_str += "-----------------------------------\n";
	for(size_t j = 0;j<m_LinesWinMoneyCount.size();++j)
	{
		l_str += "Line";
		l_str += ValueToString(j);
		l_str += "\tCount:";
		l_str += ValueToString(m_LinesWinMoneyCount[j]);
		l_str += "\n";
		++l_iNumLine;
	}
	l_str += "PastRound:";
	l_str += ValueToString(*m_piPlayedRound);
	l_str += "\n";
	l_str += "StartInCome:";
	l_str += ValueToString(m_i64StartIncome);
	l_str += "\n";
	l_str += "StartPay:";
	l_str += ValueToString(m_i64StartOut);
	l_str += "\n";
	l_str += "InCome:";
	l_str += ValueToString(*m_pi64InMoney);
	l_str += "\n";
	l_str += "Pay:";
	l_str += ValueToString(*m_pi64OutMoney);
	l_str += "\n";
	l_str += "Revenue:";
	l_str += ValueToString(*m_pi64InMoney-*m_pi64OutMoney);
	l_str += "\n";	


	m_BetResultFile.SetFilePos(0);
	//+1for empty line
	for( int i=0;i<l_iNumLine+2;++i )
	{
		m_BetResultFile.WriteToFileImmediatelyWithLine("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",false);
	}
	m_BetResultFile.SetFilePos(0);
	m_BetResultFile.WriteToFileImmediatelyWithLine(l_str.c_str());

	//player money
	m_ProbabilityFile.WriteToFile(e_iPlayerMoney);
	//probability
	float	l_fProbability = (float)((double)*m_pi64OutMoney/(*m_pi64InMoney));
	m_ProbabilityFile.WriteToFile(l_fProbability);

}


void	cSlotProbabilityStatsticData::ExtraGameWinExtraMoney(int e_iOut)
{
	*m_pi64OutMoney +=	e_iOut;
}