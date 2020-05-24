#include "GPS\NMEA_VTG.h"
#include "GPS\NMEA_FRAME.h"
#include "LinkedList\LinkedList.h"
#include "CommonTools\CommonTools.h"
#include "string.h"
#include <stdlib.h>
CNMEA_VTG::CNMEA_VTG(TS08_t VTGFrame)
: CNMEA_FRAME(VTGFrame)
, m_HeadTInDegrees(0)
, m_HeadTInMagneticDegrees(0)
, m_SpeedFromGround_InKnots(0)
, m_SpeedFromGround_InKmH(0)
{
	CCommonTools::Split(VTGFrame + 7,NMEA_LINE_FIELD_SEPARATOR, this, FieldFound);
}
CNMEA_VTG::~CNMEA_VTG()
{
}
bool CNMEA_VTG::GetField(TS08_t pField, TU32_t FieldNumber)
{
	bool Result = false;
	
	if(pField)
	{
		Result = true;
		switch(FieldNumber)
		{
			case 0:
				/** Actual heading in degrees */
				m_HeadTInDegrees = atol(pField);
				break;
			case 1:
				/** Must be T -> Do not treat */
				break;
			case 2:
				/** true heading in degrees magnetic */
				m_HeadTInMagneticDegrees = atol(pField);
				break;
			case 3:
				/** Must be M -> Do not Treat */
				break;
			case 4:
				/** Speed from ground in knots */
				m_SpeedFromGround_InKnots = atol(pField);
				break;
			case 5:
				/** Must Be N -> Do not treat */
				break;
			case 6:
				/** Speed from ground in km/h */
				m_SpeedFromGround_InKmH = atol(pField);
				break;
			case 7:
				/** Must Be K -> Do not treat */
				break;
		}
	}

	return Result;
}