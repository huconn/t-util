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
#include <pthread.h>
#include <stdint.h>
#include <limits.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <TU_JP_CommonSt.h>
#include <TU_JP_Lib_C_Std.h>
#include <TU_JP_Lib_C_D_LinkedList.h>

#include "TU_JP_Timer.h"

struct JP_Timer_T
{
	uint32_t uiTimer_ID;

	void *pvUserPrivate;

	JP_BOOL_E bThreadCreated;
	JP_BOOL_E bKillMySelf;
	JP_BOOL_E bEnabled;

	JP_Time_T tInterval;
	JP_Time_T tEnableTime;
	JP_Time_T tNextAlramTime;

	JP_Signal_T *ptSignal;

	pthread_t tThread;

	pfnTimerCallBack fnTimerCallBack;
};

static void *SF_TimerThread(void *pvPrivate)
{
	if(pvPrivate != NULL)
	{
		JP_Timer_T *ptTimer;

		P_CAST(ptTimer, pvPrivate);

		while(ptTimer->bKillMySelf == JP_FALSE)
		{
			JP_Error_E eError = JP_Error_None;

			(void)JP_SignalLock(ptTimer->ptSignal);

			if(ptTimer->bEnabled == JP_TRUE)
			{
				int32_t iDiffTime;

				JP_Time_T tExp1;
				JP_Time_T tExp2;
				JP_Time_T tExp3;
				JP_Time_T tCurrTime;

				if(ptTimer->tNextAlramTime == 0)
				{
					tCurrTime = JP_GetTime();

					tExp1 = tCurrTime - ptTimer->tEnableTime;
					tExp2 = ptTimer->tInterval - tExp1;

					ptTimer->tNextAlramTime = tCurrTime + tExp2;

					tExp3 = ptTimer->tNextAlramTime - tCurrTime;

					if((tExp3 >= INT_MIN) && (tExp3 <= INT_MAX))
					{
						iDiffTime = (int32_t)tExp3;
						iDiffTime /= 1000;

						if(iDiffTime == 0)
						{
							iDiffTime = 10;
						}
					}
					else
					{
						eError = JP_Error_TypeCasting;
					}
				}
				else
				{
					(void)JP_SignalUnlock(ptTimer->ptSignal);

					ptTimer->fnTimerCallBack(ptTimer->uiTimer_ID, ptTimer->pvUserPrivate);

					(void)JP_SignalLock(ptTimer->ptSignal);

					tCurrTime = JP_GetTime();

					tExp1 = tCurrTime - ptTimer->tNextAlramTime;

					while(tExp1 > ptTimer->tInterval)
					{
						ptTimer->tNextAlramTime += ptTimer->tInterval;

						tExp1 = tCurrTime - ptTimer->tNextAlramTime;
					}

					tExp2 = ptTimer->tInterval - tExp1;

					ptTimer->tNextAlramTime = tCurrTime + tExp2;

					tExp3 = ptTimer->tNextAlramTime - tCurrTime;

					if((tExp3 >= INT_MIN) && (tExp3 <= INT_MAX))
					{
						iDiffTime = (int32_t)tExp3;
						iDiffTime /= 1000;

						if(iDiffTime == 0)
						{
							iDiffTime = 10;
						}
					}
					else
					{
						eError = JP_Error_TypeCasting;
					}
				}

				if(eError == JP_Error_None)
				{
					eError = JP_SignalBareWait(ptTimer->ptSignal, iDiffTime);

					if(eError == JP_Error_Timeout)
					{
						eError = JP_Error_None;
					}
				}
			}
			else
			{
				eError = JP_SignalBareWait(ptTimer->ptSignal, -1);
			}

			(void)JP_SignalUnlock(ptTimer->ptSignal);

			if(eError != JP_Error_None)
			{
				break;
			}
		}
	}

	return NULL;
}

static JP_Error_E SF_CreateTimerThread(JP_Timer_T *ptTimer)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptTimer != NULL)
	{
		int iRetVal;

		iRetVal = pthread_create(&ptTimer->tThread, NULL, SF_TimerThread, ptTimer);

		if(iRetVal == 0)
		{
			ptTimer->bThreadCreated = JP_TRUE;
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

static JP_Error_E SF_DestroyTimerThread(JP_Timer_T *ptTimer)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptTimer != NULL)
	{
		if(ptTimer->bThreadCreated == JP_TRUE)
		{
			ptTimer->bKillMySelf = JP_TRUE;

			eRetErr = JP_SignalWakeUp(ptTimer->ptSignal);

			if(eRetErr == JP_Error_None)
			{
				int iRetVal;

				iRetVal = pthread_join(ptTimer->tThread, NULL);

				if(iRetVal != 0)
				{
					eRetErr = JP_Error_ReleaseResource;
				}
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_EnableTimer(JP_Timer_T *ptTimer, uint32_t uiTimer_ID, int32_t iMilliSecond, pfnTimerCallBack fnTimerCallBack, void *pvUserPrivate)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptTimer != NULL) && (iMilliSecond > 0) && (fnTimerCallBack != NULL))
	{
		(void)JP_SignalLock(ptTimer->ptSignal);

		ptTimer->bEnabled 			= JP_TRUE;
		ptTimer->uiTimer_ID 		= uiTimer_ID;
		ptTimer->tInterval 			= (JP_Time_T)iMilliSecond * 1000;
		ptTimer->tEnableTime 		= JP_GetTime();
		ptTimer->tNextAlramTime 	= 0;
		ptTimer->fnTimerCallBack 	= fnTimerCallBack;
		ptTimer->pvUserPrivate 		= pvUserPrivate;

		eRetErr = JP_SignalBareWakeUp(ptTimer->ptSignal);

		(void)JP_SignalUnlock(ptTimer->ptSignal);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_DisableTimer(JP_Timer_T *ptTimer)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptTimer != NULL)
	{
		if(ptTimer->bEnabled == JP_TRUE)
		{
			(void)JP_SignalLock(ptTimer->ptSignal);

			ptTimer->bEnabled 			= JP_FALSE;
			ptTimer->uiTimer_ID 		= 0U;
			ptTimer->tInterval 			= 0;
			ptTimer->tEnableTime 		= 0;
			ptTimer->tNextAlramTime 	= 0;

			eRetErr = JP_SignalBareWakeUp(ptTimer->ptSignal);

			(void)JP_SignalUnlock(ptTimer->ptSignal);
		}
		else
		{
			eRetErr = JP_Error_NotEnabled;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Timer_IF_T *JP_CreateTimer(void)
{
	JP_Timer_IF_T *ptRetTimer_IF;
	void* tmpAlloc = NULL;

	tmpAlloc = g_malloc0(sizeof(JP_Timer_IF_T));
	P_CAST(ptRetTimer_IF, tmpAlloc);

	if(ptRetTimer_IF != NULL)
	{
		tmpAlloc = g_malloc0(sizeof(JP_Timer_T));
		P_CAST(ptRetTimer_IF->hTimer, tmpAlloc);

		if(ptRetTimer_IF->hTimer != NULL)
		{
			JP_Error_E eError;

			JP_Timer_T *ptTimer = ptRetTimer_IF->hTimer;

			ptTimer->ptSignal = JP_CreateSignal();

			if(ptTimer->ptSignal != NULL)
			{
				ptTimer->tNextAlramTime = -1;

				eError = SF_CreateTimerThread(ptTimer);

				if(eError == JP_Error_None)
				{
					ptRetTimer_IF->EnableTimer = SF_EnableTimer;
					ptRetTimer_IF->DisableTimer = SF_DisableTimer;
				}
			}
			else
			{
				eError = JP_Error_InsufficientResources;
			}

			if(eError != JP_Error_None)
			{
				(void)JP_DestroyTimer(ptRetTimer_IF);
				ptRetTimer_IF = NULL;
			}
		}
		else
		{
			g_free(ptRetTimer_IF);
			ptRetTimer_IF = NULL;
		}
	}

	return ptRetTimer_IF;
}

JP_Error_E JP_DestroyTimer(JP_Timer_IF_T *ptTimer_IF)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptTimer_IF != NULL)
	{
		if(ptTimer_IF->hTimer != NULL)
		{
			JP_Timer_T *ptTimer = ptTimer_IF->hTimer;

			eRetErr = SF_DestroyTimerThread(ptTimer);

			(void)JP_DestroySignal(ptTimer->ptSignal);

			g_free(ptTimer);
		}

		g_free(ptTimer_IF);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}