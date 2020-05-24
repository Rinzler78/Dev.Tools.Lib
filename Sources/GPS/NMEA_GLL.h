#ifndef CNMEA_GLL_H
#define CNMEA_GLL_H
#include "Common\MyTypes.h"
#include "GPS\NMEA_FRAME.h"
#include "GPS_Common.h"

class CNMEA_GLL : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_GLL::CNMEA_GLL(TS08_t GLLFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_GLL class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GLLFrame	The gll frame.
	 **************************************************************************************************/

	CNMEA_GLL(TS08_t GLLFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_GLL::~CNMEA_GLL();
	 *
	 * \brief	Finalizes an instance of the CNMEA_GLL class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_GLL();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_GLL::GetField(TS08_t pFiled, TU32_t FieldNumber);
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
	 * \fn	inline struct tm CNMEA_GLL::FixeTime()
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
	 * \fn	inline GPS_Position_DD_t CNMEA_GLL::Position()
	 *
	 * \brief	Gets the position.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline GPS_Position_DD_t Position(){return m_Position;}
private:

	/**********************************************************************************************//**
	 * \struct	#endif
	 *
	 * \brief	Endif.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	struct tm m_FixeTime;
	GPS_Position_DD_t m_Position;   ///< The position
};
#endif