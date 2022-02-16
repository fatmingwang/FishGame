#include "../../Core/AllCoreInclude.h"
#include "../../Core/Particle/AllParticleInclude.h"
#include "../../Core/FatmingAI/AllFatmingAIInclude.h"
#include "../../Core/AllPhysicInclude.h"
#include "../../Core/FMEvent/AllEventInclude.h"

#include "../GameApp/FileNameDefine.h"
#include "../ControlPanel/ControlPanel.h"
#include "../../IOSimulationMachine/AllIOSMInclude.h"

//0xaa,0xe1,0xcc for header,1 for keyboard code,1 for how many fish hitted
#define	NEXT_FISH_SIZE						7
#define	RECEIVE_HEAD_SIZE					3 + 1 + 1
#define	RECEIVE_FIRST_MONSTER_ID_DATA_INDEX	5
#define	RECEIVE_HOW_MANY_FISHES				4
#define	SEND_FIRST_MONSTER_ID_DATA_INDEX	4
#define	SEND_HOW_MANY_FISHES				3
#define	MAX_FISH_DIED_AT_ONE_UPLOAD	250	//ENSURE sSendFishData size is smaller than 1k
#define MAX_SEND_BUFFER 1024

extern int		g_iNumFishesCouldBeSend;
extern int		g_iNumPacketHitSend;
extern int		g_iNumPacketSend;
extern int		g_iNumPacketSendFailed;
extern int		g_iNumPacketReceived;
extern int		g_iNumPacketEmptySend;
extern int		g_iNumPacketEmptyReceived;
extern int		g_iNumPacketHitReceived;
extern int		g_iNumPacketResend;
extern int		g_iWaitingProcessSingnalID;//current fish died ID