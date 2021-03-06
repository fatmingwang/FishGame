#ifdef WIN32
/*++

Copyright (c) 1990-2000 Microsoft Corporation, All Rights Reserved

Module Name:

    jmioctl.h

Abstract:

    Include file for Generic Port I/O Example Driver

--*/

#include <winioctl.h>

#if     !defined(__JMIOCTL_H__)
#define __JMIOCTL_H__

//
// Define the IOCTL codes we will use.  The IOCTL code contains a command
// identifier, plus other information about the device, the type of access
// with which the file must have been opened, and the type of buffering.
//

//
// Device type           -- in the "User Defined" range."
//

#define GPD_TYPE 40000

// The IOCTL function codes from 0x800 to 0xFFF are for customer use.

#define IOCTL_JMGB_GET_NUM_OF_MAPPED_MEMORY_BASES \
    CTL_CODE( GPD_TYPE, 0x820, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_GET_MAPPED_MEMORY_BASES \
    CTL_CODE( GPD_TYPE, 0x821, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_GET_MAPPED_MEMORY_LENGTH \
    CTL_CODE( GPD_TYPE, 0x822, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_WRITE_MEMORY_UCHAR \
    CTL_CODE(GPD_TYPE,  0x823, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_WRITE_MEMORY_USHORT \
    CTL_CODE(GPD_TYPE,  0x824, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_WRITE_MEMORY_ULONG \
    CTL_CODE(GPD_TYPE,  0x825, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_MEMORY_UCHAR \
    CTL_CODE( GPD_TYPE, 0x826, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_READ_MEMORY_USHORT \
    CTL_CODE( GPD_TYPE, 0x827, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_READ_MEMORY_ULONG \
    CTL_CODE( GPD_TYPE, 0x828, METHOD_BUFFERED, FILE_READ_ACCESS )


#define IOCTL_JMGB_GET_NUM_OF_PORT_BASES \
    CTL_CODE( GPD_TYPE, 0x829, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_GET_PORT_BASES \
    CTL_CODE( GPD_TYPE, 0x82A, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_GET_PORT_COUNT \
    CTL_CODE( GPD_TYPE, 0x82B, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_WRITE_PORT_UCHAR \
    CTL_CODE(GPD_TYPE,  0x82C, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_WRITE_PORT_USHORT \
    CTL_CODE(GPD_TYPE,  0x82D, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_WRITE_PORT_ULONG \
    CTL_CODE(GPD_TYPE,  0x82E, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_PORT_UCHAR \
    CTL_CODE( GPD_TYPE, 0x82F, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_READ_PORT_USHORT \
    CTL_CODE( GPD_TYPE, 0x830, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_READ_PORT_ULONG \
    CTL_CODE( GPD_TYPE, 0x831, METHOD_BUFFERED, FILE_READ_ACCESS )



#define IOCTL_JMGB_WRITE_MEMORY_UCHARS \
    CTL_CODE(GPD_TYPE,  0x832, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_MEMORY_UCHARS \
    CTL_CODE( GPD_TYPE, 0x833, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_WRITE_PORT_UCHARS \
    CTL_CODE(GPD_TYPE,  0x834, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_PORT_UCHARS \
    CTL_CODE( GPD_TYPE, 0x835, METHOD_BUFFERED, FILE_READ_ACCESS )


#define IOCTL_JMGB_WRITE_MEMORY_USHORTS \
    CTL_CODE(GPD_TYPE,  0x836, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_MEMORY_USHORTS \
    CTL_CODE( GPD_TYPE, 0x837, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_WRITE_PORT_USHORTS \
    CTL_CODE(GPD_TYPE,  0x838, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_PORT_USHORTS \
    CTL_CODE( GPD_TYPE, 0x839, METHOD_BUFFERED, FILE_READ_ACCESS )


#define IOCTL_JMGB_WRITE_MEMORY_ULONGS \
    CTL_CODE(GPD_TYPE,  0x83A, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_MEMORY_ULONGS \
    CTL_CODE( GPD_TYPE, 0x83B, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_JMGB_WRITE_PORT_ULONGS \
    CTL_CODE(GPD_TYPE,  0x83C, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_READ_PORT_ULONGS \
    CTL_CODE( GPD_TYPE, 0x83D, METHOD_BUFFERED, FILE_READ_ACCESS )



// The following IOCTLs use direct I/O


#define IOCTL_JMGB_PIO_WRITE_MEMORY_UCHAR \
    CTL_CODE(GPD_TYPE,  0x860, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_WRITE_MEMORY_USHORT \
    CTL_CODE(GPD_TYPE,  0x861, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_WRITE_MEMORY_ULONG \
    CTL_CODE(GPD_TYPE,  0x862, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_MEMORY_UCHAR \
    CTL_CODE( GPD_TYPE, 0x863, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_READ_MEMORY_USHORT \
    CTL_CODE( GPD_TYPE, 0x864, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_READ_MEMORY_ULONG \
    CTL_CODE( GPD_TYPE, 0x865, METHOD_OUT_DIRECT, FILE_READ_ACCESS )


#define IOCTL_JMGB_PIO_WRITE_PORT_UCHAR \
    CTL_CODE(GPD_TYPE,  0x866, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_WRITE_PORT_USHORT \
    CTL_CODE(GPD_TYPE,  0x867, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_WRITE_PORT_ULONG \
    CTL_CODE(GPD_TYPE,  0x868, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_PORT_UCHAR \
    CTL_CODE( GPD_TYPE, 0x869, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_READ_PORT_USHORT \
    CTL_CODE( GPD_TYPE, 0x86A, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_READ_PORT_ULONG \
    CTL_CODE( GPD_TYPE, 0x86B, METHOD_OUT_DIRECT, FILE_READ_ACCESS )



#define IOCTL_JMGB_PIO_WRITE_MEMORY_UCHARS \
    CTL_CODE(GPD_TYPE,  0x86C, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_MEMORY_UCHARS \
    CTL_CODE( GPD_TYPE, 0x86D, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_WRITE_PORT_UCHARS \
    CTL_CODE(GPD_TYPE,  0x86E, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_PORT_UCHARS \
    CTL_CODE( GPD_TYPE, 0x86F, METHOD_OUT_DIRECT, FILE_READ_ACCESS )


#define IOCTL_JMGB_PIO_WRITE_MEMORY_USHORTS \
    CTL_CODE(GPD_TYPE,  0x870, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_MEMORY_USHORTS \
    CTL_CODE( GPD_TYPE, 0x871, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_WRITE_PORT_USHORTS \
    CTL_CODE(GPD_TYPE,  0x872, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_PORT_USHORTS \
    CTL_CODE( GPD_TYPE, 0x873, METHOD_OUT_DIRECT, FILE_READ_ACCESS )


#define IOCTL_JMGB_PIO_WRITE_MEMORY_ULONGS \
    CTL_CODE(GPD_TYPE,  0x874, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_MEMORY_ULONGS \
    CTL_CODE( GPD_TYPE, 0x875, METHOD_OUT_DIRECT, FILE_READ_ACCESS )

#define IOCTL_JMGB_PIO_WRITE_PORT_ULONGS \
    CTL_CODE(GPD_TYPE,  0x876, METHOD_IN_DIRECT, FILE_WRITE_ACCESS)

#define IOCTL_JMGB_PIO_READ_PORT_ULONGS \
    CTL_CODE( GPD_TYPE, 0x877, METHOD_OUT_DIRECT, FILE_READ_ACCESS )


#endif
//WIN32
#endif