#ifndef CNMEA_GSA_H
#define CNMEA_GSA_H
#include "Common\MyTypes.h"
#include "GPS\NMEA_FRAME.h"
#include "GPS_Common.h"

/**********************************************************************************************//**
 * \enum	NMEA_GSA_FIX_2D3D_Selection
 *
 * \brief	Values that represent NMEA_GSA_FIX_2D3D_Selection.
 **************************************************************************************************/

typedef enum NMEA_GSA_FIX_2D3D_Selection
{
	NMEA_GSA_FIX_2D3D_Selection_Auto,
	NMEA_GSA_FIX_2D3D_Selection_Manu,
	NMEA_GSA_FIX_2D3D_Selection_INIT,
	MAX_NMEA_GSA_FIX_2D3D_Selection
}NMEA_GSA_FIX_2D3D_Selection_t;
const TC08_t NMEA_GSA_FIX_2D3D_Selection_STR_TAB [MAX_NMEA_GSA_FIX_2D3D_Selection][2] = {"A", "M"};

/**********************************************************************************************//**
 * \enum	NMEA_GSA_3DFixe
 *
 * \brief	Values that represent NMEA_GSA_3DFixe.
 **************************************************************************************************/

typedef enum NMEA_GSA_3DFixe
{
	NMEA_GSA_3DFixe_Init,
	NMEA_GSA_3DFixe_NoFix,
	NMEA_GSA_3DFixe_Fixe2D,
	NMEA_GSA_3DFixe_Fixe3D,
	MAX_NMEA_GSA_3DFixe
}NMEA_GSA_3DFixe_t;
const TC08_t NMEA_GSA_FIX_2D3D_Selection_STR [MAX_NMEA_GSA_FIX_2D3D_Selection][2] = {"A", "M"};

class CNMEA_GSA : public CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_GSA::CNMEA_GSA(TS08_t GSAFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_GSA class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	GSAFrame	The gsa frame.
	 **************************************************************************************************/

	CNMEA_GSA(TS08_t GSAFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_GSA::~CNMEA_GSA();
	 *
	 * \brief	Finalizes an instance of the CNMEA_GSA class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_GSA();

	/**********************************************************************************************//**
	 * \fn	bool CNMEA_GSA::GetField(TS08_t pFiled, TU32_t FieldNumber);
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
	 * \fn	inline NMEA_GSA_FIX_2D3D_Selection_t CNMEA_GSA::Fix2D3DSelection()
	 *
	 * \brief	Gets the fix 2 d 3 d selection.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline NMEA_GSA_FIX_2D3D_Selection_t Fix2D3DSelection(){return m_2D3DSelection;}

	/**********************************************************************************************//**
	 * \fn	inline NMEA_GSA_3DFixe_t CNMEA_GSA::FixeInfo()
	 *
	 * \brief	Gets the fixe information.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline NMEA_GSA_3DFixe_t FixeInfo(){return m_3DFixe;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GSA::PDOP()
	 *
	 * \brief	Gets the pdop.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t PDOP(){return m_PDOP;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GSA::HDOP()
	 *
	 * \brief	Gets the hdop.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t HDOP(){return m_HDOP;}

	/**********************************************************************************************//**
	 * \fn	inline TD32_t CNMEA_GSA::VDOP()
	 *
	 * \brief	Gets the vdop.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TD32_t VDOP(){return m_VDOP;}
private:
	NMEA_GSA_FIX_2D3D_Selection_t m_2D3DSelection;  ///< The d 3 d selection
	NMEA_GSA_3DFixe_t m_3DFixe; ///< The d fixe
	TD32_t m_PDOP;	///< The pdop
	TD32_t m_HDOP;	///< The hdop
	TD32_t m_VDOP;	///< The vdop
};
#endif