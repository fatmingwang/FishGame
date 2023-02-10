#ifndef _ALL_IOSM_INCLUDE_H_
#define _ALL_IOSM_INCLUDE_H_

#include "IOSMIOData.h"
#include "IOSMDriverInterface.h"
#include "IOSMDeviceBehavior.h"
#include "IOSMBaseDevice.h"
#include "IOSMManager.h"
//#ifdef _WIN32
//#include "Comport/SerialCtrl.h"
//#endif
#ifdef	USING_GH8051
#include "8051\GH8051.h"
#endif

#ifdef	USING_FPGA
#include "Aewin\AEWINFPGA.h"
#endif


using namespace IOSM;


#endif