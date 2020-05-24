#ifndef CNMEA_RMC_H
#define CNMEA_RMC_H
#include "Common\MyTypes.h"
#include "GPS\NMEA_FRAME.h"
#include "GPS_Common.h"

/**********************************************************************************************//**
 * \enum	NMEA_RMC_BrowserAlert
 *
 * \brief	Values that represent NMEA_RMC_BrowserAlert.
 **************************************************************************************************/

typedef enum NMEA_RMC_BrowserAlert
{
	NMEA_RMC_BrowserAlert_OK,
	NMEA_RMC_BrowserAlert_Warning,
	NMEA_RMC_BrowserAlert_INIT,
	MAX_NMEA_RMC_BrowserAlert
}NMEA_RMC_BrowserAlert_t;

const TC08_t NMEA_RMC_BrowserAlert_STR [MAX_NMEA_RMC_BrowserAlert][2] = {"A", "V"};   ///< The 2]

class CNMEA_RMC : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_RMC::CNMEA_RMC(TS08_t RMCFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_RMC class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	RMCFrame	The rmc frame.
	 **************************************************************************************************/

	CNMEA_RMC(TS08_t RMCFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_RMC::~CNMEA_RMC();
	 *
	 * \brief	Finalizes an instance of the CNMEA_RMC class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_RMC();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_RMC::GetField(TS08_t pFiled, TU32_t FieldNumber);
	 *
	 * \brief	Gets a field.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pFiled	   	The filed.
	 * \param	FieldNumber	The field number.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool GetField(TS08_t pFiled, TU32_t FieldNumber);

	/**********************************************************************************************//**
	 * \fn	inline struct tm CNMEA_RMC::FixeTime()
	 *
	 * \brief	Gets the fixe time.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline struct tm FixeTime(){return m_FixeTime;}

	/**********************************************************************************************//**
	 * \fn	inline NMEA_RMC_BrowserAlert_t CNMEA_RMC::BrowserAlert()
	 *
	 * \brief	Gets the browser alert.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline NMEA_RMC_BrowserAlert_t BrowserAlert(){return m_BrowserWarning;}

	/**********************************************************************************************//**
	 * \fn	inline GPS_Position_DD_t CNMEA_RMC::Position()
	 *
	 * \brief	Gets the position.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline GPS_Position_DD_t Position(){return m_Position;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_RMC::SpeedFromGround_InKnots()
	 *
	 * \brief	Gets the speed from ground in knots.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t SpeedFromGround_InKnots(){return m_SpeedFromGround_InKnots;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_RMC::HeadTInDegrees()
	 *
	 * \brief	Gets the heading in degrees.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t HeadTInDegrees(){return m_HeadTInDegrees;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_RMC::MagneticVariation()
	 *
	 * \brief	Gets the magnetic variation.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t MagneticVariation(){return m_MagneticVariation;}
private:
	struct tm m_FixeTime;
	NMEA_RMC_BrowserAlert_t m_BrowserWarning;   ///< The browser warning
	GPS_Position_DD_t m_Position;   ///< The position
	TD32_t m_SpeedFromGround_InKnots; ///< The speed from ground in knots
	TD32_t m_HeadTInDegrees;	///< The heading in degrees
	TD32_t m_MagneticVariation;   ///< The magnetic variation
};
#endif