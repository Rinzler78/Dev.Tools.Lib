#include "GPS\NMEA_GSV.h"
#include "GPS\NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include "stdlib.h"
CNMEA_GSV::CNMEA_GSV(TS08_t GSVFrame)
: CNMEA_FRAME(GSVFrame)
, m_NumberOfGSVFrames(0)
, m_FrameIndex(0)
, m_NumberOfSat(0)
, m_FirstSatID(0)
, m_FirstSatElevationInDegrees(0)
, m_FirstSatAzimuthInDegrees(0)
, m_SignalStrength(0)
{
	CCommonTools::Split(GSVFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
}
CNMEA_GSV::~CNMEA_GSV()
{
}
bool CNMEA_GSV::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		switch(FieldNumber)
		{
			case 0:
				/** Number of GSv frames */
				m_NumberOfGSVFrames = atoi(pField);
				break;
			case 1:
				/** Frame Index */
				m_FrameIndex = atoi(pField);
				break;
			case 2:
				/** Number Of Sat */
				m_NumberOfSat = atoi(pField);
				break;
			case 3:
				/** First Sat ID */
				m_FirstSatID = atoi(pField);
				break;
			case 4:
				/** First Sat Elevation */
				m_FirstSatElevationInDegrees = atoi(pField);
				break;
			case 5:
				/** First Sat Azimuth */
				m_FirstSatAzimuthInDegrees = atoi(pField);
				break;
			case 6:
				/** Signal Strength */
				m_SignalStrength = atoi(pField);
				break;
		}
	}
	return Result;
}