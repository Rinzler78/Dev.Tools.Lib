#ifndef CBTPRINTER_H
#define CBTPRINTER_H

#define DEFAULT_DEVICENAME _T("auto")
#define DEFAULT_PRINTERNAME			_T("SPP-R200")
#define DEFAULT_FRIENDLYNAME		_T("SPP-R200 (PRINTER)")
#define DEFAULT_PREFIXE_PORT _T("BSP")

#include "BTDevice\BTDevice.h"

class CBTPrinter : public CBTDevice
{
public:

	/**********************************************************************************************//**
	 * \fn	CBTPrinter::CBTPrinter(const wchar_t * DeviceName, BT_ADDR Device_Adrress = 0);
	 *
	 * \brief	Initializes a new instance of the CBTPrinter class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	DeviceName	  	Name of the device.
	 * \param	Device_Adrress	(optional) the device adrress.
	 **************************************************************************************************/

	CBTPrinter(const wchar_t * DeviceName, BT_ADDR Device_Adrress = 0) ;

	/**********************************************************************************************//**
	 * \fn	CBTPrinter::~CBTPrinter();
	 *
	 * \brief	Finalizes an instance of the CBTPrinter class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CBTPrinter();

	/**********************************************************************************************//**
	 * \fn	bool CBTPrinter::Activate();
	 *
	 * \brief	Activates this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Activate();

	/**********************************************************************************************//**
	 * \fn	bool CBTPrinter::Deactivate();
	 *
	 * \brief	Deactivates this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Deactivate();

	/**********************************************************************************************//**
	 * \fn	bool CBTPrinter::Write(const char * TexteToPrint);
	 *
	 * \brief	Writes.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	TexteToPrint	The const char * to write.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Write(const char * TexteToPrint);
private:
};

#endif