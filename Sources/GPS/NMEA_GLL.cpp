#include "GPS\NMEA_GLL.h"
#include "GPS\NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include <stdlib.h>
CNMEA_GLL::CNMEA_GLL(TS08_t GLLFrame)
: CNMEA_FRAME(GLLFrame)
{
	MACRO_MEMSET(&m_Position, sizeof(m_Position));
	CCommonTools::Split(GLLFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
}
CNMEA_GLL::~CNMEA_GLL()
{
}
bool CNMEA_GLL::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		Result = true;
		switch(FieldNumber)
		{
			case 0:
				/** Latitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Latitude.Value = SexaDecimalToInt(GetLat(pField));
				}
				break;
			case 1:
				/** Latitude Direction */
				if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_North],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_North;
				else if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_South],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_South;
				else
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_Init;
				break;
			case 2:
				/** Longitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Longitude.Value = SexaDecimalToInt(GetLong(pField));
				}
				break;
			case 3:
				/** Longitude Direction */
				if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_East],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_East;
				else if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_West],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_West;
				else
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_Init;
				break;
			case 4:
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
			case 5:
				/** Fix Qualification */
				break;
		}
	}
	return Result;
}