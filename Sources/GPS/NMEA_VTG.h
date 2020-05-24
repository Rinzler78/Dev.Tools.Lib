#ifndef CNMEA_VTG_H
#define CNMEA_VTG_H
#include "Common\MyTypes.h"
#include "GPS\NMEA_FRAME.h"

class CNMEA_VTG : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_VTG::CNMEA_VTG(TS08_t VTGFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_VTG class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	VTGFrame	The vtg frame.
	 **************************************************************************************************/

	CNMEA_VTG(TS08_t VTGFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_VTG::~CNMEA_VTG();
	 *
	 * \brief	Finalizes an instance of the CNMEA_VTG class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_VTG();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_VTG::GetField(TS08_t pFiled, TU32_t FieldNumber);
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
	 * \fn	inline TD32_t CNMEA_VTG::HeadTInDegrees()
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
	 * \fn	inline TD32_t CNMEA_VTG::HeadTInMagneticDegrees()
	 *
	 * \brief	Gets the heading in magnetic degrees.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t HeadTInMagneticDegrees(){return m_HeadTInMagneticDegrees;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_VTG::SpeedFromGround_InKnots()
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
	 * \fn	inline TD32_t CNMEA_VTG::SpeedFromGround_InKmH()
	 *
	 * \brief	Gets the speed from ground in kilometres h.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t SpeedFromGround_InKmH(){return m_SpeedFromGround_InKmH;}
private:
	TD32_t m_HeadTInDegrees;	///< The heading in degrees
	TD32_t m_HeadTInMagneticDegrees;	///< The heading in magnetic in degrees
	TD32_t m_SpeedFromGround_InKnots; ///< The speed from ground in knots
	TD32_t m_SpeedFromGround_InKmH;   ///< The speed from ground in kilometres h
};
#endif