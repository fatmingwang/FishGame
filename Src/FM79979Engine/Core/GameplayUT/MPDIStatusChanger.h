#ifndef _MPDI_STATUS_CHANGER_H_
#define _MPDI_STATUS_CHANGER_H_

namespace FATMING_CORE
{
	//============
	//should I migrate particle project into CORE?
	//============
	//===============
	//<StatusObjectChanger>
	//	<Status Name="" >
	//		<MPDI File="" MPDIList="" MPDI="" >
	//		<MPDI File="" MPDIList="" MPDI="" >
	//	</Status>
	//	<ParticleFile="" prt="" >
	//	<ParticleGroup  File="" prtg="" >
	//</StatusObjectChanger>
	//NamedTypedObject could be anything even a cBehaviorObjectList,but for now I am lazy to do too much,so only support one objct for now.
	//class	cStatusObjectChanger:public cFMWorkingObjectChanger<cObjectAndName<NamedTypedObject>>,public cNodeISAX
	//{
	//	//while Status's DataCount is bigger than 1
	//	cBehaviorObjectList<NamedTypedObject>*m_pCurrentStatusContainer;
	//	//
	//	void	ProcessMPDI(TiXmlElement*e_pElement);
	//	void	ProcessParticle(TiXmlElement*e_pElement);
	//	void	ProcessParticleGroup(TiXmlElement*e_pElement);
	//	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	//public:
	//	virtual	bool	Export(char*e_strFileName);
	//	virtual	void	Start(float e_fElpaseTime = EPSIONAL){}
	//};

//end FATMING_CORE
}

#endif