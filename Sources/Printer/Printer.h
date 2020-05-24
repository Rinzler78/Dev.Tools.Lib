
#ifndef CPRINTER_H
#define CPRINTER_H

#include "Common\MyTypes.h"

class CPrinter
{
public:

	/**********************************************************************************************//**
	 * \fn	CPrinter::CPrinter();
	 *
	 * \brief	Initializes a new instance of the CPrinter class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	CPrinter();

	/**********************************************************************************************//**
	 * \fn	CPrinter::~CPrinter();
	 *
	 * \brief	Finalizes an instance of the CPrinter class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CPrinter();

	/**********************************************************************************************//**
	 * \fn	bool CPrinter::WritePage(const TS08_t pComments, ...);
	 *
	 * \brief	Writes a page.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pComments	The comments.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool WritePage(const TS08_t  pComments, ...);

	/**********************************************************************************************//**
	 * \fn	bool CPrinter::WriteLine(const TS08_t pComments, ...);
	 *
	 * \brief	Writes a line.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pComments	The comments.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool WriteLine(const TS08_t  pComments, ...);
protected:
private:

	/**********************************************************************************************//**
	 * \fn	bool CPrinter::Print(const TS08_t pString);
	 *
	 * \brief	Prints.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pString	The string.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Print(const TS08_t  pString);
};
#endif