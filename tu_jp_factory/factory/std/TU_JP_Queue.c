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
#include <stdint.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "TU_JP_CommonSt.h"
#include "TU_JP_ArithmeticOperation.h"
#include "TU_JP_Semaphore.h"

#include "TU_JP_Queue.h"

#define P_CAST(tar, src) (void)memcpy((void*)&(tar), (void*)&(src), sizeof(void*))

typedef struct
{
	void *pvData;
} ST_Element_T;

struct JP_Queue_T
{
	uint32_t uiQueueSize;
	uint32_t uiQueueSizePlusOne;
	uint32_t uiReadPos;
	uint32_t uiWritePos;

	JP_Semaphore_T *ptSemaphore;

	ST_Element_T *ptElements;
};

uint32_t JP_GetQueueCount(const JP_Queue_T *ptQueue)
{
	uint32_t uiRetQueueCount = 0;

	if(ptQueue != NULL)
	{
		if(ptQueue->uiReadPos != ptQueue->uiWritePos)
		{
			if(ptQueue->uiReadPos > ptQueue->uiWritePos)
			{
				uiRetQueueCount = ptQueue->uiQueueSizePlusOne - (ptQueue->uiReadPos - ptQueue->uiWritePos);
			}
			else
			{
				uiRetQueueCount = ptQueue->uiWritePos - ptQueue->uiReadPos;
			}
		}
	}

	return uiRetQueueCount;
}

JP_Error_E JP_PushQueue(JP_Queue_T *ptQueue, void *pvData)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptQueue != NULL) && (pvData != NULL))
	{
		(void)JP_ObtainSemaphore(ptQueue->ptSemaphore, -1);

		if(JP_GetQueueCount(ptQueue) != ptQueue->uiQueueSize)
		{
			ST_Element_T *ptElement = &ptQueue->ptElements[ptQueue->uiWritePos];

			ptElement->pvData = pvData;

			ptQueue->uiWritePos++;
			ptQueue->uiWritePos %= ptQueue->uiQueueSizePlusOne;
		}
		else
		{
			eRetErr = JP_Error_QueueFull;
		}

		(void)JP_ReleaseSemaphore(ptQueue->ptSemaphore);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_PopQueue(JP_Queue_T *ptQueue, void **ppvData)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptQueue != NULL) && (ppvData != NULL))
	{
		*ppvData = NULL;

		(void)JP_ObtainSemaphore(ptQueue->ptSemaphore, -1);

		if(JP_GetQueueCount(ptQueue) > 0U)
		{
			ST_Element_T *ptElement = &ptQueue->ptElements[ptQueue->uiReadPos];

			*ppvData = ptElement->pvData;

			ptElement->pvData = NULL;

			ptQueue->uiReadPos++;
			ptQueue->uiReadPos %= ptQueue->uiQueueSizePlusOne;
		}
		else
		{
			eRetErr = JP_Error_QueueEmpty;
		}

		(void)JP_ReleaseSemaphore(ptQueue->ptSemaphore);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Queue_T *JP_CreateQueue(uint32_t uiQueueSize)
{
	JP_Queue_T *ptQueue = NULL;
	void* tmpAlloc = NULL;

	if(uiQueueSize > 0U)
	{
		tmpAlloc = g_malloc0(sizeof(JP_Queue_T));
		P_CAST(ptQueue, tmpAlloc);

		if(ptQueue != NULL)
		{
			ptQueue->uiQueueSize 			= uiQueueSize;
			ptQueue->uiQueueSizePlusOne 	= uiQueueSize + 1U;
			ptQueue->uiReadPos 				= 0U;
			ptQueue->uiWritePos 			= 0U;

			ptQueue->ptSemaphore = JP_CreateSemaphore(1U, 1U);

			if(ptQueue->ptSemaphore != NULL)
			{
				tmpAlloc = g_malloc0(ptQueue->uiQueueSizePlusOne * sizeof(ST_Element_T));
				P_CAST(ptQueue->ptElements, tmpAlloc);

				if(ptQueue->ptElements != NULL)
				{
					(void)memset(ptQueue->ptElements, 0, ptQueue->uiQueueSizePlusOne * sizeof(ST_Element_T));
				}
				else
				{
					(void)JP_DestroySemaphore(ptQueue->ptSemaphore);

					g_free(ptQueue);
					ptQueue = NULL;
				}
			}
			else
			{
				g_free(ptQueue);
				ptQueue = NULL;
			}
		}
	}

	return ptQueue;
}

JP_Error_E JP_DestroyQueue(JP_Queue_T *ptQueue)
{
	JP_Error_E eError = JP_Error_None;

	if(ptQueue != NULL)
	{
		eError = JP_DestroySemaphore(ptQueue->ptSemaphore);

		if(eError != JP_Error_None)
		{
			eError = JP_Error_ReleaseResource;
		}

		g_free(ptQueue->ptElements);
		g_free(ptQueue);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}