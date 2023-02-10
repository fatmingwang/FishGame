#ifndef _GUID_DEFINE_H
#define _GUID_DEFINE_H

#include "INITGUID.H"
//
// Define an Interface Guid for genport device class.
// This GUID is used to register (IoRegisterDeviceInterface)
// an instance of an interface so that user application
// can control the genport device.
//

// {846976E4-E45F-4cdc-AF76-06775CAE9F11}
DEFINE_GUID(GUID_DEVINTERFACE_JMGB_M01, 
0x846976e4, 0xe45f, 0x4cdc, 0xaf, 0x76, 0x6, 0x77, 0x5c, 0xae, 0x9f, 0x11);


#endif