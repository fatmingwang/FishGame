// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FPGAWINXPLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FPGAWINXPLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
// History:
//	2009/06/30 Adding edge setting and counter for SPR inputting
//	2009/08/03	2.2.3	Add SPRIRQ to exclude SPR interrupt.
//	2009/10/16	2.3.0	Add RTC and fs8816 for GP100.
//	2009/10/28	2.3.0	Add FameG
//	2009/11/03	2.3.0	Add GA1000 and build module.
//						5.1 General items:
//							GPI, GPO, GPI Interrupt, Timer, Memory(NRam), Random, Information.
//						5.2 Option items:
//							SPR, Door, Door Interrupt, Intrusion, RTC, FameG, Memory(Flash)
//  2009/11/13	2.3.0	Random number length : 16 bits --> 32 bits (unsigned short --> unsigned long)
//  2009/12/28	2.4.0	Implement flash functions -  Add erasing functions.
//  2010/04/16	2.6.0	FameG - Add new APIs ReadNVMOpr() and WriteNVMOpr().
//  2010/04/16	2.6.0	Add WDT(Watch dog Timer).
//  2010/11/18	2.8.0	Memory - NVRam and Flash add new APIs to get size.
//  2010/11/23	2.8.1	Add Intrusion interrupt.
//  2010/12/10			Add power off test.
//  2010/12/24	2.8.2	Memory - NVRam add mirror function.
//  2011/01/13  2.8.3	Add "EEPROM" Function.
//  2011/02/21	2.8.5	Add "Intrusion door status" function.	
//  2011/04/19	2.9.0	Modified Intrusion memory mapping(GA2200) and add following items:
//							01.Bool _FlashEnable()
//							02.Bool _EEPROMEnable()
//							03.Bool _IntrusionEnable()
//							04.Bool _PowerOffEnable()
//							05.Bool _DoorEnable()
//							06.Bool _FameGEnable()
//							07.Bool _RTCEnable()
//							08.Bool _WDTEnable()
//							09.Bool _SPREnable()
//							10.Bool _AutoTimerEnable()
//							11.Bool _TimerEnable()
//							12.Bool _RandomEnable()
//	2011/04/19	2.9.0	Add setting ram no function.


#ifndef _FPGAWINXPLIB_H
#define _FPGAWINXPLIB_H 

#ifdef FPGAWINXPLIB_EXPORTS
#define FPGAWINXPLIB_API __declspec(dllexport)
#else
#define FPGAWINXPLIB_API __declspec(dllimport)
#endif

//FPGA Information Structure
typedef struct
{
	char szManufactureID[5+1];			// 00000~00004	Manufacture ID
	char szCustomerID[8+1];				// 00005~0000C	Customer ID
	char szModuleName[7+1];				// 0000D~00013	Module Name
	unsigned int szVersionNumber[3];	// 00014~00016	Version Number
	char szBuildtime[8+1];				// 00017~0001E	Buildtime
	int szGPIOdir[5];					// 0001F~00023	GPIO output define
	unsigned int szAPIVersion[3];		// API Version
	char szFPGADriverVerion[12];		// FPGA Driver
}	FPGAInfo, *pFPGAInfo;

enum _MEMORY_TYPE
{
	RAM,
	FLASH,
	// 2011/01/13 Add "EEPROM" Function
	EEPROM,
	TYPEMAX
};

// ============================================================================
// "FPGA Initial and Uninitial Function (These functions just for Window XP.)
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _FPGAInit(DWORD * dwError, PVOID funcIntHandler);
extern "C" FPGAWINXPLIB_API BOOL _FPGAUnInit();
// ============================================================================
// "General Output" Function (GPO)
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAOCount(unsigned char *ucCount);						// Valid output port counts. 
extern "C" FPGAWINXPLIB_API BOOL _SetFPGAOIdx(unsigned long uIdx, bool bFlag);					// Set bFlag to uIdx position of output port. 
extern "C" FPGAWINXPLIB_API BOOL _SetFPGAOAll(bool bFlag);										// Set bFlag to all position of output port.
extern "C" FPGAWINXPLIB_API BOOL _SetFPGAOutput(unsigned long ulData1,unsigned long ulData2);	// Set ulData1 to 0~31 position of output port and set ulData2 to 32~63 position.   
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAOIdx(unsigned long uIdx, bool *bFlag);					// Get bFlag from uIdx position of output port.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAOutput(unsigned long* ulData1,unsigned long* ulData2);	// Get ulData1 from 0~31 position of output port and get ulData2 from 32~63 position.

// ============================================================================
// "General Input" Function (GPI)
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAICount(unsigned char *ucCount);						// Valid input port counts.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAIIdx(unsigned long uIdx, bool *bFlag);					// Get bFlag from uIdx position of input port.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAInput(unsigned long* ulData1,unsigned long* ulData2);	// Get ulData1 from 0~31 position of input port and get ulData2 from 32~63 position.
// Set debounce time
extern "C" FPGAWINXPLIB_API BOOL _SetFPGAIHWDebTime(unsigned long uIdxGroup, unsigned long time);

// ============================================================================
// "RANDOM" Function
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetRandomCount(unsigned char *ucCount);						// Get random counts which hardware support.
//2009/11/13 Modified Random number length 16bits -> 32bits (unsigned short -> unsigned long)
extern "C" FPGAWINXPLIB_API BOOL _GetRandom(unsigned long uIdx, unsigned long* ulData);		// Get hardware random number from uIdx.
//2011/04/19 Add Random enable function.
extern "C" FPGAWINXPLIB_API BOOL _RandomEnable(bool *bEnable);		

// ============================================================================
// "INFORMATION" Function
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetFPGAInfor(FPGAInfo * pInfo);

// ============================================================================
// "FLASH/RAM ACCESS" Function
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _RamWriteByte(unsigned long dwOffset, unsigned char ucData);
extern "C" FPGAWINXPLIB_API BOOL _RamReadByte(unsigned long dwOffset, unsigned char* ucData);
// 2010/11/18 Memory - NVRam and Flash add new APIs to get size.
extern "C" FPGAWINXPLIB_API BOOL _GetRamSize(unsigned long* ulSize);
// 2010/12/24 Memory - NVRam add mirror function.
extern "C" FPGAWINXPLIB_API BOOL _GetRamMirrorEn(bool *bFlag);
extern "C" FPGAWINXPLIB_API BOOL _SetRamMirrorRange(unsigned char ucMirrorMode, unsigned long ulAddrS, unsigned long ulAddrE);
extern "C" FPGAWINXPLIB_API BOOL _SetRamMirrorStart();
extern "C" FPGAWINXPLIB_API BOOL _GetRamMirrorIsFinished(bool *bFlag);
// 2011/04/19 Memory - NVRam add ram no.
extern "C" FPGAWINXPLIB_API BOOL _RamNoEnable(bool *bEnable);
extern "C" FPGAWINXPLIB_API BOOL _GetRamNo(unsigned char *ucRamNo);
extern "C" FPGAWINXPLIB_API BOOL _SetRamNo(unsigned char ucRamNo);

// ============================================================================
//  GPI Interrupt Function
// ============================================================================
// PI Enable Register
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPICount(unsigned char *ucCount);
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPIEnableIdx(unsigned long uIdx, bool bFlag);		
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPIEnableAll(bool bFlag); 				
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPIEnable(unsigned long ulData1,unsigned long ulData2);   
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPIEnableIdx(unsigned long uIdx, bool *bFlag);				
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPIEnable(unsigned long* ulData1,unsigned long* ulData2);

// PI Edge Setting Register
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPISettingIdx(unsigned long uIdx, bool bFlag);			
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPISettingAll(bool bFlag);							
extern "C" FPGAWINXPLIB_API BOOL _SetIrqPISetting(unsigned long ulData1,unsigned long ulData2);	
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPISettingIdx(unsigned long uIdx, bool *bFlag);				
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPISetting(unsigned long* ulData1,unsigned long* ulData2);

// PI Counter 
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPICounter(unsigned long uIdx, unsigned long *ulCount);
extern "C" FPGAWINXPLIB_API BOOL _GetIrqPICounterPack(unsigned long * ulCounterBuff, unsigned long ulLen);

// ============================================================================
//  Timer Functions
// ============================================================================
// Auto Timer
extern "C" FPGAWINXPLIB_API BOOL _GetAutoTimerCount(unsigned char *ucCount);
extern "C" FPGAWINXPLIB_API BOOL _GetAutoTimerEnableIdx(unsigned long uIdx, bool *bFlag);
extern "C" FPGAWINXPLIB_API BOOL _SetAutoTimerEnableIdx(unsigned long uIdx, bool bFlag);
//2011/04/19 Add AutoTimer enable function.
extern "C" FPGAWINXPLIB_API BOOL _AutoTimerEnable(bool *bEnable);		

// General Timer
extern "C" FPGAWINXPLIB_API BOOL _GetTimerCount(unsigned char *ucCount);
extern "C" FPGAWINXPLIB_API BOOL _GetTimerEnableIdx(unsigned long uIdx, bool *bFlag);
extern "C" FPGAWINXPLIB_API BOOL _SetTimerEnableIdx(unsigned long uIdx, bool bFlag);
extern "C" FPGAWINXPLIB_API BOOL _GetTimerIdx(unsigned long uIdx, bool *bReload, unsigned long *ulCount, unsigned long *ulInterval);
extern "C" FPGAWINXPLIB_API BOOL _SetTimerIdx(unsigned long uIdx, bool bReload, unsigned long ulCount, unsigned long ulInterval);
//2011/04/19 Add Timer enable function.
extern "C" FPGAWINXPLIB_API BOOL _TimerEnable(bool *bEnable);		

// Timer's counter.
extern "C" FPGAWINXPLIB_API BOOL _GetTimerCounter(unsigned long uIdx, unsigned long *ulCount);
extern "C" FPGAWINXPLIB_API BOOL _GetTimerCounterPack(unsigned long * ulCounterBuff, unsigned long ulLen);

// ============================================================================
//  "DOOR" FUNCTION
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetFPGADoorCount(unsigned char *ucCount);
extern "C" FPGAWINXPLIB_API BOOL _GetFPGADoorIdx(unsigned long uIdx, bool *bFlag);					
extern "C" FPGAWINXPLIB_API BOOL _GetFPGADoor(unsigned short* ulData);			
extern "C" FPGAWINXPLIB_API BOOL _SetFPGADoorHWDebTime(unsigned long time);
//2011/04/19 Add Door enable function.
extern "C" FPGAWINXPLIB_API BOOL _DoorEnable(bool *bEnable);		

// ============================================================================
//  Door Interrupt functions.
// ============================================================================
//DR Enable Register
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDRCount(unsigned char *ucCount);	
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDREnableIdx(unsigned long uIdx, bool bFlag);	
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDREnableAll(bool bFlag); 
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDREnable(unsigned short usData);	   
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDREnableIdx(unsigned long uIdx, bool *bFlag);	
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDREnable(unsigned short* usData);	

//DR Edge Setting Register
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDRSettingIdx(unsigned long uIdx, bool bFlag);		
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDRSettingAll(bool bFlag); 
extern "C" FPGAWINXPLIB_API BOOL _SetIrqDRSetting(unsigned short usData);	   
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDRSettingIdx(unsigned long uIdx, bool *bFlag);	
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDRSetting(unsigned short* usData);	

// DR Counter 
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDRCounter(unsigned long uIdx, unsigned long *ulCount);
extern "C" FPGAWINXPLIB_API BOOL _GetIrqDRCounterPack(unsigned long * ulCounterBuff, unsigned long ulLen);

// =============================================================================================
// SPR : "SPECIALL IO" Function - Attribute value  '0'(false): output, '1'(true): input (default)
// =============================================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetFPGASPRCount(unsigned char *ucCount);				// Valid SPR port counts. 

//2011/04/19 Add SPR enable function.
extern "C" FPGAWINXPLIB_API BOOL _SPREnable(bool *bEnable);		

// SPR I/O attribute setting functions. 
extern "C" FPGAWINXPLIB_API BOOL _GetFPGASPRIdxAtt(unsigned long uIdx, bool *bFlag);	// Get bFlag(I/O) from uIdx position of Attribute.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGASPRAtt(unsigned short *usData);				// Get usData from 0~31 position of Attribute.
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRIdxAtt(unsigned long uIdx, bool bFlag);		// Set bFlag(I/O) to uIdx position of Attribute.
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRAllIdxAtt(bool bFlag);						// Set bFlag(I/O) to all position of Attribute. 
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRAtt(unsigned short usData);					// Set usData to 0~31 position of Attribute.

// SPR value functions.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGASPRIdxData(unsigned long uIdx, bool *bFlag);	// Get bFlag from uIdx position of Data.
extern "C" FPGAWINXPLIB_API BOOL _GetFPGASPRData(unsigned short *usData);				// Get usData from 0~31 position of Data.
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRIdxData(unsigned long ulIdx, bool bFlag);	// Set bFlag to uIdx position of Data. 
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRAllIdxData(bool bFlag);						// Set bFlag to all position of Data.
extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRData(unsigned short usData);					// Set usData to 0~31 position of Data.

#ifdef SPRIRQ
	//  2009/06/30 Add SPR interrupt Enable Register
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPREnableIdx(unsigned long uIdx, bool *bFlag);				
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPREnable(unsigned short* usData);
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPREnableIdx(unsigned long uIdx, bool bFlag);		
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPREnableAll(bool bFlag); 				
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPREnable(unsigned short usData);   

	// 2009/06/30 Add input edge setting
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPRSettingIdx(unsigned long uIdx, bool bFlag);			
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPRSettingAll(bool bFlag);							
	extern "C" FPGAWINXPLIB_API BOOL _SetIrqSPRSetting(unsigned short usData);	
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPRSettingIdx(unsigned long uIdx, bool *bFlag);				
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPRSetting(unsigned short* usData);

	// 2009/06/30 Add input counter
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPRCounter(unsigned long uIdx, unsigned long *ulCount);
	extern "C" FPGAWINXPLIB_API BOOL _GetIrqSPRCounterPack(unsigned long * ulCounterBuff, unsigned long ulLen);

	// 2009/06/30 Add debounce time
	extern "C" FPGAWINXPLIB_API BOOL _SetFPGASPRHWDebTime(unsigned long uIdxGroup, unsigned long time);
#endif

// ==========================================================
//       Intrusion functions.
// ==========================================================
struct _INTRUSION_LOG
{
	int iEventID;
	SYSTEMTIME st;
};

struct _INTRUSION_INFOR
{
//	char szSWVersionNo[10];
	char szFWVersionNo[10];
	char szBuildTime[11];
};

// Define Intrusion event code
#define INTR_ENABLE			0x0
#define INTR_RESET			0x1
#define INTR_CLEAR_BUF		0x2
#define INTR_LOAD_BUF		0x3
#define INTR_GET_VER		0x4
// Define Intrusion Log Event Table.
#define INTR_LOG_VER_NO		0x22

extern "C" FPGAWINXPLIB_API BOOL _IntrInit();
extern "C" FPGAWINXPLIB_API BOOL _IntrUnInit();
extern "C" FPGAWINXPLIB_API BOOL _IntrResetTime(SYSTEMTIME st);
extern "C" FPGAWINXPLIB_API BOOL _IntrReadTime(SYSTEMTIME * pst);
extern "C" FPGAWINXPLIB_API BOOL _IntrGetLogCount(unsigned char * ucNumber);
extern "C" FPGAWINXPLIB_API BOOL _IntrLoadLog();
extern "C" FPGAWINXPLIB_API BOOL _IntrClearLog();
extern "C" FPGAWINXPLIB_API BOOL _IntrCheckLog(unsigned long ulIndex, _INTRUSION_LOG * pLog);
extern "C" FPGAWINXPLIB_API BOOL _IntrGetInfor(_INTRUSION_INFOR * pInfor);
extern "C" FPGAWINXPLIB_API BOOL _IntrGetLogCount(unsigned char * ucNumber);
// 2010/11/23 Add Intrusion interrupt.
extern "C" FPGAWINXPLIB_API BOOL _IntrGetIrqEn(bool *bFlag);
extern "C" FPGAWINXPLIB_API BOOL _IntrGetIrqOut(unsigned long *ulIrqOut);
// 2011/02/21 Add "Intrusion door status" function.	
extern "C" FPGAWINXPLIB_API BOOL _IntrGetDoorCount(unsigned char * ucCount);
extern "C" FPGAWINXPLIB_API BOOL _IntrGetDoorStatus(unsigned long ulIndex, bool *bFlag);
//2011/04/19 Add intrusion enable function.
extern "C" FPGAWINXPLIB_API BOOL _IntrusionEnable(bool *bEnable);		

// Test
extern "C" FPGAWINXPLIB_API BOOL _IntrGetTestIrq(unsigned long *ulIrqOut, unsigned long *ulIrqIn);


// ===================================================================
//	2009/10/16 GP100 - RTC FUNCTION
// ===================================================================
typedef struct
{
	unsigned short Year;			// 00000	Year (00~99)
	unsigned short Month;			// 00002	Month (1~12)
	unsigned short day;				// 00004	Day (1~7)
	unsigned short date;			// 00006	Date (1~31)
	unsigned short hour;			// 00008	Hour (00~23)
	unsigned short Min;				// 00010	Minute (00~59)
	unsigned short Sec;				// 00012	Second (00~59)
	unsigned short Century;			// 00014	century(0~1) 
}	FPGA_RTC, *pFPGA_RTC;

extern "C" FPGAWINXPLIB_API BOOL _GetFPGARTCStatus(bool *bWriteFlag);
extern "C" FPGAWINXPLIB_API BOOL _GetFPGARTC(FPGA_RTC *RTCTime);
extern "C" FPGAWINXPLIB_API BOOL _SetFPGARTC(FPGA_RTC RTCTime);
//2011/04/19 Add RTC enable function.
extern "C" FPGAWINXPLIB_API BOOL _RTCEnable(bool *bEnable);		

// ============================================================================
// 2009/10/28 Add "FameG" Function
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _FameGCommTest(unsigned char SlvAdr);
extern "C" FPGAWINXPLIB_API BOOL _FameGInit(unsigned char SlvAdr, unsigned char* KEY);
extern "C" FPGAWINXPLIB_API BOOL _FameGAuthen(unsigned long MessageAddress, LPCWSTR filename);
// 2010/04/16 Add new APIs ReadNVMOpr() and WriteNVMOpr().
extern "C" FPGAWINXPLIB_API BOOL _FameGWriteNVMOpr (unsigned char ucNvmoffset, unsigned char ucLen, unsigned char * ucBuffer);
extern "C" FPGAWINXPLIB_API BOOL _FameGReadNVMOpr (unsigned char ucNvmoffset, unsigned char uclen, unsigned char * ucbuffer);
// 2010/11/18 Memory - NVRam and Flash add new APIs to get size.
extern "C" FPGAWINXPLIB_API BOOL _GetFlashSize(unsigned long* ulSize);
//2011/04/19 Add FameG enable function.
extern "C" FPGAWINXPLIB_API BOOL _FameGEnable(bool *bEnable);		

// ============================================================================
//  "Flash" Functions
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _FlashWriteByte(unsigned long dwOffset, unsigned char ucData);
extern "C" FPGAWINXPLIB_API BOOL _FlashReadByte(unsigned long dwOffset, unsigned char* ucData);
// 2009/12/28 Add flash erase functions.
extern "C" FPGAWINXPLIB_API BOOL _FlashEraseAll();
extern "C" FPGAWINXPLIB_API BOOL _FlashEraseSector(unsigned long ulSector);
//2011/04/19 Add Flash enable function.
extern "C" FPGAWINXPLIB_API BOOL _FlashEnable(bool *bEnable);		

// ============================================================================
//  2010/04/16 Add "WDT" Functions
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _SetWDTEnable(bool bFlag);
extern "C" FPGAWINXPLIB_API BOOL _SetWDTReset(bool bFlag);
extern "C" FPGAWINXPLIB_API BOOL _GetWDTCountValue(unsigned short *usCount);
extern "C" FPGAWINXPLIB_API BOOL _SetWDTCountValue(unsigned short usCount);
//2011/04/19 Add WDT enable function.
extern "C" FPGAWINXPLIB_API BOOL _WDTEnable(bool *bEnable);		

// ============================================================================
//  2010/12/10 Add "Power Off Test" Functions
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetPWOffIrqEn(bool *bFlag);
extern "C" FPGAWINXPLIB_API BOOL _GetPWOffIrqValue(bool *bFlag);
//2011/04/19 Add Power Off enable function.
extern "C" FPGAWINXPLIB_API BOOL _PowerOffEnable(bool *bEnable);
// 2011/04/29 Add poweroff open function.
extern "C" FPGAWINXPLIB_API BOOL _SetPWOffIrqOpen(bool bOpen);

// ============================================================================
// 2011/01/13 Add "EEPROM" Function
// ============================================================================
extern "C" FPGAWINXPLIB_API BOOL _GetEEPRomSize(unsigned long * ulSize);
extern "C" FPGAWINXPLIB_API BOOL _EEPRomWrite( unsigned char StartAddr, unsigned char * ucBuffer, unsigned long ulLen);
extern "C" FPGAWINXPLIB_API BOOL _EEPRomRead( unsigned char StartAddr, unsigned char * ucbuffer, unsigned long ullen);
//2011/04/19 Add EEPROM enable function.
extern "C" FPGAWINXPLIB_API BOOL _EEPROMEnable(bool *bEnable);		

// This class is exported from the FPGAWinCELib.dll
class FPGAWINXPLIB_API cFPGA {
public:
	cFPGA();
	virtual ~cFPGA();
	// TODO: add your methods here.
	// ============================================================================
	// "FPGA Initial and Uninitial Function (These functions just for Window XP.)
	// ============================================================================
	BOOL FPGAInit(DWORD * dwError, PVOID funcIntHandler);
	BOOL FPGAUnInit();
	// ========================================================================
	// "GENERAL IO" Function
	// ========================================================================
	BOOL GetFPGAOCount(unsigned char *ucCount);
	BOOL GetFPGAICount(unsigned char *ucCount);
	BOOL SetFPGAOIdx(unsigned long uIdx, bool bFlag);
	BOOL SetFPGAOAll(bool bFlag);
	BOOL SetFPGAOutput(unsigned long ulData1,unsigned long ulData2);
	BOOL GetFPGAOIdx(unsigned long uIdx, bool *bFlag);
	BOOL GetFPGAOutput(unsigned long* ulData1,unsigned long* ulData2);
	BOOL GetFPGAIIdx(unsigned long uIdx, bool *bFlag);	
	BOOL GetFPGAInput(unsigned long* ulData1,unsigned long* ulData2);
	BOOL SetFPGAIHWDebTime(unsigned long uIdxGroup, unsigned long time);

	// ****************************************************************************
	// "DOOR" FUNCTION
	// ****************************************************************************
	BOOL GetFPGADoorCount(unsigned char *ucCount);
	BOOL GetFPGADoorIdx(unsigned long uIdx, bool *bFlag);			
	BOOL GetFPGADoor(unsigned short* ulData);			
	BOOL SetFPGADoorHWDebTime(unsigned long time);
	//2011/04/19 Add Door enable function.
	BOOL DoorEnable(bool *bEnable);		

	// ============================================================================
	// "RANDOM" Function
	// ============================================================================
	BOOL GetRandomCount(unsigned char *ucCount);
	//2009/11/13 Modified Random number length 16bits -> 32bits (unsigned short -> unsigned long)
	BOOL GetRandom(unsigned long uIdx, unsigned long* ulData);	
	//2011/04/19 Add Random enable function.
	BOOL RandomEnable(bool *bEnable);		

	// ========================================================================
	// "INFORMATION" Function
	// ========================================================================
	BOOL GetFPGAInfor(FPGAInfo * pInfo);

	// ============================================================================
	// "FLASH RAM ACCESS" Function
	// ============================================================================
	BOOL RamWriteByte(unsigned long dwOffset, unsigned char ucData);
	BOOL RamReadByte(unsigned long dwOffset, unsigned char* ucData);
	// 2010/11/18 Memory - NVRam and Flash add new APIs to get size.
	BOOL GetRamSize(unsigned long* ulSize);
	// 2010/12/24 Memory - NVRam add mirror function.
	BOOL GetRamMirrorEn(bool *bFlag);
	BOOL SetRamMirrorRange(unsigned char ucMirrorMode, unsigned long ulAddrS, unsigned long ulAddrE);
	BOOL SetRamMirrorStart();
	BOOL GetRamMirrorIsFinished(bool *bFlag);
	// 2011/04/19 Memory - NVRam add ram no.
	BOOL RamNoEnable(bool *bEnable);
	BOOL GetRamNo(unsigned char *ucRamNo);
	BOOL SetRamNo(unsigned char ucRamNo);

	BOOL FlashWriteByte(unsigned long dwOffset, unsigned char ucData);
	BOOL FlashReadByte(unsigned long dwOffset, unsigned char* ucData);
	// 2009/12/28 Add flash erase functions.
	BOOL FlashEraseAll();
	BOOL FlashEraseSector(unsigned long ulSector);
	// 2010/11/18 Memory - NVRam and Flash add new APIs to get size.
	BOOL GetFlashSize(unsigned long* ulSize);
	//2011/04/19 Add Flash enable function.
	BOOL FlashEnable(bool *bEnable);

	// ============================================================================
	// "IRQ" Function
	// ============================================================================
	// PI Enable Register
	BOOL GetIrqPICount(unsigned char *ucCount);
	BOOL SetIrqPIEnableIdx(unsigned long uIdx, bool bFlag);				
	BOOL SetIrqPIEnableAll(bool bFlag); 
	BOOL SetIrqPIEnable(unsigned long ulData1,unsigned long ulData2);	
	BOOL GetIrqPIEnableIdx(unsigned long uIdx, bool *bFlag);		
	BOOL GetIrqPIEnable(unsigned long* ulData1,unsigned long* ulData2);	

	//DR Enable Register
	BOOL GetIrqDRCount(unsigned char *ucCount);		
	BOOL SetIrqDREnableIdx(unsigned long uIdx, bool bFlag);		
	BOOL SetIrqDREnableAll(bool bFlag); 
	BOOL SetIrqDREnable(unsigned short usData);	
	BOOL GetIrqDREnableIdx(unsigned long uIdx, bool *bFlag);	
	BOOL GetIrqDREnable(unsigned short* usData);	

	// PI Enable Register
	BOOL SetIrqPISettingIdx(unsigned long uIdx, bool bFlag);	
	BOOL SetIrqPISettingAll(bool bFlag); 
	BOOL SetIrqPISetting(unsigned long ulData1,unsigned long ulData2);	
	BOOL GetIrqPISettingIdx(unsigned long uIdx, bool *bFlag);				
	BOOL GetIrqPISetting(unsigned long* ulData1,unsigned long* ulData2);	

	//DR Enable Register
	BOOL SetIrqDRSettingIdx(unsigned long uIdx, bool bFlag);	
	BOOL SetIrqDRSettingAll(bool bFlag); 
	BOOL SetIrqDRSetting(unsigned short usData);	
	BOOL GetIrqDRSettingIdx(unsigned long uIdx, bool *bFlag);			
	BOOL GetIrqDRSetting(unsigned short* usData);	

	// Counter
	BOOL GetIrqPICounter(unsigned long uIdx, unsigned long *ulCount);
	BOOL GetIrqPICounterPack(unsigned long *ulCounterBuff, unsigned long ulLen);
	BOOL GetIrqDRCounter(unsigned long uIdx, unsigned long *ulCount);
	BOOL GetIrqDRCounterPack(unsigned long *ulCounterBuff, unsigned long ulLen);
	BOOL GetTimerCounter(unsigned long uIdx, unsigned long *ulCount);
	BOOL GetTimerCounterPack(unsigned long *ulCounterBuff, unsigned long ulLen);

	// ============================================================================
	// "Timer" Function
	// ============================================================================
	BOOL GetTimerCount(unsigned char *ucCount);
	BOOL GetAutoTimerCount(unsigned char *ucCount);
	BOOL SetAutoTimerEnableIdx(unsigned long uIdx, bool bFlag);
	BOOL GetAutoTimerEnableIdx(unsigned long uIdx, bool *bFlag);
	//2011/04/19 Add AutoTimer enable function.
	BOOL AutoTimerEnable(bool *bEnable);		

	BOOL SetTimerEnableIdx(unsigned long uIdx, bool bFlag);
	BOOL GetTimerEnableIdx(unsigned long uIdx, bool *bFlag);
	BOOL SetTimerIdx(unsigned long uIdx, bool bReload, unsigned long ulCount, unsigned long ulInterval);
	BOOL GetTimerIdx(unsigned long uIdx, bool *bReload, unsigned long *ulCount, unsigned long *ulInterval);
	//2011/04/19 Add Timer enable function.
	BOOL TimerEnable(bool *bEnable);	

	// ****************************************************************************
	// "SPR" FUNCTION
	// ****************************************************************************
	// Attribute
	BOOL GetFPGASPRCount(unsigned char *ucCount);
	BOOL GetFPGASPRIdxAtt(unsigned long uIdx, bool *bFlag);
	BOOL GetFPGASPRAtt(unsigned short *usData);			
	BOOL SetFPGASPRIdxAtt(unsigned long uIdx, bool bFlag);		
	BOOL SetFPGASPRAllIdxAtt(bool bFlag);					
	BOOL SetFPGASPRAtt(unsigned short usData);			
	// Data
	BOOL GetFPGASPRIdxData(unsigned long uIdx, bool *bFlag);	
	BOOL GetFPGASPRData(unsigned short *ulData);				
	BOOL SetFPGASPRIdxData(unsigned long ulIdx, bool bFlag);	
	BOOL SetFPGASPRAllIdxData(bool bFlag);						
	BOOL SetFPGASPRData(unsigned short ulData);	

	//2011/04/19 Add SPR enable function.
	BOOL SPREnable(bool *bEnable);

#ifdef SPRIRQ
	// 2009/06/30 Add SPR interrupt Enable Register
	BOOL GetIrqSPREnableIdx(unsigned long uIdx, bool *bFlag);				
	BOOL GetIrqSPREnable(unsigned short* usData);
	BOOL SetIrqSPREnableIdx(unsigned long uIdx, bool bFlag);		
	BOOL SetIrqSPREnableAll(bool bFlag); 				
	BOOL SetIrqSPREnable(unsigned short usData);  

	// 2009/06/30 Add input edge setting
	BOOL GetIrqSPRSettingIdx(unsigned long uIdx, bool *bFlag);				
	BOOL GetIrqSPRSetting(unsigned short* usData);
	BOOL SetIrqSPRSettingIdx(unsigned long uIdx, bool bFlag);			
	BOOL SetIrqSPRSettingAll(bool bFlag);							
	BOOL SetIrqSPRSetting(unsigned short usData);	

	// 2009/06/30 Add input counter
	BOOL GetIrqSPRCounter(unsigned long uIdx, unsigned long *ulCount);
	BOOL GetIrqSPRCounterPack(unsigned long * ulCounterBuff, unsigned long ulLen);
	// 2009/06/30 Add debounce time
	BOOL SetFPGASPRHWDebTime(unsigned long uIdxGroup, unsigned long time);
#endif

// ****************************************************************************
//	2009/10/16 GP100 - RTC FUNCTION
// ****************************************************************************
	BOOL GetFPGARTCStatus(bool *bWriteFlag);
	BOOL GetFPGARTC(FPGA_RTC *RTCTime);
	BOOL SetFPGARTC(FPGA_RTC RTCTime);
	//2011/04/19 Add RTC enable function.
	BOOL RTCEnable(bool *bEnable);	

// ============================================================================
//  2010/04/16 Add "WDT" Functions
// ============================================================================
	BOOL SetWDTEnable(bool bFlag);
	BOOL SetWDTReset(bool bFlag);
	BOOL GetWDTCountValue(unsigned short *usCount);
	BOOL SetWDTCountValue(unsigned short usCount);
	//2011/04/19 Add WDT enable function.
	BOOL WDTEnable(bool *bEnable);		

// ============================================================================
//  2010/12/10 Add "Power Off Test" Functions
// ============================================================================
	BOOL GetPWOffIrqEn(bool *bFlag);
	BOOL GetPWOffIrqValue(bool *bFlag);
	//2011/04/19 Add Power Off enable function.
	BOOL PowerOffEnable(bool *bEnable);		
	// 2011/04/29 Add poweroff open function.
	BOOL SetPWOffIrqOpen(bool bOpen);

// ============================================================================
// 2011/01/13 Add "EEPROM" Function
// ============================================================================
	BOOL GetEEPRomSize( unsigned long * ulSize);
	BOOL EEPRomWrite( unsigned char StartAddr, unsigned char * ucBuffer, unsigned long ucLen);
	BOOL EEPRomRead( unsigned char StartAddr, unsigned char * ucbuffer, unsigned long uclen);
	//2011/04/19 Add EEPROM enable function.
	BOOL EEPROMEnable(bool *bEnable);	
};

// This class is exported from the FPGAWinCELib.dll 
class FPGAWINXPLIB_API cINTR {
public:
	cINTR();
	virtual ~cINTR();
	// TODO: add your methods here.		
	// ****************************************************************************
	// "INTRUSION" FUNCTION
	// ****************************************************************************
	BOOL IntrInit();
	BOOL IntrUnInit();
	BOOL IntrResetTime(SYSTEMTIME st);
	BOOL IntrReadTime(SYSTEMTIME * pst);
	BOOL IntrGetLogCount(unsigned char * ucNumber);
	BOOL IntrLoadLog();
	BOOL IntrClearLog();
	BOOL IntrCheckLog(unsigned long ulIndex, _INTRUSION_LOG * pLog);
	BOOL IntrGetInfor(_INTRUSION_INFOR * pInfor);
	// 2010/11/23 Add Intrusion interrupt.
	BOOL IntrGetIrqEn(bool *bFlag);
	BOOL IntrGetIrqOut(unsigned long *ulIrqOut);
	// 2011/02/21 Add "Intrusion door status" function.	
	BOOL IntrGetDoorCount(unsigned char * ucCount);
	BOOL IntrGetDoorStatus(unsigned long ulIndex, bool *bFlag);

	//2011/04/19 Add intrusion enable function.
	BOOL IntrusionEnable(bool *bEnable);

	// Test
	BOOL IntrGetTestIrq(unsigned long *ulIrqOut, unsigned long *ulIrqIn);
};

// ============================================================================
// 2009/07/15 Add "FameG" Class.
// ============================================================================
class FPGAWINXPLIB_API cFameG {
public:
	cFameG();
	virtual ~cFameG();
	BOOL FameGCommTest(unsigned char SlvAdr);
	BOOL FameGInit(unsigned char SlvAdr, unsigned char* KEY);
	BOOL FameGAuthen(unsigned long MessageAddress, LPCWSTR filename);
	// 2010/04/16 Add new APIs ReadNVMOpr() and WriteNVMOpr().
	BOOL FameGWriteNVMOpr (unsigned char ucNvmoffset, unsigned char ucLen, unsigned char * ucBuffer);
	BOOL FameGReadNVMOpr (unsigned char ucNvmoffset, unsigned char uclen, unsigned char * ucbuffer);
	//2011/04/19 Add FameG enable function.
	BOOL FameGEnable(bool *bEnable);
};


#endif _FPGAWINXPLIB_H 