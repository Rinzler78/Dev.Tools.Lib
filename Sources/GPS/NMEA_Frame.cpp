#include "GPS.h"
#include "GPS\NMEA_Frame.h"
#include "GPS\NMEA_GGA.h"
#include "GPS\NMEA_GLL.h"
#include "GPS\NMEA_GSA.h"
#include "GPS\NMEA_GSV.h"
#include "GPS\NMEA_VTG.h"
#include "GPS\NMEA_RMC.h"
#include <string.h>
#include "Log\Log.h"

CNMEA_FRAME::CNMEA_FRAME(TS08_t NMEAFrame)
: m_ReceptorType(GPS_ReceptorID_INIT)
, m_FrameID(GPS_FrameID_INIT)
{
	if(NMEAFrame && (NMEAFrame[0] == NMEA_BEGIN_LINE))
	{
		if(!memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_GP],2))
		{
			m_ReceptorType = GPS_ReceptorID_GP;
		}
		else if (!memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_LC],2))
		{
			m_ReceptorType = GPS_ReceptorID_LC;
		}
		else if(!memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_OM],2))
		{
			m_ReceptorType = GPS_ReceptorID_OM;
		}
		else if(!memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_II],2))
		{
			m_ReceptorType = GPS_ReceptorID_II;
		}

		if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GGA],3))
		{
			m_FrameID = GPS_FrameID_GGA;
		}
		else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GLL],3))
		{
			m_FrameID = GPS_FrameID_GLL;
		}
		else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GSA],3))
		{
			m_FrameID = GPS_FrameID_GSA;
		}
		else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GSV],3))
		{
			m_FrameID = GPS_FrameID_GSV;
		}
		else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_VTG],3))
		{
			m_FrameID = GPS_FrameID_VTG;
		}
		else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_RMC],3))
		{
			m_FrameID = GPS_FrameID_RMC;
		}
	}
}
CNMEA_FRAME::~CNMEA_FRAME()
{
}
CNMEA_FRAME * CNMEA_FRAME::GetFrame(TS08_t NMEAFrame)
{
	CNMEA_FRAME * Result = NULL;
	
	if(NMEAFrame && (NMEAFrame[0] == NMEA_BEGIN_LINE))
	{
		if(!memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_GP],2)
			|| !memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_LC],2)
			|| !memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_OM],2)
			|| !memcmp(&NMEAFrame[1], GPS_ReceptorID_Tab[GPS_ReceptorID_II],2))
		{
			/** Check NMEA Frame ID */
			if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GGA],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_GGA(NMEAFrame);
			}
			else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GLL],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_GLL(NMEAFrame);
			}
			else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GSA],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_GSA(NMEAFrame);
			}
			else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_GSV],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_GSV(NMEAFrame);
			}
			else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_VTG],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_VTG(NMEAFrame);
			}
			else if(!memcmp(&NMEAFrame[3], GPS_FrameID_STR[GPS_FrameID_RMC],3))
			{
				Result = (CNMEA_FRAME *) new CNMEA_RMC(NMEAFrame);
			}
		}
#if defined (_DEBUG) && ! defined (UNDER_CE)
		else
			printf("Unknown Frame received %s\n", NMEAFrame);
#endif
	}
	#if defined (_DEBUG)
	else
		printf("Bad Frame received %s\n", NMEAFrame);
#endif
	return Result;
}
void FrameFound(void * pToObj, TS08_t Frame, TU32_t FieldNumber)
{
	CGPS *pGPS = NULL;

	if(pToObj)
		pGPS = (CGPS *)pToObj;

	if(Frame)
	{
		if(pGPS)
		{
			if(pGPS->Log())
			{
				pGPS->Log()->Log(NOCOMMENTS, "Frame received %s", Frame);
			}

#if !defined(UNDER_CE) && defined(_DEBUG)
			printf("%s\n", Frame);
#endif
		}

		CNMEA_FRAME * pframe = NULL;

		if(Frame && (Frame[0] == NMEA_BEGIN_LINE))
		{
			pframe = CNMEA_FRAME::GetFrame(Frame);
		}
		else
		{
			if(pGPS)
			{
				if(pGPS->Log())
				{
					pGPS->Log()->Log(NOCOMMENTS, "Bad Frame received %s", Frame);
				}
#if !defined(UNDER_CE) && defined(_DEBUG)
			printf("%s\n", Frame);
#endif
			}
		}

		if(pframe)
		{
			pGPS->GetInfos(pframe);
			MACRO_DELET(pframe);
		}
	}
}
void FieldFound(void * pToObj, TS08_t Field, TU32_t FieldNumber)
{
	CNMEA_FRAME *pframe = NULL;

	if(pToObj)
		pframe = (CNMEA_FRAME *)pToObj;

	if(Field)
	{
		pframe->GetField(Field, FieldNumber);
	}
}