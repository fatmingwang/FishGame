#ifndef _SEND_GAME_START_PACKET_H_
#define _SEND_GAME_START_PACKET_H_

#define MAX_SEND_BUFFER 1024
class cSerialIO_MasterLee;
typedef bool   (*ReceivedSingnalCheck)(void *e_pData);

//struct	sPacketSendAndReceivedProcess
//{
//	DoButtonGoal_Callback	Send;
//	ReceivedSingnalCheck	Received;
//	sPacketSendAndReceivedProcess( DoButtonGoal_Callback e_Send,ReceivedSingnalCheck e_Received ){ Send = e_Send; Received = e_Received; }
//};



struct	sSendGameStartPacket
{
	static int		siErrorCount;//if over than 10 times accept it
	static int		siGameMode;//0~9
	static int		siPlace;//0~3
	struct	sPacketSendAndReceivedProcess
	{
		DoButtonGoal_Callback	Send;
		ReceivedSingnalCheck	Received;
		sPacketSendAndReceivedProcess( DoButtonGoal_Callback e_Send,ReceivedSingnalCheck e_Received ){ Send = e_Send; Received = e_Received; }
	};
	
	cSerialIO_MasterLee		*m_pSerialIO_MasterLee;
	void						ProcessReceivedSingnal();
	int							m_iCurrentStep;
	sSendGameStartPacket(cSerialIO_MasterLee*e_pLiSchoolMasterSerialIO);
	~sSendGameStartPacket();
	std::vector<sPacketSendAndReceivedProcess*>	m_StartSendDataList;
	bool						IsDone();
	void						Send();
	void						Received(char*e_pData);
};
//

#endif