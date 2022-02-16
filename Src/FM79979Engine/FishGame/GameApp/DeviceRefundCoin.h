#ifndef _cRefoundCoinDevice_H_
#define _cRefoundCoinDevice_H_
//
//
class	cPlayerBehaviorBase;
//
class	cRefoundCoinDevice : public NamedTypedObject
{
	int					m_iExchangeDebugValue;
	int					m_iExchangeDebugValueTotal;
	void				DoExchangeIn(int e_iMultiplier);
private:
	int64				m_i64RestCoinToRefund;
	//over than 5 second show no coin messae
	UT::sTimeCounter	m_RefundToCheckCoinTC;
	//UT::sTimeCounter	m_Wait8051SingnalToActive;
	bool				m_bRefundCoin;
	bool				m_bRefundCoinStart;
	cBinaryFile*		m_pBinaryFile;
	bool				m_bWorking;//bool of return	
	cPlayerBehaviorBase*m_pPlayer;//Player

	cIOSMIOPackData*	m_pInsertCoinCount;//the coin player tosted

	cIOSMIOPackData*	m_pWantRefundCountDevice;//
	cIOSMIOPackData*	m_pCurrentRefundCountDevice;//rest coin or ticket refund count
	cIOSMIOPackData*	m_pExchangeInSequence;//now exchange button is store as a byte
	cIOSMIOData*		m_pResetWantRefundCountDevice;//reset refund device value as expect so device keep walking

	WCHAR				m_strReceive[MAX_PATH];
	int*				m_piCoinToScore;//
	int*				m_piRefundCoinOrTicketValue;//
	int*				m_piExchangeInScore;
	UT::sTimeCounter	m_RefundCoinKeyPressTimeTC;
	UT::sTimeCounter	m_ExchangeInKeyPressTimeTC;
	cClickMouseBehavior	m_RefundKey;//
	cClickMouseBehavior	m_ExchangeInKey;//
	cClickMouseBehavior	m_ExchangeOutKey;//
	int					m_playerID;
	void				Update_ExchangeBehaviod( float e_fElpaseTime );
	void				Update_CoinBehavior( float e_fElpaseTime );
	void				CoinRefund();
	//
public:
	cRefoundCoinDevice(
						cPlayerBehaviorBase*e_pPlayer,//player ID
						cIOSMIOPackData*e_pInsertCoinCount,//the coin player tosted
						cIOSMIOPackData*e_pHopperValue,//the coin player reufned wanted
						cIOSMIOPackData*e_pResetCoinToRefund,//rest coin for refund
						cIOSMIOPackData*e_pTicketValue,//the ticket refuned wanted
						cIOSMIOPackData*e_pRestTicketToRefund,//rest ticket for refund
						cIOSMIOPackData*e_pExchangeInSequence,//new stand for card reader machine
						cIOSMIOData*	e_pResetWantRefundCountCoinDevice,	//coin not enough send message to IO card reset rest coin  to 0
						cIOSMIOData*	e_pResetWantRefundCountTicketDevice,//ticket not enough send message to IO card reset rest coin  to 0
						int*e_piCoinToScore,//how many score for a coin
						int*e_piRefundCoinOrTicketValue,//
						unsigned char e_ucReturnKey,//refund coin button
						unsigned char e_ucOpenKey,//exchageInButton
						unsigned char e_ucWashKey//Exchange out button
					  );
	virtual ~cRefoundCoinDevice(); 
	void			Update(float e_fElpaseTime);
	void			Render();
};




class	cRefoundCoinDeviceManager : public cNamedTypedObjectVector<cRefoundCoinDevice>
{
private:
	//
public:
	cRefoundCoinDeviceManager();
	virtual ~cRefoundCoinDeviceManager();

	void	Init( const char *filename );
	void	Update( float e_fElpaseTime );
	void	Render();
	void	Destroy();
};


extern bool		LONG_PRESSED_BUTTON_TIME_FOR_REFUND_CHECK;
extern int		SAFE_REFUND_COIN_COUNT;
extern float	LONG_PRESSED_BUTTON_TIME_FOR_REFUND	;
extern float	LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_IN;
extern float	LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_OUT;

#endif