#ifndef _SLOT_OBJECT_H_
#define _SLOT_OBJECT_H_

#include "SlotRule.h"

//make object from up to down looply
struct  sLoopMovingObject
{
	cBasicSound*pSpinSound;
	//which image is show
	int					iImageIndex;
	//
    Vector3    			vCurrentPos;
    Vector3    			vEndPos;
	Vector3    			vStartPos;
	bool				bOverEndPos;
	bool				bRender;
	sLoopMovingObject();
	~sLoopMovingObject();
	void	SetData(Vector3 e_vStartPos,Vector3 e_vEndPos);
    void    Update(float e_fElpaseTime,Vector3 e_vSpeed);
	void    Update(Vector3 e_vDistance);
	Vector3	DisToStartPos();
};


class	cShowSpindResultPlayer;
class	cSlotObjectManagerList;
class	cSlotObjectManager:public NamedTypedObject
{
	friend class	cShowSpindResultPlayer;
	friend class	cSlotObjectManagerList;
	cShowSpindResultPlayer*	m_pShowSpindResultPlayer;
	//spin Object position info,the first one is the result as we expect
	sLoopMovingObject*		m_pLoopMovingObject;
	//sLoopMovingObject		m_pLoopMovingObject[15];
	//how many object is visible,but 1 is invisible so please use call GetRowCount instead directly to use m_iRowCount
	int						m_iRowCount;
	Vector3					m_vStartPos;
	Vector3					m_vEndPos;
	//the images after this index is special image for free game
	int						m_iNormalImageRange;
	cPuzzleImage*			m_pAllSpinObjectPI;
	float					m_fImageHeight;
	float					m_fImageWidth;
	Vector4					m_vShowArea;
	//how long will it soin
	sTimeCounter			m_SpinTime;
	//the distance will spin at least
	float					m_fLeastSpinDis;
	void					AdjustOffsetPosition(bool e_bRandomImage = false);
	//
	void					UpdateShowResult();
	void					RenderShowResult();
	//
	int						GetNormalImageWithRandom();
public:
	cSlotObjectManager(Vector4 e_vShowArea,float e_fSpinTime,float	e_fLeastSpinDis,cPuzzleImage*e_pAllSpinObjectPI,int e_iRowCount,int e_iNormalImageRange);
	~cSlotObjectManager();
	//only first call should set random index
	void					Init();
	void					SetupResult(std::vector<std::wstring>*e_pResultName,bool e_bWithRandomImage = false);
	void					Update(float e_fElpaseTime);
	void					Render();
	void					DebugRender();
	void					Destroy(){}
	//
	bool					IsSpinedDone();
	float					GetRestTime();
	//=======================================
	cPuzzleImage*			GetAllSpinObjectPI(){return m_pAllSpinObjectPI;}
	//
	Vector3					IndexToResultShowPos(int e_iIndex);
	//the 1 for invisible,so skip it
	int						GetRowCount(){return m_iRowCount-1;}
	const	WCHAR*			GetNameByResultIndex(int e_iIndex);
	sLoopMovingObject*		GetLoopMovingObject(int e_iIndex){return &m_pLoopMovingObject[e_iIndex];}
};
//spin result object to its right position as well
//we could do a nice spin as need as  speed to modify rest distance of m_vRestDisToDestination
//
class	cShowSpindResultPlayer
{
	cCueToStartCurveWithTime*	m_pSpinObjectAnimatedMovingBehaviorSubMPDI;
	cMPDIList*					m_pSpinObjectAnimatedMovingBehaviorMPDIList;
	cSlotObjectManager*			m_pSlotObjectManager;
	//the final result
	int*						m_piResultForShow;
	int							m_iCountForAllResultSetDone;
	//check the order to show result,make sure m_pSlotObjectManager's resutlt as we expect,start at index 0.
	bool						m_bFirstTimeDataCheck;
	Vector3						m_vRestDisToDestination;
	//scale which m_pSpinObjectAnimatedMovingBehaviorSubMPDI's total length and m_vRestDisToDestination's total length
	float						m_fScaleForSpin;
	//check all spinobject has change result as well
	void						CheckResultImage();
	static	int					m_siFreeGameImageCount;
public:
	cShowSpindResultPlayer(cSlotObjectManager*e_pSlotObjectManager);
	~cShowSpindResultPlayer();
	void			SetResultData(std::vector<std::wstring>*e_pResultName);
	void			Init();
	void			Update(float e_fElpaseTime);
	bool			IsDone();
	float			GetRestTime();
};

class	cSlotObjectManagerList:public cBehaviorObjectList<cSlotObjectManager>
{
public:
	cSlotObjectManagerList();
	~cSlotObjectManagerList();
	void	DoScissor();
	void	UnDoScissor();
	virtual	void	Render();
	bool	IsSpinedDone();
	int		GetNameCount(const WCHAR*e_strName);
};

//=====================
//for result inex is set by
//0, 1, 2, 3, 4
//5, 6, 7, 8 ,9
//10,11,12,13,14....
//=====================
std::wstring		GetSpinObjectNameByIndex(int e_iIndex);
sLoopMovingObject*	GetLoopMovingObjectByIndex(int e_iIndex);
//int				GetSpinObjectManagerChildIndexByLineRuleIndex(cSlotObjectManager*e_pSlotObjectManager,int e_iLineRuleIndex);


#endif