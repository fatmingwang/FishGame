#pragma once
#include "../Core/AllCoreInclude.h"

//#include "8051/GH8051.h"
//#include "Aewin/AEWINFPGA.h"

#if defined(USING_GH8051) || defined(_JMGB_)
#else
	#define	EMPTY_DEVICE
#endif