#ifdef WIN32

#include "stdafx.h"
#include "IOSMDriverInterface.h"
#include <setupapi.h>
#include "JMIOCTL.H"
#include "JMGBStructureDefine.h"
namespace	IOSM
{
	bool	g_bSimulation = false;
	cIOSMDriverInterface*g_pIOSMDriverInterface = 0;
	cIOSMDriverInterface::cIOSMDriverInterface()
	{
		g_pIOSMDriverInterface = this;
		m_hHandle = INVALID_HANDLE_VALUE;
		m_pIOMemory =  0;
		m_iCurrentSelectedDeviceIndex = -1;
		memset(m_MemBases,0,sizeof(unsigned long )*6);
	}

	cIOSMDriverInterface::~cIOSMDriverInterface()
	{
		Destory();
	}

	void	cIOSMDriverInterface::Destory()
	{
		if( m_hHandle )
			CloseHandle(m_hHandle);
	}

	void	cIOSMDriverInterface::Init(GUID e_GUID,int e_iStartAddress,int e_iDataLength)
	{
		HDEVINFO							HWDevInfo;
		SP_DEVICE_INTERFACE_DATA			DevInterfaceData;
		PSP_DEVICE_INTERFACE_DETAIL_DATA    DevInterfaceDetailData = NULL;
		ULONG                               PredictedLen = 0;
		ULONG                               NeededLen = 0, i;
		DWORD                               error;
		TCHAR								szBuff[MAX_PATH+256];

		OutputDebugString(TEXT("Find_JMGB_Devices"));

		GUID	l_InterfaceGUID = e_GUID;
		HWDevInfo = SetupDiGetClassDevs (
						   (LPGUID)&l_InterfaceGUID,
						   NULL, // Define no enumerator (global)
						   NULL, // Define no
						   (DIGCF_PRESENT | // Only Devices present
						   DIGCF_DEVICEINTERFACE)); // Function class devices.
		if(INVALID_HANDLE_VALUE == HWDevInfo)
		{

			MessageBox(0,TEXT("Failed to retrieve the device information set of the specified class."),TEXT("WARNING"),MB_OK);
			return;
		}

		//
		// Enumerate devices of a specific interface class
		//
		DevInterfaceData.cbSize = sizeof(DevInterfaceData);

		for(i=0; SetupDiEnumDeviceInterfaces (HWDevInfo,
									 0, // No care about specific PDOs
									 (LPGUID)&l_InterfaceGUID,
									 i, //
									 &DevInterfaceData); i++ )
		{
			//
			// Allocate a function class device data structure to
			// receive the information about this particular device.
			//

			//
			// First find out required length of the buffer
			//
			if (DevInterfaceDetailData)
			{
				HeapFree (GetProcessHeap(), 0, DevInterfaceDetailData);
			}

			if(!SetupDiGetDeviceInterfaceDetail (
					HWDevInfo,
					&DevInterfaceData,
					NULL, // probing so no output buffer yet
					0, // probing so output buffer length of zero
					&NeededLen,
					NULL) && (error = ::GetLastError()) != ERROR_INSUFFICIENT_BUFFER)
			{
				SetupDiDestroyDeviceInfoList (HWDevInfo);
				MessageBox(0,TEXT("Failed to retrieve the device info."),TEXT("Warning"),MB_OK);
				return;
			}

			PredictedLen = NeededLen;

			DevInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, PredictedLen);
			if (DevInterfaceDetailData == NULL) {
				SetupDiDestroyDeviceInfoList (HWDevInfo);
				MessageBox(0,TEXT("Failed to allocate memory."),TEXT("Warning"),MB_OK);
				return;
			}

			DevInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (! SetupDiGetDeviceInterfaceDetail (
					   HWDevInfo,
					   &DevInterfaceData,
					   DevInterfaceDetailData,
					   PredictedLen,
					   &NeededLen,
					   NULL)) {

				if(DevInterfaceDetailData) {
					HeapFree (GetProcessHeap(), 0, DevInterfaceDetailData);
				}

				SetupDiDestroyDeviceInfoList (HWDevInfo);
				MessageBox(0,TEXT("Failed to GetDeviceInterfaceDetail."),TEXT("Warning"),MB_OK);
				return;
			}

			//
			// Copy the device path so that we can open the device using CreateFile.
			//
			std::wstring	l_strForDevicePath = DevInterfaceDetailData->DevicePath;
			this->m_DevicePathVector.push_back(l_strForDevicePath);

	#if 1	// for debug only

	#ifdef UNICODE
			swprintf(szBuff, TEXT("DevicePath = %s"), DevInterfaceDetailData->DevicePath);
	#else
			sprintf(szBuff, TEXT("DevicePath = %s"), DevInterfaceDetailData->DevicePath);
	#endif
			TCHAR	l_strDeviceName[MAX_PATH];
			OutputDebugString(szBuff);
			GetDevDescription(DevInterfaceDetailData->DevicePath, l_strDeviceName, sizeof(l_strDeviceName));

	#ifdef UNICODE
			if (lstrlen(l_strDeviceName) < sizeof(szBuff))
				swprintf(szBuff, TEXT("DeviceName = %s"), l_strDeviceName);
	#else
			if (lstrlen(l_strDeviceName) < sizeof(szBuff))
				sprintf(szBuff, TEXT("DeviceName = %s"), l_strDeviceName);
	#endif

	#endif
			std::wstring	l_strDeviceNameForPush = l_strDeviceName;
			m_AllDeviceNameVector.push_back(l_strDeviceNameForPush);
		}
		if( m_AllDeviceNameVector.size() )
			m_iCurrentSelectedDeviceIndex = 0;
		assert(m_AllDeviceNameVector.size() == m_DevicePathVector.size());

		if(DevInterfaceDetailData)
		{
			HeapFree (GetProcessHeap(), 0, DevInterfaceDetailData);
		}

		SetupDiDestroyDeviceInfoList (HWDevInfo);
		CreateHandle();
		Get_Num_MemMapBases();
		Get_MemMapBases();

		if (m_Num_MemMapBases > 0)
		{
			GetMemMappedLength(m_MemBases[0]);
	#ifdef UNICODE
			swprintf(szBuff, TEXT("%x"), m_MemBases[0]);
	#else
			sprintf(szBuff, TEXT("%x"), m_MemBases[0]);
	#endif
		}
	}

	bool cIOSMDriverInterface::GetDevDescription(LPTSTR DevPath, LPTSTR OutBuffer, unsigned long OutBufferLen)
	{
		HDEVINFO                            HWDevInfo = NULL;
		SP_DEVICE_INTERFACE_DATA            DevInterfaceData;
		SP_DEVINFO_DATA                     deviceInfoData;
		DWORD                               dwRegType, error;

		OutputDebugString(TEXT("GetDevDescription"));

		HWDevInfo = SetupDiCreateDeviceInfoList(NULL, NULL);
		if(INVALID_HANDLE_VALUE == HWDevInfo)
		{
			MessageBox(0,TEXT("Failed to create DeviceInfoList"),TEXT("Waring"),MB_OK);
			return FALSE;
		}

		//
		// Enumerate devices of toaster class
		//
		DevInterfaceData.cbSize = sizeof(DevInterfaceData);

		SetupDiOpenDeviceInterface (HWDevInfo, DevPath, 0, &DevInterfaceData);

		deviceInfoData.cbSize = sizeof(deviceInfoData);
		if(!SetupDiGetDeviceInterfaceDetail (
				HWDevInfo,
				&DevInterfaceData,
				NULL, // probing so no output buffer yet
				0, // probing so output buffer length of zero
				NULL,
				&deviceInfoData) && (error = ::GetLastError()) != ERROR_INSUFFICIENT_BUFFER)
		{
			MessageBox(0,TEXT("Failed to get interface detail"),TEXT("Waring"),MB_OK);
			return FALSE;
		}

		//
		// Get the friendly name for this instance, if that fails
		// try to get the device description.
		//

		if(!SetupDiGetDeviceRegistryProperty(HWDevInfo, &deviceInfoData,
										 SPDRP_FRIENDLYNAME,
										 &dwRegType,
										 (BYTE*) OutBuffer,
										 OutBufferLen,
										 NULL))
		{
			if(!SetupDiGetDeviceRegistryProperty(HWDevInfo, &deviceInfoData,
										 SPDRP_DEVICEDESC,
										 &dwRegType,
										 (BYTE*) OutBuffer,
										 OutBufferLen,
										 NULL))
			{
				MessageBox(0,TEXT("Failed to get friendlyname"),TEXT("Waring"),MB_OK);
				return FALSE;
			}
		}

		if(HWDevInfo)
		{
			SetupDiDestroyDeviceInfoList (HWDevInfo);
		}

		return TRUE;	
	}

	void	cIOSMDriverInterface::CreateHandle()
	{

		if ( this->m_AllDeviceNameVector.size() == 0 )
			return;
		if( m_iCurrentSelectedDeviceIndex  == -1 )
			return;
		m_hHandle = CreateFile(
			(LPCTSTR )(this->m_DevicePathVector[m_iCurrentSelectedDeviceIndex].c_str()),
					GENERIC_READ|GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					0,
					NULL
					);
		if (m_hHandle == INVALID_HANDLE_VALUE)        
		{
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
		}
	}

	void cIOSMDriverInterface::Get_Num_MemMapBases(void)
	{
		TCHAR szBuff[128];

		OutputDebugString(TEXT("Get_Num_MemMapBases"));

		m_Num_MemMapBases = 0;

		if (m_hHandle == INVALID_HANDLE_VALUE)        
		{
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			return;
		}
		
		long	l_IoctlCode = IOCTL_JMGB_GET_NUM_OF_MAPPED_MEMORY_BASES;
		DWORD	l_BytesReturned;
		bool	l_bResult = DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								NULL,        // Buffer to driver.
								0, // Length of buffer in bytes.
								&m_Num_MemMapBases,        // Buffer from driver.
								sizeof(m_Num_MemMapBases),         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								);

		if (!l_bResult)                            // Did the IOCTL succeed?
		{
			MessageBox(0,TEXT("Can't get number of mapped memory bases."),TEXT("WARNING"),MB_OK);
		}

	#if 1	// for debug only

		else
		{
	#ifdef UNICODE
			swprintf(szBuff, TEXT("%lu"), m_Num_MemMapBases);
	#else
			sprintf(szBuff, TEXT("%lu"), m_Num_MemMapBases);
	#endif
			OutputDebugString(szBuff);
		}

	#endif
	}

	void cIOSMDriverInterface::Get_MemMapBases(void)
	{
		TCHAR szBuff[128];
		ULONG ulSize;

		OutputDebugString(TEXT("Get_MemMapBases"));

		if (this->m_AllDeviceNameVector.size() == 0)
			return;
		
		ulSize = sizeof(m_MemBases);


		long l_IoctlCode = IOCTL_JMGB_GET_MAPPED_MEMORY_BASES;
		DWORD	l_BytesReturned;
		bool l_Result = DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								NULL,        // Buffer to driver.
								0, // Length of buffer in bytes.
								m_MemBases,        // Buffer from driver.
								ulSize,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								);

		if (!l_Result)                            // Did the IOCTL succeed?
		{
			MessageBox(0,TEXT("Can't get mapped memory base."),TEXT("WARNING"),MB_OK);
		}
		else
		{
			for (ulSize = 0; ulSize < m_Num_MemMapBases; ulSize++)
			{

	#if 1	// for debug only

	#ifdef UNICODE
				swprintf(szBuff, TEXT("%x"), m_MemBases[ulSize]);
	#else
				sprintf(szBuff, TEXT("%x"), m_MemBases[ulSize]);
	#endif
				OutputDebugString(szBuff);
	#endif

			}
		}
	}

	void cIOSMDriverInterface::GetMemMappedLength(unsigned long BaseAddr)
	{
		TCHAR szBuff[128];

		OutputDebugString(TEXT("GetMemMappedLength"));

		m_MappedLength = 0;

		if (this->m_AllDeviceNameVector.size() == 0)
			return;

		if (m_hHandle == INVALID_HANDLE_VALUE)        
		{
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			return;
		}
		
		long l_IoctlCode = IOCTL_JMGB_GET_MAPPED_MEMORY_LENGTH;
		DWORD l_BytesReturned;
		bool l_Result = DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&BaseAddr,        // Buffer to driver.
								sizeof(BaseAddr), // Length of buffer in bytes.
								&m_MappedLength,        // Buffer from driver.
								sizeof(m_MappedLength),         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								);

		if (!l_Result)                            // Did the IOCTL succeed?
		{
			MessageBox(0,TEXT("Can't get mapped memory count."),TEXT("WARNING"),MB_OK);
		}

	#ifdef UNICODE
		swprintf(szBuff, TEXT("%lu"), m_MappedLength);
	#else
		sprintf(szBuff, TEXT("%lu"), m_MappedLength);
	#endif
		//SetDlgItemText(IDC_MemMapped_Length, szBuff);
	}

	int	g_iBitAddressValue[8] = {1<<0,1<<1,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7};
	int	g_iBitAddressOffValue[8] = {0xff^g_iBitAddressValue[0],
									0xff^g_iBitAddressValue[1],
									0xff^g_iBitAddressValue[2],
									0xff^g_iBitAddressValue[3],
									0xff^g_iBitAddressValue[4],
									0xff^g_iBitAddressValue[5],
									0xff^g_iBitAddressValue[6],
									0xff^g_iBitAddressValue[7]};

	
	bool cIOSMDriverInterface::WriteIOBitAddress(int e_iAddress,int e_iBitAddress,bool e_bOn)
	{
		if (this->m_hHandle == INVALID_HANDLE_VALUE)
		{
			return false;
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
		}//fetch data
		assert(e_iBitAddress<8&&e_iBitAddress>-1);
		ULONG DataLength;
		JMGB_PORT_WRITE_INPUT InputBuffer;    // Input buffer for DeviceIoControl
		int	l_iSelectedIndex = 0;
		DWORD	l_MemOffset = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
		{//read original data
			char	l_cData = '0';
			if( this->ReadIOChar(e_iAddress,&l_cData) )
			{
				if( e_bOn )
					InputBuffer.CharData = l_cData|g_iBitAddressValue[e_iBitAddress];
				else
				{

					InputBuffer.CharData = l_cData&g_iBitAddressOffValue[e_iBitAddress];
				}
			}
			else
				return false;
		}
		long l_IoctlCode = IOCTL_JMGB_WRITE_MEMORY_UCHAR;
		InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
		DataLength = offsetof(JMGB_PORT_WRITE_INPUT, CharData) + sizeof(InputBuffer.CharData);
		DWORD	l_BytesReturned = 0;
		if(!DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&InputBuffer.PortNumber,        // Buffer to driver.
								DataLength, // Length of buffer in bytes.
								NULL,        // Buffer from driver.
								0,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								))
		// Did the IOCTL succeed?
		{
			return false;
			MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
		}
		return true;
	}

	bool cIOSMDriverInterface::ReadIOBitAddress(int e_iAddress,int e_iBitAddress,bool*e_pbValue)
	{
		char	l_c = 0;
		if(ReadIOChar(e_iAddress,&l_c))
		{
			*e_pbValue = (l_c&g_iBitAddressValue[e_iBitAddress])?true:false;
			return true;
		}
		return false;
	}

	bool	cIOSMDriverInterface::ReadIOChar(int e_iAddress,char*e_pcValue)
	{
		assert(e_pcValue);
		if (m_hHandle == INVALID_HANDLE_VALUE)
		{
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			return false;
		}

		ULONG DataLength = 0;
		ULONG Addr = 0;
		bool	l_bOn = false;
		long	l_IoctlCode = IOCTL_JMGB_READ_MEMORY_UCHAR;
		DataLength = sizeof(DataBuffer.CharData);
		int	l_iSelectedIndex = 0;
		Addr = m_MemBases[l_iSelectedIndex] + e_iAddress;
		DWORD	l_BytesReturned = 0;
		bool	l_bResult = DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&Addr,				// Buffer to driver.
								sizeof(Addr),		// Length of buffer in bytes.
								&DataBuffer,        // Buffer from driver.
								DataLength,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								);

		if (!l_bResult)                            // Did the IOCTL succeed?
		{
			return false;
			MessageBox(0,TEXT("Can't read byte."),TEXT("WARNING"),MB_OK);
		}
		else
		{
			*e_pcValue = DataBuffer.CharData;
		}

		return true;
	}

	bool	cIOSMDriverInterface::WriteIOChar(int e_iAddress,char e_cValue)
	{
		if (this->m_hHandle == INVALID_HANDLE_VALUE)
		{
			return false;
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			//return;
		}//fetch data
		DWORD	l_BytesReturned = 0;
		long l_IoctlCode = IOCTL_JMGB_WRITE_MEMORY_UCHAR;
		int	l_iSelectedIndex = 0;
		_JMGB_PORT_WRITE_INPUT	InputBuffer;

		InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
		InputBuffer.CharData = e_cValue;
		ULONG	DataLength = offsetof(_JMGB_PORT_WRITE_INPUT, CharData) + sizeof(InputBuffer.CharData);

		if(!DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&InputBuffer.PortNumber,        // Buffer to driver.
								DataLength, // Length of buffer in bytes.
								NULL,        // Buffer from driver.
								0,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								))
		// Did the IOCTL succeed?
		{
			return false;
			MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
		}
		return true;		
	}

	bool	cIOSMDriverInterface::WriteIORange(int e_iAddress,char* e_cValue,int e_iSize)
	{
		if (this->m_hHandle == INVALID_HANDLE_VALUE)
		{
			return false;
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			//return;
		}//fetch data
		int	l_iSelectedIndex = 0;
		_JMGB_PORT_WRITE_BYTES	InputBuffer;
		memcpy(InputBuffer.DataBuffer,e_cValue,sizeof(char)*e_iSize);
		long l_IoctlCode = IOCTL_JMGB_WRITE_MEMORY_UCHARS;
		InputBuffer.PortAddr = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
		InputBuffer.ulLen = e_iSize*sizeof(char);
		ULONG	InputBufferSize = sizeof(JMGB_PORT_WRITE_BYTES);
		DWORD	l_BytesReturned = 0;
		if(!DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&InputBuffer,        // Buffer to driver.
								InputBufferSize, // Length of buffer in bytes.
								NULL,        // Buffer from driver.
								0,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								))
		// Did the IOCTL succeed?
		{
			return false;
			MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
		}
		return true;			
	}

	bool	cIOSMDriverInterface::ReadIORange(int e_iAddress,int e_iSize,char*e_pcOutData)
	{
		if (m_hHandle == INVALID_HANDLE_VALUE)
		{
			MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
			return false;
		}

		JMGB_PORT_READ_BYTES	l_JMGB_PORT_READ_BYTES;
		long	l_IoctlCode = IOCTL_JMGB_READ_MEMORY_UCHARS;
		int	l_iSelectedIndex = 0;
		DWORD	l_BytesReturned = 0;

		l_JMGB_PORT_READ_BYTES.PortAddr = m_MemBases[l_iSelectedIndex] + e_iAddress;
		l_JMGB_PORT_READ_BYTES.ulLen = sizeof(char)*e_iSize;
		
		bool	l_bResult = DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&l_JMGB_PORT_READ_BYTES,				// Buffer to driver.
								sizeof(l_JMGB_PORT_READ_BYTES),		// Length of buffer in bytes.
								e_pcOutData,        // Buffer from driver.
								l_JMGB_PORT_READ_BYTES.ulLen,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								);

		if (!l_bResult)                            // Did the IOCTL succeed?
		{
			//return false;
			MessageBox(0,TEXT("Can't read byte."),TEXT("WARNING"),MB_OK);
			return false;
		}
		return true;
	}


	bool	cIOSMDriverInterface::ReadIO4ByteOnce(int e_iAddress,int*e_piValue)
	{
		ULONG DataLength;
		int	l_iSelectedIndex = 0;
		DWORD	l_BytesReturned = 0;
		ULONG Addr = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
		long	l_IoctlCode = IOCTL_JMGB_READ_MEMORY_ULONG;
		DataLength = sizeof(DataBuffer.LongData);
		if(DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&Addr,				// Buffer to driver.
								sizeof(Addr),		// Length of buffer in bytes.
								&DataBuffer,        // Buffer from driver.
								DataLength,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								))
		{
			*e_piValue = DataBuffer.LongData;
			return true;
		}

		return false;
	}

	bool	cIOSMDriverInterface::WriteIO4ByteOnce(int e_iAddress,int e_iValue)
	{
		ULONG DataLength;
		JMGB_PORT_WRITE_INPUT InputBuffer;    // Input buffer for DeviceIoControl
		DWORD	l_BytesReturned = 0;
		int	l_iSelectedIndex = 0;
		long	l_IoctlCode = IOCTL_JMGB_WRITE_MEMORY_ULONG;
		InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;;
		InputBuffer.LongData = (ULONG)e_iValue;
		DataLength = offsetof(JMGB_PORT_WRITE_INPUT, LongData) + sizeof(InputBuffer.LongData);

		if(DeviceIoControl(
								m_hHandle,            // Handle to device
								l_IoctlCode,          // IO Control code for Read
								&(InputBuffer),        // Buffer to driver.
								DataLength, // Length of buffer in bytes.
								NULL,        // the ULONG to be written to port.
								0,         // Length of buffer in bytes.
								&l_BytesReturned,    // Bytes placed in DataBuffer.
								NULL                // NULL means wait till op. completes.
								))
								return true;
		return false;
	}
	//
//====================================================================================================================
//for PIO
//====================================================================================================================

	//bool cIOSMDriverInterface::WriteIOBitAddressByPIO(int e_iAddress,int e_iBitAddress,bool e_bOn)
	//{
	//	if (this->m_hHandle == INVALID_HANDLE_VALUE)
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
	//	}//fetch data
	//	assert(e_iBitAddress<8&&e_iBitAddress>-1);
	//	JMGB_PORT_WRITE_INPUT InputBuffer;    // Input buffer for DeviceIoControl
	//	int	l_iSelectedIndex = 0;
	//	{//read original data
	//		char	l_cData = '0';
	//		if( this->ReadIOCharByPIO(e_iAddress,&l_cData) )
	//		{
	//			if( e_bOn )
	//				InputBuffer.CharData = l_cData|g_iBitAddressValue[e_iBitAddress];
	//			else
	//			{
	//				InputBuffer.CharData = l_cData&g_iBitAddressOffValue[e_iBitAddress];
	//			}
	//		}
	//		else
	//			return false;
	//	}
	//	DWORD	l_MemOffset = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	long l_IoctlCode = IOCTL_JMGB_PIO_WRITE_MEMORY_UCHAR;
	//	InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	DWORD	l_BytesReturned = 0;
	//	if(!DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&InputBuffer.PortNumber,        // Buffer to driver.
	//							sizeof(ULONG), // Length of buffer in bytes.
	//							&(InputBuffer.CharData),        // Buffer from driver.
	//							sizeof(UCHAR),         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							))
	//	// Did the IOCTL succeed?
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
	//	}
	//	return true;
	//}

	//bool cIOSMDriverInterface::ReadIOBitAddressByPIO(int e_iAddress,int e_iBitAddress,bool*e_pbValue)
	//{
	//	char	l_c = 0;
	//	if(ReadIOCharByPIO(e_iAddress,&l_c))
	//	{
	//		*e_pbValue = (l_c&g_iBitAddressValue[e_iBitAddress])?true:false;
	//		return true;
	//	}
	//	return false;
	//}
	////幹你娘
	////對方給的sample沒改道這個不知道對不對
	//bool	cIOSMDriverInterface::ReadIOCharByPIO(int e_iAddress,char*e_pcValue)
	//{
	//	assert(e_pcValue);
	//	if (m_hHandle == INVALID_HANDLE_VALUE)
	//	{
	//		MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
	//		return false;
	//	}

	//	ULONG DataLength = 0;
	//	ULONG Addr = 0;
	//	bool	l_bOn = false;
	//	long	l_IoctlCode = IOCTL_JMGB_PIO_READ_MEMORY_UCHAR;
	//	DataLength = sizeof(DataBuffer.CharData);
	//	int	l_iSelectedIndex = 0;
	//	Addr = m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	DWORD	l_BytesReturned = 0;
	//	bool	l_bResult = DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&Addr,				// Buffer to driver.
	//							sizeof(Addr),		// Length of buffer in bytes.
	//							&DataBuffer,        // Buffer from driver.
	//							DataLength,         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							);

	//	if (!l_bResult)                            // Did the IOCTL succeed?
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Can't read byte."),TEXT("WARNING"),MB_OK);
	//	}
	//	else
	//	{
	//		*e_pcValue = DataBuffer.CharData;
	//	}

	//	return true;
	//}

	//bool	cIOSMDriverInterface::WriteIOCharByPIO(int e_iAddress,char e_cValue)
	//{
	//	if (this->m_hHandle == INVALID_HANDLE_VALUE)
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
	//		//return;
	//	}//fetch data
	//	DWORD	l_BytesReturned = 0;
	//	long l_IoctlCode = IOCTL_JMGB_PIO_WRITE_MEMORY_UCHAR;
	//	int	l_iSelectedIndex = 0;
	//	_JMGB_PORT_WRITE_INPUT	InputBuffer;

	//	InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	InputBuffer.CharData = e_cValue;
	//	ULONG	DataLength = offsetof(_JMGB_PORT_WRITE_INPUT, CharData) + sizeof(InputBuffer.CharData);

	//	if(!DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&InputBuffer.PortNumber,        // Buffer to driver.
	//							sizeof(ULONG), // Length of buffer in bytes.
	//							&(InputBuffer.CharData),        // Buffer from driver.
	//							sizeof(UCHAR),         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							))
	//	// Did the IOCTL succeed?
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
	//	}
	//	return true;		
	//}

	//bool	cIOSMDriverInterface::WriteIORangeByPIO(int e_iAddress,char* e_cValue,int e_iSize)
	//{
	//	if (this->m_hHandle == INVALID_HANDLE_VALUE)
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
	//		//return;
	//	}//fetch data
	//	int	l_iSelectedIndex = 0;
	//	_JMGB_PORT_WRITE_BYTES	InputBuffer;
	//	memcpy(InputBuffer.DataBuffer,e_cValue,sizeof(char)*e_iSize);
	//	long l_IoctlCode = IOCTL_JMGB_PIO_WRITE_MEMORY_UCHARS;
	//	InputBuffer.PortAddr = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	InputBuffer.ulLen = e_iSize*sizeof(char);
	//	ULONG	InputBufferSize = sizeof(JMGB_PORT_WRITE_BYTES);
	//	DWORD	l_BytesReturned = 0;
	//	if(!DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&InputBuffer,        // Buffer to driver.
	//							InputBufferSize, // Length of buffer in bytes.
	//							&(InputBuffer.DataBuffer),        // Buffer from driver.
	//							sizeof(InputBuffer.DataBuffer),         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							))
	//	// Did the IOCTL succeed?
	//	{
	//		return false;
	//		MessageBox(0,TEXT("Can't write byte."),TEXT("WARNING"),MB_OK);
	//	}
	//	return true;			
	//}


	////
	//bool	cIOSMDriverInterface::WriteIO4ByteOnceByPIO(int e_iAddress,int e_iValue)
	//{
	//	ULONG DataLength;
	//	JMGB_PORT_WRITE_INPUT InputBuffer;    // Input buffer for DeviceIoControl
	//	DWORD	l_BytesReturned = 0;
	//	int	l_iSelectedIndex = 0;
	//	long	l_IoctlCode = IOCTL_JMGB_PIO_WRITE_MEMORY_ULONG;
	//	InputBuffer.PortNumber = this->m_MemBases[l_iSelectedIndex] + e_iAddress;;
	//	InputBuffer.LongData = (ULONG)e_iValue;
	//	DataLength = offsetof(JMGB_PORT_WRITE_INPUT, LongData) + sizeof(InputBuffer.LongData);

	//	if(DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&(InputBuffer.PortNumber),        // Buffer to driver.
	//							sizeof(ULONG), // Length of buffer in bytes.
	//							&(InputBuffer.LongData),        // the ULONG to be written to port.
	//							sizeof(ULONG),         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							))
	//							return true;
	//	return false;
	//}
	////
	//bool	cIOSMDriverInterface::ReadIO4ByteOnceByPIO(int e_iAddress,int*e_piValue)
	//{
	//	// TODO: 在此加入控制項告知處理常式程式碼
	//	ULONG DataLength;
	//	int	l_iSelectedIndex = 0;
	//	DWORD	l_BytesReturned = 0;
	//	ULONG Addr = this->m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	long	l_IoctlCode = IOCTL_JMGB_PIO_READ_MEMORY_ULONG;
	//	DataLength = sizeof(DataBuffer.LongData);
	//	if(DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&Addr,				// Buffer to driver.
	//							sizeof(Addr),		// Length of buffer in bytes.
	//							&DataBuffer,        // Buffer from driver.
	//							DataLength,         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							))
	//	{
	//		*e_piValue = DataBuffer.LongData;
	//		return true;
	//	}

	//	return false;
	//}

	//bool	cIOSMDriverInterface::ReadIORangeByPIO(int e_iAddress,int e_iSize,char*e_pcOutData)
	//{
	//	if (m_hHandle == INVALID_HANDLE_VALUE)
	//	{
	//		MessageBox(0,TEXT("Open device failed."),TEXT("WARNING"),MB_OK);
	//		return false;
	//	}

	//	JMGB_PORT_READ_BYTES	l_InputBuffer;
	//	long	l_IoctlCode = IOCTL_JMGB_PIO_READ_MEMORY_UCHARS;
	//	int	l_iSelectedIndex = 0;
	//	DWORD	l_BytesReturned = 0;

	//	l_InputBuffer.PortAddr = m_MemBases[l_iSelectedIndex] + e_iAddress;
	//	l_InputBuffer.ulLen = sizeof(char)*e_iSize;
	//	
	//	bool	l_bResult = DeviceIoControl(
	//							m_hHandle,            // Handle to device
	//							l_IoctlCode,          // IO Control code for Read
	//							&l_InputBuffer,				// Buffer to driver.
	//							sizeof(l_InputBuffer),		// Length of buffer in bytes.
	//							e_pcOutData,        // Buffer from driver.
	//							l_InputBuffer.ulLen,         // Length of buffer in bytes.
	//							&l_BytesReturned,    // Bytes placed in DataBuffer.
	//							NULL                // NULL means wait till op. completes.
	//							);

	//	if (!l_bResult)                            // Did the IOCTL succeed?
	//	{
	//		//return false;
	//		MessageBox(0,TEXT("Can't read byte."),TEXT("WARNING"),MB_OK);
	//		return false;
	//	}
	//	return true;
	//}

	void	cIOSMDriverInterface::WriteGameKey()
	{
		WriteIO4ByteOnce(0x3000,0xb3a29180);
		WriteIO4ByteOnce(0x3004,0xf7e6d5c4);
	}

	void	cIOSMDriverInterface::WriteInputIOSetup()
	{
		//1020,1024,1028,102c,1030=>00
		//1034,1038,103c=>ff
		//int	l_iInputKeyWriteFor00[5] = { 0x1020,0x1024,0x1028,0x102c,0x1030 };	//count 5,00
		//int	l_iInputKeyWriteForff[3] = { 0x1034,0x1038,0x103c };				//count 3,ff
		int	l_For00[5] = {0,0,0,0,0};
		int	l_Forff[3] = {0xff,0xff,0xff};
		this->WriteIORange(0x1020,(char*)l_For00,sizeof(int)*5);
		this->WriteIORange(0x1034,(char*)l_Forff,sizeof(int)*3);
	}
//end namespace
}

#endif