#ifndef CSERIALPORT_CONFIG_H
#define CSERIALPORT_CONFIG_H

#include "Common\MyTypes.h"

/** Deisred Acces to serial port */
typedef enum SerialPort_Config_DesiredAccess
{
	SerialPort_Config_DesiredAccess_Read = 0, 
	SerialPort_Config_DesiredAccess_Write,
	SerialPort_Config_DesiredAccess_Both_ReadWrite,
	MAX_SerialPort_Config_DesiredAccess

}SerialPort_Config_DesiredAccess_t;
static const TC08_t SerialPort_Config_DesiredAccess_STR_TAB [MAX_SerialPort_Config_DesiredAccess][16]
= {"Read", "Write", "Both read Write"};

/** Desired Shared Mode */
typedef enum SerialPort_Config_ShareMode
{
	SerialPort_Config_ShareMode_ExclusiveAccess = 0,
	SerialPort_Config_ShareMode_ReadOnly,
	SerialPort_Config_ShareMode_WriteOnly,
	SerialPort_Config_ShareMode_BothReadWrite,
	MAX_SerialPort_Config_ShareMode

}SerialPort_Config_ShareMode_t;
static const TC08_t SerialPort_Config_ShareMode_STR_TAB [MAX_SerialPort_Config_ShareMode][17]
= {"Exclusive Access", "Read Only", "Write only", "Both Read Write"};

/** Parity */
typedef enum SerialPort_Config_Parity
{
	SerialPort_Config_Parity_None = 0,
	SerialPort_Config_Parity_Pair,
	SerialPort_Config_Parity_UnPair,
	SerialPort_Config_Parity_Mark,
	SerialPort_Config_Parity_Space,
	MAX_SerialPort_Config_Parity

}SerialPort_Config_Parity_t;
static const TC08_t SerialPort_Config_Parity_STR_TAB [MAX_SerialPort_Config_Parity][7]
= {"None", "Pair", "UnPair", "Mark", "Space"};

/** Serial Port Opening action */
typedef enum SerialPort_Config_Action
{
	SerialPort_Config_Action_CreateNew = 0,
	SerialPort_Config_Action_CreateAlways,
	SerialPort_Config_Action_OpenExisting,
	SerialPort_Config_Action_OpenAlways,
	SerialPort_Config_Action_TruncateExisting,
	MAX_SerialPort_Config_Action

}SerialPort_Config_Action_t;
static const TC08_t SerialPort_Config_Action_STR_TAB [MAX_SerialPort_Config_Action][20]
= {"Create New", "Create Always", "Open Existing", "Open Always", "Truncate Existing"};

/** Baud Rate */
typedef enum SerialPort_Config_Baudrate
{
	SerialPort_Config_Baudrate_110 = 0,
	SerialPort_Config_Baudrate_300,
	SerialPort_Config_Baudrate_600,
	SerialPort_Config_Baudrate_1200,
	SerialPort_Config_Baudrate_2400,
	SerialPort_Config_Baudrate_4800,
	SerialPort_Config_Baudrate_9600,
	SerialPort_Config_Baudrate_14400,
	SerialPort_Config_Baudrate_19200,
	SerialPort_Config_Baudrate_38400,
	SerialPort_Config_Baudrate_57600,
	SerialPort_Config_Baudrate_115200,
	SerialPort_Config_Baudrate_128000,
	SerialPort_Config_Baudrate_256000,
	MAX_SerialPort_Config_Baudrate

}SerialPort_Config_Baudrate_t;
static const TC08_t SerialPort_Config_Baudrate_STR [MAX_SerialPort_Config_Baudrate][7]
= {"110", "300", "600", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200", "128000", "256000"};

/** Flow Control */
/** DTR DSR Flow Control */
typedef enum SerialPort_Config_DTR_DSR_Control
{
	SerialPort_Config_DTR_DSR_Control_Disable = 0,
	SerialPort_Config_DTR_DSR_Control_Enable,
	SerialPort_Config_DTR_DSR_Control_HandShake,
	MAX_SerialPort_Config_DTR_DSR_Control

}SerialPort_Config_DTR_DSR_Control_t;
static const TC08_t SerialPort_Config_DTR_DSR_Control_STR_TAB [MAX_SerialPort_Config_DTR_DSR_Control][10]
= {"Disable", "Enable", "HandShake"};

/** RTS CTS Flow Control */
typedef enum SerialPort_Config_RTS_CTS_Control
{
	SerialPort_Config_RTS_CTS_Control_Disable = 0,
	SerialPort_Config_RTS_CTS_Control_Enable,
	SerialPort_Config_RTS_CTS_Control_HandShake,
	SerialPort_Config_RTS_CTS_Control_Toggle,
	MAX_SerialPort_Config_RTS_CTS_Control

}SerialPort_Config_RTS_CTS_Control_t;
static const TC08_t SerialPort_Config_RTS_CTS_Control_STR_TAB [MAX_SerialPort_Config_RTS_CTS_Control][10]
= {"Disable", "Enable", "HandShake", "Toggle"};

typedef enum SerialPort_Config_FlowControl
{
	//http://brette.stephane.free.fr/rs232/asynch_serial.pdf
	SerialPort_Config_FlowControl_None,
	SerialPort_Config_FlowControl_XOn_XOff, /** Sofware Flow Control on 3 Wires */
	SerialPort_Config_FlowControl_RTS_CTS, /** Hardware Flow Control on 5 Wires */
	SerialPort_Config_FlowControl_DTR_DSR, /** Hardware Flow Control on 9 Wires */
	MAX_SerialPort_Config_FlowControl
}SerialPort_Config_FlowControl_t;
static const TC08_t SerialPort_Config_FlowControl_STR_TAB [MAX_SerialPort_Config_FlowControl][9]
= {"None", "XOn XOff", "RTS CTS", "DTR DSR"};

/** Data Bytes */
typedef enum SerialPort_Config_DataBits
{
	SerialPort_Config_DataBits_4 = 0,
	SerialPort_Config_DataBits_5,
	SerialPort_Config_DataBits_6,
	SerialPort_Config_DataBits_7,
	SerialPort_Config_DataBits_8,
	SerialPort_Config_DataBits,
	MAX_SerialPort_Config_DataBits
}SerialPort_Config_DataBits_t;

static const TC08_t SerialPort_Config_DataBits_STR_TAB [MAX_SerialPort_Config_DataBits][2]
= {"4", "5", "6", "7", "8"};

/** Bits Stop */
typedef enum SerialPort_Config_StopBits
{
	SerialPort_Config_StopBits_1 = 0,
	SerialPort_Config_StopBits_1AndAHalf,
	SerialPort_Config_StopBits_2,
	MAX_SerialPort_Config_StopBits
}SerialPort_Config_StopBits_t;
static const TC08_t SerialPort_Config_StopBits_STR_TAB [MAX_SerialPort_Config_StopBits][4]
= {"1", "1.5", "2"};

#endif