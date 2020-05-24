#ifndef CNMEA_GGA_H
#define CNMEA_GGA_H
#include "Common\MyTypes.h"
#include "GPS_Common.h"
#include "GPS\NMEA_FRAME.h"

/**********************************************************************************************//**
 * \enum	NMEA_GGA_FixeQualification
 *
 * \brief	Values that represent NMEA_GGA_FixeQualification.
 **************************************************************************************************/

typedef enum NMEA_GGA_FixeQualification
{
	NMEA_GGA_FixeQualification_Invalid,
	NMEA_GGA_FixeQualification_FixGPS,
	NMEA_GGA_FixeQualification_FixSGPS,
	MAX_NMEA_GGA_FixeQualification
}NMEA_GGA_FixeQualification_t;

class CNMEA_GGA : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_GGA::CNMEA_GGA(TS08_t GGAFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_GGA class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GGAFrame	The gga frame.
	 **************************************************************************************************/

	CNMEA_GGA(TS08_t GGAFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_GGA::~CNMEA_GGA();
	 *
	 * \brief	Finalizes an instance of the CNMEA_GGA class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_GGA();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_GGA::GetField(TS08_t pFiled, TU32_t FieldNumber);
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
	 * \fn	inline struct tm CNMEA_GGA::FixeTime()
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
	 * \fn	inline GPS_Position_DD_t CNMEA_GGA::Position()
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
	 * \fn	inline NMEA_GGA_FixeQualification_t CNMEA_GGA::FixeQualification()
	 *
	 * \brief	Gets the fixe qualification.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline NMEA_GGA_FixeQualification_t FixeQualification(){return m_FixeQualification;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GGA::NumberOfSate()
	 *
	 * \brief	Gets the number of sate.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The total number of sate.
	 **************************************************************************************************/

	inline TU08_t NumberOfSate(){return m_NumberOfSat;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GGA::DOP()
	 *
	 * \brief	Gets the dop.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t DOP(){return m_DOP;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GGA::Altitude()
	 *
	 * \brief	Gets the altitude.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t Altitude(){return m_Altitude;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GGA::Height()
	 *
	 * \brief	Gets the height.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t Height(){return m_Height;}
private:

	/**********************************************************************************************//**
	 * \struct	m_Height
	 *
	 * \brief	Height.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	struct tm m_FixeTime;
	GPS_Position_DD_t m_Position;   ///< The position
	NMEA_GGA_FixeQualification_t m_FixeQualification;   ///< The fixe qualification
	TU08_t m_NumberOfSat; ///< Number of sats
	TD32_t m_DOP; ///< The dop
	TD32_t m_Altitude;	///< The altitude
	TD32_t m_Height;  ///< The height
};
#endif