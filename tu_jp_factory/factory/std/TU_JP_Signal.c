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

#include "TU_JP_Signal.h"

#define P_CAST(tar, src) (void)memcpy((void*)&(tar), (void*)&(src), sizeof(void*))

struct JP_Signal_T
{
	JP_BOOL_E bSignaled;

	pthread_cond_t tCond;
	pthread_mutex_t tMutex;
};

static JP_Error_E SF_InitCond(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		int32_t iRetVal;

		pthread_condattr_t tCondAttr;

		iRetVal = pthread_condattr_init(&tCondAttr);

		if(iRetVal == 0)
		{
			iRetVal = pthread_condattr_setclock(&tCondAttr, CLOCK_MONOTONIC);

			if(iRetVal == 0)
			{
				iRetVal = pthread_cond_init(&ptSignal->tCond, &tCondAttr);

				if(iRetVal != 0)
				{
					eRetErr = JP_Error_InsufficientResources;
				}
			}
			else
			{
				eRetErr = JP_Error_InsufficientResources;
			}

			(void)pthread_condattr_destroy(&tCondAttr);
		}
		else
		{
			eRetErr = JP_Error_InsufficientResources;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_SignalWait(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		int32_t iRetVal = 0;

		while((ptSignal->bSignaled == JP_FALSE) && (iRetVal == 0))
		{
			iRetVal = pthread_cond_wait(&ptSignal->tCond, &ptSignal->tMutex);
		}

		ptSignal->bSignaled = JP_FALSE;

		if(iRetVal != 0)
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

static JP_Error_E SF_SignalTimeWait(JP_Signal_T *ptSignal, int iTimeout)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptSignal != NULL) && (iTimeout >= 0))
	{
		int32_t iRetVal;

		uint32_t uiOperandCnt;

		int64_t lSecond;
		int64_t lNanoSecond;
		int64_t lWaitTimeSec;
		int64_t lWaitTimeNanoSec;
		int64_t lOperands[3];

		struct timespec tspec;

		iRetVal = clock_gettime(CLOCK_MONOTONIC, &tspec);

		if(iRetVal == 0)
		{
			lSecond = (int64_t)iTimeout / 1000;
			lNanoSecond = ((int64_t)iTimeout % 1000) * 1000000;

			uiOperandCnt = 2;

			lOperands[0] = tspec.tv_nsec;
			lOperands[1] = lNanoSecond;

			eRetErr = JP_Add_Int64(uiOperandCnt, lOperands, &lWaitTimeNanoSec);

			if(eRetErr == JP_Error_None)
			{
				uiOperandCnt = 3;

				lOperands[0] = tspec.tv_sec;
				lOperands[1] = lSecond;
				lOperands[2] = (lWaitTimeNanoSec / 1000000000);

				eRetErr = JP_Add_Int64(uiOperandCnt, lOperands, &lWaitTimeSec);

				if(eRetErr == JP_Error_None)
				{
					tspec.tv_sec = lWaitTimeSec;
					tspec.tv_nsec = (lWaitTimeNanoSec % 1000000000);

					while((ptSignal->bSignaled == JP_FALSE) && (iRetVal == 0))
					{
						iRetVal = pthread_cond_timedwait(&ptSignal->tCond, &ptSignal->tMutex, &tspec);
					}

					ptSignal->bSignaled = JP_FALSE;

					if(iRetVal != 0)
					{
						if(iRetVal == ETIMEDOUT)
						{
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

JP_Error_E JP_SignalWait(JP_Signal_T *ptSignal, int iTimeout)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		(void)pthread_mutex_lock(&ptSignal->tMutex);

		if(iTimeout >= 0)
		{
			eRetErr = SF_SignalTimeWait(ptSignal, iTimeout);
		}
		else
		{
			eRetErr = SF_SignalWait(ptSignal);
		}

		(void)pthread_mutex_unlock(&ptSignal->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_SignalWakeUp(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		int32_t iRetVal;

		(void)pthread_mutex_lock(&ptSignal->tMutex);

		ptSignal->bSignaled = JP_TRUE;

		iRetVal = pthread_cond_signal(&ptSignal->tCond);

		if(iRetVal != 0)
		{
			eRetErr = JP_Error_Signal;
		}

		(void)pthread_mutex_unlock(&ptSignal->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_SignalLock(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		(void)pthread_mutex_lock(&ptSignal->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_SignalUnlock(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		(void)pthread_mutex_unlock(&ptSignal->tMutex);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_SignalBareWait(JP_Signal_T *ptSignal, int iTimeout)
{
	JP_Error_E eRetErr;

	if(ptSignal != NULL)
	{
		if(iTimeout >= 0)
		{
			eRetErr = SF_SignalTimeWait(ptSignal, iTimeout);
		}
		else
		{
			eRetErr = SF_SignalWait(ptSignal);
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_SignalBareWakeUp(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		int32_t iRetVal;

		ptSignal->bSignaled = JP_TRUE;

		iRetVal = pthread_cond_signal(&ptSignal->tCond);

		if(iRetVal != 0)
		{
			eRetErr = JP_Error_Signal;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Signal_T *JP_CreateSignal(void)
{
	JP_Signal_T *ptRetSignal;
	void* tmpAlloc = NULL;

	tmpAlloc = g_malloc0(sizeof(JP_Signal_T));
	P_CAST(ptRetSignal, tmpAlloc);

	if(ptRetSignal != NULL)
	{
		int32_t iRetVal;

		JP_Error_E eError;

		ptRetSignal->bSignaled = JP_FALSE;

		eError = SF_InitCond(ptRetSignal);

		if(eError == JP_Error_None)
		{
			iRetVal = pthread_mutex_init(&ptRetSignal->tMutex, NULL);

			if(iRetVal != 0)
			{
				(void)pthread_cond_destroy(&ptRetSignal->tCond);

				g_free(ptRetSignal);
				ptRetSignal = NULL;
			}
		}
		else
		{
			g_free(ptRetSignal);
			ptRetSignal = NULL;
		}
	}

	return ptRetSignal;
}

JP_Error_E JP_DestroySignal(JP_Signal_T *ptSignal)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptSignal != NULL)
	{
		int32_t iRetVal;

		iRetVal = pthread_cond_destroy(&ptSignal->tCond);

		if(iRetVal == 0)
		{
			iRetVal = pthread_mutex_destroy(&ptSignal->tMutex);

			if(iRetVal != 0)
			{
				eRetErr = JP_Error_ReleaseResource;
			}
		}
		else
		{
			eRetErr = JP_Error_ReleaseResource;
		}

		g_free(ptSignal);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}