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

#include <stdint.h>
#include <limits.h>
#include <time.h>

#include "TU_JP_CommonSt.h"
#include "TU_JP_Time.h"
#include "TU_JP_ArithmeticOperation.h"


JP_Time_T JP_GetTime(void)
{
	int32_t iRetVal;

	JP_Time_T iRetTime;
	JP_Time_T iTemp;

	struct timespec tspec;

	iRetVal = clock_gettime(CLOCK_MONOTONIC , &tspec);

	if(iRetVal != 0)
	{
		iRetTime = 0;
	}
	else
	{
		iTemp = JP_Multiply_Int64((JP_Time_T)tspec.tv_sec, 1000000LL);

		if(iTemp == 0)
		{
			iRetTime = 0;
		}
		else
		{
			int64_t lOperands[2];
			JP_Error_E eError;

			lOperands[0] = iTemp;
			lOperands[1] = JP_Divide_Int64((JP_Time_T)tspec.tv_nsec, 1000LL);

			eError = JP_Add_Int64(2U, lOperands, &iRetTime);

			if(eError != JP_Error_None)
			{
				iRetTime = 0;
			}
		}
	}

	return iRetTime;
}

JP_Time_T JP_GetRealTime(void)
{
	int32_t iRetVal;

	JP_Time_T iRetRealTime;
	JP_Time_T iTemp;

	struct timespec tspec;

	iRetVal = clock_gettime(CLOCK_REALTIME , &tspec);

	if(iRetVal != 0)
	{
		iRetRealTime = 0;
	}
	else
	{
		iTemp = JP_Multiply_Int64((JP_Time_T)tspec.tv_sec, 1000000);

		if(iTemp == 0)
		{
			iRetRealTime = 0;
		}
		else
		{
			int64_t lOperands[2];
			JP_Error_E eError;

			lOperands[0] = iTemp;
			lOperands[1] = JP_Divide_Int64((JP_Time_T)tspec.tv_nsec, 1000);

			eError = JP_Add_Int64(2U, lOperands, &iRetRealTime);

			if(eError != JP_Error_None)
			{
				iRetRealTime = 0;
			}
		}
	}

	return iRetRealTime;
}