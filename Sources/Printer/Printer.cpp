#include "Printer.h"
#include "CommonTools\CommonTools.h"
#include "windows.h"

CPrinter::CPrinter()
{
}
CPrinter::~CPrinter()
{
}
bool CPrinter::WritePage(const TS08_t  pComments, ...)
{
	bool Result = false;
	TC08_t StringBuffer [200] = {0};
	va_list vl = NULL;
	
	/** Get Comments And Var List */
	va_start( vl, pComments );
	//sprintf(StringBuffer,"%s", CCommonTools::GetVarList(vl,pComments));
	va_end( vl );

	/** To DO */

	return Result;
}
bool CPrinter::WriteLine(const TS08_t  pComments, ...)
{
	bool Result = false;
	TC08_t StringBuffer [200] = {0};
	va_list vl = NULL;
	
	/** Get Comments And Var List */
	va_start( vl, pComments );
	//sprintf(StringBuffer,"%s", CCommonTools::GetVarList(vl,pComments));
	va_end( vl );

	/** To DO */

	return Result;
}
bool CPrinter::Print(const TS08_t  pString)
{
	bool Result = false;

	return Result;
}

