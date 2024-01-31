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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "TU_JP_CommonSt.h"
#include "TU_JP_ArithmeticOperation.h"

#include "TU_JP_Semaphore.h"

#define P_CAST(tar, src) (void)memcpy((void*)&(tar), (void*)&(src), sizeof(void*))

struct JP_Semaphore_T
{
	uint32_t uiCount;
	uint32_t uiMaximumCount;

	pthread_cond_t tCond;
	pthread_mutex_t tMutex;
};

static JP_Error_E SF_ObtainSema(JP_Semaphore_T *ptSemaphore)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSemaphore != NULL)
	{
		int32_t iVal = 0;

		while((ptSemaphore->uiCount == 0U) && (iVal == 0))
		{
			iVal = pthread_cond_wait(&ptSemaphore->tCond, &ptSemaphore->tMutex);
		}

		if(iVal != 0)
		{
			eRetErr = JP_Error_Wait;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_ObtainSemaInTime(JP_Semaphore_T *ptSemaphore, int iTimeout)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptSemaphore != NULL) && (iTimeout >= 0))
	{
		int32_t iVal;

		int64_t lSecond;
		int64_t lNanoSecond;
		int64_t lWaitTimeSec;
		int64_t lWaitTimeNanoSec;
		int64_t lOperands[3];

		struct timespec tTimeSpec;

		iVal = clock_gettime(CLOCK_MONOTONIC , &tTimeSpec);

		if(iVal == 0)
		{
			lSecond 		= (int64_t)iTimeout / 1000;
			lNanoSecond 	= ((int64_t)iTimeout % 1000) * 1000000;

			lOperands[0] = tTimeSpec.tv_nsec;
			lOperands[1] = lNanoSecond;

			eRetErr = JP_Add_Int64(2U, lOperands, &lWaitTimeNanoSec);

			if(eRetErr == JP_Error_None)
			{
				lOperands[0] = tTimeSpec.tv_sec;
				lOperands[1] = lSecond;
				lOperands[2] = (lWaitTimeNanoSec / 1000000000);

				eRetErr = JP_Add_Int64(3U, lOperands, &lWaitTimeSec);

				if(eRetErr == JP_Error_None)
				{
					tTimeSpec.tv_sec 	= lWaitTimeSec;
					tTimeSpec.tv_nsec 	= (lWaitTimeNanoSec % 1000000000);

					while((ptSemaphore->uiCount == 0U) && (iVal == 0))
					{
						iVal = pthread_cond_timedwait(&ptSemaphore->tCond, &ptSemaphore->tMutex , &tTimeSpec);
					}

					if(iVal != 0)
					{
						if(iVal == ETIMEDOUT)
						{
							if(ptSemaphore->uiCount < ptSemaphore->uiMaximumCount)
							{
								ptSemaphore->uiCount++;
							}

							eRetErr = JP_Error_Timeout;
						}
						else
						{
							eRetErr = JP_Error_TimeWait;
						}
					}
				}
			}
		}
		else
		{
			eRetErr = JP_Error_GetTime;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_ObtainSemaphore(JP_Semaphore_T *ptSemaphore, int iTimeout)
{
	JP_Error_E eRetErr;

	if(ptSemaphore != NULL)
	{
		(void)pthread_mutex_lock(&ptSemaphore->tMutex);

		if(iTimeout >= 0)
		{
			eRetErr = SF_ObtainSemaInTime(ptSemaphore, iTimeout);
		}
		else
		{
			eRetErr = SF_ObtainSema(ptSemaphore);
		}

		if((eRetErr == JP_Error_None) || (eRetErr == JP_Error_Timeout))
		{
			ptSemaphore->uiCount--;
		}

		(void)pthread_mutex_unlock(&ptSemaphore->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_ReleaseSemaphore(JP_Semaphore_T *ptSemaphore)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSemaphore != NULL)
	{
		int32_t iVal;

		(void)pthread_mutex_lock(&ptSemaphore->tMutex);

		if(ptSemaphore->uiCount < ptSemaphore->uiMaximumCount)
		{
			ptSemaphore->uiCount++;

			iVal = pthread_cond_signal(&ptSemaphore->tCond);

			if(iVal != 0)
			{
				eRetErr = JP_Error_Signal;
			}
		}

		(void)pthread_mutex_unlock(&ptSemaphore->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Semaphore_T *JP_CreateSemaphore(uint32_t uiInitialCount, uint32_t uiMaximumCount)
{
	JP_Semaphore_T *ptRetSema;
	void* tmpAlloc = NULL;

	tmpAlloc = g_malloc0(sizeof(JP_Semaphore_T));
	P_CAST(ptRetSema, tmpAlloc);

	if(ptRetSema != NULL)
	{
		int32_t iVal;

		ptRetSema->uiCount 			= uiInitialCount;
		ptRetSema->uiMaximumCount 	= uiMaximumCount;

		iVal = pthread_cond_init(&ptRetSema->tCond, NULL);

		if(iVal == 0)
		{
			iVal = pthread_mutex_init(&ptRetSema->tMutex, NULL);

			if(iVal != 0)
			{
				(void)pthread_cond_destroy(&ptRetSema->tCond);

				g_free(ptRetSema);
				ptRetSema = NULL;
			}
		}
		else
		{
			g_free(ptRetSema);
			ptRetSema = NULL;
		}
	}

	return ptRetSema;
}

JP_Error_E JP_DestroySemaphore(JP_Semaphore_T *ptSemaphore)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSemaphore != NULL)
	{
		int32_t iVal;

		iVal = pthread_cond_destroy(&ptSemaphore->tCond);

		if(iVal == 0)
		{
			iVal = pthread_mutex_destroy(&ptSemaphore->tMutex);

			if(iVal != 0)
			{
				eRetErr = JP_Error_ReleaseResource;
			}
		}
		else
		{
			eRetErr = JP_Error_ReleaseResource;
		}

		g_free(ptSemaphore);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}