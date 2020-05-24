#include "GPS\NMEA_GSA.h"
#include "GPS\NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include "stdlib.h"
CNMEA_GSA::CNMEA_GSA(TS08_t GSAFrame)
: CNMEA_FRAME(GSAFrame)
, m_2D3DSelection(NMEA_GSA_FIX_2D3D_Selection_INIT)
, m_3DFixe(NMEA_GSA_3DFixe_Init)
, m_PDOP(0)
, m_HDOP(0)
, m_VDOP(0)
{
	CCommonTools::Split(GSAFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
}
CNMEA_GSA::~CNMEA_GSA()
{
}
bool CNMEA_GSA::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		Result = true;
		switch(FieldNumber)
		{
			case 0:
				/** Fix 2D 3D Selection */
				if(!memcmp(pField, NMEA_GSA_FIX_2D3D_Selection_STR_TAB[NMEA_GSA_FIX_2D3D_Selection_Auto],1))
					m_2D3DSelection = NMEA_GSA_FIX_2D3D_Selection_Auto;
				else if(!memcmp(pField, NMEA_GSA_FIX_2D3D_Selection_STR_TAB[NMEA_GSA_FIX_2D3D_Selection_Manu],1))
					m_2D3DSelection = NMEA_GSA_FIX_2D3D_Selection_Manu;
				else
					m_2D3DSelection = NMEA_GSA_FIX_2D3D_Selection_INIT;

				break;
			case 1:
				/** Fixe 3D */
				m_3DFixe = (NMEA_GSA_3DFixe_t)atoi(pField);
				break;
			case 2:
				{
					/** Sat IDs */
				}
				break;
			case 3:
				/** PDOP */
				m_PDOP = atof(pField);
				break;
			case 4:
				/** HDOP */
				m_HDOP = atof(pField);
				break;
			case 5:
				/** VDOP */
				m_VDOP = atof(pField);
				break;
		}
	}

	return Result;
}