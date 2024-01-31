/*******************************************************************************

*   Copyright (c) Telechips Inc.

*   TCC Version 1.0

This source code contains confidential information of Telechips.

Any unauthorized use without a written permission of Telechips including not
limited to re-distribution in source or binary form is strictly prohibited.

This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without
limitation, any warranty of merchantability, fitness for a particular purpose
or non-infringement of any patent, copyright or other third party intellectual
property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.

In no event shall Telechips be liable for any claim, damages or other
liability arising from, out of or in connection with this source code or
the use in the source code.

This source code is provided subject to the terms of a Mutual Non-Disclosure
Agreement between Telechips and Company.
*
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <math.h>

#include "TU_JP_CommonSt.h"

#include "TU_JP_TypeCast.h"

JP_RetType_UInt32_T JP_TypeCast_Int16ToUInt32(int16_t sValue)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(sValue >= 0)
	{
		tRetVal.uiRetVal = (uint32_t)sValue;
	}
	else
	{
		tRetVal.eError = JP_Error_TypeCasting;
	}

	return tRetVal;
}

JP_RetType_UInt64_T JP_TypeCast_Int32ToUInt64(int32_t iValue)
{
	JP_RetType_UInt64_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.ulRetVal = 0UL;

	if(iValue >= 0)
	{
		tRetVal.ulRetVal = (uint64_t)iValue;
	}
	else
	{
		tRetVal.eError = JP_Error_TypeCasting;
	}

	return tRetVal;
}

JP_RetType_Int32_T JP_TypeCast_UInt64ToInt32(uint64_t ulValue)
{
	JP_RetType_Int32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.iRetVal = 0;

	if(ulValue < (UINT32_MAX/2U))
	{
		tRetVal.iRetVal = (int32_t)ulValue;
	}
	else
	{
		tRetVal.eError = JP_Error_TypeCasting;
	}

	return tRetVal;
}

JP_RetType_UInt32_T JP_TypeCast_UInt64ToUInt32(uint64_t ulValue)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(ulValue <= UINT32_MAX)
	{
		tRetVal.uiRetVal = (uint32_t)ulValue;
	}
	else
	{
		tRetVal.eError = JP_Error_TypeCasting;
	}

	return tRetVal;
}