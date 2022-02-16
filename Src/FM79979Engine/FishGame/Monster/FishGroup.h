#ifndef _FISH_GROUP_H_
#define _FISH_GROUP_H_

class	cMonster;
class	cMonsterManager;

class	cFishGroup : public NamedTypedObject
{
	friend class	cMonsterManager;
	friend class	cMonster;
	//
	int		FishGroupGoFishesCount(int e_iIndex);//how many fisehes out
	int		FishGroupGo(int e_iIndex,bool e_bForceFetch = false);
	int		FishGroupGo(int e_iIndex, cCurveWithTime e_TargetPath,bool e_bForceFetch = false);
	void	f_PrizeII_Go( cCurveWithTime e_TargetPath );

	//
	class	cFishGroupPointData:public NamedTypedObject
	{
	public:
		struct	sFishPosList
		{
			int						iFishIndex;
			std::vector<Vector3>	vPosList;
		};
		std::vector<cCurveWithTime>	vCurveList;
		std::vector<sFishPosList>	m_FishGroupData;
		Vector2						m_vDrawSize;
	};
	cNamedTypedObjectVector<cFishGroupPointData>	m_FishGroup;
	//moving firection offset
	UT::sMinMaxData<float>			m_fDirectionOffsetX;
	UT::sMinMaxData<float>			m_fDirectionOffsetY;
	float							m_fGroupShowOffsetTime;
	//
public:
	//
	cFishGroup( );
	cFishGroup( int e_nId, const char* e_strFishGroupMPDI );
	virtual ~cFishGroup();
	
	void	ProcessData( const char* e_strFishGroupMPDI );

	void	FishGroupGoUpToDown(int e_iIndex,float e_fTime);
	void	FishGroupGoDownToUp(int e_iIndex,float e_fTime);

	void	FishGroupGoLeftToRight(int e_iIndex,float e_fTime);
	void	FishGroupGRightTooLeft(int e_iIndex,float e_fTime);
	void	f_PrizeII_Call( float e_fTime );
};

#endif