#ifndef CNMEA_FRAME_H
#define CNMEA_FRAME_H
#include "Common\MyTypes.h"

/** Frame Marker */
#define NMEA_BEGIN_LINE '$'
#define NMEA_LINE_FIELD_SEPARATOR ","
#define NMEA_EOF "\r\n"

/** Receptor ID */

/**********************************************************************************************//**
 * \enum	GPS_ReceptorID
 *
 * \brief	Values that represent GPS_ReceptorID.
 **************************************************************************************************/

typedef enum GPS_ReceptorID
{
	GPS_ReceptorID_GP,
	GPS_ReceptorID_LC,
	GPS_ReceptorID_OM,
	GPS_ReceptorID_II,
	GPS_ReceptorID_INIT,
	MAX_GPS_ReceptorID
}GPS_ReceptorID_t;
const TC08_t GPS_ReceptorID_Tab [MAX_GPS_ReceptorID][3] = {"GP", "LC", "OM", "II"};

/** Frame ID */

/**********************************************************************************************//**
 * \enum	GPS_FrameID
 *
 * \brief	Values that represent GPS_FrameID.
 **************************************************************************************************/

typedef enum GPS_FrameID
{
	GPS_FrameID_GGA,
	GPS_FrameID_GLL,
	GPS_FrameID_GSA,
	GPS_FrameID_GSV,
	GPS_FrameID_VTG,
	GPS_FrameID_RMC,
	GPS_FrameID_INIT,
	MAX_GPS_FrameID

}GPS_FrameID_t;

const TC08_t GPS_FrameID_STR [MAX_GPS_FrameID][4] = {"GGA", "GLL", "GSA", "GSV", "VTG", "RMC"};

/**********************************************************************************************//**
 * \fn	void CNMEA_FRAME::FrameFound(void * pToObj, TS08_t Frame, TU32_t FieldNumber);
 *
 * \brief	Frame found.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param [in,out]	pToObj	If non-null, to object.
 * \param	Frame		  	The frame.
 * \param	FieldNumber   	The field number.
 **************************************************************************************************/

void FrameFound(void * pToObj, TS08_t Frame, TU32_t FieldNumber);

/**********************************************************************************************//**
 * \fn	void FieldFound(void * pToObj, TS08_t Field, TU32_t FieldNumber);
 *
 * \brief	Field found.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param [in,out]	pToObj	If non-null, to object.
 * \param	Field		  	The field.
 * \param	FieldNumber   	The field number.
 **************************************************************************************************/

void FieldFound(void * pToObj, TS08_t Field, TU32_t FieldNumber);

class CNMEA_FRAME
{
public:

	/**********************************************************************************************//**
	 * \fn	CNMEA_FRAME::CNMEA_FRAME(TS08_t NMEAFrame);
	 *
	 * \brief	Initializes a new instance of the CNMEA_FRAME class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	NMEAFrame	The nmea frame.
	 **************************************************************************************************/

	CNMEA_FRAME(TS08_t NMEAFrame);

	/**********************************************************************************************//**
	 * \fn	CNMEA_FRAME::~CNMEA_FRAME();
	 *
	 * \brief	Finalizes an instance of the CNMEA_FRAME class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CNMEA_FRAME();

	/**********************************************************************************************//**
	 * \fn	static CNMEA_FRAME * CNMEA_FRAME::GetFrame(TS08_t NMEAFrame);
	 *
	 * \brief	Gets a frame.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	NMEAFrame	The nmea frame.
	 *
	 * \return	null if it fails, else the frame.
	 **************************************************************************************************/

	static CNMEA_FRAME * GetFrame(TS08_t NMEAFrame);

	/**********************************************************************************************//**
	 * \fn	inline GPS_ReceptorID_t CNMEA_FRAME::ReceptorID()
	 *
	 * \brief	Gets the receptor identifier.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline GPS_ReceptorID_t ReceptorID(){return m_ReceptorType;}

	/**********************************************************************************************//**
	 * \fn	inline GPS_FrameID_t CNMEA_FRAME::FrameID()
	 *
	 * \brief	Gets the frame identifier.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline GPS_FrameID_t FrameID(){return m_FrameID;}

	/**********************************************************************************************//**
	 * \fn	virtual bool CNMEA_FRAME::GetField(TS08_t pFiled, TU32_t FieldNumber) = 0;
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

	virtual bool GetField(TS08_t pFiled, TU32_t FieldNumber) = 0;
protected:
	GPS_ReceptorID_t m_ReceptorType;	///< Type of the receptor
	GPS_FrameID_t m_FrameID;	///< Identifier for the frame
};
#endif