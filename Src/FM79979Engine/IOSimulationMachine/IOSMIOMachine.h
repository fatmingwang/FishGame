#ifndef _IOSM_IO_MACHINE_H
#define _IOSM_IO_MACHINE_H

#include "IOSMBaseIO.h"

namespace IOSM
{

//=====================================
//push data into it,it will update all object
//=====================================

	class	cIOSMIOMachine:public cObjectListByName<cIOSMBaseIO*>,public NamedTypedObject
	{
	public:
		cIOSMIOMachine();
		~cIOSMIOMachine();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
	};

//=====================================
//dupulate data list and create a new object list and write new data into 
//dupulate one
//=====================================
	class	cIOSMIORecordMachine:public cIOSMIOMachine
	{
	public:
		cIOSMIORecordMachine();
		~cIOSMIORecordMachine();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();	
	};
//end namespace
}
#endif