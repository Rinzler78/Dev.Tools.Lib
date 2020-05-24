#ifndef CNMEA_GSV_H
#define CNMEA_GSV_H
#include "Common\MyTypes.h"
#include "GPS\NMEA_FRAME.h"

class CNMEA_GSV : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_GSV::CNMEA_GSV(TS08_t GSVFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_GSV class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GSVFrame	The gsv frame.
	 **************************************************************************************************/

	CNMEA_GSV(TS08_t GSVFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_GSV::~CNMEA_GSV();
	 *
	 * \brief	Finalizes an instance of the CNMEA_GSV class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_GSV();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_GSV::GetField(TS08_t pFiled, TU32_t FieldNumber);
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
	 * \fn	inline TU08_t CNMEA_GSV::NumberOfGSVFrames()
	 *
	 * \brief	Gets the number of gsv frames.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The total number of gsv frames.
	 **************************************************************************************************/

	inline TU08_t NumberOfGSVFrames(){return m_NumberOfGSVFrames;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GSV::FrameIndex()
	 *
	 * \brief	Gets the frame index.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TU08_t FrameIndex(){return m_FrameIndex;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GSV::NumberOfSat()
	 *
	 * \brief	Gets the number of sat.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The total number of sat.
	 **************************************************************************************************/

	inline TU08_t NumberOfSat(){return m_NumberOfSat;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GSV::FirstSatID()
	 *
	 * \brief	Gets the first sat identifier.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TU08_t FirstSatID(){return m_FirstSatID;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GSV::FirstSatElevationInDegrees()
	 *
	 * \brief	Gets the first sat elevation in degrees.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TU08_t FirstSatElevationInDegrees(){return m_FirstSatElevationInDegrees;}

	/**********************************************************************************************//**
	 * \fn	inline TU16_t CNMEA_GSV::FirstSatAzimuthInDegrees()
	 *
	 * \brief	Gets the first sat azimuth in degrees.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TU16_t FirstSatAzimuthInDegrees(){return m_FirstSatAzimuthInDegrees;}

	/**********************************************************************************************//**
	 * \fn	inline TU08_t CNMEA_GSV::SignalStrength()
	 *
	 * \brief	Raises the strength event.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TU08_t SignalStrength(){return m_SignalStrength;}
private:
	TU08_t m_NumberOfGSVFrames;   ///< Number of gsv frames
	TU08_t m_FrameIndex;  ///< Zero-based index of the m frame
	TU08_t m_NumberOfSat; ///< Number of sats
	TU08_t m_FirstSatID;  ///< Identifier for the first sat
	TU08_t m_FirstSatElevationInDegrees;  ///< The first sat elevation in degrees
	TU16_t m_FirstSatAzimuthInDegrees;	///< The first sat azimuth in degrees
	TU08_t m_SignalStrength;  ///< The signal strength
};
#endif