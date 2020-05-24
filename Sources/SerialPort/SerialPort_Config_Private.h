#ifndef CSERIALPORT_CONFIG_PRIVATE_H
#define CSERIALPORT_CONFIG_PRIVATE_H
#include <windows.h>
#include "Common\MyTypes.h"
#include "SerialPort\SerialPort_Config.h"

/** Baud Rate Values OS Depending*/
static const TU32_t SerialPort_Config_Baudrate_Values_Tab[MAX_SerialPort_Config_Baudrate]
= {CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_57600, CBR_115200, CBR_128000, CBR_256000};

/** Desired Access to Serial Port OS Depending */
static const TU32_t SerialPort_Config_DesiredAccess_Values_Tab [MAX_SerialPort_Config_DesiredAccess]
= {GENERIC_READ, GENERIC_WRITE, (GENERIC_READ | GENERIC_WRITE)};

/** Desired Share Mode for Serial Port OS Depending */
static const TU32_t SerialPort_Config_ShareMode_Values_Tab [MAX_SerialPort_Config_ShareMode]
= {0, FILE_SHARE_READ, FILE_SHARE_WRITE, (FILE_SHARE_READ | FILE_SHARE_WRITE)};

/** Parity for Serial Port OS Depending */
static const TU32_t SerialPort_Config_Parity_Values_Tab [MAX_SerialPort_Config_Parity]
= {NOPARITY, EVENPARITY, ODDPARITY, MARKPARITY, SPACEPARITY};

/** Action On SerialPort Open OS Depending */
static const TU32_t SerialPort_Config_Action_Values_Tab [MAX_SerialPort_Config_Action]
= {CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING};

/** DTR Control OS Depending */
static const TU32_t SerialPort_Config_DTR_DSR_Control_Values_Tab [MAX_SerialPort_Config_DTR_DSR_Control]
= {DTR_CONTROL_DISABLE, DTR_CONTROL_ENABLE, DTR_CONTROL_HANDSHAKE};

/** RTS Control OS Depending */
static const TU32_t SerialPort_Config_RTS_CTS_Control_Values_Tab [MAX_SerialPort_Config_RTS_CTS_Control]
= {RTS_CONTROL_DISABLE, RTS_CONTROL_ENABLE, RTS_CONTROL_HANDSHAKE, RTS_CONTROL_TOGGLE};

/** Data Bits */
static const TU32_t SerialPort_Config_DataBits_Values_Tab [MAX_SerialPort_Config_DataBits]
= {4, 5, 6, 7, 8};

/** Stop Bits */
static const TU32_t SerialPort_Config_StopBits_Values_Tab [MAX_SerialPort_Config_StopBits]
= {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS}; //1.5 is specified in Serial spécification
#endif