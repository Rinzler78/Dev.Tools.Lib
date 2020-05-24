#include "GPS\NMEA_GGA.h"
#include "GPS\NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include <stdlib.h>
#include "time.h"
CNMEA_GGA::CNMEA_GGA(TS08_t GGAFrame)
: CNMEA_FRAME(GGAFrame)
, m_FixeQualification(NMEA_GGA_FixeQualification_Invalid)
, m_NumberOfSat(0)
, m_DOP(0)
, m_Altitude(0)
, m_Height(0)
{
	MACRO_MEMSET(&m_FixeTime, sizeof(m_FixeTime));
	MACRO_MEMSET(&m_Position, sizeof(m_Position));

	if(m_FrameID == GPS_FrameID_GGA)
	{
		CCommonTools::Split(GGAFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
	}
}
CNMEA_GGA::~CNMEA_GGA()
{
}
bool CNMEA_GGA::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		Result = true;
		switch(FieldNumber)
		{
			case 0:
				{
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
				/** Latitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Latitude.Value = SexaDecimalToInt(GetLat(pField));
				}
				break;
			case 2:
				/** Latitude Direction */
				if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_North],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_North;
				else if(!memcmp(pField, GPS_Position_Latitude_Direction_STR_TAB[GPS_Position_Latitude_Direction_South],1))
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_South;
				else
					m_Position.Latitude.Direction = GPS_Position_Latitude_Direction_Init;

				break;
			case 3:
				/** Longitude */
				if(pField[0] >= 0x30 && pField[0] <= 0x39)
				{
					m_Position.Longitude.Value = SexaDecimalToInt(GetLong(pField));
				}
				break;
			case 4:
				/** Longitude Direction */
				if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_East],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_East;
				else if(!memcmp(pField, GPS_Position_Longitude_Direction_STR_TAB[GPS_Position_Longitude_Direction_West],1))
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_West;
				else
					m_Position.Longitude.Direction = GPS_Position_Longitude_Direction_Init;
				break;
			case 5:
				/** Fix Qualification */
				m_FixeQualification = (NMEA_GGA_FixeQualification_t)atoi(pField);
				break;
			case 6:
				/** Number of Sat */
				m_NumberOfSat = atoi(pField); 
				break;
			case 7:
				/** DOP */
				m_DOP = atof(pField);
				break;
			case 8:
				/** Altitude */
				m_Altitude = atof(pField);
				break;
			case 9:
				/** Altitude Unit -> M */
				break;
			case 10:
				/** Height Correction */
				m_Height = atof(pField);
				break;
			case 11:
				/** Hight Correction Unit -> M*/
				break;
			case 12:/** Empty */
			case 13:/** Empty */
				break;
			case 14:
				/** Frame CheckSum */
				break;
		}
	}
	return Result;
}