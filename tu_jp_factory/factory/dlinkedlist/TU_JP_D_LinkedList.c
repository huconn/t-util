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

#include <TU_JP_Lib_C_Std.h>

#include "TU_JP_D_LinkedList.h"

struct JP_D_LinkedListItem_T
{
	char *pstrName;
	void *pvData;

	int32_t iDataSize;

	JP_D_LinkedListItem_T *ptPrev;
	JP_D_LinkedListItem_T *ptNext;
};

struct JP_D_LinkedList_T
{
	char *pstrName;
	void *pvData;

	int32_t iDataSize;

	JP_D_LinkedListItem_T *ptHead;
	JP_D_LinkedListItem_T *ptTail;

	JP_D_LinkedList_T *ptPrev;
	JP_D_LinkedList_T *ptNext;
};

struct JP_D_LinkedListSt_T
{
	JP_D_LinkedList_T *ptHead;
	JP_D_LinkedList_T *ptTail;
};

static JP_Error_E SF_Term_D_LinkedList(JP_D_LinkedListSt_T *ptListSt);
static JP_Error_E SF_Term_D_LinkedListItem(JP_D_LinkedList_T *ptList);
static JP_Error_E SF_DeleteList(JP_D_LinkedList_T *ptList);
static JP_Error_E SF_DeleteListName(JP_D_LinkedList_T *ptList);
static JP_Error_E SF_DeleteListData(JP_D_LinkedList_T *ptList);
static JP_Error_E SF_SetListName(JP_D_LinkedList_T *ptList, const char *pcstrName);
static JP_Error_E SF_SetListData(JP_D_LinkedList_T *ptList, void *pvData, int iDataSize);
static JP_Error_E SF_DeleteItemAll(const JP_D_LinkedList_T *pctList);
static JP_Error_E SF_DeleteItem(JP_D_LinkedListItem_T *ptItem);
static JP_Error_E SF_DeleteItemName(JP_D_LinkedListItem_T *ptItem);
static JP_Error_E SF_DeleteItemData(JP_D_LinkedListItem_T *ptItem);
static JP_Error_E SF_SetItemName(JP_D_LinkedListItem_T *ptItem, const char *pcstrName);
static JP_Error_E SF_SetItemData(JP_D_LinkedListItem_T *ptItem, void *pvData, int iDataSize);

static JP_Error_E SF_Init_D_LinkedList(JP_D_LinkedListSt_T *ptListSt)
{
	JP_Error_E eError = JP_Error_None;
	void* tmpAlloc = NULL;

	if(ptListSt != NULL)
	{
		if((ptListSt->ptHead == NULL) && (ptListSt->ptTail == NULL))
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_T));
			P_CAST(ptListSt->ptHead, tmpAlloc);

			if(ptListSt->ptHead != NULL)
			{
				tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_T));
				P_CAST(ptListSt->ptTail, tmpAlloc);

				if(ptListSt->ptTail != NULL)
				{
					ptListSt->ptHead->ptPrev = ptListSt->ptHead;
					ptListSt->ptHead->ptNext = ptListSt->ptTail;

					ptListSt->ptTail->ptPrev = ptListSt->ptHead;
					ptListSt->ptTail->ptNext = ptListSt->ptTail;
				}
				else
				{
					(void)SF_Term_D_LinkedList(ptListSt);

					eError = JP_Error_InsufficientResources;
				}
			}
			else
			{
				(void)SF_Term_D_LinkedList(ptListSt);

				eError = JP_Error_InsufficientResources;
			}
		}
		else
		{
			eError = JP_Error_BadParameter;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_Init_D_LinkedListItem(JP_D_LinkedList_T *ptList)
{
	JP_Error_E eError = JP_Error_None;
	void* tmpAlloc = NULL;

	if(ptList != NULL)
	{
		if((ptList->ptHead == NULL) && (ptList->ptTail == NULL))
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListItem_T));
			P_CAST(ptList->ptHead, tmpAlloc);

			if(ptList->ptHead != NULL)
			{
				tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListItem_T));
				P_CAST(ptList->ptTail, tmpAlloc);

				if(ptList->ptTail != NULL)
				{
					ptList->ptHead->ptPrev = ptList->ptHead;
					ptList->ptHead->ptNext = ptList->ptTail;

					ptList->ptTail->ptPrev = ptList->ptHead;
					ptList->ptTail->ptNext = ptList->ptTail;
				}
				else
				{
					(void)SF_Term_D_LinkedListItem(ptList);

					eError = JP_Error_InsufficientResources;
				}
			}
			else
			{
				(void)SF_Term_D_LinkedListItem(ptList);

				eError = JP_Error_InsufficientResources;
			}
		}
		else
		{
			eError = JP_Error_BadParameter;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_Term_D_LinkedList(JP_D_LinkedListSt_T *ptListSt)
{
	JP_Error_E eError = JP_Error_None;

	if(ptListSt != NULL)
	{
		if(ptListSt->ptHead != NULL)
		{
			g_free(ptListSt->ptHead);
			ptListSt->ptHead = NULL;
		}

		if(ptListSt->ptTail != NULL)
		{
			g_free(ptListSt->ptTail);
			ptListSt->ptTail = NULL;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_Term_D_LinkedListItem(JP_D_LinkedList_T *ptList)
{
	JP_Error_E eError = JP_Error_None;

	if(ptList != NULL)
	{
		if(ptList->ptHead != NULL)
		{
			g_free(ptList->ptHead);
			ptList->ptHead = NULL;
		}

		if(ptList->ptTail != NULL)
		{
			g_free(ptList->ptTail);
			ptList->ptTail = NULL;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_BOOL_E SF_IsListHead(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList)
{
	JP_BOOL_E bIsListHead = JP_FALSE;

	if((pctListSt != NULL) && (pctList != NULL))
	{
		bIsListHead = (pctList == pctListSt->ptHead) ? JP_TRUE : JP_FALSE;
	}

	return bIsListHead;
}

static JP_BOOL_E SF_IsListTail(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList)
{
	JP_BOOL_E bIsListTail = JP_FALSE;

	if((pctListSt != NULL) && (pctList != NULL))
	{
		bIsListTail = (pctList == pctListSt->ptTail) ? JP_TRUE : JP_FALSE;
	}

	return bIsListTail;
}

static JP_D_LinkedList_T *SF_PrevListHandle(const JP_D_LinkedList_T *pctList)
{
	JP_D_LinkedList_T *ptPrevList = NULL;

	if(pctList != NULL)
	{
		ptPrevList = pctList->ptPrev;
	}

	return ptPrevList;
}

static JP_D_LinkedList_T *SF_NextListHandle(const JP_D_LinkedList_T *pctList)
{
	JP_D_LinkedList_T *ptNextList = NULL;

	if(pctList != NULL)
	{
		ptNextList = pctList->ptNext;
	}

	return ptNextList;
}

static JP_BOOL_E SF_IsItemHead(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem)
{
	JP_BOOL_E bItemHead = JP_FALSE;

	if((pctList != NULL) && (pctItem != NULL))
	{
		bItemHead = (pctItem == pctList->ptHead) ? JP_TRUE : JP_FALSE;
	}

	return bItemHead;
}

static JP_BOOL_E SF_IsItemTail(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem)
{
	JP_BOOL_E bItemTail = JP_FALSE;

	if((pctList != NULL) && (pctItem != NULL))
	{
		bItemTail = (pctItem == pctList->ptTail) ? JP_TRUE : JP_FALSE;
	}

	return bItemTail;
}

static JP_D_LinkedListItem_T *SF_PrevItemHandle(const JP_D_LinkedListItem_T *pctItem)
{
	JP_D_LinkedListItem_T *ptPrevItem = NULL;

	if(pctItem != NULL)
	{
		ptPrevItem = pctItem->ptPrev;
	}

	return ptPrevItem;
}

static JP_D_LinkedListItem_T *SF_NextItemHandle(const JP_D_LinkedListItem_T *pctItem)
{
	JP_D_LinkedListItem_T *ptNextItem = NULL;

	if(pctItem != NULL)
	{
		ptNextItem = pctItem->ptNext;
	}

	return ptNextItem;
}

static JP_D_LinkedList_T *SF_InsertList(const JP_D_LinkedListSt_T *pctListSt, const char *pcstrName, void *pvData, int iDataSize)
{
	JP_D_LinkedList_T *ptNewList = NULL;
	void* tmpAlloc = NULL;

	if(pctListSt != NULL)
	{
		if(pctListSt->ptTail != NULL)
		{
			JP_D_LinkedList_T *ptOldList = pctListSt->ptTail;

			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_T));
			P_CAST(ptNewList, tmpAlloc);

			if(ptNewList != NULL)
			{
				JP_Error_E eError;

				ptOldList->ptPrev->ptNext	= ptNewList;
				ptNewList->ptPrev			= ptOldList->ptPrev;
				ptOldList->ptPrev			= ptNewList;
				ptNewList->ptNext			= ptOldList;

				if (pcstrName != NULL)
				{
					eError = SF_SetListName(ptNewList, pcstrName);
				}
				else
				{
					eError = JP_Error_None;
				}

				if(eError == JP_Error_None)
				{
					if (pvData != NULL)
					{
						eError = SF_SetListData(ptNewList, pvData, iDataSize);
					}
					else
					{
						eError = JP_Error_None;
					}

					if(eError == JP_Error_None)
					{
						eError = SF_Init_D_LinkedListItem(ptNewList);

						if(eError != JP_Error_None)
						{
							(void)SF_DeleteList(ptNewList);
							ptNewList = NULL;
						}
					}
					else
					{
						(void)SF_DeleteList(ptNewList);
						ptNewList = NULL;
					}
				}
				else
				{
					(void)SF_DeleteList(ptNewList);
					ptNewList = NULL;
				}
			}
		}
	}

	return ptNewList;
}

static JP_D_LinkedList_T *SF_InsertListTo(
	JP_D_LinkedList_T *ptBaseList,
	JP_ListInsertType_E eInsertType,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedList_T *ptNewList = NULL;
	void* tmpAlloc = NULL;

	if((ptBaseList != NULL)
		&& ((eInsertType == JP_LIST_INSERT_TYPE_BEFORE)
			|| (eInsertType == JP_LIST_INSERT_TYPE_AFTER)))
	{
		if(eInsertType == JP_LIST_INSERT_TYPE_BEFORE)
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_T));
			P_CAST(ptNewList, tmpAlloc);

			if(ptNewList != NULL)
			{
				ptBaseList->ptPrev->ptNext	= ptNewList;
				ptNewList->ptPrev			= ptBaseList->ptPrev;
				ptBaseList->ptPrev			= ptNewList;
				ptNewList->ptNext			= ptBaseList;
			}
		}
		else	// JP_LIST_INSERT_TYPE_AFTER
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_T));
			P_CAST(ptNewList, tmpAlloc);

			if(ptNewList != NULL)
			{
				ptBaseList->ptNext->ptPrev	= ptNewList;
				ptNewList->ptNext			= ptBaseList->ptNext;
				ptBaseList->ptNext			= ptNewList;
				ptNewList->ptPrev			= ptBaseList;
			}
		}

		if(ptNewList != NULL)
		{
			JP_Error_E eError;

			eError = SF_SetListName(ptNewList, pcstrName);

			if(eError == JP_Error_None)
			{
				eError = SF_SetListData(ptNewList, pvData, iDataSize);

				if(eError == JP_Error_None)
				{
					eError = SF_Init_D_LinkedListItem(ptNewList);

					if(eError != JP_Error_None)
					{
						(void)SF_DeleteList(ptNewList);
						ptNewList = NULL;
					}
				}
				else
				{
					(void)SF_DeleteList(ptNewList);
					ptNewList = NULL;
				}
			}
			else
			{
				(void)SF_DeleteList(ptNewList);
				ptNewList = NULL;
			}
		}
	}

	return ptNewList;
}

static JP_Error_E SF_DeleteListAll(const JP_D_LinkedListSt_T *pctListSt)
{
	JP_Error_E eError = JP_Error_None;

	if(pctListSt != NULL)
	{
		JP_D_LinkedList_T *ptList = pctListSt->ptHead->ptNext;

		while(ptList != pctListSt->ptTail)
		{
			JP_D_LinkedList_T *ptTempList;

			eError = SF_DeleteItemAll(ptList);

			if(eError == JP_Error_None)
			{
				ptList->ptPrev->ptNext = ptList->ptNext;
				ptList->ptNext->ptPrev = ptList->ptPrev;

				ptTempList	= ptList;
				ptList		= ptList->ptNext;

				eError = SF_Term_D_LinkedListItem(ptTempList);

				if(eError == JP_Error_None)
				{
					eError = SF_DeleteListName(ptTempList);

					if(eError == JP_Error_None)
					{
						eError = SF_DeleteListData(ptTempList);

						if(eError == JP_Error_None)
						{
							g_free(ptTempList);
						}
					}
				}
			}

			if(eError != JP_Error_None)
			{
				break;
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteList(JP_D_LinkedList_T *ptList)
{
	JP_Error_E eError = JP_Error_None;

	if(ptList != NULL)
	{
		eError = SF_DeleteItemAll(ptList);

		if(eError == JP_Error_None)
		{
			eError = SF_Term_D_LinkedListItem(ptList);

			if(eError == JP_Error_None)
			{
				ptList->ptPrev->ptNext = ptList->ptNext;
				ptList->ptNext->ptPrev = ptList->ptPrev;

				eError = SF_DeleteListName(ptList);

				if(eError == JP_Error_None)
				{
					eError = SF_DeleteListData(ptList);

					if(eError == JP_Error_None)
					{
						g_free(ptList);
					}
				}
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteListName(JP_D_LinkedList_T *ptList)
{
	JP_Error_E eError = JP_Error_None;

	if(ptList != NULL)
	{
		if(ptList->pstrName != NULL)
		{
			g_free(ptList->pstrName);
			ptList->pstrName = NULL;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteListData(JP_D_LinkedList_T *ptList)
{
	JP_Error_E eError = JP_Error_None;

	if(ptList != NULL)
	{
		if(ptList->pvData != NULL)
		{
			if(ptList->iDataSize == -1)
			{
				ptList->pvData = NULL;
			}
			else
			{
				g_free(ptList->pvData);
				ptList->pvData = NULL;
			}

			ptList->iDataSize = 0;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static uint32_t SF_GetListCount(const JP_D_LinkedListSt_T *pctListSt)
{
	uint32_t uiListCount = 0;

	if(pctListSt != NULL)
	{
		const JP_D_LinkedList_T *pctNextList;

		pctNextList = SF_NextListHandle(pctListSt->ptHead);

		while(SF_IsListTail(pctListSt, pctNextList) == JP_FALSE)
		{
			uint32_t uiOperands[2];

			JP_Error_E eError;

			uiOperands[0] = uiListCount;
			uiOperands[1] = 1U;

			eError = JP_Add_UInt32(2U, uiOperands, &uiListCount);

			if(eError == JP_Error_None)
			{
				pctNextList = SF_NextListHandle(pctNextList);
			}
			else
			{
				uiListCount = 0;
				break;
			}
		}
	}

	return uiListCount;
}

static JP_D_LinkedList_T *SF_GetFirstList(const JP_D_LinkedListSt_T *pctListSt)
{
	JP_D_LinkedList_T *ptFirstList = NULL;

	if(pctListSt != NULL)
	{
		JP_D_LinkedList_T *ptNextList;

		ptNextList = SF_NextListHandle(pctListSt->ptHead);

		if(SF_IsListTail(pctListSt, ptNextList) == JP_FALSE)
		{
			ptFirstList = ptNextList;
		}
	}

	return ptFirstList;
}

static JP_D_LinkedList_T *SF_GetPrevList(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList)
{
	JP_D_LinkedList_T *ptPrevList = NULL;

	if((pctListSt != NULL) && (pctList != NULL))
	{
		ptPrevList = SF_PrevListHandle(pctList);

		if(SF_IsListHead(pctListSt, ptPrevList) == JP_TRUE)
		{
			ptPrevList = NULL;
		}
	}

	return ptPrevList;
}

static JP_D_LinkedList_T *SF_GetNextList(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList)
{
	JP_D_LinkedList_T *ptNextList = NULL;

	if((pctListSt != NULL) && (pctList != NULL))
	{
		ptNextList = SF_NextListHandle(pctList);

		if(SF_IsListTail(pctListSt, ptNextList) == JP_TRUE)
		{
			ptNextList = NULL;
		}
	}

	return ptNextList;
}

static JP_Error_E SF_SetListName(JP_D_LinkedList_T *ptList, const char *pcstrName)
{
	JP_Error_E eError = JP_Error_None;
	void* tmpAlloc = NULL;

	if((ptList != NULL) && (pcstrName != NULL))
	{
		eError = SF_DeleteListName(ptList);

		if(eError == JP_Error_None)
		{
			uint64_t ulTotalStrLen = 0UL;
			uint64_t ulOperands[2];

			size_t tStrLen;

			tStrLen = strlen(pcstrName);

			ulOperands[0] = tStrLen;
			ulOperands[1] = 1UL;

			eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

			if(eError == JP_Error_None)
			{
				tmpAlloc = g_malloc0(ulTotalStrLen);
				P_CAST(ptList->pstrName, tmpAlloc);

				if(ptList->pstrName != NULL)
				{
					(void)memcpy(ptList->pstrName, pcstrName, tStrLen);

					ptList->pstrName[tStrLen] = '\0';
				}
				else
				{
					eError = JP_Error_InsufficientResources;
				}
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_SetListData(JP_D_LinkedList_T *ptList, void *pvData, int iDataSize)
{
	JP_Error_E eError = JP_Error_None;

	if((ptList != NULL) && (pvData != NULL)
		&& ((iDataSize == -1) || (iDataSize > 0)))
	{
		eError = SF_DeleteListData(ptList);

		if(eError == JP_Error_None)
		{
			if(iDataSize > 0)
			{
				JP_RetType_UInt64_T tRet_I32ToU64;

				tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

				if(tRet_I32ToU64.eError == JP_Error_None)
				{
					size_t tDataSize = tRet_I32ToU64.ulRetVal;

					ptList->pvData = g_malloc0(tDataSize);

					if(ptList->pvData != NULL)
					{
						(void)memcpy(ptList->pvData, pvData, tDataSize);

						ptList->iDataSize = iDataSize;
					}
					else
					{
						eError = JP_Error_InsufficientResources;
					}
				}
				else
				{
					eError = tRet_I32ToU64.eError;
				}
			}
			else
			{
				ptList->pvData = pvData;

				ptList->iDataSize = iDataSize;
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static char *SF_GetListName(const JP_D_LinkedList_T *pctList)
{
	char *pstrName = NULL;

	if(pctList != NULL)
	{
		pstrName = pctList->pstrName;
	}

	return pstrName;
}

static void *SF_GetListData(const JP_D_LinkedList_T *pctList)
{
	void *pvData = NULL;

	if(pctList != NULL)
	{
		pvData = pctList->pvData;
	}

	return pvData;
}

static int SF_GetListDataSize(const JP_D_LinkedList_T *pctList)
{
	int iDataSize = 0;

	if(pctList != NULL)
	{
		iDataSize = pctList->iDataSize;
	}

	return iDataSize;
}

static JP_D_LinkedList_T *SF_FindList(const JP_D_LinkedListSt_T *pctListSt, const char *pcstrName)
{
	JP_D_LinkedList_T *ptNextList = NULL;

	if((pctListSt != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptNextList = SF_NextListHandle(pctListSt->ptHead);

		while(SF_IsListTail(pctListSt, ptNextList) == JP_FALSE)
		{
			const char *pcstrListName = SF_GetListName(ptNextList);

			if(pcstrListName != NULL)
			{
				if(strcmp(pcstrName, pcstrListName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptNextList = SF_NextListHandle(ptNextList);
		}

		if(bFound == JP_FALSE)
		{
			ptNextList = NULL;
		}
	}

	return ptNextList;
}

static JP_D_LinkedList_T *SF_FindPrevList(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList, const char *pcstrName)
{
	JP_D_LinkedList_T *ptPrevList = NULL;

	if((pctListSt != NULL) && (pctList != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptPrevList = SF_PrevListHandle(pctList);

		while(SF_IsListHead(pctListSt, ptPrevList) == JP_FALSE)
		{
			const char *pcstrListName = SF_GetListName(ptPrevList);

			if(pcstrListName != NULL)
			{
				if(strcmp(pcstrName, pcstrListName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptPrevList = SF_PrevListHandle(ptPrevList);
		}

		if(bFound == JP_FALSE)
		{
			ptPrevList = NULL;
		}
	}

	return ptPrevList;
}

static JP_D_LinkedList_T *SF_FindNextList(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList, const char *pcstrName)
{
	JP_D_LinkedList_T *ptNextList = NULL;

	if((pctListSt != NULL) && (pctList != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptNextList = SF_NextListHandle(pctList);

		while(SF_IsListTail(pctListSt, ptNextList) == JP_FALSE)
		{
			const char *pcstrListName = SF_GetListName(ptNextList);

			if(pcstrListName != NULL)
			{
				if(strcmp(pcstrName, pcstrListName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptNextList = SF_NextListHandle(ptNextList);
		}

		if(bFound == JP_FALSE)
		{
			ptNextList = NULL;
		}
	}

	return ptNextList;
}

static JP_D_LinkedListItem_T *SF_InsertItem(
	const JP_D_LinkedList_T *pctList,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedListItem_T *ptNewItem = NULL;
	void* tmpAlloc = NULL;

	if(pctList != NULL)
	{
		JP_D_LinkedListItem_T *ptOldItem = pctList->ptTail;

		tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListItem_T));
		P_CAST(ptNewItem, tmpAlloc);

		if(ptNewItem != NULL)
		{
			JP_Error_E eError = JP_Error_None;

			ptOldItem->ptPrev->ptNext	= ptNewItem;
			ptNewItem->ptPrev			= ptOldItem->ptPrev;
			ptOldItem->ptPrev			= ptNewItem;
			ptNewItem->ptNext			= ptOldItem;

			if(pcstrName != NULL)
			{
				eError = SF_SetItemName(ptNewItem, pcstrName);
			}

			if(eError == JP_Error_None)
			{
				if (pvData != NULL)
				{
					eError = SF_SetItemData(ptNewItem, pvData, iDataSize);
				}
				else
				{
					eError = JP_Error_None;
				}

				if(eError != JP_Error_None)
				{
					(void)SF_DeleteItem(ptNewItem);
					ptNewItem = NULL;
				}
			}
			else
			{
				(void)SF_DeleteItem(ptNewItem);
				ptNewItem = NULL;
			}
		}
	}

	return ptNewItem;
}

static JP_D_LinkedListItem_T *SF_InsertItemTo(
	JP_D_LinkedListItem_T *ptBaseItem,
	JP_ListInsertType_E eInsertType,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedListItem_T *ptNewItem = NULL;
	void* tmpAlloc = NULL;

	if((ptBaseItem != NULL)
		&& ((eInsertType == JP_LIST_INSERT_TYPE_BEFORE)
			|| (eInsertType == JP_LIST_INSERT_TYPE_AFTER)))
	{
		if(eInsertType == JP_LIST_INSERT_TYPE_BEFORE)
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListItem_T));
			P_CAST(ptNewItem, tmpAlloc);

			if(ptNewItem != NULL)
			{
				ptBaseItem->ptPrev->ptNext	= ptNewItem;
				ptNewItem->ptPrev			= ptBaseItem->ptPrev;
				ptBaseItem->ptPrev			= ptNewItem;
				ptNewItem->ptNext			= ptBaseItem;
			}
		}
		else	// JP_LIST_INSERT_TYPE_AFTER
		{
			tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListItem_T));
			P_CAST(ptNewItem, tmpAlloc);

			if(ptNewItem != NULL)
			{
				ptBaseItem->ptNext->ptPrev	= ptNewItem;
				ptNewItem->ptNext			= ptBaseItem->ptNext;
				ptBaseItem->ptNext			= ptNewItem;
				ptNewItem->ptPrev			= ptBaseItem;
			}
		}

		if(ptNewItem != NULL)
		{
			JP_Error_E eError;

			eError = SF_SetItemName(ptNewItem, pcstrName);

			if(eError == JP_Error_None)
			{
				eError = SF_SetItemData(ptNewItem, pvData, iDataSize);

				if(eError != JP_Error_None)
				{
					(void)SF_DeleteItem(ptNewItem);
					ptNewItem = NULL;
				}
			}
			else
			{
				(void)SF_DeleteItem(ptNewItem);
				ptNewItem = NULL;
			}
		}
	}

	return ptNewItem;
}

static JP_Error_E SF_DeleteItemAll(const JP_D_LinkedList_T *pctList)
{
	JP_Error_E eError = JP_Error_None;

	if(pctList != NULL)
	{
		if (pctList->ptHead != NULL)
		{
			JP_D_LinkedListItem_T *ptItem = pctList->ptHead->ptNext;

			while(ptItem != pctList->ptTail)
			{
				JP_D_LinkedListItem_T *ptTempItem;

				ptItem->ptPrev->ptNext = ptItem->ptNext;
				ptItem->ptNext->ptPrev = ptItem->ptPrev;

				ptTempItem	= ptItem;
				ptItem		= ptItem->ptNext;

				eError = SF_DeleteItemName(ptTempItem);

				if(eError == JP_Error_None)
				{
					eError = SF_DeleteItemData(ptTempItem);

					if(eError == JP_Error_None)
					{
						g_free(ptTempItem);
					}
				}

				if(eError != JP_Error_None)
				{
					break;
				}
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteItem(JP_D_LinkedListItem_T *ptItem)
{
	JP_Error_E eError = JP_Error_None;

	if(ptItem != NULL)
	{
		ptItem->ptPrev->ptNext = ptItem->ptNext;
		ptItem->ptNext->ptPrev = ptItem->ptPrev;

		eError = SF_DeleteItemName(ptItem);

		if(eError == JP_Error_None)
		{
			eError = SF_DeleteItemData(ptItem);

			if(eError == JP_Error_None)
			{
				g_free(ptItem);
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteItemName(JP_D_LinkedListItem_T *ptItem)
{
	JP_Error_E eError = JP_Error_None;

	if(ptItem != NULL)
	{
		if(ptItem->pstrName != NULL)
		{
			g_free(ptItem->pstrName);
			ptItem->pstrName = NULL;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteItemData(JP_D_LinkedListItem_T *ptItem)
{
	JP_Error_E eError = JP_Error_None;

	if(ptItem != NULL)
	{
		if(ptItem->pvData != NULL)
		{
			if(ptItem->iDataSize == -1)
			{
				ptItem->pvData = NULL;
			}
			else
			{
				g_free(ptItem->pvData);
				ptItem->pvData = NULL;
			}

			ptItem->iDataSize = 0;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static uint32_t SF_GetItemCount(const JP_D_LinkedList_T *pctList)
{
	uint32_t uiItemCount = 0;

	if(pctList != NULL)
	{
		const JP_D_LinkedListItem_T *ptNextItem;

		ptNextItem = SF_NextItemHandle(pctList->ptHead);

		while(SF_IsItemTail(pctList, ptNextItem) == JP_FALSE)
		{
			uint32_t uiOperands[2];

			JP_Error_E eError;

			uiOperands[0] = uiItemCount;
			uiOperands[1] = 1U;

			eError = JP_Add_UInt32(2U, uiOperands, &uiItemCount);

			if(eError == JP_Error_None)
			{
				ptNextItem = SF_NextItemHandle(ptNextItem);
			}
			else
			{
				uiItemCount = 0;
				break;
			}
		}
	}

	return uiItemCount;
}

static JP_D_LinkedListItem_T *SF_GetFirstItem(const JP_D_LinkedList_T *pctList)
{
	JP_D_LinkedListItem_T *ptFirstItem = NULL;

	if(pctList != NULL)
	{
		JP_D_LinkedListItem_T *ptNextItem;

		ptNextItem = SF_NextItemHandle(pctList->ptHead);

		if(SF_IsItemTail(pctList, ptNextItem) == JP_FALSE)
		{
			ptFirstItem = ptNextItem;
		}
	}

	return ptFirstItem;
}

static JP_D_LinkedListItem_T *SF_GetPrevItem(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem)
{
	JP_D_LinkedListItem_T *ptPrevItem = NULL;

	if((pctList != NULL) && (pctItem != NULL))
	{
		ptPrevItem = SF_PrevItemHandle(pctItem);

		if(SF_IsItemHead(pctList, ptPrevItem) == JP_TRUE)
		{
			ptPrevItem = NULL;
		}
	}

	return ptPrevItem;
}

static JP_D_LinkedListItem_T *SF_GetNextItem(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem)
{
	JP_D_LinkedListItem_T *ptNextItem = NULL;

	if((pctList != NULL) && (pctItem != NULL))
	{
		ptNextItem = SF_NextItemHandle(pctItem);

		if(SF_IsItemTail(pctList, ptNextItem) == JP_TRUE)
		{
			ptNextItem = NULL;
		}
	}

	return ptNextItem;
}

static JP_Error_E SF_SetItemName(JP_D_LinkedListItem_T *ptItem, const char *pcstrName)
{
	JP_Error_E eError = JP_Error_None;
	void* tmpAlloc = NULL;

	if((ptItem != NULL) && (pcstrName != NULL))
	{
		eError = SF_DeleteItemName(ptItem);

		if(eError == JP_Error_None)
		{
			uint64_t ulTotalStrLen = 0UL;
			uint64_t ulOperands[2];

			size_t tStrLen;

			tStrLen = strlen(pcstrName);

			ulOperands[0] = tStrLen;
			ulOperands[1] = 1UL;

			eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

			if(eError == JP_Error_None)
			{
				tmpAlloc = g_malloc0(ulTotalStrLen);
				P_CAST(ptItem->pstrName, tmpAlloc);

				if(ptItem->pstrName != NULL)
				{
					(void)memcpy(ptItem->pstrName, pcstrName, tStrLen);

					ptItem->pstrName[tStrLen] = '\0';
				}
				else
				{
					eError = JP_Error_InsufficientResources;
				}
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_SetItemData(JP_D_LinkedListItem_T *ptItem, void *pvData, int iDataSize)
{
	JP_Error_E eError = JP_Error_None;

	if((ptItem != NULL) && (pvData != NULL)
		&& ((iDataSize == -1) || (iDataSize > 0)))
	{
		eError = SF_DeleteItemData(ptItem);

		if(eError == JP_Error_None)
		{
			if(iDataSize > 0)
			{
				JP_RetType_UInt64_T tRet_I32ToU64;

				tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

				if(tRet_I32ToU64.eError == JP_Error_None)
				{
					size_t tDataSize = tRet_I32ToU64.ulRetVal;

					ptItem->pvData = g_malloc0(tDataSize);

					if(ptItem->pvData != NULL)
					{
						(void)memcpy(ptItem->pvData, pvData, tDataSize);

						ptItem->iDataSize = iDataSize;
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
			}
			else
			{
				ptItem->pvData = pvData;

				ptItem->iDataSize = iDataSize;
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static char *SF_GetItemName(const JP_D_LinkedListItem_T *pctItem)
{
	char *pstrName = NULL;

	if(pctItem != NULL)
	{
		pstrName = pctItem->pstrName;
	}

	return pstrName;
}

static void *SF_GetItemData(const JP_D_LinkedListItem_T *pctItem)
{
	void *pvData = NULL;

	if(pctItem != NULL)
	{
		pvData = pctItem->pvData;
	}

	return pvData;
}

static int SF_GetItemDataSize(const JP_D_LinkedListItem_T *pctItem)
{
	int iDataSize = 0;

	if(pctItem != NULL)
	{
		iDataSize = pctItem->iDataSize;
	}

	return iDataSize;
}

static JP_D_LinkedListItem_T *SF_FindItem(const JP_D_LinkedList_T *pctList, const char *pcstrName)
{
	JP_D_LinkedListItem_T *ptNextItem = NULL;

	if((pctList != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptNextItem = SF_NextItemHandle(pctList->ptHead);

		while(SF_IsItemTail(pctList, ptNextItem) == JP_FALSE)
		{
			const char *pcstrItemName = SF_GetItemName(ptNextItem);

			if(pcstrItemName != NULL)
			{
				if(strcmp(pcstrName, pcstrItemName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptNextItem = SF_NextItemHandle(ptNextItem);
		}

		if(bFound == JP_FALSE)
		{
			ptNextItem = NULL;
		}
	}

	return ptNextItem;
}

static JP_D_LinkedListItem_T *SF_FindPrevItem(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem, const char *pcstrName)
{
	JP_D_LinkedListItem_T *ptPrevItem = NULL;

	if((pctList != NULL) && (pctItem != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptPrevItem = SF_PrevItemHandle(pctItem);

		while(SF_IsItemHead(pctList, ptPrevItem) == JP_FALSE)
		{
			const char *pcstrItemName = SF_GetItemName(ptPrevItem);

			if(pcstrItemName != NULL)
			{
				if(strcmp(pcstrName, pcstrItemName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptPrevItem = SF_PrevItemHandle(ptPrevItem);
		}

		if(bFound == JP_FALSE)
		{
			ptPrevItem = NULL;
		}
	}

	return ptPrevItem;
}

static JP_D_LinkedListItem_T *SF_FindNextItem(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem, const char *pcstrName)
{
	JP_D_LinkedListItem_T *ptNextItem = NULL;

	if((pctList != NULL) && (pctItem != NULL) && (pcstrName != NULL))
	{
		JP_BOOL_E bFound = JP_FALSE;

		ptNextItem = SF_NextItemHandle(pctItem);

		while(SF_IsItemTail(pctList, ptNextItem) == JP_FALSE)
		{
			const char *pcstrItemName = SF_GetItemName(ptNextItem);

			if(pcstrItemName != NULL)
			{
				if(strcmp(pcstrName, pcstrItemName) == 0)
				{
					bFound = JP_TRUE;
					break;
				}
			}

			ptNextItem = SF_NextItemHandle(ptNextItem);
		}

		if(bFound == JP_FALSE)
		{
			ptNextItem = NULL;
		}
	}

	return ptNextItem;
}

static JP_D_LinkedList_T *SF_GraftList(const JP_D_LinkedList_IF_T *pct_GL, const JP_D_LinkedList_T *pctGlList)
{
	JP_D_LinkedList_T *ptRetList = NULL;

	if(pct_GL != NULL)
	{
		uint32_t i;
		uint32_t uiGlListCount;

		if(pctGlList == NULL)
		{
			uiGlListCount = pct_GL->GetListCount(pct_GL->ptListSt);
		}
		else
		{
			uiGlListCount = 1U;
		}

		for(i = 0U; i < uiGlListCount; i++)
		{
			uint32_t j;
			uint32_t uiGlItemCount;

			const JP_D_LinkedList_T *pctNextGlList = NULL;

			JP_D_LinkedList_T *ptInsertList;

			if(i == 0U)
			{
				if(pctGlList == NULL)
				{
					pctNextGlList = pct_GL->GetFirstList(pct_GL->ptListSt);
				}
				else
				{
					pctNextGlList = pctGlList;
				}
			}
			else
			{
				pctNextGlList = pct_GL->GetNextList(pct_GL->ptListSt, pctNextGlList);
			}

			ptInsertList = SF_InsertList(pct_GL->ptListSt, pct_GL->GetListName(pctNextGlList), pct_GL->GetListData(pctNextGlList), pct_GL->GetListDataSize(pctNextGlList));

			if(i == 0U)
			{
				ptRetList = ptInsertList;
			}

			uiGlItemCount = pct_GL->GetItemCount(pctNextGlList);

			for(j = 0U;  j <uiGlItemCount; j++)
			{
				const JP_D_LinkedListItem_T *pctNextGlItem = NULL;

				if(j == 0U)
				{
					pctNextGlItem = pct_GL->GetFirstItem(pctNextGlList);
				}
				else
				{
					pctNextGlItem = pct_GL->GetNextItem(pctNextGlList, pctNextGlItem);
				}

				(void)SF_InsertItem(ptInsertList, pct_GL->GetItemName(pctNextGlItem), pct_GL->GetItemData(pctNextGlItem), pct_GL->GetItemDataSize(pctNextGlItem));
			}
		}
	}

	return ptRetList;
}

static JP_D_LinkedList_T *SF_GraftListTo(
	JP_D_LinkedList_T *ptBaseList,
	JP_ListInsertType_E eInsertType,
	const JP_D_LinkedList_IF_T *pct_GL,
	const JP_D_LinkedList_T *pctGlList)
{
	JP_D_LinkedList_T *ptRetList = NULL;

	if(ptBaseList != NULL)
	{
		uint32_t i;
		uint32_t uiGlListCount;

		if(pctGlList == NULL)
		{
			uiGlListCount = pct_GL->GetListCount(pct_GL->ptListSt);
		}
		else
		{
			uiGlListCount = 1U;
		}

		for(i = 0U; i < uiGlListCount; i++)
		{
			uint32_t j;
			uint32_t uiGlItemCount;

			JP_D_LinkedList_T *ptInsertList = NULL;

			const JP_D_LinkedList_T *pctNextGlList = NULL;

			if(i == 0U)
			{
				if(pctGlList == NULL)
				{
					pctNextGlList = pct_GL->GetFirstList(pct_GL->ptListSt);
				}
				else
				{
					pctNextGlList = pctGlList;
				}
			}
			else
			{
				pctNextGlList = pct_GL->GetNextList(pct_GL->ptListSt, pctNextGlList);
			}

			if(i == 0U)
			{
				ptInsertList = SF_InsertListTo(
					ptBaseList,
					eInsertType,
					pct_GL->GetListName(pctNextGlList),
					pct_GL->GetListData(pctNextGlList),
					pct_GL->GetListDataSize(pctNextGlList));

				ptRetList = ptInsertList;
			}
			else
			{
				ptInsertList = SF_InsertListTo(
					ptInsertList,
					JP_LIST_INSERT_TYPE_AFTER,
					pct_GL->GetListName(pctNextGlList),
					pct_GL->GetListData(pctNextGlList),
					pct_GL->GetListDataSize(pctNextGlList));
			}

			uiGlItemCount = pct_GL->GetItemCount(pctNextGlList);

			for(j = 0U; j < uiGlItemCount; j++)
			{
				const JP_D_LinkedListItem_T *pctNextGlItem = NULL;

				if(j == 0U)
				{
					pctNextGlItem = pct_GL->GetFirstItem(pctNextGlList);
				}
				else
				{
					pctNextGlItem = pct_GL->GetNextItem(pctNextGlList, pctNextGlItem);
				}

				(void)SF_InsertItem(ptInsertList, pct_GL->GetItemName(pctNextGlItem), pct_GL->GetItemData(pctNextGlItem), pct_GL->GetItemDataSize(pctNextGlItem));
			}
		}
	}

	return ptRetList;
}

JP_D_LinkedList_IF_T *JP_CreateList(void)
{
	JP_D_LinkedList_IF_T *ptList_IF;
	void* tmpAlloc = NULL;

	tmpAlloc = g_malloc0(sizeof(JP_D_LinkedList_IF_T));
	P_CAST(ptList_IF, tmpAlloc);

	if(ptList_IF != NULL)
	{
		tmpAlloc = g_malloc0(sizeof(JP_D_LinkedListSt_T));
		P_CAST(ptList_IF->ptListSt, tmpAlloc);

		if(ptList_IF->ptListSt != NULL)
		{
			JP_Error_E eError;

			eError = SF_Init_D_LinkedList(ptList_IF->ptListSt);

			if(eError == JP_Error_None)
			{
				ptList_IF->InsertList 		= SF_InsertList;
				ptList_IF->InsertListTo 	= SF_InsertListTo;
				ptList_IF->DeleteListAll 	= SF_DeleteListAll;
				ptList_IF->DeleteList 		= SF_DeleteList;
				ptList_IF->DeleteListName 	= SF_DeleteListName;
				ptList_IF->DeleteListData 	= SF_DeleteListData;
				ptList_IF->GetListCount 	= SF_GetListCount;
				ptList_IF->GetFirstList 	= SF_GetFirstList;
				ptList_IF->GetPrevList 		= SF_GetPrevList;
				ptList_IF->GetNextList 		= SF_GetNextList;
				ptList_IF->SetListName 		= SF_SetListName;
				ptList_IF->SetListData 		= SF_SetListData;
				ptList_IF->GetListName 		= SF_GetListName;
				ptList_IF->GetListData 		= SF_GetListData;
				ptList_IF->GetListDataSize 	= SF_GetListDataSize;
				ptList_IF->FindList 		= SF_FindList;
				ptList_IF->FindPrevList 	= SF_FindPrevList;
				ptList_IF->FindNextList 	= SF_FindNextList;
				ptList_IF->InsertItem 		= SF_InsertItem;
				ptList_IF->InsertItemTo 	= SF_InsertItemTo;
				ptList_IF->DeleteItemAll 	= SF_DeleteItemAll;
				ptList_IF->DeleteItem 		= SF_DeleteItem;
				ptList_IF->DeleteItemName 	= SF_DeleteItemName;
				ptList_IF->DeleteItemData 	= SF_DeleteItemData;
				ptList_IF->GetItemCount 	= SF_GetItemCount;
				ptList_IF->GetFirstItem 	= SF_GetFirstItem;
				ptList_IF->GetPrevItem 		= SF_GetPrevItem;
				ptList_IF->GetNextItem 		= SF_GetNextItem;
				ptList_IF->SetItemName 		= SF_SetItemName;
				ptList_IF->SetItemData 		= SF_SetItemData;
				ptList_IF->GetItemName 		= SF_GetItemName;
				ptList_IF->GetItemData 		= SF_GetItemData;
				ptList_IF->GetItemDataSize 	= SF_GetItemDataSize;
				ptList_IF->FindItem 		= SF_FindItem;
				ptList_IF->FindPrevItem 	= SF_FindPrevItem;
				ptList_IF->FindNextItem 	= SF_FindNextItem;
				ptList_IF->GraftList 		= SF_GraftList;
				ptList_IF->GraftListTo 		= SF_GraftListTo;
			}
			else
			{
				g_free(ptList_IF->ptListSt);
				g_free(ptList_IF);
				ptList_IF = NULL;
			}
		}
		else
		{
			g_free(ptList_IF);
			ptList_IF = NULL;
		}

	}

	return ptList_IF;
}

JP_Error_E JP_DestroyList(JP_D_LinkedList_IF_T *ptList_IF)
{
	JP_Error_E eError = JP_Error_None;

	if(ptList_IF != NULL)
	{
		if(ptList_IF->ptListSt != NULL)
		{
			eError = SF_DeleteListAll(ptList_IF->ptListSt);

			if(eError == JP_Error_None)
			{
				eError = SF_Term_D_LinkedList(ptList_IF->ptListSt);

				if(eError == JP_Error_None)
				{
					g_free(ptList_IF->ptListSt);
				}
			}
		}

		if(eError == JP_Error_None)
		{
			g_free(ptList_IF);
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}
