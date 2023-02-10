#ifndef IOSM_IO_MANAGER_H
#define IOSM_IO_MANAGER_H

#include "IOSMBaseDevice.h"
#include "IOSMDeviceBehavior.h"
#include "IOSMDriverInterface.h"

namespace IOSM
{
	//=====================================
	//parse data
	//=====================================
	enum	eIODeviceBehaviorList
	{
		eIODBL_IOSM_ROOT = 0,
		eIODBL_BASE_DEVICE,
		eIODBL_DEVICE_BEHAVIOR,
		eIODBL_PROBABLE_IO,
		eIODBL_ACTIVE_IO_TIMES_WITH_SINGNAL_CIRCLE,
		eIODBL_LISTEN_MULTI_SINGNAL_TO_ACTIVE_IO,
		eIODBL_SEND_HIGN_SINGNAL,
		eIODBL_SEND_LOW_SINGNAL,
		eIODBL_MAX
	};

	class	cIOSMIOManager:public cNamedTypedObjectVector<cIOSMBaseDevice>,public ISAXCallback
	{
		//temp for parse data
		cIOSMDeviceBehavior*m_pCurrentIOSMDeviceBehavior;
		cIOSMBaseDevice*m_pCurrentIOSMBaseDevice;
		cIOSMIOData*m_pCurrentIOSMIOData;
		//for data assign
		eIODeviceBehaviorList	m_eIODeviceBehaviorList;
		void	ProcessProbableIOData();
		void	ProcessActiveIOTimesWithSingnalCircleData();
		void	ProcessListenMultiSingnalToActiveIOData();
		void	ProcessSendHighSingnalData();
		void	ProcessSendLowSingnalData();
		void	ProcessBaseDeviceData();
		void	ProcessIOSMData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:

		cIOSMIOManager();
		virtual ~cIOSMIOManager();

		cIOSMDriverInterface*m_pIOSMDriverInterface;
		//after parse call this to sort behavior piority
		virtual	void	Init();
		//first to call all IOSM data upate to setup singnal,if singnal is not match previous,the singnal is changed by device
		//second to call device update
		virtual	void	Update(float e_fElpaseTime);
#ifdef WIN32
		bool	Export(CHAR* strFileName);
#endif
		//
		//cIOSMIOMachine*	CreateMachine(WCHAR*e_strName);
		//
		//cIOSMIORecordMachine*	CreateRecordMachine(WCHAR*e_strName);
	};
//end namespace
}
#endif
