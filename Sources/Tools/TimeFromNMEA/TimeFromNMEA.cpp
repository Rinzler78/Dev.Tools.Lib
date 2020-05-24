// TimeFromNMEA.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "Log\Log.h"
#include "CGPS.h"
#include "CommonTools\CommonTools.h"
#include "stdlib.h"

#define AUTOLOG_ARG L"-al"
#define AUTOLOG_ARG_verbose L"-alv"
#define SPECIFIED_LOG_ARG L"-sl"
#define SPECIFIED_LOG_ARG_verbose L"-slv"
#define PORTNUMBER_ARG L"-p"
#define BAUDRATE_ARG L"-br"
#define TIMEZONE_ARG L"-tz"

#define REFERENCE_YEAR 2012
#define REFERENCE_MONTH 2
#define REFERENCE_MDAY 1
HANDLE StopEvent = NULL;
CLog *pTrace = NULL;
void NewPVTReceived(GPS_PVT_t NewPVT);
bool PrintOnScreen = false;
TC08_t Hour = 0;
TC08_t Minutes = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	TU08_t PortNumber = 0;
	TU32_t BaudRate = 0;
	pTrace = new CLog();

	if(argc > 0)
	{
		for(TU08_t i = 1 ; i < argc ; ++i)
		{
			if(!wcscmp(TIMEZONE_ARG,argv[i]))
			{
				++i;
				TU32_t Len = 0;
				Len = wcslen(argv[i]);
				if(Len == 6)
				{
					TC08_t temp [6] = {0};
					wcstombs(temp, argv[i],Len);
					Minutes = atoi(temp + 4);
					temp[3] = 0;
					Hour = atoi(temp);
				}
			}
			else if(!wcscmp(PORTNUMBER_ARG,argv[i]))
			{
				++i;
				PortNumber = (TU08_t)_wtoi(argv[i]);
			}
			else if(!wcscmp(BAUDRATE_ARG,argv[i]))
			{
				++i;
				BaudRate = _wtoi(argv[i]);
			}
			else if(!wcscmp(AUTOLOG_ARG,argv[i]))
			{
				/** Active Log With Default Log Name */
				pTrace->SetPathAndFileName("%w.txt", argv[0]);
				pTrace->Active_Debug(true);
			}
			else if(!wcscmp(SPECIFIED_LOG_ARG,argv[i]))
			{
				++i;
				/** Active Log With Default Log Name */
				pTrace->SetPathAndFileName("%w", argv[i]);
				pTrace->Active_Debug();
			}
			else if(!wcscmp(AUTOLOG_ARG_verbose,argv[i]))
			{
				/** Active Log With Default Log Name */
				pTrace->SetPathAndFileName("%w.txt", argv[0]);
				pTrace->Active_Debug(true, LOG_OUT_FILE | LOG_OUT_CONSOLE);
			}
			else if(!wcscmp(SPECIFIED_LOG_ARG_verbose,argv[i]))
			{
				++i;
				/** Active Log With Default Log Name */
				pTrace->SetPathAndFileName("%w", argv[i]);
				pTrace->Active_Debug(false, LOG_OUT_FILE | LOG_OUT_CONSOLE);
			}
		}
	


		pTrace->Active_Debug(true, LOG_OUT_CONSOLE);
		pTrace->Log(NOCOMMENTS, "Start");
		
		StopEvent = CreateEvent(NULL,false,false,NULL);
		//CGPS GpsDevice(PortNumber,BaudRate);
		CGPS GpsDevice(PortNumber);
		if(pTrace)
			pTrace->Log(NOCOMMENTS, "Listening GPS Device on COM%u at %u bauds", PortNumber, BaudRate);

		GpsDevice.StartAcquisition(NewPVTReceived);
		if(GpsDevice.IsInAcquisition())
		{
			WaitForSingleObject(StopEvent, INFINITE);
			CloseHandle(StopEvent);
		}	
	}
	else
	{
		printf("Not Enought Arguments");
		Sleep(2000);
	}

	if(pTrace)
	{
		delete pTrace;
		pTrace = NULL;
	}
	return 0;
}
void NewPVTReceived(GPS_PVT_t NewPVT)
{
	SYSTEMTIME SystemTime = {0};
	
	SystemTime.wYear = NewPVT.Time.tm_year;
	SystemTime.wMonth = NewPVT.Time.tm_mon;
	SystemTime.wDay = NewPVT.Time.tm_mday;
	SystemTime.wHour = NewPVT.Time.tm_hour;
	SystemTime.wMinute = NewPVT.Time.tm_min;
	SystemTime.wSecond = NewPVT.Time.tm_sec;
	
	if(PrintOnScreen)
	{
		printf("%02u/%02u/%04u %02u:%02u:%02u\r"	, SystemTime.wDay
													, SystemTime.wMonth
													, SystemTime.wYear
													, SystemTime.wHour
													, SystemTime.wMinute
													, SystemTime.wSecond);
	}

	if(	SystemTime.wYear >= REFERENCE_YEAR 
		&& SystemTime.wMonth >= REFERENCE_MONTH 
		&& SystemTime.wDay >= REFERENCE_MDAY)
	{
		
#if defined (UNDER_CE)
		if(pTrace)
			pTrace->Log(NOCOMMENTS,"Set System Time to %u/%u/%u %u:%u:%u Time Zone : %02i:%02u"	, SystemTime.wDay
																								, SystemTime.wMonth
																								, SystemTime.wYear
																								, SystemTime.wHour
																								, SystemTime.wMinute
																								, SystemTime.wSecond
																								, Hour
																								, Minutes);
		TIME_ZONE_INFORMATION TimeZone = {0};

		GetTimeZoneInformation(&TimeZone);
		TimeZone.Bias = (Hour * 60 + Minutes) * -1;
		SetTimeZoneInformation(&TimeZone);
		SetSystemTime(&SystemTime);
		
		/** Make a sound to informe success*/
#if !defined (_DEBUG)
		MessageBeep(MB_ICONASTERISK);
		MessageBeep(MB_ICONASTERISK);
#endif
		if(StopEvent)
			SetEvent(StopEvent);
#endif
	}
}