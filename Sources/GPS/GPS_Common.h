#ifndef GPS_COMMON_H
#define GPS_COMMON_H
#include "Common\MyTypes.h"
#include "time.h"

/**********************************************************************************************//**
 * \typedef	struct Sexadecimal_Value
 *
 * \brief	Defines an alias representing the sexadecimal value.
 **************************************************************************************************/

typedef struct Sexadecimal_Value
{
	TU08_t Degrees;
	TU08_t Minutes;
	TU08_t Secondes;

}Sexadecimal_Value_t;   ///< .
/**¨Latitude */

/**********************************************************************************************//**
 * \enum	GPS_Position_Latitude_Direction
 *
 * \brief	Values that represent GPS_Position_Latitude_Direction.
 **************************************************************************************************/

typedef enum GPS_Position_Latitude_Direction
{
	GPS_Position_Latitude_Direction_North,
	GPS_Position_Latitude_Direction_South,
	GPS_Position_Latitude_Direction_Init,
	MAX_GPS_Position_Latitude_Direction
};
const TC08_t GPS_Position_Latitude_Direction_STR_TAB[MAX_GPS_Position_Latitude_Direction][2] = {"N", "S"};	///< The gps position latitude direction string tab[ 2]

/**********************************************************************************************//**
 * \typedef	struct GPS_Latitude_DD
 *
 * \brief	Defines an alias representing the gps latitude dd.
 **************************************************************************************************/

typedef struct GPS_Latitude_DD
{
	TI32_t Value;
	GPS_Position_Latitude_Direction Direction;

}GPS_Latitude_DD_t; ///< .

/**********************************************************************************************//**
 * \typedef	struct GPS_Latitude_DMS
 *
 * \brief	Defines an alias representing the gps latitude dms.
 **************************************************************************************************/

typedef struct GPS_Latitude_DMS
{
	Sexadecimal_Value_t Value;
	GPS_Position_Latitude_Direction Direction;

}GPS_Latitude_DMS_t;	///< .
/** Longitude */

/**********************************************************************************************//**
 * \enum	GPS_Position_Longitude_Direction
 *
 * \brief	Values that represent GPS_Position_Longitude_Direction.
 **************************************************************************************************/

typedef enum GPS_Position_Longitude_Direction
{
	GPS_Position_Longitude_Direction_West,
	GPS_Position_Longitude_Direction_East,
	GPS_Position_Longitude_Direction_Init,
	MAX_GPS_Position_Longitude_Direction
};
const TC08_t GPS_Position_Longitude_Direction_STR_TAB[MAX_GPS_Position_Longitude_Direction][2] = {"W", "E"};  ///< The gps position longitude direction string tab[ 2]

/**********************************************************************************************//**
 * \typedef	struct GPS_Longitude_DD
 *
 * \brief	Defines an alias representing the gps longitude dd.
 **************************************************************************************************/

typedef struct GPS_Longitude_DD
{
	TI32_t Value;
	GPS_Position_Longitude_Direction Direction;

}GPS_Longitude_DD_t;	///< .

/**********************************************************************************************//**
 * \typedef	struct GPS_Longitude_DMS
 *
 * \brief	Defines an alias representing the gps longitude dms.
 **************************************************************************************************/

typedef struct GPS_Longitude_DMS
{
	Sexadecimal_Value_t Value;
	GPS_Position_Latitude_Direction Direction;

}GPS_Longitude_DMS_t;   ///< .
/** Position */

/**********************************************************************************************//**
 * \typedef	struct GPS_Position_DD
 *
 * \brief	Defines an alias representing the gps position dd.
 **************************************************************************************************/

typedef struct GPS_Position_DD
{
	GPS_Latitude_DD_t Latitude;
	GPS_Longitude_DD_t Longitude;

}GPS_Position_DD_t; ///< .

/**********************************************************************************************//**
 * \typedef	struct GPS_Position_DMS
 *
 * \brief	Defines an alias representing the gps position dms.
 **************************************************************************************************/

typedef struct GPS_Position_DMS
{
	GPS_Latitude_DMS_t Latitude;
	GPS_Longitude_DMS_t Longitude;

}GPS_Position_DMS_t;	///< .

/**********************************************************************************************//**
 * \typedef	struct GPS_PVT
 *
 * \brief	Defines an alias representing the gps pvt.
 **************************************************************************************************/

typedef struct GPS_PVT
{
	GPS_Position_DD_t Position;
	TD32_t Speed;
	TD32_t Heading;
	struct tm Time;

}GPS_PVT_t; ///< .

/**********************************************************************************************//**
 * \fn	Sexadecimal_Value_t GetLat(TS08_t NMEA_Lat_Field);
 *
 * \brief	Gets a lat.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param	NMEA_Lat_Field	The nmea lat field.
 *
 * \return	The lat.
 **************************************************************************************************/

Sexadecimal_Value_t GetLat(TS08_t NMEA_Lat_Field);

/**********************************************************************************************//**
 * \fn	TI32_t SexaDecimalToInt(Sexadecimal_Value_t &Value);
 *
 * \brief	Sexa decimal to int.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param [in,out]	Value	The value.
 *
 * \return	.
 **************************************************************************************************/

TI32_t SexaDecimalToInt(Sexadecimal_Value_t &Value);

/**********************************************************************************************//**
 * \fn	Sexadecimal_Value_t GetLong(TS08_t NMEA_Long_Field);
 *
 * \brief	Gets a long.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param	NMEA_Long_Field	The nmea long field.
 *
 * \return	The long.
 **************************************************************************************************/

Sexadecimal_Value_t GetLong(TS08_t NMEA_Long_Field);
#endif