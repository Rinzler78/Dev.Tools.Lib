#include "GPS_Common.h"
#include "stdlib.h"
#include "string.h"

Sexadecimal_Value_t GetLat(TS08_t NMEA_Lat_Field)
{
	/** Lat 5321.6802 */
	Sexadecimal_Value_t Result = {0};
	TC08_t Temp [5] = {0};
	
	if(NMEA_Lat_Field)
	{
		memcpy(Temp, NMEA_Lat_Field, 2);
		Result.Degrees = atoi(Temp);

		memcpy(Temp, NMEA_Lat_Field + 2, 2);
		Result.Minutes = atoi(Temp);
		
		if(NMEA_Lat_Field[4] == '.')
		{
			memcpy(Temp, NMEA_Lat_Field + 5, 4);
			Result.Minutes = atoi(Temp);
		}
	}
	
	return Result;
}
TI32_t SexaDecimalToInt(Sexadecimal_Value_t &Value)
{
	TI32_t Result = 0;
	double temp = 0;

	temp = (double)Value.Degrees + ((double)Value.Minutes/60) + ((double)Value.Secondes/3600);
	Result = (TI32_t)(temp * 100000);
	
	return Result;
}
Sexadecimal_Value_t GetLong(TS08_t NMEA_Long_Field)
{
	/** Long 00630.3372 */
	Sexadecimal_Value_t Result = {0};
	TC08_t Temp [5] = {0};

	if(NMEA_Long_Field)
	{
		memcpy(Temp, NMEA_Long_Field, 3);
		Result.Degrees = atoi(Temp);

		memcpy(Temp, NMEA_Long_Field + 3, 2);
		Result.Minutes = atoi(Temp);
		
		if(NMEA_Long_Field[5] == '.')
		{
			memcpy(Temp, NMEA_Long_Field + 6, 4);
			Result.Minutes = atoi(Temp);
		}
	}

	return Result;
}