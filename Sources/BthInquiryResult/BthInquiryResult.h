#ifndef CBTHINQUIRYRESULT_H
#define CBTHINQUIRYRESULT_H

#include "CommonBluetooth\CommonBluetooth.h"

class CBthInquiryResult
{
public:

	/**********************************************************************************************//**
	 * \fn	CBthInquiryResult::CBthInquiryResult(bool RequestSuccess, unsigned int NbDeviceFounded,
	 * 		BthInquiryResult * BtList);
	 *
	 * \brief	Initializes a new instance of the CBthInquiryResult class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	RequestSuccess 	true if the request operation was a success, false if it failed.
	 * \param	NbDeviceFounded	The nb device founded.
	 * \param [in,out]	BtList 	If non-null, list of bts.
	 **************************************************************************************************/

	CBthInquiryResult(bool RequestSuccess, unsigned int NbDeviceFounded, BthInquiryResult * BtList);

	/**********************************************************************************************//**
	 * \fn	CBthInquiryResult::~CBthInquiryResult();
	 *
	 * \brief	Finalizes an instance of the CBthInquiryResult class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CBthInquiryResult();

	/**********************************************************************************************//**
	 * \fn	bool CBthInquiryResult::GetRequestSuccess()
	 *
	 * \brief	Gets the request success.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool GetRequestSuccess(){return m_RequestSuccess;}

	/**********************************************************************************************//**
	 * \fn	unsigned int CBthInquiryResult::GetNbDeviceFounded()
	 *
	 * \brief	Gets the nb device founded.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The nb device founded.
	 **************************************************************************************************/

	unsigned int GetNbDeviceFounded(){return m_NbDeviceFounded;}

	/**********************************************************************************************//**
	 * \fn	BthInquiryResult * CBthInquiryResult::GetBthInquiryResult()
	 *
	 * \brief	Gets the bth inquiry result.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	null if it fails, else the bth inquiry result.
	 **************************************************************************************************/

	BthInquiryResult * GetBthInquiryResult(){return m_BtList;}
private:
	bool m_RequestSuccess;  ///< true if the request operation was a success, false if it failed
	unsigned int m_NbDeviceFounded; ///< The nb device founded
	BthInquiryResult * m_BtList;	///< List of bts
};

#endif