#include "stdafx.h"
#include "ControlPanel.h"
#include <time.h>


#define	CONTROLPANEL_TIMESTAMP			"Fish/ControlPanel/TimeStamp.xml"
#define	CONTROLPANEL_VERSION			"Fish/ControlPanel/Version.xml"

#define SmallFontFileName			"Fish/ControlPanel/EventFont_12"

#define	VARIABLE_FILE_NAME			"Fish/ControlPanel/WithMachineNumberVariableData.xml"
#define	VARIABLE_BACKUP_FILE_NAME	"Fish/ControlPanel/WithMachineNumberVariableDataBackup.xml"
#define	VARIABLE_DEFAULT_FILE_NAME	"Fish/ControlPanel/WithMachineNumberVariableDataDefault.xml"


#define	TIME_STAMP_FILE_NAME		"Fish/ControlPanel/TimeStamp.xml"
#define	TIME_STAMP_BACKUP_FILE_NAME	"Fish/ControlPanel/TimeStampBackup.xml"

bool			g_bExportBinary = true;
const wchar_t* ParaAdjustList[ eParaAdjust_Max ] = { L"BulletPayRateLimit", L"BulletPayRateMinPoint", L"BulletPayRateAddPoint",L"RefundCoinOrTicket", L"Sound", L"PlayerCount"}; 
const wchar_t* DecodeList[ eDecode_Max ] = { L"Probability", L"Exchange_In", L"CoinToScore", L"TicketToScore",L"MoneyLimit", L"ReportDay",L"PlaceType" };
extern bool*	g_pbIsUsingMasterLeeProbability;
//
cControlPanel *g_pControlPanel;
//
extern unsigned int	DeCodeHintCode( unsigned int e_iValue);
extern int			GenerationHintCode( int e_iValue);
extern void			GeneratingVendorHintCode();
extern void			GeneratingAuditHintCode();
extern void			FigureCheckCode(void*e_pData);
extern void			BulletPayRateMinPoint_Check(void*e_pData);
extern void			BulletPayRateAddPoint_Check(void*e_pData);
extern void			FigureSetupCode(void*e_pData);

extern void	ExternalFunc_SaveData( void* e_pData );
extern void	ExternalFunc_RestoreOldData( void* e_pData );

extern void	ExternalFunc_ReportDelayTime( void* e_pData );

extern void	ExternalFunc_CheckPassword( void* e_pData );
extern void	ExternalFunc_CancelInputPassword( void* e_pData );
extern void	ExternalFunc_IncreasePasswordValue( void* e_pData );
extern void	ExternalFunc_DecreasePasswordValue( void* e_pData );


extern void	ExternalFunc_CheckReportInput( void* e_pData );
extern void	ExternalFunc_IncreaseReportInputValue( void* e_pData );
extern void	ExternalFunc_DecreaseReportInputValue( void* e_pData );

extern void ExternalFunc_ReloadGame( void* e_pData );
extern void ExternalFunc_ReportInputResultClick( void* e_pData );

extern void ExternalFunc_CheckClearRecodePassword( void* e_pData );
extern void ExternalFunc_IncreaseClearRecordPasswordValue( void* e_pData );
extern void ExternalFunc_DecreaseClearRecordPasswordValue( void* e_pData );

extern void ExternalFunc_GenerateAuditRandCode( void* e_pData );

extern void ClearLatestRecord(bool e_bClearPlayerMoney);

cControlPanel::cControlPanel( const char* e_strPackagesFileName, int ControlPanelType )
{
	m_iBulletPayRateLimit		= 0;
	m_iBulletPayRateMinPoint	= 0;
	m_iBulletPayRateAddPoint	= 0;
	m_i64MoneyLimit				= 0;
	m_pReportTipImage = 0;
	m_bShowReportTipImage = false;
	if ( !m_pReportTipImage )
	{
		m_pReportTipImage = new cBaseImage( "Fish/Image/ReportTip.png" );

		if ( m_pReportTipImage )
		{
			m_pReportTipImage->SetWidth( ( int )cGameApp::m_spOpenGLRender->m_vGameResolution.x );
			m_pReportTipImage->SetHeight( ( int )cGameApp::m_spOpenGLRender->m_vGameResolution.y );
		}
	}
	m_iSound = 100;
	//m_bLuckyTime = false;
	m_bGameReload = false;
	g_pControlPanel = this;
	m_bLoadTimeStampFial = false;
	m_bInControlPanelMode = false;
	m_bPressOpenControlPanel = false;
	m_fOpenControlPanelPressTm = 0.f;

	m_pEventManager = new cEventManager();

	m_pEventPackageRunner = m_pEventManager->m_pEventPackageRunner;

	m_pEventManager->ParseDataIntoXMLNode( e_strPackagesFileName );

	GetTimeStamp();
	FileBackup();

	m_pEventManager->AddExternFunctionName(L"FigureCheckCode",FigureCheckCode);
	m_pEventManager->AddExternFunctionName(L"BulletPayRateMinPoint_Check",BulletPayRateMinPoint_Check);
	m_pEventManager->AddExternFunctionName(L"BulletPayRateAddPoint_Check",BulletPayRateAddPoint_Check);
	m_pEventManager->AddExternFunctionName(L"FigureSetupCode",FigureSetupCode);

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_SaveData",       ExternalFunc_SaveData );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_RestoreOldData", ExternalFunc_RestoreOldData );

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_ReportDelayTime", ExternalFunc_ReportDelayTime );

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_CheckPassword",         ExternalFunc_CheckPassword );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_CancelInputPassword",   ExternalFunc_CancelInputPassword );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_IncreasePasswordValue", ExternalFunc_IncreasePasswordValue );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_DecreasePasswordValue", ExternalFunc_DecreasePasswordValue );



	m_pEventManager->AddExternFunctionName( L"ExternalFunc_CheckReportInput",         ExternalFunc_CheckReportInput );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_IncreaseReportInputValue", ExternalFunc_IncreaseReportInputValue );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_DecreaseReportInputValue", ExternalFunc_DecreaseReportInputValue );

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_ReloadGame",             ExternalFunc_ReloadGame );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_ReportInputResultClick", ExternalFunc_ReportInputResultClick );

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_CheckClearRecodePassword",         ExternalFunc_CheckClearRecodePassword );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_IncreaseClearRecordPasswordValue", ExternalFunc_IncreaseClearRecordPasswordValue );
	m_pEventManager->AddExternFunctionName( L"ExternalFunc_DecreaseClearRecordPasswordValue", ExternalFunc_DecreaseClearRecordPasswordValue );

	m_pEventManager->AddExternFunctionName( L"ExternalFunc_GenerateAuditRandCode", ExternalFunc_GenerateAuditRandCode );

	m_bAjustSetting = *m_pEventManager->m_pEventVariableManager->GetObject( L"AjustSetting" )->GetInt() == 1 ? true : false;

	StoreOldData();

	CheckMachineNumber();

	CheckReport();
	m_fReportRoutine = 0.0f;
	m_spGlyphSmallFontRender = new cGlyphFontRender(SmallFontFileName, 2000);

	GetVersion();
}


cControlPanel::~cControlPanel()
{
	SAFE_DELETE(m_pEventManager);
	SAFE_DELETE(m_spGlyphSmallFontRender);
	SAFE_DELETE(m_pReportTipImage);
}




int	cControlPanel::GetCoinToScore()
{
	return *m_pEventManager->m_pEventVariableManager->GetObject(L"CoinToScore")->GetInt();
}


void	cControlPanel::StoreOldData()
{
	cEventVariable*l_pProbability = m_pEventManager->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");	
	cEventVariable*l_pMoneyLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pExchange_In = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");

	cEventVariable*l_pPlayerCount = m_pEventManager->m_pEventVariableManager->GetObject(L"PlayerCount");
	cEventVariable*l_pBulletPayRateLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit");
	cEventVariable*l_pBulletPayRateAddPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint");
	cEventVariable*l_pBulletPayRateMinPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateMinPoint");

	cEventVariable*l_pReportCount = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportCount");
	cEventVariable*l_pReportFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	cEventVariable*l_pReportDay = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportDay");
	cEventVariable*l_pSaveTime = m_pEventManager->m_pEventVariableManager->GetObject(L"SaveTime");

	cEventVariable*l_pLatestIncome =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay =   m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pLatestExchangeIn = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatestExchangeOut = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_pLatestTicket = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_Ticket");
	cEventVariable*l_pLatestToastCoin =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatestRefundCoin = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_RefudCoin");

	m_pi64LatestInMoney = l_pLatestIncome->GetInt64();
	m_pi64LatestOutMoney = l_pLatestPay->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64LatestExchangeIn = l_pLatestExchangeIn->GetInt64();
	m_pi64LatestExchangeOut = l_pLatestExchangeOut->GetInt64();
	m_pi64LatestTicket = l_pLatestTicket->GetInt64();
	m_pi64LatestCoinIn = l_pLatestToastCoin->GetInt64();
	m_pi64LatestCoinOut = l_pLatestRefundCoin->GetInt64();
	//======================
	m_iSound = *l_pSound->GetInt();
	cGameApp::m_spSoundParser->SetVolume(m_iSound/100.f);
	m_RealProbility = *m_pEventManager->m_pEventVariableManager->GetObject( L"RealProbability" )->GetFloat();
	//Report Varabile
	m_iReportCount = *l_pReportCount->GetInt();
	m_iReportFlag = *l_pReportFlag->GetInt();
	m_iReportDay = *l_pReportDay->GetInt();
	m_iSaveTime = *l_pSaveTime->GetInt();

	for ( int i = 0; i < eParaAdjust_Max; i++ )
	{
		m_pnOldParameterAdjustListData[i] = m_pEventManager->m_pEventVariableManager->GetObject( ParaAdjustList[i] )->ConvertToString();
	}

	for ( int i = 0; i < eDecode_Max; i++ )
	{
		m_pnOldDecodeListData[i] = m_pEventManager->m_pEventVariableManager->GetObject( DecodeList[i] )->ConvertToString();
	}

	const WCHAR* LatestValueStr[ eAuditList_Max ]         = { L"Latest_TotalIncome",  L"Latest_TotalPay",      L"Latest_ExchangeIn",      L"Latest_ExchangeOut",      L"Latest_Ticket",      L"Latest_ToastCoin",   L"Latest_RefudCoin",    L"Latest_Revenue" }; 
	const WCHAR* VariableAuditLatestStr[ eAuditList_Max ] = { L"Audit_LatestInMoney", L"Audit_LatestOutMoney", L"Audit_LatestExchangeIn", L"Audit_LatestExchangeOut", L"Audit_LatestTicket", L"Audit_LatestCoinIn", L"Audit_LatestCoinOut", L"Audit_LatestRevenue" }; 

	for ( int i = 0; i < eAuditList_Max; i++ )
	{
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( LatestValueStr[i] )->GetInt64() == 0 ? 0 : *m_pEventManager->m_pEventVariableManager->GetObject( LatestValueStr[i] )->GetInt64() / GetCoinToScore() ); 
	}

	const WCHAR* VariableAuditTotalStr[ eAuditList_Max  ]    = { L"TotalIncome",       L"TotalPay",           L"TotalExchangeIn",      L"TotalExchangeOut",      L"TotalTicket",      L"TotalToastCoin",   L"TotalRefudCoin",    L"TotalRevenue" }; 
	const WCHAR* VariableAuditShowTotalStr[ eAuditList_Max ] = { L"Show_TotalInMoney", L"Show_TotalOutMoney", L"Show_TotalExchangeIn", L"Show_TotalExchangeOut", L"Show_TotalTicket", L"Show_TotalCoinIn", L"Show_TotalCoinOut", L"Show_TotalRevenue" }; 

	for ( int i = 0; i < eAuditList_Max; i++ )
	{
		int64	l_i64Value1 = *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i] )->GetInt64();
		int64	l_i64Value2 = *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->GetInt64();
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditShowTotalStr[i] )->SetInt64(  l_i64Value1+l_i64Value2  );
	}

	//vendor ask show coin in+exchang in-coin out-exchange out
	{
		int64	l_i64LatestRevenue = GetLatestRevenue();
		int64	l_i64TotalRevenue = GetTotalRevenue();
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestRevenue" )->SetInt64( l_i64LatestRevenue  );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalRevenue" )->SetInt64( l_i64TotalRevenue  );
	}
	m_pEventManager->m_pEventVariableManager->GetObject( L"CheckPasswordResult" )->SetInt(1);

	m_iBulletPayRateLimit		= *m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit")->GetInt();
	m_iBulletPayRateMinPoint	= *m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateMinPoint")->GetInt();
	m_iBulletPayRateAddPoint	= *m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint")->GetInt();
	m_i64MoneyLimit				= *m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit")->GetInt64();
	m_iPlayerCount				= *m_pEventManager->m_pEventVariableManager->GetObject(L"PlayerCount")->GetInt();
	
}

int64	cControlPanel::GetRestTimeToReport()
{
	int	l_iMinForADay = 86400;
	cEventVariable*l_pReportFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	cEventVariable*l_pDelayTimeFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"DelayTimeFlag");
	int64 l_iNowTime = GetTimeForInt64();
	int64 l_iIntervalTime = l_iNowTime - m_iReportTime;
	int l_iReportDay = m_iReportDay;
	if ( *l_pDelayTimeFlag->GetInt() == 1 )
	{
		l_iReportDay = m_iReportDay + 2;
	}
	if ( ( l_iIntervalTime / l_iMinForADay ) >= l_iReportDay )
	{
		return 0;
	}
	return (l_iReportDay*l_iMinForADay)-l_iIntervalTime;
}

void cControlPanel::CheckReport( bool bLoadReportInput )
{
	return;
	//Report Parameter
	cEventVariable*l_pReportFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	cEventVariable*l_pDelayTimeFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"DelayTimeFlag");

	//if TimeStamp != 0, Check, or GetTimeINt64ToTimeStamp;
	if(m_iTimeStamp)
	{
		int64 l_iNowTime = GetTimeForInt64();

		if ( m_iTimeStamp < l_iNowTime )
		{
			m_iTimeStamp = l_iNowTime;

			int64 l_iIntervalTime = m_iTimeStamp - m_iReportTime;

			int l_iReportDay = m_iReportDay;

			if ( *l_pDelayTimeFlag->GetInt() == 1 )
			{
				l_iReportDay = m_iReportDay + 2;
			}

			if ( ( l_iIntervalTime / 86400 ) >= l_iReportDay )
			{
				m_iReportFlag = 1;
			}
			else
			{
				m_iReportFlag = 0;
			}
		}
		else
		{
			m_iReportFlag = 2; // Report Time is Error
		}
	}
	else
	{
		//Init Game
		m_iReportFlag = 0;
		m_iTimeStamp = GetTimeForInt64();
		m_iReportTime = m_iTimeStamp;
		WriteTimeStamp();
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"LastReportTime" )->SetWString( GetTime( m_iReportTime ) );

	int l_iReportFlag = *l_pReportFlag->GetInt();

	if ( !cGameApp::m_sbDebugFunctionWorking )
	{
		if ( m_iReportFlag ) 
		{
			if ( bLoadReportInput )
			{
				m_bInControlPanelMode = true;
				FigureCheckCodeWithMachineNumber();
				m_pEventPackageRunner->LoadPackage( L"ReportInput" );
			}
			else
			{
				//this->m_bShowReportTipImage = true;
				//cFishApp::m_bShowReportTipImage = true;
			}
		}

		l_pReportFlag->SetInt(m_iReportFlag);
	}
}








int64	cControlPanel::GetTimeForInt64()
{
	time_t t = time(NULL);
	return (int64)time(NULL);
}








const wchar_t*	cControlPanel::GetTime( int64 e_iTime )
{
	return L"";
	//time_t t = (time_t)e_iTime;
	//tm* p;
	//p = localtime( &t );
	//static wchar_t l_str[MAX_PATH];
	//swprintf(l_str, MAX_PATH, L"%04d/%02d/%02d %02d:%02d:%02d", p->tm_year + 1900, 
	//	p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	//return l_str;
}








void	cControlPanel::SoundCheck()
{
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	if( *l_pSound->GetInt() != m_iSound )
	{
		m_iSound = *l_pSound->GetInt();
		cGameApp::m_spSoundParser->SetVolume(m_iSound/100.f);
	}
}








void	cControlPanel::InternalReset()
{
	//int l_iCount = (int)m_IntoMenuDataList.size();
	//for( int i=0;i<l_iCount;++i ){m_IntoMenuDataList[i]->Init();}
}


void cControlPanel::GenerateAuditRandCode()
{
	int MachineNumber = *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt();

	int64 InData  = *m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalInMoney" )->GetInt64();
	int64 OutData = *m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalOutMoney" )->GetInt64();

	int64 TempInt = 9999999999;

	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_TotalIn" )->SetInt64( TempInt - ( InData ^ MachineNumber ) );
	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_TotalOut" )->SetInt64( TempInt - ( OutData ^ MachineNumber ) );

	InData  = *m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestInMoney" )->GetInt64();
	OutData = *m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestOutMoney" )->GetInt64();

	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_LatestIn" )->SetInt64( TempInt - ( InData ^ MachineNumber ) );
	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_LatestOut" )->SetInt64( TempInt - ( OutData ^ MachineNumber ) );

	InData  = *m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_TotalIncome" )->GetInt64();
	OutData = *m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_TotalPay" )->GetInt64();

	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_PriorIn" )->SetInt64( TempInt - ( InData ^ MachineNumber ) );
	m_pEventManager->m_pEventVariableManager->GetObject( L"RandCode_PriorOut" )->SetInt64( TempInt  - ( OutData ^ MachineNumber ) );

	m_pEventManager->m_pEventVariableManager->Export( m_pEventManager->m_pEventVariableManager->GetFileName(),g_bExportBinary );
}



void	cControlPanel::FileBackup()
{
	int	l_iCount = m_pEventManager->m_pEventVariableManager->Count();
	//lost variable file load default
	bool	l_bOpenBKFileOk = false;
	if ( l_iCount == 0 )
	{//open back file
		cNodeISAX	l_cNodeISAXBackup;
		if(l_cNodeISAXBackup.ParseDataIntoXMLNode(VARIABLE_BACKUP_FILE_NAME))
		{//check bkfile is vaild
			if(l_cNodeISAXBackup.GetRootElement())
			{
				if(l_cNodeISAXBackup.Export(VARIABLE_FILE_NAME,g_bExportBinary))
				{//rewrite again next
					l_bOpenBKFileOk = true;
				}
			}
		}
		if( l_bOpenBKFileOk == false )
		{//restore machine to start status
			cNodeISAX	l_NodeISAX2Default;
			if(l_NodeISAX2Default.ParseDataIntoXMLNode(VARIABLE_DEFAULT_FILE_NAME))
			{
				if(!l_NodeISAX2Default.Export(VARIABLE_FILE_NAME,g_bExportBinary))
				{

					return;
				}
			}
		}
		if(!m_pEventManager->m_pEventVariableManager->Parse(VARIABLE_FILE_NAME))
		{

			return;
		}
		else
		{

		}
		
	}
	else
	{//backup file
		m_pEventManager->m_pEventVariableManager->Export(VARIABLE_BACKUP_FILE_NAME,g_bExportBinary);
	}

	if ( m_bLoadTimeStampFial )
	{
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(TIME_STAMP_BACKUP_FILE_NAME))
		{
			l_cNodeISAX.Export(TIME_STAMP_FILE_NAME,g_bExportBinary);
		}
		else
		{//create default time stamp,and ask report immediately
			TiXmlDocument doc;
			TiXmlElement* l_pRoot = new TiXmlElement( L"Root" );
			TiXmlElement* l_pTimeStamp = new TiXmlElement( L"Data" );
			TiXmlElement* l_pReportTime = new TiXmlElement( L"Data" );
			doc.LinkEndChild(l_pRoot);
			l_pRoot->LinkEndChild(l_pTimeStamp);
			l_pRoot->LinkEndChild(l_pReportTime);
			l_pTimeStamp->SetAttribute(L"Name", L"TimeStamp");
			l_pTimeStamp->SetAttribute(L"Value", 1);
			l_pReportTime->SetAttribute(L"Name", L"ReportTime");
			l_pReportTime->SetAttribute(L"Value", 1);
			doc.SaveFile( TIME_STAMP_FILE_NAME ,g_bExportBinary);  
			m_iReportTime = m_iTimeStamp = 1;
		}
		GetTimeStamp();
	}
	else
	{
		TiXmlDocument doc;
		TiXmlElement* l_pRoot = new TiXmlElement( L"Root" );
		TiXmlElement* l_pTimeStamp = new TiXmlElement( L"Data" );
		TiXmlElement* l_pReportTime = new TiXmlElement( L"Data" );
		doc.LinkEndChild(l_pRoot);
		l_pRoot->LinkEndChild(l_pTimeStamp);
		l_pRoot->LinkEndChild(l_pReportTime);
		l_pTimeStamp->SetAttribute(L"Name", L"TimeStamp");
		l_pTimeStamp->SetAttribute(L"Value", ValueToStringW(m_iTimeStamp));
		l_pReportTime->SetAttribute(L"Name", L"ReportTime");
		l_pReportTime->SetAttribute(L"Value", ValueToStringW(m_iReportTime));
		doc.SaveFile( TIME_STAMP_BACKUP_FILE_NAME ,g_bExportBinary);
	}
}

void	cControlPanel::IntoControlPanel()
{
	//show report page
	if( m_bShowReportTipImage )
	{
		StoreOldData();
		CheckReport( true );
	}//show main menu page
	else
	{
		m_fOpenControlPanelPressTm = 0.f;
		m_bInControlPanelMode = true;
		GeneratingVendorHintCode();
		GeneratingAuditHintCode();
		m_pEventPackageRunner->LoadPackage( L"MainMenu" );
		StoreOldData();
		FileBackup();
		cGameApp::m_spSoundParser->Pause(true);
		return;
	}
}

void	cControlPanel::Update( float e_fElpaseTime )
{
	if( m_bInControlPanelMode )
	{
		m_pEventPackageRunner->Update(e_fElpaseTime);
		SoundCheck();
		if( !m_pEventPackageRunner->GetCurrentEventPackage() )
		{
			m_bInControlPanelMode = false;
			InternalReset();
		}
	}
	else
	{
		if(m_bPressOpenControlPanel)
		{
			m_fOpenControlPanelPressTm += e_fElpaseTime;

			if ( m_fOpenControlPanelPressTm > 3.f )
			{
				IntoControlPanel();
			}
		}
		else
		{
			//if ( m_IntoMenuDataList[i] && m_IntoMenuDataList[i]->strMenuName == L"MainMenu" )
			m_fOpenControlPanelPressTm = 0.f;
		}
	}
	m_fReportRoutine += e_fElpaseTime;
	if(m_fReportRoutine > m_iSaveTime)
	{
		m_fReportRoutine = 0;
		CheckReport();
		WriteTimeStamp();
	}
}








void cControlPanel::Render()
{
	if ( m_bInControlPanelMode )
	{
		m_pEventPackageRunner->Render();



		m_spGlyphSmallFontRender->RenderFont( 850, 103, GetTime( GetTimeForInt64() ) );

		m_spGlyphSmallFontRender->RenderFont( 850, 60, L"GameVersion:" );
		m_spGlyphSmallFontRender->RenderFont( 1010, 60, m_wchVersion.c_str() );

		const WCHAR* PageName = m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->GetWString()->c_str();

		if ( !wcscmp( PageName, L"Audit" ) || !wcscmp( PageName, L"ReportInput" ) )
		{
			if ( *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportCount" )->GetInt() > 0 )
			{
				if ( !wcscmp( PageName, L"Audit" ) )
					m_spGlyphSmallFontRender->RenderFont( 645, 563, m_pEventManager->m_pEventVariableManager->GetObject( L"LastReportTime" )->GetWString()->c_str() );
				else
					m_spGlyphSmallFontRender->RenderFont( 532, 605, m_pEventManager->m_pEventVariableManager->GetObject( L"LastReportTime" )->GetWString()->c_str() );
			}
		}


		//WCHAR MachineNumberStr[20] = {0};
		//_itow( *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt(), MachineNumberStr, 10 );
		//m_spGlyphSmallFontRender->RenderFont( 600, 60, MachineNumberStr );
	}
}







void cControlPanel::ReloadGame()
{
	StoreOldData();

	m_pEventManager->m_pEventVariableManager->Export( m_pEventManager->m_pEventVariableManager->GetFileName(),g_bExportBinary);

	WriteTimeStamp();

	m_bGameReload = true;
	m_bInControlPanelMode = false;
}








bool	cControlPanel::IsInControlPanelMode()
{
	return m_bInControlPanelMode;
}








void	cControlPanel::MouseDown(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseDown(e_iPosX,e_iPosY);
}








void	cControlPanel::MouseMove(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseMove(e_iPosX,e_iPosY);
}








void	cControlPanel::MouseUp(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseUp(e_iPosX,e_iPosY);
}


void	cControlPanel::GetTimeStamp()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(	CONTROLPANEL_TIMESTAMP) )
	{
		m_bLoadTimeStampFial = false;

		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					if(!wcscmp(l_strName, L"TimeStamp"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						m_iTimeStamp =  GetInt(ValueToString(l_strValue));
					} 
					else if(!wcscmp(l_strName, L"ReportTime"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						m_iReportTime =  GetInt(ValueToString(l_strValue));
					}
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		m_bLoadTimeStampFial = false;
	}
	else
		m_bLoadTimeStampFial = true;
}

void cControlPanel::CheckMachineNumber()
{
	int		l_iMachineNumber = *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt();
	int64	l_i64ClearRecordValue = *m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordValue" )->GetInt64();
	if ( l_iMachineNumber == 0 )
	{
	}
}

void	cControlPanel::WriteTimeStamp()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(	CONTROLPANEL_TIMESTAMP) )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					if(!wcscmp(l_strName, L"TimeStamp"))
					{
						l_pElement->SetAttribute(L"Value", ValueToStringW(m_iTimeStamp));
					} 
					else if(!wcscmp(l_strName, L"ReportTime"))
					{
						l_pElement->SetAttribute(L"Value", ValueToStringW(m_iReportTime));
					}
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
	}
	l_VendorAskMachineStartStatusDataNodeISAX.Export( CONTROLPANEL_TIMESTAMP,g_bExportBinary);
}








void	cControlPanel::GetVersion()
{
	FMLOG("cControlPanel::GetVersion 0")
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(CONTROLPANEL_VERSION) )
	{
		FMLOG("cControlPanel::GetVersion 1")
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					m_wchVersion = (WCHAR*)l_pElement->Attribute(L"Value");
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		FMLOG("cControlPanel::GetVersion 2")
	}
}


void cControlPanel::SaveParameterData()
{

	for ( int i = 0; i < eParaAdjust_Max; i++ )
	{
		m_pnOldParameterAdjustListData[i] = m_pEventManager->m_pEventVariableManager->GetObject( ParaAdjustList[i] )->ConvertToString();
	}

	for ( int i = 0; i < eDecode_Max; i++ )
	{
		m_pnOldDecodeListData[i] = m_pEventManager->m_pEventVariableManager->GetObject( DecodeList[i] )->ConvertToString();
	}

	m_pEventManager->m_pEventVariableManager->Export( m_pEventManager->m_pEventVariableManager->GetFileName(),g_bExportBinary);
}








void cControlPanel::RestoreOldData()
{
	const WCHAR* NowPackageName = m_pEventPackageRunner->GetCurrentEventPackage()->GetName();

	if ( !wcscmp( NowPackageName, L"ParameterAdjust" ) )
	{		
		for ( int i = 0; i < eParaAdjust_Max; i++ )
		{
			m_pEventManager->m_pEventVariableManager->GetObject( ParaAdjustList[i] )->SetDataByString( m_pnOldParameterAdjustListData[i].c_str() );
		}
	}
	else if ( !wcscmp( NowPackageName, L"DecodeAdjust" ) )
	{		
		for ( int i = 0; i < eDecode_Max; i++ )
		{
			m_pEventManager->m_pEventVariableManager->GetObject( DecodeList[i] )->SetDataByString( m_pnOldDecodeListData[i].c_str() );
		}

	}
}









const wchar_t* GetNextReportTimeData( int64 e_iTime )
{
	return L"";
	//time_t t = (time_t)e_iTime;

	//tm *p = localtime( &t );

	//static WCHAR l_str[ MAX_PATH ];

	//swprintf( l_str, MAX_PATH, L"%04d/%02d/%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday );

	//return l_str;
}

void cControlPanel::ReportDelayTime()
{
	bool bDelayTime(0);

	if ( *m_pEventManager->m_pEventVariableManager->GetObject( L"DelayTimeFlag" )->GetInt() == 0 )
	{
		bDelayTime = true;
		m_pEventManager->m_pEventVariableManager->GetObject( L"DelayTimeFlag" )->SetInt(1);

		int64 NextReportTime = m_iReportTime + ( *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportDay" )->GetInt() * 86400 + ( 2 * 86400 ) );

		m_pEventManager->m_pEventVariableManager->GetObject( L"NextReportTime" )->SetWString( GetNextReportTimeData( NextReportTime ) );
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"DelayTimeResult" )->SetInt( bDelayTime ? 1 : 0 );

	m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"DelayTimeResult" );
}








void cControlPanel::CheckPassword()
{

}








void cControlPanel::CancelInputPassword()
{

}


void cControlPanel::AutoReport()
{

}




void cControlPanel::FigureCheckCodeWithMachineNumber()
{

}





int64	cControlPanel::GetReportInputResultValue()
{


	return 0;
}


void cControlPanel::CheckReportInputWithMachineNumber()
{

}








void cControlPanel::ReportInputResultClick()
{

}








void cControlPanel::ChangeInputValue( int InputKind, int ValueChangeType, int NumCount )
{

}








void cControlPanel::ChangeValueImg( cMPDI *pNumericTex, WCHAR* ChangeName[], int Num, int Pos )
{

}








void cControlPanel::CheckClearRecodePassword()
{

}

void	cControlPanel::RenderOverReportTime()
{
	if ( m_bShowReportTipImage && m_pReportTipImage )
		m_pReportTipImage->Render();
}

int64	cControlPanel::GetLatestRevenue()
{
	return 0;
}

int64	cControlPanel::GetTotalRevenue()
{
	
	
	return 0;
}