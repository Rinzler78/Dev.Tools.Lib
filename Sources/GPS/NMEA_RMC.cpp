#include "NMEA_RMC.h"
#include "NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include <stdlib.h>
CNMEA_RMC::CNMEA_RMC(TS08_t RMCFrame)
: CNMEA_FRAME(RMCFrame)
, m_SpeedFromGround_InKnots(0)
, m_HeadTInDegrees(0)
, m_MagneticVariation(0)
{
	MACRO_MEMSET(&m_FixeTime, sizeof(m_FixeTime));
	MACRO_MEMSET(&m_Position, sizeof(m_Position));

	CCommonTools::Split(RMCFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
}
CNMEA_RMC::~CNMEA_RMC()
{
}
bool CNMEA_RMC::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		Result = true;
		switch(FieldNumber)
		{
			case 0:
				{
					/** Fixe Time */
					TC08_t Temp [4] = {0};
					/** Fixe Hour */
					memcpy(Temp, pField, 2);
					m_FixeTime.tm_hour = atoi(Temp);
					/** Fixe Minutes */
					memcpy(Temp, pField + 2, 2);
					m_FixeTime.tm_min = atoi(Temp);
					/** Fixe Secondes */
					memcpy(Temp, pField + 4, 2);
					m_FixeTime.tm_sec = atoi(Temp);
				}
				break;
			case 1:
				/** Browser Warning */
				if(!memcmp(pField, &NMEA_RMC_BrowserAlert_STR[NMEA_RMC_BrowserAlert_OK],1))
					m_BrowserWarning = NMEA_RMC_BrowserAlert_OK;
				else if(!memcmp(pField, &NMEA_RMC_BrowserAlert_STR[NMEA_RMC_BrowserAlert_Warning],1))
					m_BrowserWarning = NMEA_RMC_BrowserAlert_Warning;
				break;
			case 2:
				/** Latitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Latitude.Value = SexaDecimalToInt(GetLat(pField));
				}
				break;
			case 3:
				/** Latitude Direction */
				if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_North],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_North;
				else if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_South],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_South;
				else
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_Init;
				break;
			case 4:
				/** Longitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Longitude.Value = SexaDecimalToInt(GetLong(pField));
				}
				break;
			case 5:
				/** Longitude Direction */
				if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_East],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_East;
				else if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_West],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_West;
				else
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_Init;
				break;
			case 6:
				/** Speed from ground in knots */
				m_SpeedFromGround_InKnots = atol(pField);
				break;
			case 7:
				/** Actual heading in degrees */
				m_HeadTInDegrees = atol(pField);
				break;
			case 8:
				/** Fixe Date */
				{
					/** Fixe Time */
					TC08_t Temp [4] = {0};
					/** Fixe Hour */
					memcpy(Temp, pField, 2);
					m_FixeTime.tm_mday = atoi(Temp);
					/** Fixe Minutes */
					memcpy(Temp, pField + 2, 2);
					m_FixeTime.tm_mon = atoi(Temp);
					/** Fixe Secondes */
					memcpy(Temp, pField + 4, 2);
					m_FixeTime.tm_year = 2000 + atoi(Temp);
				}
				break;
			
		}
	}

	return Result;
}