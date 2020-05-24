#include "BthInquiryResult\BthInquiryResult.h"
#include "stdio.h"

CBthInquiryResult::CBthInquiryResult(bool RequestSuccess
									, unsigned int NbDeviceFounded
									, BthInquiryResult * BtList) :	m_RequestSuccess(RequestSuccess)
																	, m_NbDeviceFounded(NbDeviceFounded)
														
{
	if(m_RequestSuccess)
	{
		if(m_NbDeviceFounded > 0)
		{
			m_BtList = new BthInquiryResult[m_NbDeviceFounded];
			memcpy(m_BtList, BtList, sizeof(BthInquiryResult) * m_NbDeviceFounded);
		}
	}
}
CBthInquiryResult::~CBthInquiryResult()
{
	if(m_BtList)
	{
		delete [] m_BtList;
		m_BtList = NULL;
	}
}