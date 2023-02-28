#include "stdafx.h"
//
#include "ControlPanel.h"
//
extern cControlPanel *g_pControlPanel;
//
#define	MAGIC_NUMBER_FOR_ENCODE	0xabcdef01
#define	MAXIMUM_DATA_COUNT	498



unsigned int	DeCodeHintCode( unsigned int e_iValue)
{
	unsigned int	l_iValue1 = e_iValue^0xffffffff;
	unsigned int	l_iValue2 = l_iValue1+MAGIC_NUMBER_FOR_ENCODE;
	//
	return l_iValue2;
}


 int	GenerationHintCode( int e_iValue)
{
	unsigned int	l_iValue1 = (unsigned int)e_iValue-MAGIC_NUMBER_FOR_ENCODE;
	unsigned int	l_iValue2 = l_iValue1^0xffffffff;
	//if( e_iValue != DeCodeHintCode(l_iValue2) )
	//{
	//	int a=0;
	//}
	return l_iValue2;
}


void	GeneratingVendorHintCode()
{
	//cEventVariable*l_pEventVariable = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"VendorHintCode");
	//cEventVariable*l_pEventVariable2 = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"VendorPassword");
	//std::wstring	l_str = l_pEventVariable2->ConvertToString();
	//std::vector<int>	l_PasswordVector = GetValueListByCommaDivide<int>(l_str.c_str());
	//int	l_iValue = 0;
	//int	l_iDecimal = 1;
	//for(size_t i=l_PasswordVector.size()-1;i>-1;--i)
	//{
	//	l_iValue += l_iDecimal*l_PasswordVector[i];
	//	l_iDecimal *= 10;
	//}
	//int	l_Encode = GenerationHintCode(l_iValue);
	////int	l_iDecode = DeCodeHintCode(l_Encode);
	//l_pEventVariable->SetInt(l_Encode);
}


void	GeneratingAuditHintCode()
{
	//cEventVariable*l_pEventVariable = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"AuditHintCode");
	//cEventVariable*l_pEventVariable2 = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"AuditPassword");
	//std::wstring	l_str = l_pEventVariable2->ConvertToString();
	//std::vector<int>	l_PasswordVector = GetValueListByCommaDivide<int>(l_str.c_str());
	//int	l_iValue = 0;
	//int	l_iDecimal = 1;
	//for(size_t i=l_PasswordVector.size()-1;i>-1;--i)
	//{
	//	l_iValue += l_iDecimal*l_PasswordVector[i];
	//	l_iDecimal *= 10;
	//}
	//int	l_Encode = GenerationHintCode(l_iValue);
	////int	l_iDecode = DeCodeHintCode(l_Encode);
	//l_pEventVariable->SetInt(l_Encode);
}


void	ResetRecordData()
{
	//const char*	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(L"DefaultVariableData.xml").c_str();
	//cEventVariableManager	l_EventVariableManager;
	//l_EventVariableManager.Parse(l_strFileName);

	//cEventVariableManager*l_pEventVariableManager = g_pControlPanel->GetEventManager()->m_pEventVariableManager;
	//int	l_iCount = l_pEventVariableManager->Count();
	//for( int i=0;i<l_iCount;++i )
	//{
	//	cEventVariable*l_pEventVariable1 = l_pEventVariableManager->GetObject(i);
	//	cEventVariable*l_pEventVariable2 = l_EventVariableManager.GetObject(l_pEventVariable1->GetName());
	//	std::wstring	l_str = l_pEventVariable2->ConvertToString();
	//	l_pEventVariable1->SetDataByString(l_str.c_str());
	//}
}


void FigureCheckCode( void *e_pData )
{
	g_pControlPanel->FigureCheckCodeWithMachineNumber();
}

void	BulletPayRateAddPoint_Check(void*e_pData)
{
	int		l_iPointAdd[] = {1,2,5,10,20,30,50,100,200};

	cEventManager*l_pEventManager = g_pControlPanel->GetEventManager();
	cEventVariable*l_pPointLimit = l_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit");
	cEventVariable*l_pPointAdd = l_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint");

	int l_iPointLimit = *l_pPointLimit->GetInt();
	int l_iPointAddN = *l_pPointAdd->GetInt();

	if(l_iPointLimit < l_iPointAddN)
	{
		for(int i = 0; i < 9; i++)
		{

			if(l_iPointAdd[i] <= l_iPointLimit && l_iPointAdd[i+1] > l_iPointLimit)
			{
				l_iPointAddN = l_iPointAdd[i];
			}
		}
	}
	l_pPointAdd->SetInt(l_iPointAddN);
}

void	BulletPayRateMinPoint_Check(void*e_pData)
{
	int		l_iPointMin[] = {1,2,5,10,20,30,50,100,200};

	cEventManager*l_pEventManager = g_pControlPanel->GetEventManager();
	cEventVariable*l_pPointLimit = l_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit");
	cEventVariable*l_pPointMinN = l_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateMinPoint");
	

	int l_iPointLimit = *l_pPointLimit->GetInt();
	int l_iPointMinN = *l_pPointMinN->GetInt();

	if(l_iPointLimit < l_iPointMinN)
	{
		for(int i = 0; i < 9; i++)
		{

			if(l_iPointMin[i] <= l_iPointLimit && l_iPointMin[i+1] > l_iPointLimit)
			{
				l_iPointMinN = l_iPointMin[i];
			}
		}
	}
	l_pPointMinN->SetInt(l_iPointMinN);
}

void	FigureSetupCode(void*e_pData)
{
	//OutputDebugString(L"aaaa");
	int64 l_iTotal = 0;
	int l_iExchangeIn[] = {50,100,200,500,1000,2000,10000,50000,100000};
	int l_iCoinToScore[]= {1,2,5,10,20,25,50,100,200,500,1000};
	int l_iTicketToScore[]= {1,2,5,10,20,30,40,50,60,70,80,90,100,200,500,1000};
	int l_iMoneyLimit[]= {200000,500000,1000000,5000000,10000000,20000000,50000000,90000000};
	int l_iReportDay[]= {5,7,10,12,14};

	//Main Prob
	cEventVariable*l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Probability");
	int64 l_iTemp = *l_pVar->GetInt();
	l_iTotal += l_iTemp;

	//Exchange_In
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");	
	for(int i = 0; i < 9; i++)
	{
		if(*l_pVar->GetInt() == l_iExchangeIn[i])
		{
			l_iTotal += i * 8;
			break;
		}
	}
	
	//CoinToScore
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CoinToScore");
	for(int i = 0; i < 11; i++)
	{
		if(*l_pVar->GetInt() == l_iCoinToScore[i])
		{
			l_iTotal += i * 128;
			break;
		}
	}

	//TicketToScore
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"TicketToScore");	
	for(int i = 0; i < 16; i++)
	{
		if(*l_pVar->GetInt() == l_iTicketToScore[i])
		{
			l_iTotal += i * 2048;
			break;
		}
	}

	//HappyTime
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"HappyTime");	
	l_iTemp = *l_pVar->GetInt();
	l_iTotal += l_iTemp * 32768;

	//MoneyLimit
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");	
	for(int i = 0; i < 8; i++)
	{
		if(*l_pVar->GetInt64() == l_iMoneyLimit[i])
		{
			l_iTotal += i * 65536;
			break;
		}
	}

	//ReportDay
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ReportDay");	
	for(int i = 0; i < 5; i++)
	{
		if(*l_pVar->GetInt() == l_iReportDay[i])
		{
			l_iTotal += i * 524288;
			break;
		}
	}

	//Area Adjust
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"PlaceType");	
	l_iTemp = *l_pVar->GetInt();
	l_iTotal += l_iTemp * 4194304;


	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"SetupCode");	
	l_pVar->SetInt64(l_iTotal);
}

void ExternalFunc_SaveData( void* e_pData )
{
	g_pControlPanel->SaveParameterData();
}

void ExternalFunc_RestoreOldData( void* e_pData )
{
	g_pControlPanel->RestoreOldData();
}

void ExternalFunc_ReportDelayTime( void* e_pData )
{
	g_pControlPanel->ReportDelayTime();
}

void ExternalFunc_CheckPassword( void* e_pData )
{
	g_pControlPanel->CheckPassword();
}

void ExternalFunc_CancelInputPassword( void* e_pData )
{
	g_pControlPanel->CancelInputPassword();
}

void ExternalFunc_IncreasePasswordValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_Password, eChangeInputValueType_Increase, 5 );
}

void ExternalFunc_DecreasePasswordValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_Password, eChangeInputValueType_Decrease, 5 );
}


void ExternalFunc_IncreaseClearRecordPasswordValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_ClearRecord, eChangeInputValueType_Increase, 10 );
}

void ExternalFunc_DecreaseClearRecordPasswordValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_ClearRecord, eChangeInputValueType_Decrease, 10 );
}

void ExternalFunc_CheckReportInput( void* e_pData )
{
	g_pControlPanel->CheckReportInputWithMachineNumber();
}








void ExternalFunc_IncreaseReportInputValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_Report, eChangeInputValueType_Increase, 10 );
}








void ExternalFunc_DecreaseReportInputValue( void* e_pData )
{
	g_pControlPanel->ChangeInputValue( eChangeInputValueKind_Report, eChangeInputValueType_Decrease, 10 );
}








void ExternalFunc_ReloadGame( void* e_pData )
{
	g_pControlPanel->ReloadGame();
}








void ExternalFunc_ReportInputResultClick( void* e_pData )
{
	g_pControlPanel->ReportInputResultClick();
}








void ExternalFunc_CheckClearRecodePassword( void* e_pData )
{
	g_pControlPanel->CheckClearRecodePassword();	
}









void ExternalFunc_GenerateAuditRandCode( void* e_pData )
{
	g_pControlPanel->GenerateAuditRandCode();	
}