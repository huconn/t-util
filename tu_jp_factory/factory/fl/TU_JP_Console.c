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
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <TU_JP_Lib_C_Std.h>

#include "TU_JP_Console.h"

typedef struct
{
	int32_t hDevice;
	int32_t iPollTimeout;

	struct pollfd tPoll;
} JP_Console_Uart_T;

struct JP_ConsoleSt_T
{
	JP_BOOL_E bThreadCreated;
	JP_BOOL_E bKillMySelf;

	void *pvPrivate;

	pfn_Console_CallBack fnCallBack;

	pthread_t tReceiveThread;

	JP_Console_Uart_T tUart;
};

static uint32_t SF_Divide(unsigned char *pucBuffer, char **ppucArgs, uint32_t uiArgsMax)
{
	uint32_t uiArgs = 0;

	char *pstrStart = pucBuffer;
	char *pchPos = pucBuffer;

	while(*pchPos != '\0')
	{
		if((*pchPos == ' ') || ((*pchPos == '\n') && (pstrStart != pchPos)))
		{
			*pchPos = '\0';

			if(uiArgs > uiArgsMax)
			{
				break;
			}

			ppucArgs[uiArgs] = pstrStart;

			uiArgs++;

			pstrStart = &pchPos[1U];
		}

		pchPos = &pchPos[1U];
	}

	return uiArgs;
}

static void *SF_ReceiveThread(void *pvPrivate)
{
	if(pvPrivate != NULL)
	{
		unsigned char ucBuffer[1024];

		JP_ConsoleSt_T *ptConsoleSt;

		P_CAST(ptConsoleSt, pvPrivate);

		JP_Console_Uart_T *ptUart = &ptConsoleSt->tUart;

		while(ptConsoleSt->bKillMySelf == JP_FALSE)
		{
			int32_t iRetVal;

			ssize_t tRetVal;

			JP_Error_E eError = JP_Error_None;

			JP_RetType_UInt32_T tU32;

			iRetVal = poll(&ptUart->tPoll, 1, ptUart->iPollTimeout);

			tU32 = JP_TypeCast_Int16ToUInt32(ptUart->tPoll.revents);

			if(tU32.eError == JP_Error_None)
			{
				if((iRetVal > 0) && ((tU32.uiRetVal & ((unsigned)POLLIN | (unsigned)POLLPRI)) != 0U))
				{
					(void)memset(ucBuffer, 0, 1024UL);

					tRetVal = read(ptUart->hDevice, &ucBuffer, 1024UL);

					if(tRetVal > 0)
					{
						ptConsoleSt->fnCallBack(ucBuffer, ptConsoleSt->pvPrivate);
					}
				}
			}
			else
			{
				eError = tU32.eError;
			}

			if(eError != JP_Error_None)
			{
				break;
			}

			(void)usleep(1000);
		}
	}

	return NULL;
}

static JP_Error_E SF_Start(JP_ConsoleSt_T *ptConsoleSt)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptConsoleSt != NULL)
	{
		int iRetVal;

		iRetVal = pthread_create(&ptConsoleSt->tReceiveThread, NULL, SF_ReceiveThread, ptConsoleSt);

		if(iRetVal == 0)
		{
			ptConsoleSt->bThreadCreated = JP_TRUE;
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

static JP_Error_E SF_Stop(JP_ConsoleSt_T *ptConsoleSt)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptConsoleSt != NULL)
	{
		if(ptConsoleSt->bThreadCreated == JP_TRUE)
		{
			int iRetVal;

			ptConsoleSt->bKillMySelf = JP_TRUE;

			iRetVal = pthread_join(ptConsoleSt->tReceiveThread, NULL);

			if(iRetVal != 0)
			{
				eRetErr = JP_Error_ReleaseResource;
			}

			ptConsoleSt->bThreadCreated = JP_FALSE;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Console_IF_T *JP_CreateConsole(const char *pcstrDev, pfn_Console_CallBack fnCallBack, void *pvPrivate)
{
	JP_Console_IF_T *ptRet_IF = NULL;
	void* tmpAlloc = NULL;

	if((pcstrDev != NULL) && (fnCallBack != NULL))
	{
		tmpAlloc = g_malloc0(sizeof(JP_Console_IF_T));
		P_CAST(ptRet_IF, tmpAlloc);

		if(ptRet_IF != NULL)
		{
			JP_Error_E eError = JP_Error_None;

			tmpAlloc = g_malloc0(sizeof(JP_ConsoleSt_T));
			P_CAST(ptRet_IF->ptConsoleSt, tmpAlloc);

			if(ptRet_IF->ptConsoleSt != NULL)
			{
				int32_t iFlags;

				JP_ConsoleSt_T *ptConsoleSt = ptRet_IF->ptConsoleSt;
				JP_Console_Uart_T *ptUart = &ptConsoleSt->tUart;

				ptConsoleSt->fnCallBack 	= fnCallBack;
				ptConsoleSt->pvPrivate 		= pvPrivate;

				iFlags = (int32_t)((unsigned)O_RDWR | (unsigned)O_NDELAY);

				ptUart->hDevice = open(pcstrDev, iFlags);

				if(ptUart->hDevice >= 0)
				{
					ptUart->iPollTimeout 	= 0;
					ptUart->tPoll.fd 		= ptUart->hDevice;
					ptUart->tPoll.events 	= (int16_t)((unsigned)POLLIN | (unsigned)POLLPRI);
					ptUart->tPoll.revents 	= 0;

					ptRet_IF->Start 	= SF_Start;
					ptRet_IF->Stop 		= SF_Stop;
					ptRet_IF->Divide 	= SF_Divide;
				}
				else
				{
					eError = JP_Error_BadParameter;
				}
			}
			else
			{
				eError = JP_Error_InsufficientResources;
			}

			if(eError != JP_Error_None)
			{
				(void)JP_DestroyConsole(ptRet_IF);
				ptRet_IF = NULL;
			}
		}
	}

	return ptRet_IF;
}

JP_Error_E JP_DestroyConsole(JP_Console_IF_T *pt_IF)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(pt_IF != NULL)
	{
		if(pt_IF->ptConsoleSt != NULL)
		{
			JP_ConsoleSt_T *ptConsoleSt = pt_IF->ptConsoleSt;

			const JP_Console_Uart_T *pctUart = &ptConsoleSt->tUart;

			eRetErr = SF_Stop(ptConsoleSt);

			if(pctUart->hDevice >= 0)
			{
				(void)close(pctUart->hDevice);
			}

			g_free(pt_IF->ptConsoleSt);
		}

		g_free(pt_IF);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}