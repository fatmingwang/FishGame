#ifndef _NUMERIAL_ROLLING_EFFECT_H_
#define _NUMERIAL_ROLLING_EFFECT_H_

class	cFixedRollingObjectManager;
class	cFixedRollingObject:public cBaseImage
{
	cBasicSound*m_pSpinSound;
	cFixedRollingObjectManager*m_pNumerialObjectManager;
	//
	float	m_fCurrentPos;
public:
	cFixedRollingObject(cFixedRollingObjectManager*e_pNumerialObjectManager,cBaseImage*e_pBaseImage,Vector3 e_vPos,float e_fCurrentPos);
	~cFixedRollingObject();
	//
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	bool			IsAtShowArea();
	void			RelativeMove(float e_fDis);
	float			GetCurrentPos(){return m_fCurrentPos;}
	void			SetCurrentPos(float);
	//just change image's UV
	void			ChangeImage(cBaseImage*e_pBaseImage);
};
//=====================================
//spin object list order as below
//0
//1
//2
//3
//4
//5
//the m_fSpinObjectIndex0StartPos is a image height
//so object 0 could be see nicely
//so the object will show in the screen is start from object index 1.
//===============================
//if we expect a animated move object just use SubMPDI to perform it while last animated object expected to show
//===============================
//=====================================
#define	MAX_VISBILE_OBJECT	20
typedef std::vector<std::wstring>	LineData;
class	cFixedRollingObjectManagerList;
class	cFixedRollingObjectManager:public cNamedTypedObjectVector<cFixedRollingObject>
{
	cPuzzleImage*	m_pAllRollingObjectPI;
	//
	friend class	cFixedRollingObject;
	friend class	cFixedRollingObjectManagerList;
	//all cSlotObject have to working at this range
	float			m_fAllSpinObjectLength;
	//
	float			m_fSpinObjectIndex_0_OffsetStartPos;
	//c above comment now I set 1 as default ResultSpinObjectIndex
	static int		m_siResultSpinObjectIndex;
	//where it show and the screen size to show
	Vector4			m_vShowArea;
	//the object could be show at m_vShowArea
	int				m_ShowObjectIndex[20];
	//==================spin data================================================
	//how long will it soin
	sTimeCounter	m_SpinTime;
	//the distance will spin at least
	float			m_fLeastSpinDis;
	//total spin distance( m_fLeastSpinDis + m_iStopWorkingIndex )
	float			m_fTargetSpinDis;
	//
	float			m_fCurrentSpeed;
	//final working index to show
	int				m_iResultIndexToStopAtTopShow;
	//
	void			AdjustOffsetPosition();
	void			UpdateSpinVisibleResult(float e_fElpaseTime);
public:
	cFixedRollingObjectManager(Vector4 e_vShowArea,float e_fSpinTime,float	e_fLeastSpinDis);
	~cFixedRollingObjectManager();
	//setup for all cSlotObject's position as well
	void			SetupData(LineData e_SlotLineData,cPuzzleImage*e_pPI);
	void			Update(float e_fElpaseTime);
	void			Render();
	void			DebugRender();
	//
	bool			IsSpinedDone();
	float			GetRestTime();
	virtual	void	Init();
	//=======================================
	//this one for setup result data as we expect,it will change the count of top view's object' data
	//but it still need m_iResultIndexToStopAtTopShow to set right position,but m_iResultIndexToStopAtTopShow
	//always be the current top of view's previous object index minus e_iCount.
	void			StartSpinData(LineData e_LineData);
	//this one for set target index to stop at top of view
	void			StartSpinData(int e_iStopWorkingIndex);
	int				GetStopAtTopShowResultIndex(){return m_iResultIndexToStopAtTopShow;}
	//after free game we have to set 
	void			ReplaceObject(int e_iIndex,WCHAR*e_strNewObjectName);
	//
	cPuzzleImage*	GetAllRollingObjectPI(){return m_pAllRollingObjectPI;}
};

//m_pFixedRollingObjectManagerList = new cFixedRollingObjectManagerList("JewelsKing/ImageData/JPNum.pi",Vector2(300,1095));
//SAFE_DELETE(m_pFixedRollingObjectManagerList);
//m_pFixedRollingObjectManagerList->SetStopResult(79979);
//if( m_pFixedRollingObjectManagerList )
//	m_pFixedRollingObjectManagerList->Update(e_fElpaseTime);
//m_pFixedRollingObjectManagerList->Render();
class	cFixedRollingObjectManagerList:public cBehaviorObjectList<cFixedRollingObjectManager>
{
	cPuzzleImage*	m_pAllRollingObjectPI;
public:
	cFixedRollingObjectManagerList(char*e_strPIName,Vector2 e_vRenderPos);
	~cFixedRollingObjectManagerList(){}
	void	DoScissor()
	{
		cFixedRollingObjectManager*l_pSlotObjectManager = this->GetObject(0);
		if( l_pSlotObjectManager )
		{
			Vector4	l_vScissor = ViewRectToOpenGLScissor(l_pSlotObjectManager->m_vShowArea);
			glEnable(GL_SCISSOR_TEST);
			glScissor((int)l_vScissor.x,(int)l_vScissor.y,(int)l_vScissor.z,(int)l_vScissor.w);	
		}
	}
	void	UnDoScissor()
	{
		glDisable(GL_SCISSOR_TEST);
	}
	virtual	void	Render();
	bool	IsSpinedDone()
	{
		int l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(!this->m_ObjectList[i]->IsSpinedDone())
				return false;
		}
		return true;
	}

	void	SetStopResult(int e_iValue);
};
#endif