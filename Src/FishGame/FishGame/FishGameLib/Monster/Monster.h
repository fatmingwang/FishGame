#pragma once
//
class	cPlayerBehaviorBase;
class	cBullet;
class	cBulletManager;
class	cProbabilityFish;
class	cFishGroup;

//
enum	eMonsterType
{
	eMT_NORMAL = 0,
	eMT_BOSS,
	eMT_MAX,
};
//
enum	eMonsterStatus
{
	eMS_NONE = 0,				//not on the scene
	eMS_ALIVE,					//on the scene,do AIMachine
	eMS_HITTED,					//probability give
	eMS_STRUGGLE,				//while fish is hitted it will has a fake probability to play this
	eMS_DYING,					//hitted play died animation
	eMS_DIED_SHOW,				//
	eMS_WAITING_FOR_CLEAN,		//
	//eMS_CONTROL_BY_MINI_GAME,	//
	eMS_MAX,

};
//
enum	eBodyType
{
	eBT_SMALLE = 0,
	eBT_MIDIUM,
	eBT_BIG,
	eBT_MAX,
};
//
eBodyType	GetBodyType( const WCHAR *e_str );

int	NewPrizeWithRule(int e_iGap,int e_iStartScore,int e_iScore);

class cMonster : public NamedTypedObject
{
private:
	virtual	void				ProcessPhysicalData(TiXmlElement*e_pElement) = 0;
	//
	GET_SET_DEC(eBodyType,m_eBodyType,GetBodyType,SetBodyType);
	//
protected:
	friend class				cProbabilityFish;
	friend class				cProbability_Fatming;
	friend class				cProbability_MasterLee;
	friend class				cBulletManager;
	friend class				cMonsterManager;
	//how long s it alive
	float						m_fAliveTime;
	//
	Vector4						m_vHittedHintColor;
	UT::sTimeCounter			m_HittedHintTC;
	//FishData.xml
	float						m_fProbabilityToDied;
	float						m_fRestProbabilityValueToDied;
	//while fish if update m_iTotalBiteMoney will add m_iCurrentFrameBiteMoney
	//and m_iCurrentFrameBiteMoney will be 0 .
	int							m_iCurrentFrameBiteMoney;
	int							m_iTotalBiteMoney;
	//for out of screen test
	float						m_fRadius;
	eMonsterType				m_eMonsterType;

	sMinMaxData<int>*			m_pFloatingPrize;

	void						ProcessMonsterData(TiXmlElement*e_pTiXmlElement);
	eMonsterStatus				m_eMonsterStatus;
	cPlayerBehaviorBase*		m_pHittedPlayer;
	cBullet*					m_pBullet;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	int							m_iID;
	bool						m_bMiniGameMonster;
	float						m_fScale;
	//
public:
	int*						m_piPrize;
	std::wstring				m_strMiniGameName;
	std::wstring				m_strPrizeAnimationName;
	std::wstring				m_strPrizeParticleName;
	std::wstring				m_strDiedPrizeAnimationName;
	//
	cMonster();
	virtual ~cMonster();
	cMonster(cMonster*e_pMonster);
	float						GetRadius(){return m_fRadius;}
	void						SetRadius(float e_fRadius){m_fRadius = e_fRadius;}
	int							GetID(){ return m_iID; }
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	virtual	void				Render() = 0;
	bool						IsMiniGameMonster();
	void						SetMiniGameMonster(bool e_b,const WCHAR*e_strName);
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision) = 0;
	virtual	bool				IsCollide(cMonster*e_pMonster);
	int							GetTotalBiteMoney(){return m_iTotalBiteMoney;}
	virtual	bool				IsVisible() = 0;
	virtual	bool				IsInFiled() = 0;
	virtual	void				LeaveScene() = 0;
	virtual	void				DebugRender(){}
	virtual	void				RandomPath() = 0;
	virtual	void				SetPos(Vector3 e_vPos) = 0;
	virtual	Vector3				GetAngle() = 0;
	virtual	void				SetAngle(Vector3 e_vAngle) = 0;
	virtual	Vector3				GetPos() = 0;
	virtual	cMatrix44			GetTransform() = 0;
	virtual	void				SetPath() = 0;
	virtual	void				SetPath(cCurveWithTime* e_pCurveWithTime) = 0;
	virtual	cCurveWithTime*		GetPath() = 0;
	void						AddBiteMoney(int e_iMoney);
	eMonsterStatus				GetMonsterStatus(){ return m_eMonsterStatus; }
	virtual	void				SetMonsterStatus(eMonsterStatus e_eMonsterStatus);
	virtual	void				CollisionDataCheck() = 0;
	cPlayerBehaviorBase*		GetHittedPlayer(){return  m_pHittedPlayer;}
	cBullet*					GetBullet(){return m_pBullet;}
	int							GetPrize();
	//while monster status is wait for clear or control by mini game cannot be died
	bool						IsStatusAllowToDied();
	bool						IsMiniGameShotOkToDied();
	virtual	void				MonsterLeaveScene() = 0;
	float						GetFishAliveTime(){return m_fAliveTime;}
	virtual	Vector3				GetCenterPos() = 0;
};