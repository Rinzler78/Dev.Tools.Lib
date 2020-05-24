#ifndef COMMON_BLUETOOTH_H
#define COMMON_BLUETOOTH_H

#if defined UNDER_CE
#include <altcecrt.h>
#include <winsock2.h>
#include <bt_sdp.h>
#include <bt_api.h> 
#include <bthapi.h>
#include <initguid.h>
#else
#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")
using namespace std;
#ifndef BT_ADDR
#define BT_ADDR BTH_ADDR
#endif
#endif

#endif