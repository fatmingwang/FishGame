#ifndef _cProbability_MasterLee_H_
#define _cProbability_MasterLee_H_
//
#include "../../../Core/Network/Network.h"
//
struct  sSendDataWithMonster;
struct  sReceiveFishData;
class	cSerialIO_MasterLee;
//
class	cProbability_MasterLee
{
	friend class	cSerialIO_MasterLee;
	float											m_fFishDiedTimeOutCheck;
	float											m_fIOTimeOutCheck;
private:
	static unsigned short							m_susCurrentID; //0~9999
	//
	bool											m_bReceivedFishDataFromIO;
	//
	//for send,while send function is called,assign m_SchoolMasterLiSendDataWithMonster::m_SendData to this
	sSendDataWithMonster							*m_pSendDataWithMonster;
	//
	//for received,compare send and received data
	cVectorDataSynchornized<sReceiveFishData>		*m_vReceiveFishData;
	//
	cSerialIO_MasterLee*							m_pSerialIO_MasterLee;
	//
	bool											ProcessFishesDied();
	void											ProcessHittedFishes(float e_fElpaseTime);
	void											ProcessSingnal(float e_fElpaseTime);
	void											ProcessTimeout(float e_fElpaseTime);
public:
	std::vector<sSendDataWithMonster*>				*m_pSendDataWithMonsterList;
	//
	cProbability_MasterLee();
	virtual ~cProbability_MasterLee();
	//
	void											Initial();
	void											Update( float e_fdt );
	void											Render();
	//
	void											SetReceivedFishDataFromIO(bool e_b){ m_bReceivedFishDataFromIO = e_b;}
	bool											IsReceivedFishDataFromIO(){ return m_bReceivedFishDataFromIO;}
	cVectorDataSynchornized<sReceiveFishData>*		GetReceivedFishData(){ return m_vReceiveFishData; }

	bool											Get_SentData( unsigned char *e_pData, int *e_piLength );
	//

};

#endif