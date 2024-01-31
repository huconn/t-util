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
#include <glib.h>
#include <glib/gprintf.h>

#include <TU_JP_CommonSt.h>
#include <TU_JP_Lib_C_Std.h>
#include <TU_JP_Lib_C_D_LinkedList.h>

#include "TU_JP_Message.h"

typedef struct
{
	int iRetVal;

	JP_Signal_T *ptSignal;
	JP_Message_T tMessage;
} JP_SendMessage_T;

struct JP_MessageSt_T
{
	JP_BOOL_E bThreadCreated;
	JP_BOOL_E bKillMySelf;

	pthread_t tThead;

	JP_Signal_T *ptSignal;

	JP_D_LinkedList_IF_T *ptMessageList_IF;

	pfnMessageHandler fnMessageHandler;
	pfnMessageFlushHandler fnMessageFlushHandler;
};

static JP_RetType_Int32_T SF_SendMessage(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int32_t iParam, void *pvParam)
{
	JP_RetType_Int32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.iRetVal = 0;

	if(pctMessageSt != NULL)
	{
		JP_D_LinkedList_T *ptMessageList;

		JP_SendMessage_T tSendMessage;

		const JP_D_LinkedList_IF_T *pctMessageList_IF = pctMessageSt->ptMessageList_IF;

		tSendMessage.ptSignal = JP_CreateSignal();

		if(tSendMessage.ptSignal != NULL)
		{
			const JP_D_LinkedList_T *pctInsertedList;

			tSendMessage.iRetVal = -1;

			tSendMessage.tMessage.uiMessage = uiMessage;
			tSendMessage.tMessage.iParam 	= iParam;
			tSendMessage.tMessage.pvParam 	= pvParam;

			(void)JP_SignalLock(pctMessageSt->ptSignal);

			ptMessageList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);

			if(ptMessageList == NULL)
			{
				pctInsertedList = pctMessageList_IF->InsertList(pctMessageList_IF->ptListSt, "Send", &tSendMessage, -1);
			}
			else
			{
				pctInsertedList = pctMessageList_IF->InsertListTo(ptMessageList, JP_LIST_INSERT_TYPE_BEFORE, "Send", &tSendMessage, -1);
			}

			if(pctInsertedList != NULL)
			{
				(void)JP_SignalBareWakeUp(pctMessageSt->ptSignal);

				// Send Message Signal Lock
				(void)JP_SignalUnlock(pctMessageSt->ptSignal);

				(void)JP_SignalLock(tSendMessage.ptSignal);

				(void)JP_SignalBareWait(tSendMessage.ptSignal, -1);

				tRetVal.iRetVal = tSendMessage.iRetVal;

				// Send Message Signal Unlock
				(void)JP_SignalUnlock(tSendMessage.ptSignal);
			}
			else
			{
				(void)JP_SignalUnlock(pctMessageSt->ptSignal);

				tRetVal.eError = JP_Error_InsufficientResources;
			}
			(void)JP_DestroySignal(tSendMessage.ptSignal);
		}
		else
		{
			tRetVal.eError = JP_Error_InsufficientResources;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_Error_E SF_PostMessage(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int iParam, void *pvParam)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(pctMessageSt != NULL)
	{
		JP_Message_T tMessage;

		const JP_D_LinkedList_T *pctInsertedList;

		const JP_D_LinkedList_IF_T *pctMessageList = pctMessageSt->ptMessageList_IF;

		tMessage.uiMessage 	= uiMessage;
		tMessage.iParam 	= iParam;
		tMessage.pvParam 	= pvParam;

		(void)JP_SignalLock(pctMessageSt->ptSignal);

		pctInsertedList = pctMessageList->InsertList(pctMessageList->ptListSt, "Post", &tMessage, sizeof(JP_Message_T));

		if(pctInsertedList != NULL)
		{
			(void)JP_SignalBareWakeUp(pctMessageSt->ptSignal);
		}
		else
		{
			eRetErr = JP_Error_InsufficientResources;
		}

		(void)JP_SignalUnlock(pctMessageSt->ptSignal);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_PostMessageEx(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int iParam, void *pvParam, uint32_t uiOption)
{
	JP_Error_E eRetErr = JP_Error_None;
	void* tmpData = NULL;

	if(pctMessageSt != NULL)
	{
		JP_Message_T tMessage;

		const JP_D_LinkedList_T *pctInsertedList;

		const JP_D_LinkedList_IF_T *pctMessageList_IF = pctMessageSt->ptMessageList_IF;

		tMessage.uiMessage 	= uiMessage;
		tMessage.iParam 	= iParam;
		tMessage.pvParam 	= pvParam;

		(void)JP_SignalLock(pctMessageSt->ptSignal);

		if((uiOption & JP_MSG_OPT_REMOVE_DUPLICATED) == JP_MSG_OPT_REMOVE_DUPLICATED)
		{
			JP_D_LinkedList_T *ptMessageList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);

			while(ptMessageList != NULL)
			{
				JP_Message_T *pctMessage = NULL;
				tmpData = pctMessageList_IF->GetListData(ptMessageList);
				P_CAST(pctMessage, tmpData);

				if(pctMessage->uiMessage == uiMessage)
				{
					JP_D_LinkedList_T *ptTempList = pctMessageList_IF->GetPrevList(pctMessageList_IF->ptListSt, ptMessageList);

					(void)pctMessageList_IF->DeleteList(ptMessageList);
					ptMessageList = ptTempList;
				}

				ptMessageList = pctMessageList_IF->GetNextList(pctMessageList_IF->ptListSt, ptMessageList);
			}
		}

		if((uiOption & JP_MSG_OPT_PUSH_FIRST) == JP_MSG_OPT_PUSH_FIRST)
		{
			JP_D_LinkedList_T *ptFirstList;

			ptFirstList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);

			if(ptFirstList == NULL)
			{
				pctInsertedList = pctMessageList_IF->InsertList(pctMessageList_IF->ptListSt, "Post", &tMessage, sizeof(JP_Message_T));
			}
			else
			{
				pctInsertedList = pctMessageList_IF->InsertListTo(ptFirstList, JP_LIST_INSERT_TYPE_BEFORE, "Post", &tMessage, sizeof(JP_Message_T));
			}
		}
		else
		{
			pctInsertedList = pctMessageList_IF->InsertList(pctMessageList_IF->ptListSt, "Post", &tMessage, sizeof(JP_Message_T));
		}

		if(pctInsertedList != NULL)
		{
			(void)JP_SignalBareWakeUp(pctMessageSt->ptSignal);
		}
		else
		{
			eRetErr = JP_Error_InsufficientResources;
		}

		(void)JP_SignalUnlock(pctMessageSt->ptSignal);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_ProcessSendMessage(const JP_MessageSt_T *pctMessageSt, JP_SendMessage_T *ptSendMessage)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((pctMessageSt != NULL) && (ptSendMessage != NULL))
	{
		ptSendMessage->iRetVal = -1;

		if(pctMessageSt->fnMessageHandler != NULL)
		{
			ptSendMessage->iRetVal = pctMessageSt->fnMessageHandler(&ptSendMessage->tMessage);
		}

		(void)JP_SignalWakeUp(ptSendMessage->ptSignal);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_ProcessPostMessage(const JP_MessageSt_T *pctMessageSt, const JP_Message_T *ptMessage)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((pctMessageSt != NULL) && (ptMessage != NULL))
	{
		if(pctMessageSt->fnMessageHandler != NULL)
		{
			(void)pctMessageSt->fnMessageHandler(ptMessage);
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static void SF_FlushMessage(const JP_MessageSt_T *pctMessageSt)
{
	void* tmpData = NULL;

	if(pctMessageSt != NULL)
	{
		JP_D_LinkedList_T *ptMessageList;

		const JP_D_LinkedList_IF_T *pctMessageList_IF = pctMessageSt->ptMessageList_IF;

		ptMessageList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);

		while(ptMessageList != NULL)
		{
			const char *pcstrMessageType;

			pcstrMessageType = pctMessageList_IF->GetListName(ptMessageList);

			if(pcstrMessageType != NULL)
			{
				JP_Message_T *ptMessage;

				if(strcmp(pcstrMessageType, "Send") == 0)
				{
					JP_SendMessage_T *ptSendMessage;
					tmpData = pctMessageList_IF->GetListData(ptMessageList);
					P_CAST(ptSendMessage, tmpData);

					if(ptSendMessage != NULL)
					{
						ptMessage = &ptSendMessage->tMessage;

						pctMessageSt->fnMessageFlushHandler(&ptSendMessage->tMessage);

						ptSendMessage->iRetVal = -1;

						(void)JP_SignalWakeUp(ptSendMessage->ptSignal);
					}
				}
				else if(strcmp(pcstrMessageType, "Post") == 0)
				{
					tmpData = pctMessageList_IF->GetListData(ptMessageList);
					P_CAST(ptMessage, tmpData);

					if(ptMessage != NULL)
					{
						pctMessageSt->fnMessageFlushHandler(ptMessage);
					}
				}
				else
				{
					;
				}
			}

			(void)pctMessageList_IF->DeleteList(ptMessageList);

			ptMessageList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);
		}
	}
}

static unsigned int SF_GetMessageCount(const JP_MessageSt_T *pctMessageSt)
{
	unsigned int uiListCount = 0u;

	if(pctMessageSt != NULL)
	{
		const JP_D_LinkedList_IF_T *pctMessageList_IF = pctMessageSt->ptMessageList_IF;

		(void)JP_SignalLock(pctMessageSt->ptSignal);

		uiListCount = pctMessageList_IF->GetListCount(pctMessageList_IF->ptListSt);

		(void)JP_SignalUnlock(pctMessageSt->ptSignal);
	}

	return uiListCount;
}

static JP_Error_E SF_MessageHandleMsg(const JP_D_LinkedList_IF_T *pctMessageList_IF,
	 const JP_D_LinkedList_T *ptMessageList, const JP_MessageSt_T *pctMessageSt)
{
	JP_Error_E eError = JP_Error_None;
	JP_Message_T tMessage;
	const char *pstrMessageType = pctMessageList_IF->GetListName(ptMessageList);

	if(pstrMessageType != NULL)
	{
		if(strcmp(pstrMessageType, "Post") == 0)
		{
			(void)memcpy(&tMessage, pctMessageList_IF->GetListData(ptMessageList), sizeof(JP_Message_T));

			eError = SF_ProcessPostMessage(pctMessageSt, &tMessage);
		}
		else if(strcmp(pstrMessageType, "Send") == 0)
		{
			if(ptMessageList != NULL)
			{
				JP_SendMessage_T* ptSendMessage;
				void* tmpData = pctMessageList_IF->GetListData(ptMessageList);
				P_CAST(ptSendMessage, tmpData);

				if (ptSendMessage != NULL)
				{
					eError = SF_ProcessSendMessage(pctMessageSt, ptSendMessage);
				}
			}
		}
		else
		{
			// Do nothing
		}
	}
	else
	{
		eError = JP_Error_InsufficientResources;
	}
	return eError;
}

static void *SF_MessageThread(void *pvPrivate)
{
	if(pvPrivate != NULL)
	{
		JP_MessageSt_T *pctMessageSt;
		const JP_D_LinkedList_IF_T *pctMessageList_IF;

		P_CAST(pctMessageSt, pvPrivate);
		pctMessageList_IF = pctMessageSt->ptMessageList_IF;

		while(pctMessageSt->bKillMySelf == JP_FALSE)
		{
			JP_D_LinkedList_T *ptMessageList;

			(void)JP_SignalLock(pctMessageSt->ptSignal);

			if(pctMessageList_IF->GetListCount(pctMessageList_IF->ptListSt) == 0U)
			{
				(void)JP_SignalBareWait(pctMessageSt->ptSignal, -1);
			}
			(void)JP_SignalUnlock(pctMessageSt->ptSignal);
			ptMessageList = pctMessageList_IF->GetFirstList(pctMessageList_IF->ptListSt);

			if(ptMessageList != NULL)
			{
				(void)SF_MessageHandleMsg(pctMessageList_IF, ptMessageList, pctMessageSt);
				(void)pctMessageList_IF->DeleteList(ptMessageList);
			}
		}

		(void)SF_FlushMessage(pctMessageSt);
	}

	return NULL;
}

static JP_Error_E SF_CreateMessageThread(JP_MessageSt_T *ptMessageSt)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptMessageSt != NULL)
	{
		int iRetVal;

		iRetVal = pthread_create(&ptMessageSt->tThead, NULL, SF_MessageThread, ptMessageSt);

		if(iRetVal == 0)
		{
			ptMessageSt->bThreadCreated = JP_TRUE;
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

static JP_Error_E SF_DestroyMessageThread(JP_MessageSt_T *ptMessageSt)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptMessageSt != NULL)
	{
		if(ptMessageSt->bThreadCreated == JP_TRUE)
		{
			int iRetVal;

			ptMessageSt->bKillMySelf = JP_TRUE;

			(void)JP_SignalWakeUp(ptMessageSt->ptSignal);

			iRetVal = pthread_join(ptMessageSt->tThead, NULL);

			if(iRetVal != 0)
			{
				eRetErr = JP_Error_ReleaseResource;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Message_IF_T *JP_CreateMessage(pfnMessageHandler fnMessageHandler, pfnMessageFlushHandler fnMessageFlushHandler)
{
	JP_Message_IF_T *ptRet_IF;
	void* tmpAlloc = NULL;

	tmpAlloc = g_malloc0(sizeof(JP_Message_IF_T));
	P_CAST(ptRet_IF, tmpAlloc);

	if(ptRet_IF != NULL)
	{
		JP_Error_E eError;

		tmpAlloc = g_malloc0(sizeof(JP_MessageSt_T));
		P_CAST(ptRet_IF->ptMessageSt, tmpAlloc);

		if(ptRet_IF->ptMessageSt != NULL)
		{
			JP_MessageSt_T *ptMessageSt = ptRet_IF->ptMessageSt;

			ptMessageSt->bKillMySelf = JP_FALSE;

			ptMessageSt->ptSignal 			= JP_CreateSignal();
			ptMessageSt->ptMessageList_IF 	= JP_CreateList();

			if((ptMessageSt->ptSignal != NULL) && (ptMessageSt->ptMessageList_IF != NULL))
			{
				ptMessageSt->fnMessageHandler 		= fnMessageHandler;
				ptMessageSt->fnMessageFlushHandler 	= fnMessageFlushHandler;

				ptRet_IF->SendMessage 		= SF_SendMessage;
				ptRet_IF->PostMessage 		= SF_PostMessage;
				ptRet_IF->PostMessageEx 	= SF_PostMessageEx;
				ptRet_IF->FlushMessage 		= SF_FlushMessage;
				ptRet_IF->GetMessageCount	= SF_GetMessageCount;
				eError = SF_CreateMessageThread(ptMessageSt);
			}
			else
			{
				eError = JP_Error_InsufficientResources;
			}
		}
		else
		{
			eError = JP_Error_InsufficientResources;
		}

		if(eError != JP_Error_None)
		{
			(void)JP_DestroyMessage(ptRet_IF);
			ptRet_IF = NULL;
		}
	}

	return ptRet_IF;
}

JP_Error_E JP_DestroyMessage(JP_Message_IF_T *pt_IF)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(pt_IF != NULL)
	{
		if(pt_IF->ptMessageSt != NULL)
		{
			JP_MessageSt_T *ptMessageSt = pt_IF->ptMessageSt;

			eRetErr = SF_DestroyMessageThread(ptMessageSt);

			(void)JP_DestroySignal(ptMessageSt->ptSignal);
			(void)JP_DestroyList(ptMessageSt->ptMessageList_IF);

			g_free(pt_IF->ptMessageSt);
		}

		g_free(pt_IF);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}