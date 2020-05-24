#ifndef CINI_FILE_CPP
#define CINI_FILE_CPP
#include "IniFile\IniFile.h"
#include "IniSection.h"
#include "IniFile\IniSectionValue.h"
#include "MyString\MyString.h"
#include "CommonTools\CommonTools.h"
#include "FileInfo\FileInfo.h"
#include "Log\Log.h"

#define INI_FILE_COMMENT_MARK ';'
#define INI_FILE_START_SECTION_MARK '['
#define INI_FILE_STOP_SECTION_MARK ']'
#define INI_FILE_VALUE_PRESENCE_MARK '='
#define INI_FILE_VALUE_TYPE_STRING_MARK '\"'

//CIniFile::CIniFile(CLog *pLog) 
//: m_pFileInfo(NULL)
//, m_pLog(pLog)
//, m_Loaded(false)
//, m_pCommentList(NULL)
//, m_pSectionList(NULL)
//{
//	m_pFileInfo = new CFileInfo();
//}
CIniFile::CIniFile(const TS08_t pIniPathAndFileName, CLog *pLog) 
: m_pFileInfo(NULL)
, m_pLog(pLog)
, m_Loaded(false)
, m_pCommentList(new CLinkedList<CMyString<char> *>())
, m_pSectionList(new CLinkedList<CIniSection *>())
{
	m_pFileInfo = new CFileInfo(pIniPathAndFileName);
}
CIniFile::~CIniFile()
{
	ClearCommentList();
	MACRO_DELET(m_pCommentList)
	ClearSectionList();
	MACRO_DELET(m_pSectionList)

	MACRO_DELET(m_pFileInfo)
}
bool CIniFile::IsLoaded()const{return m_Loaded;}
bool CIniFile::Exist()const {return m_pFileInfo->Exist();};
TU64_t CIniFile::Size()const {return m_pFileInfo->Size();};
const CMyString<char> & CIniFile::FullName()const {return m_pFileInfo->FullName();};
bool CIniFile::Load()
{
	bool Result = false;

	if(m_pFileInfo)
	{
		if(m_pFileInfo->Exist())
		{
			if(m_pFileInfo->Size() > 0)
			{
				/** Get ini File info */
				ClearCommentList();
				ClearSectionList();
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS,"Loading Ini file %s", m_pFileInfo->FullName().String());
				Result = RetrieveInfoFromFile();
				m_Loaded = Result;
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS,"Ini file %s loaded", m_pFileInfo->FullName().String());
			}
			else
				if(m_pLog)
					m_pLog->Log(ALERT,"ini file %s is empty", m_pFileInfo->FullName());
		}
		else
			if(m_pLog)
				m_pLog->Log(ALERT,"ini file %s does not exist", m_pFileInfo->FullName());
	}
	else
		if(m_pLog)
			m_pLog->Log(ALERT,"m_pFileInfo pointer empty");

	return Result;
}
bool CIniFile::Load(const TS08_t  pIniPathAndFileName)
{
	bool Result = false;

	if(pIniPathAndFileName)
	{
		MACRO_DELET(m_pFileInfo);

		m_pFileInfo = new CFileInfo(pIniPathAndFileName,m_pLog);
		Result = Load();
	}

	return Result;
}
TI32_t CIniFile::GetSectionIndex(const TS08_t pSectionName) const
{
	TI32_t Result = -1;
	
	if(pSectionName)
	{
		for(TU32_t  i = 0 ; i < (TU32_t)SectionList().Count() ; ++i)
		{
			if(SectionList()[i]->SectionName() == pSectionName)
			{
				Result = i;
				break;
			}
		}
	}

	return Result;
}
CIniSection * CIniFile::operator [] (const TS08_t pSectionName)const
{
	CIniSection *pResult = NULL;

	if(pSectionName)
	{
		TI32_t Index = GetSectionIndex(pSectionName);
		if(Index > -1)
			pResult = SectionList()[Index];
	}
	return pResult;
}
CIniSection * CIniFile::operator [] (const TU32_t Index)const
{
	return SectionList()[Index];
}
bool CIniFile::UpdateFile()
{
	return UpdateFile(	(CLinkedList<CMyString<char> *> *)&CommentList()
						, (CLinkedList<CIniSection *> *)&SectionList());
}
bool CIniFile::Delete()
{
	bool Result = false;
	
	if(m_pFileInfo)
		Result = m_pFileInfo->Delete();

	return Result;
}
bool CIniFile::SetContentInFile(CLinkedList<CMyString<char> *> *pCommentList, CLinkedList<CIniSection *> * pSectionList)
{
	return UpdateFile(	pCommentList
						,pSectionList);
}
bool CIniFile::SetContentInFile(const TS08_t pCharStringToPutInfile)
{
	bool Result = false;

	if(pCharStringToPutInfile)
	{
		if(m_pFileInfo->Exist())
			m_pFileInfo->Delete();
		
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Put a content in ini file");
		Result = m_pFileInfo->Write(pCharStringToPutInfile);
		m_pFileInfo->Close();
		m_pFileInfo->Refresh();
	}

	return Result;
}
bool CIniFile::UpdateFile(CLinkedList<CMyString<char> *> * pCommentList, CLinkedList<CIniSection *> * pSectionList)
{
	bool Result = false;
	
	if(m_pFileInfo && (pCommentList || pSectionList))
	{
		Result = true;
		if(m_pFileInfo->Exist())
			m_pFileInfo->Delete();

		m_pFileInfo->LockAutoClose();

		/** Comments */
		if(pCommentList)
		{
			for(TU32_t i = 0 ; i < (TU32_t)pCommentList->Count() ; ++i)
			{
				m_pFileInfo->WriteLine(";%s", (*pCommentList)[i]->String());
			}
		}
		/** Sections */
		if(pSectionList)
		{
			for(TU32_t i = 0 ; i < (TU32_t)pSectionList->Count() ; ++i)
			{
				if((*pSectionList)[i]->SectionNameComment().Length() > 0)
					m_pFileInfo->WriteLine("[%s];%s", (*pSectionList)[i]->SectionName().String(), (*pSectionList)[i]->SectionNameComment().String());
				else
					m_pFileInfo->WriteLine("[%s]", (*pSectionList)[i]->SectionName().String());

				
				/** Value List */
				CLinkedList<CIniSectionValue *> & ValueList = (*pSectionList)[i]->ValueList();

				for(TU32_t j = 0 ; j < (TU32_t)ValueList.Count() ; ++j)
				{
					/** Put Comment Befor value */
					for(TU32_t k = 0 ; k < (TU32_t)ValueList[j]->CommentList().Count() ; ++k)
					{
						m_pFileInfo->WriteLine(";%s", ValueList[j]->CommentList()[k]->String());
					}
					/** Put value */
					if(ValueList[j]->CommentValueOnLine().Length() > 0)
						m_pFileInfo->WriteLine("%s=%s;%s", ValueList[j]->ValueName().String(), ValueList[j]->Value().String(), ValueList[j]->CommentValueOnLine().String());
					else
						m_pFileInfo->WriteLine("%s=%s", ValueList[j]->ValueName().String(), ValueList[j]->Value().String());
				}
			}
		}

		if(m_pFileInfo->AutoCloseLocked())
		{
			m_pFileInfo->UnLockAutoClose();
			m_pFileInfo->Close();
		}
	}
	return Result;
}

bool CIniFile::RetrieveInfoFromFile()
{
	bool Result = false;
	
	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "RetrieveInfoFromFile");
	if(m_pFileInfo && m_pFileInfo->Exist() && (m_pFileInfo->Size() > 0))
	{
		m_pFileInfo->LoadContent();
		Result = Parse(m_pFileInfo->GetContent());
	}

	return Result;
}

bool CIniFile::Parse(const TS08_t pFileContent)
{
	bool Result = false;
	CLinkedList<CMyString<char> *> *pLineList = NULL;
	CIniSection * pSection = NULL;
	bool CommentBeforeValue = false;
#if defined (_DEBUG)
	TU32_t ListCount = 0;
#endif
	
	if(pFileContent)
	{
		CMyString<char> FileContent(pFileContent);

		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Parse ini file");
		//pLineList = CCommonTools::Split(pFileContent,"\r\n");
		pLineList = FileContent.Split("\r\n", 2);
		
		if(pLineList)
		{
#if defined (_DEBUG)
			ListCount = pLineList->Count();
#endif
			while(pLineList->Count())
			{
				CMyString<char> * pStringLine = NULL;
				TS08_t pLine = NULL;
				int CharResearchIndex = 0;
				TC08_t SectionName [SECTIONNAME_SIZE] = {0};
				TC08_t Comment [SECTIONCOMMENT_SIZE] = {0};
				
				pStringLine = (*pLineList)[0];
				
#if defined (_DEBUG)
				TU32_t CurrentLineIndex = (ListCount - pLineList->Count());

				if(m_pLog)
					m_pLog->Log(NOCOMMENTS, "Working on line %u", CurrentLineIndex);
#endif
				if(pStringLine)
				{
					if(pStringLine->Length())
					{
						pLine = pStringLine->String();

						switch(*pLine)
						{
							case INI_FILE_COMMENT_MARK:
								/** Retrieve File comment */
								if(SectionList().Count() == 0)
									CommentList().Add(new CMyString<char>(GetComment(pLine)));
								else
								{
									CIniSection *pSection = NULL;
									pSection = SectionList().GetLast();
									if(pSection)
									{
										if(pSection->ValueList().Count() == 0 || !CommentBeforeValue)
											pSection->ValueList().Add(new CIniSectionValue());
										
										if(pSection->ValueList().Count() > 0)
										{	
											CIniSectionValue *pSectionValue = pSection->ValueList().GetLast();
											pSectionValue->CommentList().Add(new CMyString<char>(GetComment(pLine)));
											CommentBeforeValue = true;
										}
									}
								}

								/** Go To Next Line */
								//pLine += LineSize;
								break;
							case INI_FILE_START_SECTION_MARK:
								SectionList().Add(new CIniSection(GetSectionInLine(pLine)));
								/** Go To Next Line */
								//pLine += LineSize;
								break;
							default:
								if(SectionList().Count() > 0 && (pStringLine->Length() > 0))
								{
									/** Add Comment to The Last Section of The List */
									if(CommentBeforeValue)
									{
										CIniSectionValue ValueInLine = GetValueInLine(pLine);
										CIniSectionValue *pValue = SectionList().GetLast()->ValueList().GetLast();
										
										if(pValue)
										{
											(*pValue) = ValueInLine;
										}

										CommentBeforeValue = false;
									}
									else
										SectionList().GetLast()->ValueList().Add(new CIniSectionValue(GetValueInLine(pLine)));
								}
								break;
						}
					}
					
					pLineList->DeleteFirst();
					/** Free line memory */
					pLine = NULL;
					MACRO_DELET(pStringLine)
					
					/** Delete element in list */
					Result = true;
					
				}
				else
				{
					Result = false;
					break;
				}
			}
		}
	}

	MACRO_DELET(pLineList);
	return Result;
}

CMyString<char> CIniFile::GetComment(const TS08_t pLine, TU32_t Offset)
{
	int CharResearchIndex = 0;
	TU32_t LineSize = 0;
	TS08_t pComment = NULL;

	if(pLine && *pLine)
	{
		CMyString<char> Line(pLine);

		if(Offset < Line.Length())
		{
			CharResearchIndex = Line.FindFirst(INI_FILE_COMMENT_MARK);

			if(CharResearchIndex > -1)
				pComment = &pLine[CharResearchIndex + 1];
		}
	}

	return CMyString<char>(pComment);
}

CIniSection CIniFile::GetSectionInLine(TS08_t  pLine, TU32_t Offset)
{
	int CharResearchIndex = 0;
	TC08_t SectionName [SECTIONNAME_SIZE] = {0};
	TU32_t LineLength = 0;
	CIniSection Temp;

	if(pLine && *pLine)
	{
		if(*pLine == INI_FILE_START_SECTION_MARK)
		{
			CMyString<char> Line(pLine + 1);
			
			/** Get Section Name*/
			CharResearchIndex = Line.FindFirst(INI_FILE_STOP_SECTION_MARK);// CCommonTools::FindFirstIndex(pLine, INI_FILE_STOP_SECTION_MARK, LineLength);
			if(CharResearchIndex > -1)
			{
				Temp.SectionName().SetString(CharResearchIndex, Line.String());

				/** Search for Comment after */
				Temp.SectionNameComment() = GetComment(Line.String() + CharResearchIndex + 1);
			}
		}
	}
	
	return CIniSection(Temp);
}

CIniSectionValue CIniFile::GetValueInLine(const TS08_t  pLine, TU32_t Offset)
{
	CIniSectionValue Temp;
	TI32_t ValueMarkIndex = 0;
	TI32_t CommentMarkIndex = 0;
	
	if(pLine)
	{
		CMyString<char> Line(pLine);
		
		ValueMarkIndex = Line.FindFirst(INI_FILE_VALUE_PRESENCE_MARK);
		if(ValueMarkIndex > -1)
		{
			TU32_t ValueEndIndex = 0;

			/** Get Value Name */
			Temp.ValueName().SetString(ValueMarkIndex, pLine);
			Temp.ValueName().SupressBefore(' ');
			Temp.ValueName().SupressAfter(' ');
			
			/** Search for Comment after Value */
			CommentMarkIndex = Line.FindFirst(INI_FILE_COMMENT_MARK);

			/** Get Value */
			ValueEndIndex = (CommentMarkIndex > -1 ? CommentMarkIndex - 1 : Line.Length());
			
			Temp.Value().SetString((ValueEndIndex - ValueMarkIndex - 1), Line.String() + ValueMarkIndex + 1);
			Temp.Value().SupressBefore(' ');
			Temp.Value().SupressAfter(' ');
			
			/** Get Comment */
			if(ValueEndIndex != Line.Length())
				Temp.CommentValueOnLine() = GetComment(Line.String() + CommentMarkIndex);
		}
	}

	return Temp;
}

void CIniFile::ClearCommentList()
{
	CMyString<char> * pComment = NULL;
	/** Delete Comments List */
	if(m_pCommentList)
		while(CommentList().Count())
		{
			pComment = CommentList()[0];
			CommentList().DeleteAt(0);
			MACRO_DELET(pComment)
		}
}

void CIniFile::ClearSectionList()
{
	/** Delete Section List */
	if(m_pSectionList)
		while(SectionList().Count())
		{
			CIniSection *pSection = NULL;
			pSection = SectionList()[0];
			MACRO_DELET(pSection)
			SectionList().DeleteAt(0);
		}
}
#endif