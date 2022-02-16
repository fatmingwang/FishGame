#ifdef WIN32

// global variables and typedefs for AP programs

#pragma pack(push)	// save the compiler original data aligment
#pragma pack(1)	

// define struture and variable to read/write port
typedef struct  _JMGB_PORT_WRITE_INPUT
{
    ULONG   PortNumber;     // Port # to write to
    union   
	{               // Data to be output to port
        ULONG   LongData;
        USHORT  ShortData;
        UCHAR   CharData;
    };
}   JMGB_PORT_WRITE_INPUT;

union  
{
    ULONG   LongData;
    USHORT  ShortData;
    UCHAR   CharData;
}   DataBuffer;             // Buffer received from driver (Data).

typedef struct  _JMGB_PORT_READ_BYTES
{
	ULONG	PortAddr;
	ULONG	ulLen;				// number of bytes to read from Memory Mapped addr.
}	JMGB_PORT_READ_BYTES;

typedef struct  _JMGB_PORT_WRITE_BYTES 
{
	ULONG	PortAddr;
	ULONG	ulLen;				// number of bytes to write to Memory Mapped addr.
	UCHAR	DataBuffer[32768];	// the bytes to be written to Memory Mapped addr.
}	JMGB_PORT_WRITE_BYTES;

#pragma pack(pop)	// restore the compiler original data aligment


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


//win32
#endif