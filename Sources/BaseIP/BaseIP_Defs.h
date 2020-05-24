#ifndef CBASEIP_DEFS_H
#define CBASEIP_DEFS_H
#include "Common\MyTypes.h"

typedef enum BaseIPType
{
	BaseIPType_TCP_Client,
	BaseIPType_UDP_Client,
	BaseIPType_IP_Server,
	MAX_BaseIPType
}BaseIPType_t;

static const TC08_t BaseIPType_STR_TAB [MAX_BaseIPType][20] = {"TCP Client", "UDP Client", "IP Server"};
#endif