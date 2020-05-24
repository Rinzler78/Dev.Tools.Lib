#ifndef CSEXADECIMALVALUE_H
#define CSEXADECIMALVALUE_H
#include "Common\MyTypes.h"

class CSexaDecimalvalue
{
public:

	/**********************************************************************************************//**
	 * \fn	CSexaDecimalvalue::CSexaDecimalvalue(TU08_t Degrees, TU08_t Minutes,
	 * 		TU08_t Secondes);
	 *
	 * \brief	Initializes a new instance of the CSexaDecimalvalue class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	Degrees 	The degrees.
	 * \param	Minutes 	The minutes.
	 * \param	Secondes	The secondes.
	 **************************************************************************************************/

	CSexaDecimalvalue(TU08_t Degrees, TU08_t Minutes, TU08_t Secondes);

	/**********************************************************************************************//**
	 * \fn	CSexaDecimalvalue::CSexaDecimalvalue(TS08_t NMEA_Sexadecimal_Field_String);
	 *
	 * \brief	Initializes a new instance of the CSexaDecimalvalue class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	NMEA_Sexadecimal_Field_String	The nmea sexadecimal field string.
	 **************************************************************************************************/

	CSexaDecimalvalue(TS08_t NMEA_Sexadecimal_Field_String);

	/**********************************************************************************************//**
	 * \fn	CSexaDecimalvalue::~CSexaDecimalvalue();
	 *
	 * \brief	Finalizes an instance of the CSexaDecimalvalue class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CSexaDecimalvalue();
private:
	TU08_t m_Degrees; ///< The degrees
	TU08_t m_Minutes; ///< The minutes
	TU08_t m_Secondes;	///< The secondes
};
#endif