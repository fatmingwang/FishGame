#ifndef	_PLAYER_DATA_H_
#define	_PLAYER_DATA_H_


<PlayerData>
	<Item Maxium="" RecoverTime="">
</PlayerData>

class	cPlayerData
{
	//Calendar c = Calendar.getInstance(); 
	//int seconds = c.get(Calendar.SECOND);
	//http://stackoverflow.com/questions/13064750/how-to-get-current-time-from-internet-in-android
	UINT					m_LastTimeGetHeart;//write to file to compare current to next heart get time.
	float					m_fTimeToGetNewHeart;
	//
	int						m_iNumGlasses;
	int						m_iNumHearts;
	int						m_iMaximumGlasses;
	int						m_iMaximumHearts;

	void					ParsePlayerSaveFile();
	void					Write();
	cPuzzleImage*			m_pPI;
	cBaseImage*				m_pGlasses;
	cBaseImage*				m_pHearts;
	cNumeralImage*			m_pNumeralImage;
	//
	GET_SET_DEC(bool,m_bItemClickEnable,IsItemClickEnable,SetItemClickEnable);
	//
	cSkipHeaderBinaryFile	m_ItemFile;
public:
	cPlayerData(char*e_strPlayerDataFileName,char*e_strPlayerSaveFileName);
	~cPlayerData();

	void			Update(float e_fElpaseTime);
	void			Render();

	void			MouseDown(int e_iX,int e_iY);
	void			MouseUp(int e_iX,int e_iY);
	void			MouseMove(int e_iX,int e_iY);

	bool			SaveFile();
};

#endif