#pragma once
//
#define eAuditList_Max 8
//

enum eParameterAdjustList
{
	eParaAdjust_PayRateLimit,
	eParaAdjust_PayRateMinPoint,
	eParaAdjust_PayRateAddPoint,
	eParaAdjust_RefundCoinOrTicket,
	eParaAdjust_SoundVolume,
	eParaAdjust_PlayerCount,
	eParaAdjust_Max,
};

enum eDecodeList
{
	eDecode_Probability,
	eDecode_ExchangeIn,
	eDecode_CoinToScore,
	eDecode_TicketToScore,
	eDecode_MoneyLimit,
	eDecode_ReportDay,
	eDecode_PlaceType,
	eDecode_Max,
};
extern WCHAR* ParaAdjustList[ eParaAdjust_Max ] ;
extern WCHAR* DecodeList[ eDecode_Max ]; 
enum eChangePasswordStep
{
	eChangePasswordNone,
	eInputOldPassword,
	eInputNewPassword,
	eConfirmPassword,
};

enum eChangeInputValueKind
{
	eChangeInputValueKind_Password,
	eChangeInputValueKind_Barcode,
	eChangeInputValueKind_BarcodeB,
	eChangeInputValueKind_Report,
	eChangeInputValueKind_ClearRecord,
	eChangeInputValueKind_Max,
};

enum eChangeInputValueType
{
	eChangeInputValueType_Increase,
	eChangeInputValueType_Decrease,
};



class	cControlPanel
{
	//avoid file write failed
	void						FileBackup();
public:
	//while data changed reload game
	bool						m_bGameReload;
	void						SoundCheck();
	//
	void						InternalReset();
	//
	cEventManager*				m_pEventManager;
	cEventPackageRunner*		m_pEventPackageRunner;
	bool						m_bInControlPanelMode;
	void						IntoControlPanel();

	cGlyphFontRender*			m_spGlyphSmallFontRender;

	int							m_iReportDay;
	float						m_fReportRoutine;
	int							m_iSaveTime;
	
	void						CheckReport( bool bLoadReportInput = false ); //Check Report Count & Date

	int64*						m_pi64LatestInMoney;
	int64*						m_pi64LatestOutMoney;
	int64*						m_pi64LatestRevenue;
	int64*						m_pi64LatestExchangeIn;
	int64*						m_pi64LatestExchangeOut;
	int64*						m_pi64LatestTicket;
	int64*						m_pi64LatestCoinIn;
	int64*						m_pi64LatestCoinOut;

	void						GetTimeStamp();
	
	void						GetVersion();
	//
	cControlPanel( char* e_strPackagesFileName, int ControlPanelType );
	virtual ~cControlPanel();
	//
	void	Update(float e_fElpaseTime);
	void	Render();
	bool	IsInControlPanelMode();
	void	MouseDown(int e_iPosX,int e_iPosY);
	void	MouseMove(int e_iPosX,int e_iPosY);
	void	MouseUp(int e_iPosX,int e_iPosY);

	cEventManager*				GetEventManager(){return m_pEventManager;}
	int							m_iSound;
	//int							m_fProbability;
	float						m_RealProbility;
	int							m_iPlayerCount;
	int							m_iBulletPayRateLimit;
	int							m_iBulletPayRateMinPoint;
	int							m_iBulletPayRateAddPoint;
	int64						m_i64MoneyLimit;

	int							GetCoinToScore();//one coin for how many credit
	//while control panel mode is on,store the old data
	//and compare necessary data if control panel mode is off
	void						StoreOldData();

	int64						m_iReportTime;
	int							m_iReportFlag;
	int							m_iReportCount;

	int64						GetTimeForInt64();
	WCHAR*						GetTime(int64 e_iTime);
	int64						GetRestTimeToReport();
	void						WriteTimeStamp();

	std::wstring				m_wchVersion;

	std::wstring				m_pnOldParameterAdjustListData[ eParaAdjust_Max ];
	std::wstring				m_pnOldDecodeListData[ eDecode_Max ];

	int64						m_iTimeStamp;

	bool						m_bAjustSetting;
	bool						m_bLoadTimeStampFial;
	bool						m_bPressOpenControlPanel;

	float						m_fOpenControlPanelPressTm;

	bool						m_bShowReportTipImage;
	cBaseImage*					m_pReportTipImage;

	void ReloadGame();

	void SaveParameterData();
	void RestoreOldData();

	void ReportDelayTime();

	void AutoReport();
	void CheckPassword();
	void CancelInputPassword();

	int64	GetReportInputResultValue();

	void CheckReportInputWithMachineNumber();
	void FigureCheckCodeWithMachineNumber();
	void ReportInputResultClick();
	void GenerateAuditRandCode();

	void ChangeInputValue( int InputKind, int ValueChangeType, int NumCount );
	void ChangeValueImg( cMPDI *pNumericTex, WCHAR* ChangeName[], int Num, int Pos );

	void CheckMachineNumber();
	void CheckClearRecodePassword();
	void RenderOverReportTime();

	int64	GetLatestRevenue();
	int64	GetTotalRevenue();
};