#ifndef CNMEA_POSITION_H
#define CNMEA_POSITION_H
#include "Common\MyTypes.h"
#include "GPS_Common.h"

class CNMEA_Position
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_Position::CNMEA_Position(GPS_Position_DD_t GPS_Position_DD);
	 *
	 * \brief	Initializes a new instance of the CNMEA_Position class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GPS_Position_DD	The gps position dd.
	 **************************************************************************************************/

	CNMEA_Position(GPS_Position_DD_t GPS_Position_DD);

	/**********************************************************************************************//**
	 * \fn	CNMEA_Position::CNMEA_Position(GPS_Position_DDMS_t GPS_Position_DMS);
	 *
	 * \brief	Initializes a new instance of the CNMEA_Position class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GPS_Position_DMS	The gps position dms.
	 **************************************************************************************************/

	CNMEA_Position(GPS_Position_DDMS_t GPS_Position_DMS);

	/**********************************************************************************************//**
	 * \fn	CNMEA_Position::~CNMEA_Position();
	 *
	 * \brief	Finalizes an instance of the CNMEA_Position class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_Position();

/**********************************************************************************************//**
 * \property	private : GPS_Position_DD_t m_GPS_Position_DD
 *
 * \brief	Gets the gps position dd.
 *
 * \return	The m gps position dd.
 **************************************************************************************************/

private :
	GPS_Position_DD_t m_GPS_Position_DD;
	GPS_Position_DMS_t m_GPS_Position_DMS;  ///< The gps position dms
};
#endif