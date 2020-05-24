// FileSystemCRC16.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#if !defined (UNDER_CE) && defined (MEMORY_LEAK_CHECK)
#include "vld.h"
#endif
#include "CRCReportFile.h"
#include "Log\Log.h"
#include "IniFile\IniFile.h"
#include "IniSection.h"
#include "IniFile\IniSectionValue.h"
#include "FileInfo\FileInfo.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include "CommonTools\CommonTools.h"
#include "MyString\MyString.h"
#define DEFAULT_CRC_REPORT_REF_INI_PATH_AND_FILE_NAME "./CRCReport_Reference.ini"
#define DEFAULT_CRC_REPORT_CURRENT_INI_PATH_AND_FILE_NAME "./CRCReport.ini"

#define AUTOLOG_ARG L"-al"
#define AUTOLOG_ARG_verbose L"-alv"
#define SPECIFIED_LOG_ARG L"-sl"
#define SPECIFIED_LOG_ARG_verbose L"-slv"
#define REFERENCE_CRC_FILE_ARG L"-ref"
#define REPORT_CRC_FILE_ARG L"-report"

int _tmain(int argc, _TCHAR* argv[])
{
	ING_STATUS_t Return = ING_STATUS_FAILURE;
	CLog Trace;
	CMyString<char> ReferenceFile;
	CMyString<char> ReportFile;

	for(TU08_t i = 1 ; i < argc ; ++i)
	{
		if(!wcscmp(AUTOLOG_ARG,argv[i]))
		{
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w.txt", argv[0]);
			Trace.Active_Debug(true);
		}
		else if(!wcscmp(SPECIFIED_LOG_ARG,argv[i]))
		{
			++i;
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w", argv[i]);
			Trace.Active_Debug();
		}
		if(!wcscmp(AUTOLOG_ARG_verbose,argv[i]))
		{
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w.txt", argv[0]);
			Trace.Active_Debug(true, LOG_OUT_FILE | LOG_OUT_CONSOLE);
		}
		else if(!wcscmp(SPECIFIED_LOG_ARG_verbose,argv[i]))
		{
			++i;
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w", argv[i]);
			Trace.Active_Debug(false, LOG_OUT_FILE | LOG_OUT_CONSOLE);
		}
		else if(!wcscmp(REFERENCE_CRC_FILE_ARG,argv[i]))
		{
			++i;
			ReferenceFile.SetString("%w", argv[i]);
		}
		else if(!wcscmp(REPORT_CRC_FILE_ARG,argv[i]))
		{
			++i;
			ReportFile.SetString("%w", argv[i]);
		}
	}
	if(argc == 1)
	{
		/** Active verbose */
		Trace.Active_Debug(false,LOG_OUT_CONSOLE,false,false);
	}
	
	CRCReportFile Reference_CRCReport((ReferenceFile.String() ? ReferenceFile.String() : DEFAULT_CRC_REPORT_REF_INI_PATH_AND_FILE_NAME),&Trace);
	bool CRCCheckFailure = false;
	
	if(Reference_CRCReport.Load_Success())
	{
		if(	Reference_CRCReport.GlobalCRC() > 0 
			&& Reference_CRCReport.NumberOfCRCDetails() > 0
			&& (Reference_CRCReport.NumberOfCRCDetails() == Reference_CRCReport.NumberOfReferenceDetails()))
		{
			Trace.Log(	NOCOMMENTS,"CRC Check");

			CRCReportFile Current_CRCreport((ReportFile.String() ? ReportFile.String() : DEFAULT_CRC_REPORT_CURRENT_INI_PATH_AND_FILE_NAME), &Trace);
			
			if(!Current_CRCreport.Load_Success())
				Current_CRCreport.Load(); /** Load Default Content to retrieve comments */

			Current_CRCreport.SetReferenceDirectory(Reference_CRCReport.ReferenceDirectory().String());
			
			CIniSection * pRef_DirectoryOrFileToSkip = Reference_CRCReport.IniFile()[DEFAULT_CRC_REPORT_SECTION_NAME_2_];
			CIniSection * pCur_DirectoryOrFileToSkip = Current_CRCreport.IniFile()[DEFAULT_CRC_REPORT_SECTION_NAME_2_];

			if(pRef_DirectoryOrFileToSkip && pCur_DirectoryOrFileToSkip)
			{
				pCur_DirectoryOrFileToSkip->ClearValueList();

				for(TU32_t i = 0 ; i < pRef_DirectoryOrFileToSkip->ValueList().Count() ; ++i)
				{
					CIniSectionValue * pValue = pRef_DirectoryOrFileToSkip->ValueList()[i];
					
					if(pValue)
					{
						Current_CRCreport.AddSkipDirectory(pValue->Value().String());
						/** Add Comment */
						CIniSectionValue * pCurvalue = pCur_DirectoryOrFileToSkip->ValueList()[i];
						for(TU08_t j = 0 ; j < pValue->CommentList().Count() ; ++j)
						{
							pCurvalue->CommentList().Add(pValue->CommentList()[j]);
						}
					}
				}
			}
			
			Current_CRCreport.UpdateDetails();
			Current_CRCreport.SaveReport();

			if( Reference_CRCReport.GlobalCRC() == Current_CRCreport.GlobalCRC() 
				&& Reference_CRCReport.NumberOfCRCDetails() == Current_CRCreport.NumberOfCRCDetails() )
			{
				/** CRC Check Success */
				Trace.Log(NOCOMMENTS,"CRC Check Success");
				Return = ING_STATUS_SUCCESS;
			}
			else
			{
				Trace.Log(ALERT,"Global CRC Different than Source SnapShot");
				/** CRC Details Compare */
				if(Reference_CRCReport.NumberOfCRCDetails() >= Current_CRCreport.NumberOfCRCDetails())
				{
					/** Not need to check all details */
					if(Reference_CRCReport.NumberOfCRCDetails() > Current_CRCreport.NumberOfCRCDetails())
						Trace.Log(ALERT,"File have been delete since CRC SnapShot");
					else
						Trace.Log(ALERT,"File have been modified since CRC SnapShot");

					CRCCheckFailure = true;
				}
				else
				{
					CIniSection * pCRCDetailsSection_Ref = Reference_CRCReport.IniFile()[DEFAULT_CRC_REPORT_SECTION_NAME_3_];
					
					if(pCRCDetailsSection_Ref)
					{
						CIniSection * pCRCDetailsSection_Report = Current_CRCreport.IniFile()[DEFAULT_CRC_REPORT_SECTION_NAME_3_];

						if(pCRCDetailsSection_Report)
						{
							/** Compare CRC Details */
							TU32_t Index_Report = 0;
							for(TU32_t i = 0 ; (i < Reference_CRCReport.NumberOfCRCDetails()) && (Index_Report < pCRCDetailsSection_Report->ValueList().Count()) ; ++i)
							{
								CIniSectionValue * pCRCDetailsSectionValue_Ref = NULL;
								CIniSectionValue * pCRCDetailsSectionValue_Report = NULL;
								
								pCRCDetailsSectionValue_Ref = (*pCRCDetailsSection_Ref)[i];
								
								if(pCRCDetailsSectionValue_Ref && (pCRCDetailsSectionValue_Ref->ValueName().String()[0] != 'D'))
								{
									//Index_Report = i;
									pCRCDetailsSectionValue_Report = (*pCRCDetailsSection_Report)[Index_Report++];

									if(pCRCDetailsSectionValue_Report)
									{
										if((*pCRCDetailsSectionValue_Ref).Value() != pCRCDetailsSectionValue_Report->Value())
										{
											/** Search for similare value */
											while(((*pCRCDetailsSectionValue_Ref).Value() != pCRCDetailsSectionValue_Report->Value())
												&& (Index_Report < pCRCDetailsSection_Report->ValueList().Count()))
											{
												pCRCDetailsSectionValue_Report = (*pCRCDetailsSection_Report)[Index_Report++];
											}
											
											if(pCRCDetailsSectionValue_Report)
											{
												if((*pCRCDetailsSectionValue_Ref).Value() != pCRCDetailsSectionValue_Report->Value())
												{
													Trace.Log(ALERT	,"File have Been modified since CRC SnapShot %s=%s"
																	, pCRCDetailsSectionValue_Ref->ValueName().String()
																	, pCRCDetailsSectionValue_Ref->Value().String());

													CRCCheckFailure = true;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if(!CRCCheckFailure)
				{
					Trace.Log(NOCOMMENTS,"Some files have been add since snapshot, but orignal files have not been modified");
				}
				else
					Return = ING_STATUS_FAILURE;
			}
		}
		else
		{
			Trace.Log(	NOCOMMENTS,"CRC Report in progress");

			Reference_CRCReport.UpdateDetails();
			if(Reference_CRCReport.SaveReport())
				Return = ING_STATUS_SUCCESS;

		}
	}

	return Return;
}


