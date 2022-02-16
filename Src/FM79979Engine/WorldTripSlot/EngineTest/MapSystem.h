#ifndef _MAP_SYSTEM_H_
#define _MAP_SYSTEM_H_


class	cMapSystem
{
	struct	sStagePointData
	{
		std::wstring	strStageName;
		cBaseImage*		pImage;
	};
	std::vector<sStagePointData*>	m_StagePointDataVector;
public:
	cMapSystem();
	~cMapSystem();
};

#endif