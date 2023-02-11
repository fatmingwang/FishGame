#pragma once

struct	sFishDiedEffectList
{
	cMPDI**						ppWorkingList;
	cNamedTypedObjectVector<cMPDI>	FishDiedEffectList;
	int							iNumWorking;
	int							iCount;
	sFishDiedEffectList(cMPDI*e_pMPDI,int e_iCount);
	~sFishDiedEffectList();
	void						Update(float e_fElpseTime);
	void						Render();
	void						Show(Vector3 e_vPos,float e_fAngle);
};

class	cFishDiedEffect
{
	std::vector<sFishDiedEffectList*>	m_FishDiedEffectList;
public:
	cFishDiedEffect();
	virtual ~cFishDiedEffect();
	void						AddMPDI(cMPDI*e_pMPDI,int e_iCount);
	void						Update(float e_fElpseTime);
	void						Render();
	void						Show(const WCHAR*e_strName,Vector3 e_vPos,float e_fAngle);
};