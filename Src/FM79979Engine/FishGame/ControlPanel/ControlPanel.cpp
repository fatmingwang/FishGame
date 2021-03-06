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
WCHAR* ParaAdjustList[ eParaAdjust_Max ] = { L"BulletPayRateLimit", L"BulletPayRateMinPoint", L"BulletPayRateAddPoint",L"RefundCoinOrTicket", L"Sound", L"PlayerCount"}; 
WCHAR* DecodeList[ eDecode_Max ] = { L"Probability", L"Exchange_In", L"CoinToScore", L"TicketToScore",L"MoneyLimit", L"ReportDay",L"PlaceType" }; 
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

cControlPanel::cControlPanel( char* e_strPackagesFileName, int ControlPanelType )
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
			m_pReportTipImage->SetWidth( ( int ) cGameApp::m_svGameResolution.x );
			m_pReportTipImage->SetHeight( ( int ) cGameApp::m_svGameResolution.y );
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
				this->m_bShowReportTipImage = true;
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








WCHAR*	cControlPanel::GetTime( int64 e_iTime )
{
	time_t t = (time_t)e_iTime;
	tm* p;
	p = localtime( &t );
	static WCHAR l_str[MAX_PATH];
	swprintf(l_str, MAX_PATH, L"%04d/%02d/%02d %02d:%02d:%02d", p->tm_year + 1900, 
		p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return l_str;
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
					::MessageBox(0,L"硬盤損毀請替換新硬盤",L"硬盤損毀!",MB_OK);
					return;
				}
			}
		}
		if(!m_pEventManager->m_pEventVariableManager->Parse(VARIABLE_FILE_NAME))
		{
			::MessageBox(0,L"硬盤損毀請替換新硬盤",L"硬盤損毀!",MB_OK);
			return;
		}
		else
		{
			if( l_bOpenBKFileOk == false )
				::MessageBox(0,L"資料遺失回復原廠設置",L"資料遺失回復原廠設置!",MB_OK);
			else
				::MessageBox(0,L"資料遺失已載入備份檔案",L"資料遺失已載入備份檔案!!",MB_OK);
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

		if ( m_iReportFlag == 1)
		{
			m_pEventManager->m_pEventFont->RenderFont( 875, 560, L"(打码报账已逾期)" );
		}
		else if ( m_iReportFlag == 2 )
		{
			m_pEventManager->m_pEventFont->RenderFont( 875, 560, L"(打码报账時間錯誤)" );
		}

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


		WCHAR MachineNumberStr[20] = {0};
		_itow( *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt(), MachineNumberStr, 10 );
		m_spGlyphSmallFontRender->RenderFont( 600, 60, MachineNumberStr );
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
		SYSTEMTIME LocalTime;

		memset( &LocalTime, 0, sizeof( LPSYSTEMTIME ) );

		GetSystemTime( &LocalTime );

		int MachineNumber = ( ( LocalTime.wYear * LocalTime.wMonth * LocalTime.wDay ) + ( LocalTime.wHour + LocalTime.wMinute + LocalTime.wSecond + ( LocalTime.wMilliseconds * 1000 ) ) ) % 100000;
		m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->SetInt( MachineNumber );

		int MachinePosNum[5] = {0};

		int Idx(0);

		while ( MachineNumber )
		{
			MachinePosNum[ Idx++ ] = MachineNumber % 10;
			MachineNumber /= 10;
		}

		int64 ClearRecodeNumber(0);

		for ( int i = 0; i < ( sizeof( MachinePosNum ) / sizeof( MachinePosNum[0] ) ) - 1; i++ )
			 ClearRecodeNumber += ( int64 ) ( MachinePosNum[i] * pow( 10.f, MachinePosNum[ i + 1 ] ) );

		ClearRecodeNumber = ClearRecodeNumber % 10000000000;
		ClearRecodeNumber = 9999999999 - ClearRecodeNumber;

		m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordValue" )->SetInt64( ClearRecodeNumber );

		m_pEventManager->m_pEventVariableManager->Export( m_pEventManager->m_pEventVariableManager->GetFileName(),g_bExportBinary);
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

	HANDLE hFile = CreateFile( UT::CharToWchar(TIME_STAMP_FILE_NAME).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		std::wstring	l_str = UT::CharToWchar(TIME_STAMP_FILE_NAME);
		CopyFile( l_str.c_str(), UT::CharToWchar(TIME_STAMP_BACKUP_FILE_NAME).c_str(), false );
		CloseHandle( hFile );
	}
	else
	{
		HANDLE hFile = CreateFile( UT::CharToWchar(TIME_STAMP_FILE_NAME).c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			std::wstring	l_str = UT::CharToWchar(TIME_STAMP_FILE_NAME);
			CopyFile( UT::CharToWchar(TIME_STAMP_BACKUP_FILE_NAME).c_str(), l_str.c_str(), false );
			CloseHandle( hFile );
		}
	}
}








void	cControlPanel::GetVersion()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(CONTROLPANEL_VERSION) )
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
					m_wchVersion = (WCHAR*)l_pElement->Attribute(L"Value");
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
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









WCHAR* GetNextReportTimeData( int64 e_iTime )
{
	time_t t = (time_t)e_iTime;

	tm *p = localtime( &t );

	static WCHAR l_str[ MAX_PATH ];

	swprintf( l_str, MAX_PATH, L"%04d/%02d/%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday );

	return l_str;
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
	WCHAR InputPasswordStr[ MAX_PATH ], NextPageName[ MAX_PATH ] = {0};

	swprintf( InputPasswordStr, MAX_PATH, L"%d%", *m_pEventManager->m_pEventVariableManager->GetObject( L"InputPasswordValue" )->GetInt64() );

	bool bOK = ( !wcscmp( InputPasswordStr, m_pEventManager->m_pEventVariableManager->GetObject( L"VendorPassword" )->GetWString()->c_str() ) ) ? true : false;

	const WCHAR *pNextPageName = m_pEventManager->m_pEventVariableManager->GetObject( L"VendorMenuSelectedIndex" )->GetWString()->c_str();

	if ( !wcscmp( pNextPageName, L"ParameterAdjust" ) )
	{
		if ( bOK )
		{
			swprintf( NextPageName, MAX_PATH, L"MainMenu" );
		}
	}
	else if ( !wcscmp( pNextPageName, L"Setup" ) )
	{
		int ChangePasswordStep = *m_pEventManager->m_pEventVariableManager->GetObject( L"ChangePasswordStep" )->GetInt();

		if ( ChangePasswordStep == eChangePasswordNone )
		{
			if ( bOK )
			{
				swprintf( NextPageName, MAX_PATH, L"Setup" );
			}
		}
		else if ( ChangePasswordStep == eInputOldPassword )
		{
			if ( bOK )
			{
				ChangePasswordStep = eInputNewPassword;
				swprintf( NextPageName, MAX_PATH, L"InputNewPassword" );
			}
		}
		else if ( ChangePasswordStep == eInputNewPassword )
		{
			bOK = true;
			ChangePasswordStep = eConfirmPassword;
			m_pEventManager->m_pEventVariableManager->GetObject( L"NewPassword" )->SetWString( ValueToStringW( *m_pEventManager->m_pEventVariableManager->GetObject( L"InputPasswordValue" )->GetInt64() ) );
			swprintf( NextPageName, MAX_PATH, L"ConfirmNewPassword" );
		}
		else if ( ChangePasswordStep == eConfirmPassword )
		{
			if ( !wcscmp( ValueToStringW( *m_pEventManager->m_pEventVariableManager->GetObject( L"InputPasswordValue" )->GetInt64() ).c_str(), m_pEventManager->m_pEventVariableManager->GetObject( L"NewPassword" )->GetWString()->c_str() ) )
			{
				bOK = true;
				ChangePasswordStep = eChangePasswordNone;
				m_pEventManager->m_pEventVariableManager->GetObject( L"VendorPassword" )->SetWString( m_pEventManager->m_pEventVariableManager->GetObject( L"NewPassword" )->GetWString()->c_str() );
				swprintf( NextPageName, MAX_PATH, L"ChangePasswordOK" );
			}
			else
			{
				bOK = false;
			}
		}

		m_pEventManager->m_pEventVariableManager->GetObject( L"ChangePasswordStep" )->SetInt( ChangePasswordStep );
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"CheckPasswordResult" )->SetInt( bOK ? 1 : 0 );

	if ( bOK && wcscmp( NextPageName, L"" ) )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"InputPasswordValue"   )->SetInt64(0);
		m_pEventManager->m_pEventVariableManager->GetObject( L"PasswordUnderlineIdx" )->SetInt(0);
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName"   )->SetWString( NextPageName );
	}
}








void cControlPanel::CancelInputPassword()
{
	WCHAR NextPageName[ MAX_PATH ] = {0};

	if ( !wcscmp( m_pEventManager->m_pEventVariableManager->GetObject( L"VendorMenuSelectedIndex" )->GetWString()->c_str(), L"ParameterAdjust" ) )
	{
		swprintf( NextPageName, MAX_PATH, L"MainMenu" );
	} 
	else
	{
		int ChangePasswordStep = *m_pEventManager->m_pEventVariableManager->GetObject( L"ChangePasswordStep" )->GetInt();

		switch ( ChangePasswordStep )
		{
		case eChangePasswordNone:
			{
				swprintf( NextPageName, MAX_PATH, L"MainMenu" );
			}
			break;

		case eInputOldPassword: case eInputNewPassword: case eConfirmPassword:
			{
				swprintf( NextPageName, MAX_PATH, L"Setup" );
			}
			break;

		default: return;
		} //end switch
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"ChangePasswordStep"   )->SetInt(0);
	m_pEventManager->m_pEventVariableManager->GetObject( L"InputPasswordValue"   )->SetInt64(0);
	m_pEventManager->m_pEventVariableManager->GetObject( L"PasswordUnderlineIdx" )->SetInt(0);
	m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName"   )->SetWString( NextPageName );
}


void cControlPanel::AutoReport()
{
	const WCHAR* LatestValueStr[ eAuditList_Max ]          = { L"Latest_TotalIncome",  L"Latest_TotalPay",      L"Latest_ExchangeIn",      L"Latest_ExchangeOut",      L"Latest_Ticket",      L"Latest_ToastCoin",   L"Latest_RefudCoin",    L"Latest_Revenue" }; 
	const WCHAR* VariableAuditLatestStrA[ eAuditList_Max ] = { L"Audit_LatestInMoney", L"Audit_LatestOutMoney", L"Audit_LatestExchangeIn", L"Audit_LatestExchangeOut", L"Audit_LatestTicket", L"Audit_LatestCoinIn", L"Audit_LatestCoinOut", L"Audit_LatestRevenue" }; 

	for ( int i = 0; i < eAuditList_Max; i++ )
	{
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStrA[i] )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( LatestValueStr[i] )->GetInt64() / GetCoinToScore() ); 
	}

	const WCHAR* VariableAuditTotalStrA[ eAuditList_Max  ]    = { L"TotalIncome",       L"TotalPay",           L"TotalExchangeIn",      L"TotalExchangeOut",      L"TotalTicket",      L"TotalToastCoin",   L"TotalRefudCoin",    L"TotalRevenue" }; 
	const WCHAR* VariableAuditShowTotalStrA[ eAuditList_Max ] = { L"Show_TotalInMoney", L"Show_TotalOutMoney", L"Show_TotalExchangeIn", L"Show_TotalExchangeOut", L"Show_TotalTicket", L"Show_TotalCoinIn", L"Show_TotalCoinOut", L"Show_TotalRevenue" }; 

	for ( int i = 0; i < eAuditList_Max; i++ )
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditShowTotalStrA[i] )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStrA[i] )->GetInt64() + *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStrA[i] )->GetInt64() );

	m_iReportFlag = 0;
	m_iReportCount++;

	m_pEventManager->m_pEventVariableManager->GetObject( L"ReportCount" )->SetInt( m_iReportCount );

	m_iTimeStamp = m_iReportTime = GetTimeForInt64();
	WriteTimeStamp();

	const WCHAR* VariableAuditPriorStr[ eAuditList_Max  ]    = { L"Prior_TotalIncome",   L"Prior_TotalPay",       L"Prior_ExchangeIn",       L"Prior_ExchangeOut",       L"Prior_Ticket",       L"Prior_ToastCoin",    L"Prior_RefudCoin",     L"Prior_Revenue" }; 
	const WCHAR* VariableAuditTotalStr[ eAuditList_Max  ]    = { L"TotalIncome",         L"TotalPay",             L"TotalExchangeIn",        L"TotalExchangeOut",        L"TotalTicket",        L"TotalToastCoin",     L"TotalRefudCoin",      L"TotalRevenue" }; 
	const WCHAR* VariableAuditLatestStr[ eAuditList_Max ]    = { L"Audit_LatestInMoney", L"Audit_LatestOutMoney", L"Audit_LatestExchangeIn", L"Audit_LatestExchangeOut", L"Audit_LatestTicket", L"Audit_LatestCoinIn", L"Audit_LatestCoinOut", L"Audit_LatestRevenue" }; 
	const WCHAR* VariableAuditShowTotalStr[ eAuditList_Max ] = { L"Show_TotalInMoney",   L"Show_TotalOutMoney",   L"Show_TotalExchangeIn",   L"Show_TotalExchangeOut",   L"Show_TotalTicket",   L"Show_TotalCoinIn",   L"Show_TotalCoinOut",   L"Show_TotalRevenue" }; 

	for ( int i = 0; i < eAuditList_Max; i++ )
	{
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditPriorStr[i]  )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->GetInt64() );
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i]  )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i] )->GetInt64() + *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->GetInt64() );
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->SetInt64(0);
	}

	ClearLatestRecord(false);
	//cFishApp::m_spProbabilityFish->ClearLatestRecord();

	for ( int i = 0; i < eAuditList_Max; i++ )
	{
		 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditShowTotalStr[i] )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i] )->GetInt64() );
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"DelayTimeFlag" )->SetInt(0);
}




void cControlPanel::FigureCheckCodeWithMachineNumber()
{
	int64 TotalRevenue  = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalRevenue" )->GetInt64() );
	int64 LatestRevenue = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestRevenue" )->GetInt64() );
	int64 PriorRevenue  = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_Revenue" )->GetInt64() );

	int	MachineNumber = *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt();
	int	ReportCount   = *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportCount" )->GetInt();

	int64 CheckCode = ( int64 ) MachineNumber | ( ( int64 ) ( ReportCount * pow( ( float ) 2, 17 ) ) );

	int BitSetA(0), BitSetB(0);

	int TempNumber = MachineNumber;

	bool bBisetValueA[17]= { 0 }; 

	for ( int i = 0; i < 17; i++ )
	{
		 bBisetValueA[i] = ( ( TempNumber & 0x01 ) == 1 ) ? true : false;
		 TempNumber /= 2;
	}

	for ( int i = 0; i < 17; i++ )
	{
		 BitSetA = BitSetA ^ ( bBisetValueA[i] ? 0x01 : 0x00 );	
	}

	TempNumber = ReportCount;

	bool bBisetValueB[10]= { 0 }; 

	for ( int i = 0; i < 10; i++ )
	{
		 bBisetValueB[i] = ( ( TempNumber & 0x01 ) == 1 ) ? true : false;
		 TempNumber /= 2;		 
	}

	for ( int i = 0; i < 10; i++ )
		 BitSetB = BitSetB ^ ( bBisetValueB[i] ? 0x01 : 0x00 );

	BitSetA = ( BitSetA << 27 ) | ( BitSetB << 28 );

	CheckCode = CheckCode | BitSetA;

	BitSetA = 0;

	while ( 1 )
	{
		if ( TotalRevenue )
		{
			BitSetA += ( TotalRevenue % 10 );
			TotalRevenue /= 10;
		}
		else if ( LatestRevenue )
		{
			BitSetA += ( LatestRevenue % 10 );
			LatestRevenue /= 10;
		}
		else if ( PriorRevenue )
		{
			BitSetA += ( PriorRevenue % 10 );
			PriorRevenue /= 10;
		}
		else
		{
			break;
		}
	}

	BitSetA %= 10;

	if ( BitSetA == 9 )
	{
		BitSetA = 9;
	}
	else if ( BitSetA == 0 )
	{
		BitSetA = 8;
	}
	else
	{
		BitSetA = 8 - BitSetA;
	}

	CheckCode = CheckCode + ( ( int64 ) ( pow( ( float ) 10, ( float ) 9 ) * BitSetA ) );

	m_pEventManager->m_pEventVariableManager->GetObject( L"CheckNumber" )->SetInt64( CheckCode );
}





int64	cControlPanel::GetReportInputResultValue()
{
	int64 TotalRevenue  = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalRevenue" )->GetInt64() );
	int64 LatestRevenue = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestRevenue" )->GetInt64() );
	int64 PriorRevenue  = abs( ( long ) *m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_Revenue" )->GetInt64() );

	int	MachineNumber = *m_pEventManager->m_pEventVariableManager->GetObject( L"MachineNumber" )->GetInt();
	int	ReportCount   = *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportCount" )->GetInt();

	int64 CheckCode = MachineNumber | ( int ) ( ReportCount * pow( ( float ) 2, 17 ) );

	int BitSetA(0), BitSetB(0);

	int TempNumber = MachineNumber;

	bool bBisetValueA[17]= { 0 }; 

	for ( int i = 0; i < 17; i++ )
	{
		 bBisetValueA[i] = ( ( TempNumber & 0x01 ) == 1 ) ? true : false;
		 TempNumber /= 2;
	}

	for ( int i = 0; i < 17; i++ )
	{
		 BitSetA = BitSetA ^ ( bBisetValueA[i] ? 0x01 : 0x00 );	
	}

	TempNumber = ReportCount;

	bool bBisetValueB[10]= { 0 }; 

	for ( int i = 0; i < 10; i++ )
	{
		 bBisetValueB[i] = ( ( TempNumber & 0x01 ) == 1 ) ? true : false;
		 TempNumber /= 2;		 
	}

	for ( int i = 0; i < 10; i++ )
		 BitSetB = BitSetB ^ ( bBisetValueB[i] ? 0x01 : 0x00 );

	BitSetA = ( BitSetA << 27 ) | ( BitSetB << 28 );

	CheckCode = CheckCode | BitSetA;

	CheckCode %= 1000000000;

	int64 ResultValue = TotalRevenue ^ ( LatestRevenue * 2 ) ^ ( PriorRevenue * 3 ) ^ ( CheckCode * 5 );

	ResultValue = ResultValue << 8;

	ResultValue = ResultValue | ( ( ( ReportCount & 0x07 ) << 5 ) | ( MachineNumber & 0x1f ) );

	ResultValue = ResultValue & 0xFFFFFFFF;

	return ResultValue;
}


void cControlPanel::CheckReportInputWithMachineNumber()
{
	int64	ResultValue = GetReportInputResultValue();
	int64 ReportValue = *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportInputValue" )->GetInt64();

	bool bReportSuccess(0);

	if ( ReportValue == ResultValue )
	{
		bReportSuccess = true;

		m_iReportFlag = 0;

		m_pEventManager->m_pEventVariableManager->GetObject( L"ReportFlag" )->SetInt( m_iReportFlag );

		m_iReportCount++;

		m_pEventManager->m_pEventVariableManager->GetObject( L"ReportCount" )->SetInt( m_iReportCount );

		m_iTimeStamp = m_iReportTime = GetTimeForInt64();

		m_pEventManager->m_pEventVariableManager->GetObject( L"LastReportTime" )->SetWString( GetTime( m_iReportTime ) );

		WriteTimeStamp();

		const WCHAR* VariableAuditPriorStr[ eAuditList_Max  ]    = { L"Prior_TotalIncome",   L"Prior_TotalPay",       L"Prior_ExchangeIn",       L"Prior_ExchangeOut",       L"Prior_Ticket",       L"Prior_ToastCoin",    L"Prior_RefudCoin",     L"Prior_Revenue" }; 
		const WCHAR* VariableAuditTotalStr[ eAuditList_Max  ]    = { L"TotalIncome",         L"TotalPay",             L"TotalExchangeIn",        L"TotalExchangeOut",        L"TotalTicket",        L"TotalToastCoin",     L"TotalRefudCoin",      L"TotalRevenue" }; 
		const WCHAR* VariableAuditLatestStr[ eAuditList_Max ]    = { L"Audit_LatestInMoney", L"Audit_LatestOutMoney", L"Audit_LatestExchangeIn", L"Audit_LatestExchangeOut", L"Audit_LatestTicket", L"Audit_LatestCoinIn", L"Audit_LatestCoinOut", L"Audit_LatestRevenue" }; 
		const WCHAR* VariableAuditShowTotalStr[ eAuditList_Max ] = { L"Show_TotalInMoney",   L"Show_TotalOutMoney",   L"Show_TotalExchangeIn",   L"Show_TotalExchangeOut",   L"Show_TotalTicket",   L"Show_TotalCoinIn",   L"Show_TotalCoinOut",   L"Show_TotalRevenue" }; 

		for ( int i = 0; i < eAuditList_Max; i++ )
		{
			 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditPriorStr[i]  )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->GetInt64() );
			 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i]  )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i] )->GetInt64() + *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->GetInt64() );
			 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditLatestStr[i] )->SetInt64(0);
		}

		ClearLatestRecord(false);
		//cFishApp::m_spProbabilityFish->ClearLatestRecord();

		for ( int i = 0; i < eAuditList_Max; i++ )
		{
			 m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditShowTotalStr[i] )->SetInt64( *m_pEventManager->m_pEventVariableManager->GetObject( VariableAuditTotalStr[i] )->GetInt64() );
		}

		m_bShowReportTipImage = false;
		m_pEventManager->m_pEventVariableManager->GetObject( L"DelayTimeFlag" )->SetInt(0);
		m_pEventManager->m_pEventVariableManager->Export(VARIABLE_FILE_NAME,g_bExportBinary);
		m_pEventManager->m_pEventVariableManager->Export(VARIABLE_BACKUP_FILE_NAME,g_bExportBinary);
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"ReportInputValue"        )->SetInt64(0);
	m_pEventManager->m_pEventVariableManager->GetObject( L"ReportInputResult"       )->SetInt( bReportSuccess ? 1 : 0 );
	m_pEventManager->m_pEventVariableManager->GetObject( L"ReportInputUnderlineIdx" )->SetInt(0);
	m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName"      )->SetWString( L"ReportResult" );
}








void cControlPanel::ReportInputResultClick()
{
	//			<cEventButtonSelectedActiver Name="77" cEventVariable="CurrentPackageName" Result="ClearReportRecord"/>
	const WCHAR	*NowPage = m_pEventManager->m_pEventVariableManager->GetObject( L"VendorMenuSelectedIndex" )->GetWString()->c_str();

	if ( *m_pEventManager->m_pEventVariableManager->GetObject( L"ReportInputResult" )->GetInt() == 1 )
	{
		if ( !wcscmp( NowPage, L"Setup" ) )
		{
			m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"ClearReportRecord" );
		}
		else
		{
			m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"MainMenu" );
			this->StoreOldData();
		}
	}
	else if ( !wcscmp( NowPage, L"ParameterAdjust" ) )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"MainMenu" );
	}
	else if ( !wcscmp( NowPage, L"DecodeAdjust" ) )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"DecodeAdjust" );
	}
	else if ( !wcscmp( NowPage, L"ReportAccount" ) )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"ReportAccount" );
	}
	else if ( !wcscmp( NowPage, L"Setup" ) )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"Setup" );
	}
	else
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"ReportInput" );
	}
}








void cControlPanel::ChangeInputValue( int InputKind, int ValueChangeType, int NumCount )
{
	if ( ( ValueChangeType != eChangeInputValueType_Decrease && ValueChangeType != eChangeInputValueType_Increase ) || ( InputKind > eChangeInputValueKind_Max ) )
	{
		return;
	}

	WCHAR *ValueName[ eChangeInputValueKind_Max ]     = { L"InputPasswordValue",   L"BarcodeInputValue",        L"BarcodeInputValueB",        L"ReportInputValue",        L"ClearRecordInputValue" };
	WCHAR *UnderlineName[ eChangeInputValueKind_Max ] = { L"PasswordUnderlineIdx", L"BarcodeInputUnderlineIdx", L"BarcodeInputUnderlineIdxB", L"ReportInputUnderlineIdx", L"ClearRecordInputUnderlineIdx" }; 

	int64 NumValue     = *m_pEventManager->m_pEventVariableManager->GetObject( ValueName[ InputKind ] )->GetInt64();
	int   UnderlinePos = *m_pEventManager->m_pEventVariableManager->GetObject( UnderlineName[ InputKind ] )->GetInt();

	int OriPos = UnderlinePos;

	int Number[10];
	memset( &Number[0], 0, sizeof( Number ) );

	int i(0);

	while ( NumValue )
	{
		 Number[i] = NumValue % 10;
		 NumValue /= 10;
		 i++;
	}

	if ( UnderlinePos >= 0 && UnderlinePos < NumCount )
	{
		UnderlinePos = NumCount - UnderlinePos - 1;

		if ( ValueChangeType == eChangeInputValueType_Increase  ) //Increase
		{
			Number[ UnderlinePos ]--;

			if ( Number[ UnderlinePos ] < 0 )
			{
				Number[ UnderlinePos ] = 9;
			}
		}
		else if ( ValueChangeType == eChangeInputValueType_Decrease ) //Decrease
		{
			Number[ UnderlinePos ]++;

			if ( Number[ UnderlinePos ] > 9 )
			{
				Number[ UnderlinePos ] = 0;
			}
		}

		NumValue = 0;

		for ( int i = NumCount - 1; i >= 0; i-- )
		{
			 NumValue *= 10;
			 NumValue += Number[i];
		}

		m_pEventManager->m_pEventVariableManager->GetObject( ValueName[ InputKind ] )->SetInt64( NumValue );

		WCHAR *NumericImgName[ eChangeInputValueKind_Max ] = { L"MainInputPasswordNumericImg", L"BarcodeInputNumericImg", L"BarcodeInputNumericImgB", L"ReportInputNumericImg", L"ClearRecordNumericImg" };

		cObjectAndName< cEventObject >* pEventObject = m_pEventManager->m_pEventPackageRunner->GetCurrentEventPackage()->GetObject( NumericImgName[ InputKind ] )->GetCurrentWorkingObject();

		cEventMPDI *pEventMPDI = ( cEventMPDI* ) pEventObject->GetObject();

		if ( pEventMPDI )
		{
			cMPDI *pNumericTex = pEventMPDI->GetMPDI();
			
			if ( pNumericTex )
			{
				if ( InputKind == eChangeInputValueKind_Password )
				{
					WCHAR* ChangeName[5] = { L"ReNum", L"ReNum2", L"ReNum4", L"ReNum6", L"ReNum8" };
					ChangeValueImg( pNumericTex, ChangeName, Number[ UnderlinePos ], OriPos );
				}
				else if ( InputKind == eChangeInputValueKind_Barcode || InputKind == eChangeInputValueKind_ClearRecord )
				{
					WCHAR* ChangeName[10] = { L"ReNum", L"ReNum1", L"ReNum2", L"ReNum3", L"ReNum4", L"ReNum5", L"ReNum6", L"ReNum7", L"ReNum8", L"ReNum9" };
					ChangeValueImg( pNumericTex, ChangeName, Number[ UnderlinePos ], OriPos );
				}
				else if ( InputKind == eChangeInputValueKind_BarcodeB )
				{
					WCHAR* ChangeName[10] = { L"ReNum", L"ReNum1", L"ReNum2", L"ReNum3", L"ReNum4", L"ReNum5", L"ReNum6", L"ReNum7" };
					ChangeValueImg( pNumericTex, ChangeName, Number[ UnderlinePos ], OriPos );
				}
				else if ( InputKind == eChangeInputValueKind_Report )
				{
					WCHAR* ChangeName[10] = { L"ReNum", L"ReNum1", L"ReNum2", L"ReNum3", L"ReNum4", L"ReNum5", L"ReNum6", L"ReNum7", L"ReNum8", L"ReNum9" };
					ChangeValueImg( pNumericTex, ChangeName, Number[ UnderlinePos ], OriPos );
				}
			}
		}
	}
}








void cControlPanel::ChangeValueImg( cMPDI *pNumericTex, WCHAR* ChangeName[], int Num, int Pos )
{
	std::wstring ShowNum = UT::ComposeMsgByFormat( L"ReNum%d", Num );

	int NumIdx = pNumericTex->GetPuzzleImage( L"background" )->GetObjectIndexByName( ShowNum.c_str() );

	pNumericTex->GetObject( ChangeName[ Pos ] )->ChangeAllImageByImageIndex( NumIdx );
	pNumericTex->GetObject( ChangeName[ Pos ] )->GetCurrentPointData()->iImageIndex = NumIdx;
}








void cControlPanel::CheckClearRecodePassword()
{
	int64 InputValue       = *m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordInputValue" )->GetInt64();
	//int64 ClearRecordValue = *m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordValue" )->GetInt64();
	int64 ClearRecordValue = 1234567890;

	bool bResult = InputValue == ClearRecordValue ? true : false;

	if ( bResult )
	{
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_TotalIncome" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_TotalPay" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_Revenue" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_ExchangeIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_ExchangeOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_Ticket" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_ToastCoin" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Latest_RefudCoin" )->SetInt64( 0 );

		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalIncome" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalPay" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalExchangeIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalExchangeOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalTicket" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalToastCoin" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalRefudCoin" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"TotalRevenue" )->SetInt64( 0 );

		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestInMoney" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestOutMoney" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestExchangeIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestExchangeOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestTicket" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestCoinIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestCoinOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Audit_LatestRevenue" )->SetInt64( 0 );

		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_TotalIncome" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_TotalPay" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_ExchangeIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_ExchangeOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_Ticket" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_ToastCoin" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_RefudCoin" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Prior_Revenue" )->SetInt64( 0 );

		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalInMoney" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalOutMoney" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalExchangeIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalExchangeOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalTicket" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalCoinIn" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalCoinOut" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"Show_TotalRevenue" )->SetInt64( 0 );

		m_pEventManager->m_pEventVariableManager->Export( m_pEventManager->m_pEventVariableManager->GetFileName(),g_bExportBinary);

		m_bGameReload = true;
		m_bInControlPanelMode = false;

		ClearLatestRecord(true);
		//cFishApp::m_spProbabilityFish->ClearReportRecord();

		m_iTimeStamp  = 0;
		m_iReportTime = 0;

		m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordInputValue" )->SetInt64( 0 );
		m_pEventManager->m_pEventVariableManager->GetObject( L"ClearRecordValue" )->SetInt64( 0 );
		WriteTimeStamp();
	}

	m_pEventManager->m_pEventVariableManager->GetObject( L"CurrentPackageName" )->SetWString( L"ClearReportResult" );
}

void	cControlPanel::RenderOverReportTime()
{
	if ( m_bShowReportTipImage && m_pReportTipImage )
		m_pReportTipImage->Render();
}

int64	cControlPanel::GetLatestRevenue()
{
	int	l_iCoinToScore = GetCoinToScore();
	int64	l_i64TotalRevenue		= *m_pi64LatestRevenue/l_iCoinToScore;
	int64	l_i64TotalToastCoin		= *m_pi64LatestCoinIn/l_iCoinToScore;
	int64	l_i64TotalRefudCoin		= *m_pi64LatestCoinOut/l_iCoinToScore;
	int64	l_i64TotalExchangeIn	= *m_pi64LatestExchangeIn/l_iCoinToScore;
	int64	l_i64TotalExchangeOut	= *m_pi64LatestExchangeOut/l_iCoinToScore;
	int64	l_i64TotalTicket		= *m_pi64LatestTicket/l_iCoinToScore;
	int64	l_i64LatestRevenue		= *m_pi64LatestRevenue/l_iCoinToScore;
	return l_i64TotalToastCoin+l_i64TotalExchangeIn-l_i64TotalRefudCoin-l_i64TotalExchangeOut-l_i64TotalTicket;
}

int64	cControlPanel::GetTotalRevenue()
{
	int	l_iCoinToScore = GetCoinToScore();
	int64	l_i64TotalRevenue		= *m_pi64LatestRevenue/l_iCoinToScore;
	int64	l_i64TotalToastCoin		= *m_pi64LatestCoinIn/l_iCoinToScore;
	int64	l_i64TotalRefudCoin		= *m_pi64LatestCoinOut/l_iCoinToScore;
	int64	l_i64TotalExchangeIn	= *m_pi64LatestExchangeIn/l_iCoinToScore;
	int64	l_i64TotalExchangeOut	= *m_pi64LatestExchangeOut/l_iCoinToScore;
	int64	l_i64TotalTicket		= *m_pi64LatestTicket/l_iCoinToScore;
	int64	l_i64LatestRevenue		= *m_pi64LatestRevenue/l_iCoinToScore;
	l_i64TotalRevenue		+=	*GetEventManager()->GetEventVariable(L"TotalRevenue")->GetInt64();
	l_i64TotalToastCoin		+=	*GetEventManager()->GetEventVariable(L"TotalToastCoin")->GetInt64();
	l_i64TotalRefudCoin		+=	*GetEventManager()->GetEventVariable(L"TotalRefudCoin")->GetInt64();
	l_i64TotalExchangeIn	+=	*GetEventManager()->GetEventVariable(L"TotalExchangeIn")->GetInt64();
	l_i64TotalExchangeOut	+=	*GetEventManager()->GetEventVariable(L"TotalExchangeOut")->GetInt64();
	l_i64TotalTicket		+=	*GetEventManager()->GetEventVariable(L"TotalTicket")->GetInt64();	
	
	return l_i64TotalToastCoin+l_i64TotalExchangeIn-l_i64TotalRefudCoin-l_i64TotalExchangeOut-l_i64TotalTicket;
}