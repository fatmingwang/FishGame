#ifndef _FALLE_DOWN_SPIN_OBJECT_H_
#define _FALLE_DOWN_SPIN_OBJECT_H_


class	cFallDownNewSpinObject:public NamedTypedObject
{
protected:
	struct	sImageWithPos
	{
		Vector3		vPos;
		cBaseImage*	pImage;
	};
	sImageWithPos*	m_pMovingObjects;
	//sImageWithPos	m_pMovingObjects[3];
	Vector3			m_vEndPos;
	bool			m_bAllFallDown;
	float			m_fSpeed;
	int				m_iCount;
	float			m_fObjectGap;
public:
	cFallDownNewSpinObject(int e_iNumObject,Vector3 e_vEndPos,float	e_fSpeed,float	e_fObjectGap);
	~cFallDownNewSpinObject();
	void	SetupData(int e_iIndex,Vector3 e_vStartPos,cBaseImage*e_pImage);
	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	Init();
	virtual	bool	IsDone();
	virtual	void	Destroy(){}
};


class	cFallDownNewSpinObjectManager:public cBehaviorObjectList<cFallDownNewSpinObject>
{
public:
	bool	IsDone()
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(!this->m_ObjectList[i]->IsDone())
				return false;
		}
		return true;
	}
	virtual	void	Init()
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->m_ObjectList[i]->Init();
		}	
	}
};

#endif