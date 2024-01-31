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

#include <TU_JP_Lib_C_Std.h>
#include <TU_JP_Lib_C_D_LinkedList.h>

#include "TU_JP_D_LinkedTree.h"

typedef struct
{
	void *pvData;
	int iDataSize;

	JP_D_LinkedTree_T *ptPrev;
	JP_D_LinkedTree_T *ptNext;
} JP_D_LinkedTreeData_T;

static JP_Error_E SF_DeleteNode(JP_D_LinkedTreeSt_T *ptTreeSt, JP_D_LinkedTree_T *ptNode);
static JP_Error_E SF_DeleteNodeData(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
static uint32_t SF_GetChildNodeCount(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
static JP_D_LinkedTree_T *SF_GetChildNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

static JP_D_LinkedTree_T *SF_GraftNodeToChild(
	JP_D_LinkedTreeSt_T *ptTreeSt,
	JP_D_LinkedTree_T *ptNode,
	JP_D_LinkedTree_IF_T *pt_GT,
	const JP_D_LinkedTree_T *pct_GT_Node);

static JP_D_LinkedTree_T *FindLastNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTree_T *ptLastNode = NULL;

	if(pctTreeSt != NULL)
	{
		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		if(pctNode != NULL)
		{
			ptLastNode = pctList_IF->GetNextList(pctList_IF->ptListSt, pctNode);

			if(ptLastNode != NULL)
			{
				pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);

				if(pctTreeData->ptPrev == NULL)
				{
					while(pctTreeData->ptNext != NULL)
					{
						ptLastNode = pctTreeData->ptNext;

						pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);
					}
				}
				else
				{
					ptLastNode = NULL;
				}
			}
		}
		else
		{
			ptLastNode = pctList_IF->GetFirstList(pctList_IF->ptListSt);

			if(ptLastNode != NULL)
			{
				pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);

				while(pctTreeData->ptNext != NULL)
				{
					ptLastNode = pctTreeData->ptNext;

					pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);
				}
			}
		}
	}

	return ptLastNode;
}

static JP_D_LinkedTree_T *SF_InsertNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const char *pcstrName, void *pvData, int iDataSize)
{
	JP_D_LinkedTree_T *ptInsertNode = NULL;

	if(pctTreeSt != NULL)
	{
		JP_D_LinkedTree_T *ptLastNode;

		JP_D_LinkedTreeData_T *ptTreeData;

		ptLastNode = FindLastNode(pctTreeSt, NULL);

		ptTreeData = (JP_D_LinkedTreeData_T *)calloc(1UL, sizeof(JP_D_LinkedTreeData_T));

		if(ptTreeData != NULL)
		{
			if(pvData != NULL)
			{
				if(iDataSize > 0)
				{
					JP_RetType_UInt64_T tRet_I32ToU64;

					tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

					if(tRet_I32ToU64.eError == JP_Error_None)
					{
						size_t tDataSize = tRet_I32ToU64.ulRetVal;

						ptTreeData->pvData = calloc(1UL, tDataSize);

						if(ptTreeData->pvData != NULL)
						{
							(void)memcpy(ptTreeData->pvData, pvData, tDataSize);

							ptTreeData->iDataSize = iDataSize;
						}
						else
						{
							free(ptTreeData);
							ptTreeData = NULL;
						}
					}
					else
					{
						free(ptTreeData);
						ptTreeData = NULL;
					}
				}
				else
				{
					ptTreeData->pvData = pvData;

					ptTreeData->iDataSize = iDataSize;
				}
			}

			if(ptTreeData != NULL)
			{
				const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

				ptTreeData->ptPrev = ptLastNode;

				ptInsertNode = pctList_IF->InsertList(pctList_IF->ptListSt, pcstrName, (void *)ptTreeData, -1);

				if(ptInsertNode != NULL)
				{
					if(ptLastNode != NULL)
					{
						ptTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);

						ptTreeData->ptNext = ptInsertNode;
					}
				}
				else
				{
					free(ptTreeData);
				}
			}
		}
	}

	return ptInsertNode;
}

static JP_D_LinkedTree_T *SF_InsertNodeToChild(
	const JP_D_LinkedTreeSt_T *pctTreeSt,
	JP_D_LinkedTree_T *ptNode,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedTree_T *ptInsertNode = NULL;

	if(pctTreeSt != NULL)
	{
		JP_D_LinkedTree_T *ptLastNode;

		JP_D_LinkedTreeData_T *ptTreeData;

		ptLastNode = FindLastNode(pctTreeSt, ptNode);

		ptTreeData = (JP_D_LinkedTreeData_T *)calloc(1UL, sizeof(JP_D_LinkedTreeData_T));

		if(ptTreeData != NULL)
		{
			if(pvData != NULL)
			{
				if(iDataSize > 0)
				{
					JP_RetType_UInt64_T tRet_I32ToU64;

					tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

					if(tRet_I32ToU64.eError == JP_Error_None)
					{
						size_t tDataSize = tRet_I32ToU64.ulRetVal;

						ptTreeData->pvData = calloc(1UL, tDataSize);

						if(ptTreeData->pvData != NULL)
						{
							(void)memcpy(ptTreeData->pvData, pvData, tDataSize);

							ptTreeData->iDataSize = iDataSize;
						}
						else
						{
							free(ptTreeData);
							ptTreeData = NULL;
						}
					}
					else
					{
						free(ptTreeData);
						ptTreeData = NULL;
					}
				}
				else
				{
					ptTreeData->pvData = pvData;

					ptTreeData->iDataSize = iDataSize;
				}
			}

			if(ptTreeData != NULL)
			{
				const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

				if(ptNode != NULL)
				{
					if(ptLastNode != NULL)
					{
						JP_D_LinkedTree_T *ptChildNode;

						ptTreeData->ptPrev = ptLastNode;

						do
						{
							ptChildNode = ptLastNode;

							ptLastNode = FindLastNode(pctTreeSt, ptChildNode);
						} while(ptLastNode != NULL);

						ptInsertNode = pctList_IF->InsertListTo(ptChildNode, JP_TREE_INSERT_TYPE_AFTER, pcstrName, (void *)ptTreeData, -1);

						if(ptInsertNode != NULL)
						{
							ptTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptTreeData->ptPrev);

							ptTreeData->ptNext = ptInsertNode;
						}
					}
					else
					{
						ptInsertNode = pctList_IF->InsertListTo(ptNode, JP_TREE_INSERT_TYPE_AFTER, pcstrName, (void *)ptTreeData, -1);
					}

					if(ptInsertNode == NULL)
					{
						free(ptTreeData);
					}
				}
				else
				{
					ptTreeData->ptPrev = ptLastNode;

					ptInsertNode = pctList_IF->InsertList(pctList_IF->ptListSt, pcstrName, (void *)ptTreeData, -1);

					if(ptInsertNode != NULL)
					{
						if(ptLastNode != NULL)
						{
							ptTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptLastNode);

							ptTreeData->ptNext = ptInsertNode;
						}
					}
					else
					{
						free(ptTreeData);
					}
				}
			}
		}
	}

	return ptInsertNode;
}

static JP_D_LinkedTree_T *SF_InsertNodeToSibling(
	const JP_D_LinkedTreeSt_T *pctTreeSt,
	JP_D_LinkedTree_T *ptBaseNode,
	JP_TreeInsertType_E eInsertType,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedTree_T *ptInsertNode = NULL;

	if((pctTreeSt != NULL) && (ptBaseNode != NULL)
		&& ((eInsertType == JP_TREE_INSERT_TYPE_BEFORE)
			|| (eInsertType == JP_TREE_INSERT_TYPE_AFTER)))
	{
		JP_D_LinkedTreeData_T *ptTreeData;

		ptTreeData = (JP_D_LinkedTreeData_T *)calloc(1UL, sizeof(JP_D_LinkedTreeData_T));

		if(ptTreeData != NULL)
		{
			if(pvData != NULL)
			{
				if(iDataSize > 0)
				{
					JP_RetType_UInt64_T tRet_I32ToU64;

					tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

					if(tRet_I32ToU64.eError == JP_Error_None)
					{
						size_t tDataSize = tRet_I32ToU64.ulRetVal;

						ptTreeData->pvData = calloc(1UL, tDataSize);

						if(ptTreeData->pvData != NULL)
						{
							(void)memcpy(ptTreeData->pvData, pvData, tDataSize);

							ptTreeData->iDataSize = iDataSize;
						}
						else
						{
							free(ptTreeData);
							ptTreeData = NULL;
						}
					}
					else
					{
						free(ptTreeData);
						ptTreeData = NULL;
					}
				}
				else
				{
					ptTreeData->pvData = pvData;

					ptTreeData->iDataSize = iDataSize;
				}
			}

			if(ptTreeData != NULL)
			{
				JP_D_LinkedTreeData_T *ptTempTreeData;

				const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

				if(eInsertType == JP_TREE_INSERT_TYPE_BEFORE)
				{
					ptTreeData->ptNext = ptBaseNode;

					ptInsertNode = pctList_IF->InsertListTo(ptBaseNode, JP_TREE_INSERT_TYPE_BEFORE, pcstrName, (void *)ptTreeData, -1);

					if(ptInsertNode != NULL)
					{
						ptTempTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptBaseNode);

						if(ptTempTreeData->ptPrev != NULL)
						{
							ptTreeData->ptPrev = ptTempTreeData->ptPrev;

							ptTempTreeData->ptPrev = ptInsertNode;

							ptTempTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptTreeData->ptPrev);

							ptTempTreeData->ptNext = ptInsertNode;
						}
						else
						{
							ptTempTreeData->ptPrev = ptInsertNode;
						}
					}
					else
					{
						free(ptTreeData);
					}
				}
				else	// JP_TREE_INSERT_TYPE_AFTER
				{
					ptTreeData->ptPrev = ptBaseNode;

					ptTempTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptBaseNode);

					if(ptTempTreeData->ptNext != NULL)
					{
						ptTreeData->ptNext = ptTempTreeData->ptNext;

						ptInsertNode = pctList_IF->InsertListTo(ptTempTreeData->ptNext, JP_TREE_INSERT_TYPE_BEFORE, pcstrName, (void *)ptTreeData, -1);

						if(ptInsertNode != NULL)
						{
							ptTempTreeData->ptNext = ptInsertNode;

							ptTempTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptTreeData->ptNext);

							ptTempTreeData->ptPrev = ptInsertNode;
						}
						else
						{
							free(ptTreeData);
						}
					}
					else
					{
						JP_D_LinkedTree_T *ptChildNode;
						JP_D_LinkedTree_T *ptLastNode = ptBaseNode;

						do
						{
							ptChildNode = ptLastNode;

							ptLastNode = FindLastNode(pctTreeSt, ptChildNode);
						} while(ptLastNode != NULL);

						ptInsertNode = pctList_IF->InsertListTo(ptChildNode, JP_TREE_INSERT_TYPE_AFTER, pcstrName, (void *)ptTreeData, -1);

						if(ptInsertNode != NULL)
						{
							ptTempTreeData->ptNext = ptInsertNode;
						}
						else
						{
							free(ptTreeData);
						}
					}
				}
			}
		}
	}

	return ptInsertNode;
}

static JP_Error_E SF_DeleteNodeAll(JP_D_LinkedTreeSt_T *ptTreeSt)
{
	JP_Error_E eError = JP_Error_None;

	if(ptTreeSt != NULL)
	{
		JP_D_LinkedTree_T *ptNextNode;

		const JP_D_LinkedList_IF_T *ptList_IF = ptTreeSt;

		ptNextNode = ptList_IF->GetFirstList(ptList_IF->ptListSt);

		while(ptNextNode != NULL)
		{
			eError = SF_DeleteNode(ptTreeSt, ptNextNode);

			if(eError != JP_Error_None)
			{
				break;
			}

			ptNextNode = ptList_IF->GetFirstList(ptList_IF->ptListSt);
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteNode(JP_D_LinkedTreeSt_T *ptTreeSt, JP_D_LinkedTree_T *ptNode)
{
	JP_Error_E eError = JP_Error_None;

	if((ptTreeSt != NULL) && (ptNode != NULL))
	{
		uint32_t i;
		uint32_t uiChildCount;

		JP_D_LinkedTree_T *ptChildNode;

		JP_D_LinkedTreeData_T *ptTreeData;
		JP_D_LinkedTreeData_T *ptTreeDataTemp;

		const JP_D_LinkedList_IF_T *ptList_IF = ptTreeSt;

		uiChildCount = SF_GetChildNodeCount(ptTreeSt, ptNode);

		for(i = 0; i < uiChildCount; i++)
		{
			ptChildNode = SF_GetChildNode(ptTreeSt, ptNode);

			eError = SF_DeleteNode(ptTreeSt, ptChildNode);

			if(eError != JP_Error_None)
			{
				break;
			}
		}

		if(eError == JP_Error_None)
		{
			eError = SF_DeleteNodeData(ptTreeSt, ptNode);

			if(eError == JP_Error_None)
			{
				ptTreeData = (JP_D_LinkedTreeData_T *)ptList_IF->GetListData(ptNode);

				if(ptTreeData->ptPrev != NULL)
				{
					ptTreeDataTemp = (JP_D_LinkedTreeData_T *)ptList_IF->GetListData(ptTreeData->ptPrev);

					ptTreeDataTemp->ptNext = ptTreeData->ptNext;
				}

				if(ptTreeData->ptNext != NULL)
				{
					ptTreeDataTemp = (JP_D_LinkedTreeData_T *)ptList_IF->GetListData(ptTreeData->ptNext);

					ptTreeDataTemp->ptPrev = ptTreeData->ptPrev;
				}

				free(ptTreeData);

				eError = ptList_IF->DeleteList(ptNode);
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteNodeName(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTree_T *ptNode)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptNode != NULL))
	{
		const JP_D_LinkedList_IF_T *ptList_IF = pctTreeSt;

		eError = ptList_IF->DeleteListName(ptNode);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteNodeData(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		JP_D_LinkedTreeData_T *ptTreeData;

		const JP_D_LinkedList_IF_T *ptList_IF = pctTreeSt;

		ptTreeData = (JP_D_LinkedTreeData_T *)ptList_IF->GetListData(pctNode);

		if(ptTreeData->pvData != NULL)
		{
			if(ptTreeData->iDataSize == -1)
			{
				ptTreeData->pvData = NULL;
			}
			else
			{
				free(ptTreeData->pvData);
				ptTreeData->pvData = NULL;
			}

			ptTreeData->iDataSize = 0;
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static uint32_t SF_GetNodeCount(const JP_D_LinkedTreeSt_T *pctTreeSt)
{
	uint32_t uiNodeCount = 0U;

	if(pctTreeSt != NULL)
	{
		const JP_D_LinkedTree_T *pctRootNode;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctRootNode = pctList_IF->GetFirstList(pctList_IF->ptListSt);

		if(pctRootNode != NULL)
		{
			uint32_t uiOperands[2];

			JP_Error_E eError;

			uiOperands[0] = uiNodeCount;
			uiOperands[1] = 1U;

			eError = JP_Add_UInt32(2U, uiOperands, &uiNodeCount);

			if(eError == JP_Error_None)
			{
				const JP_D_LinkedTreeData_T *pctTreeData;

				pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctRootNode);

				while(pctTreeData->ptNext != NULL)
				{
					uiOperands[0] = uiNodeCount;
					uiOperands[1] = 1U;

					eError = JP_Add_UInt32(2U, uiOperands, &uiNodeCount);

					if(eError == JP_Error_None)
					{
						pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctTreeData->ptNext);
					}
					else
					{
						break;
					}
				}
			}

			if(eError != JP_Error_None)
			{
				uiNodeCount = 0U;
			}
		}
	}

	return uiNodeCount;
}

static uint32_t SF_GetChildNodeCount(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	uint32_t uiChildNodeCount = 0U;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTree_T *pctChildNode;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctChildNode = pctList_IF->GetNextList(pctList_IF->ptListSt, pctNode);

		if(pctChildNode != NULL)
		{
			const JP_D_LinkedTreeData_T *pctTreeData;

			pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctChildNode);

			if(pctTreeData->ptPrev == NULL)
			{
				uint32_t uiOperands[2];

				JP_Error_E eError;

				uiOperands[0] = uiChildNodeCount;
				uiOperands[1] = 1U;

				eError = JP_Add_UInt32(2U, uiOperands, &uiChildNodeCount);

				if(eError == JP_Error_None)
				{
					while(pctTreeData->ptNext != NULL)
					{
						uiOperands[0] = uiChildNodeCount;
						uiOperands[1] = 1U;

						eError = JP_Add_UInt32(2U, uiOperands, &uiChildNodeCount);

						if(eError == JP_Error_None)
						{
							pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctTreeData->ptNext);
						}
						else
						{
							break;
						}
					}
				}

				if(eError != JP_Error_None)
				{
					uiChildNodeCount = 0U;
				}
			}
		}
	}

	return uiChildNodeCount;
}

static JP_BOOL_E SF_HasParentNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_BOOL_E bHasParentNode = JP_FALSE;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTree_T *pctFirstChildNode = pctNode;

		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		while(pctTreeData->ptPrev != NULL)
		{
			pctFirstChildNode = pctTreeData->ptPrev;

			pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctTreeData->ptPrev);
		}

		if(pctList_IF->GetPrevList(pctList_IF->ptListSt, pctFirstChildNode) != NULL)
		{
			bHasParentNode = JP_TRUE;
		}
	}

	return bHasParentNode;
}

static JP_BOOL_E SF_HasChildNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_BOOL_E bHasChildNode = JP_FALSE;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTree_T *pctChildNode;

		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctChildNode = pctList_IF->GetNextList(pctList_IF->ptListSt, pctNode);

		if(pctChildNode != NULL)
		{
			pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctChildNode);

			if(pctTreeData->ptPrev == NULL)
			{
				bHasChildNode = JP_TRUE;
			}
		}
	}

	return bHasChildNode;
}

static JP_D_LinkedTree_T *SF_GetRootNode(const JP_D_LinkedTreeSt_T *pctTreeSt)
{
	JP_D_LinkedTree_T *ptRootNode = NULL;

	if(pctTreeSt != NULL)
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptRootNode = pctList_IF->GetFirstList(pctList_IF->ptListSt);
	}

	return ptRootNode;
}

static JP_D_LinkedTree_T *SF_GetParentNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTree_T *ptParentNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTree_T *pctFirstChildNode = pctNode;

		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		while(pctTreeData->ptPrev != NULL)
		{
			pctFirstChildNode = pctTreeData->ptPrev;

			pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctTreeData->ptPrev);
		}

		ptParentNode = pctList_IF->GetPrevList(pctList_IF->ptListSt, pctFirstChildNode);
	}

	return ptParentNode;
}

static JP_D_LinkedTree_T *SF_GetChildNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTree_T *ptChildNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptChildNode = pctList_IF->GetNextList(pctList_IF->ptListSt, pctNode);

		if(ptChildNode != NULL)
		{
			const JP_D_LinkedTreeData_T *pctTreeData;

			pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(ptChildNode);

			if(pctTreeData->ptPrev != NULL)
			{
				ptChildNode = NULL;
			}
		}
	}

	return ptChildNode;
}

static JP_D_LinkedTree_T *SF_GetPrevNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTree_T *ptPrevNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		ptPrevNode = pctTreeData->ptPrev;
	}

	return ptPrevNode;
}

static JP_D_LinkedTree_T *SF_GetNextNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTree_T *ptNextNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		ptNextNode = pctTreeData->ptNext;
	}

	return ptNextNode;
}

static JP_Error_E SF_SetNodeName(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTree_T *ptNode, const char *pcstrName)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptNode != NULL) && (pcstrName != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->SetListName(ptNode, pcstrName);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_SetNodeData(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, void *pvData, int iDataSize)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pvData != NULL)
		&& ((iDataSize == -1) || (iDataSize > 0)))
	{
		JP_D_LinkedTreeData_T *ptTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = SF_DeleteNodeData(pctTreeSt, pctNode);

		if(eError == JP_Error_None)
		{
			ptTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

			if(iDataSize > 0)
			{
				JP_RetType_UInt64_T tRet_I32ToU64;

				tRet_I32ToU64 = JP_TypeCast_Int32ToUInt64(iDataSize);

				if(tRet_I32ToU64.eError == JP_Error_None)
				{
					size_t tDataSize = tRet_I32ToU64.ulRetVal;

					ptTreeData->pvData = calloc(1UL, tDataSize);

					if(ptTreeData->pvData != NULL)
					{
						(void)memcpy(ptTreeData->pvData, pvData, tDataSize);

						ptTreeData->iDataSize = iDataSize;
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
				ptTreeData->pvData = pvData;

				ptTreeData->iDataSize = iDataSize;
			}
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static char *SF_GetNodeName(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	char *pstrName = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pstrName = pctList_IF->GetListName(pctNode);
	}

	return pstrName;
}

static void *SF_GetNodeData(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	void *pvData = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		pvData = pctTreeData->pvData;
	}

	return pvData;
}

static int SF_GetNodeDataSize(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	int iDataSize = 0;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedTreeData_T *pctTreeData;

		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pctTreeData = (JP_D_LinkedTreeData_T *)pctList_IF->GetListData(pctNode);

		iDataSize = pctTreeData->iDataSize;
	}

	return iDataSize;
}

static JP_D_LinkedTree_T *SF_FindRootNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const char *pcstrName)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((pctTreeSt != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTree_T *ptNextNode;

		ptNextNode = SF_GetRootNode(pctTreeSt);

		while(ptNextNode != NULL)
		{
			if(strcmp(pcstrName, SF_GetNodeName(pctTreeSt, ptNextNode)) == 0)
			{
				ptRetNode = ptNextNode;
				break;
			}

			ptNextNode = SF_GetNextNode(pctTreeSt, ptNextNode);
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTree_T *SF_FindChildNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTree_T *ptChildNode;

		ptChildNode = SF_GetChildNode(pctTreeSt, pctNode);

		while(ptChildNode != NULL)
		{
			if(strcmp(pcstrName, SF_GetNodeName(pctTreeSt, ptChildNode)) == 0)
			{
				ptRetNode = ptChildNode;
				break;
			}

			ptChildNode = SF_GetNextNode(pctTreeSt, ptChildNode);
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTree_T *SF_FindPrevNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTree_T *ptPrevNode;

		ptPrevNode = SF_GetPrevNode(pctTreeSt, pctNode);

		while(ptPrevNode != NULL)
		{
			if(strcmp(pcstrName, SF_GetNodeName(pctTreeSt, ptPrevNode)) == 0)
			{
				ptRetNode = ptPrevNode;
				break;
			}

			ptPrevNode = SF_GetPrevNode(pctTreeSt, ptPrevNode);
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTree_T *SF_FindNextNode(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTree_T *ptNextNode;

		ptNextNode = SF_GetNextNode(pctTreeSt, pctNode);

		while(ptNextNode != NULL)
		{
			if(strcmp(pcstrName, SF_GetNodeName(pctTreeSt, ptNextNode)) == 0)
			{
				ptRetNode = ptNextNode;
				break;
			}

			ptNextNode = SF_GetNextNode(pctTreeSt, ptNextNode);
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTreeAtt_T *SF_InsertAtt(
	const JP_D_LinkedTreeSt_T *pctTreeSt,
	const JP_D_LinkedTree_T *pctNode,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedTreeAtt_T *ptInsertAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptInsertAtt = pctList_IF->InsertItem(pctNode, pcstrName, pvData, iDataSize);
	}

	return ptInsertAtt;
}

static JP_D_LinkedTreeAtt_T *SF_InsertAttTo(
	const JP_D_LinkedTreeSt_T *pctTreeSt,
	JP_D_LinkedTreeAtt_T *ptBaseAtt,
	JP_TreeInsertType_E eInsertType,
	const char *pcstrName,
	void *pvData,
	int iDataSize)
{
	JP_D_LinkedTreeAtt_T *ptInsertAtt = NULL;

	if((pctTreeSt != NULL) && (ptBaseAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptInsertAtt = pctList_IF->InsertItemTo(ptBaseAtt, eInsertType, pcstrName, pvData, iDataSize);
	}

	return ptInsertAtt;
}

static JP_Error_E SF_DeleteAttAll(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->DeleteItemAll(pctNode);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->DeleteItem(ptAtt);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteAttName(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->DeleteItemName(ptAtt);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_DeleteAttData(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->DeleteItemData(ptAtt);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static uint32_t SF_GetAttCount(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	uint32_t uiAttCount = 0U;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		uiAttCount = pctList_IF->GetItemCount(pctNode);
	}

	return uiAttCount;
}

static JP_BOOL_E SF_HasAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_BOOL_E bHasAtt = JP_FALSE;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		if(pctList_IF->GetFirstItem(pctNode) != NULL)
		{
			bHasAtt = JP_TRUE;
		}
	}

	return bHasAtt;
}

static JP_D_LinkedTreeAtt_T *SF_GetFirstAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode)
{
	JP_D_LinkedTreeAtt_T *ptFirstAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptFirstAtt = pctList_IF->GetFirstItem(pctNode);
	}

	return ptFirstAtt;
}

static JP_D_LinkedTreeAtt_T *SF_GetPrevAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt)
{
	JP_D_LinkedTreeAtt_T *ptPrevAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pctAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptPrevAtt = pctList_IF->GetPrevItem(pctNode, pctAtt);
	}

	return ptPrevAtt;
}

static JP_D_LinkedTreeAtt_T *SF_GetNextAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt)
{
	JP_D_LinkedTreeAtt_T *ptNextAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pctAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		ptNextAtt = pctList_IF->GetNextItem(pctNode, pctAtt);
	}

	return ptNextAtt;
}

static JP_Error_E SF_SetAttName(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt, const char *pcstrName)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptAtt != NULL) && (pcstrName != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->SetItemName(ptAtt, pcstrName);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static JP_Error_E SF_SetAttData(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt, void *pvData, int iDataSize)
{
	JP_Error_E eError = JP_Error_None;

	if((pctTreeSt != NULL) && (ptAtt != NULL) && (pvData != NULL)
		&& ((iDataSize == -1) || (iDataSize > 0)))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		eError = pctList_IF->SetItemData(ptAtt, pvData, iDataSize);
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}

static char *SF_GetAttName(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt)
{
	char *pstrName = NULL;

	if((pctTreeSt != NULL) && (pctAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pstrName = pctList_IF->GetItemName(pctAtt);
	}

	return pstrName;
}

static void *SF_GetAttData(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt)
{
	void *pvData = NULL;

	if((pctTreeSt != NULL) && (pctAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		pvData = pctList_IF->GetItemData(pctAtt);
	}

	return pvData;
}

static int SF_GetAttDataSize(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt)
{
	int iDataSize = 0;

	if((pctTreeSt != NULL) && (pctAtt != NULL))
	{
		const JP_D_LinkedList_IF_T *pctList_IF = pctTreeSt;

		iDataSize = pctList_IF->GetItemDataSize(pctAtt);
	}

	return iDataSize;
}

static JP_D_LinkedTreeAtt_T *SF_FindAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName)
{
	JP_D_LinkedTreeAtt_T *ptRetAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTreeAtt_T *ptNextAtt;

		ptNextAtt = SF_GetFirstAtt(pctTreeSt, pctNode);

		while(ptNextAtt != NULL)
		{
			if(strcmp(pcstrName, SF_GetAttName(pctTreeSt, ptNextAtt)) == 0)
			{
				ptRetAtt = ptNextAtt;
				break;
			}

			ptNextAtt = SF_GetNextAtt(pctTreeSt, pctNode, ptNextAtt);
		}
	}

	return ptRetAtt;
}

static JP_D_LinkedTreeAtt_T *SF_FindPrevAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt, const char *pcstrName)
{
	JP_D_LinkedTreeAtt_T *ptRetAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pctAtt != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTreeAtt_T *ptPrevAtt;

		ptPrevAtt = SF_GetPrevAtt(pctTreeSt, pctNode, pctAtt);

		while(ptPrevAtt != NULL)
		{
			if(strcmp(pcstrName, SF_GetAttName(pctTreeSt, ptPrevAtt)) == 0)
			{
				ptRetAtt = ptPrevAtt;
				break;
			}

			ptPrevAtt = SF_GetPrevAtt(pctTreeSt, pctNode, ptPrevAtt);
		}
	}

	return ptRetAtt;
}

static JP_D_LinkedTreeAtt_T *SF_FindNextAtt(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt, const char *pcstrName)
{
	JP_D_LinkedTreeAtt_T *ptRetAtt = NULL;

	if((pctTreeSt != NULL) && (pctNode != NULL) && (pctAtt != NULL) && (pcstrName != NULL))
	{
		JP_D_LinkedTreeAtt_T *ptNextAtt;

		ptNextAtt = SF_GetNextAtt(pctTreeSt, pctNode, pctAtt);

		while(ptNextAtt != NULL)
		{
			if(strcmp(pcstrName, SF_GetAttName(pctTreeSt, ptNextAtt)) == 0)
			{
				ptRetAtt = ptNextAtt;
				break;
			}

			ptNextAtt = SF_GetNextAtt(pctTreeSt, pctNode, ptNextAtt);
		}
	}

	return ptRetAtt;
}

static JP_D_LinkedTree_T *SF_GraftNode(JP_D_LinkedTreeSt_T *ptTreeSt, JP_D_LinkedTree_IF_T *pt_GT, const JP_D_LinkedTree_T *pct_GT_Node)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if(ptTreeSt != NULL)
	{
		uint32_t i;
		uint32_t j;
		uint32_t ui_GT_NodeCount;

		if(pct_GT_Node == NULL)
		{
			ui_GT_NodeCount = pt_GT->GetNodeCount(pt_GT->ptTreeSt);
		}
		else
		{
			ui_GT_NodeCount = 1U;
		}

		for(i = 0U; i < ui_GT_NodeCount; i++)
		{
			uint32_t ui_GT_AttCount;
			uint32_t ui_GT_ChildNodeCount;

			JP_D_LinkedTree_T *ptInsertNode;

			const JP_D_LinkedTree_T *pctNextGtNode = NULL;

			if(i == 0U)
			{
				if(pct_GT_Node == NULL)
				{
					pctNextGtNode = pt_GT->GetRootNode(pt_GT->ptTreeSt);
				}
				else
				{
					pctNextGtNode = pct_GT_Node;
				}
			}
			else
			{
				pctNextGtNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtNode);
			}

			ptInsertNode = SF_InsertNode(
				ptTreeSt,
				pt_GT->GetNodeName(pt_GT->ptTreeSt, pctNextGtNode),
				pt_GT->GetNodeData(pt_GT->ptTreeSt, pctNextGtNode),
				pt_GT->GetNodeDataSize(pt_GT->ptTreeSt, pctNextGtNode));

			if(i == 0U)
			{
				ptRetNode = ptInsertNode;
			}

			ui_GT_AttCount = pt_GT->GetAttCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_AttCount; j++)
			{
				const JP_D_LinkedTreeAtt_T *pctNextGtAtt = NULL;

				if(j == 0U)
				{
					pctNextGtAtt = pt_GT->GetFirstAtt(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtAtt = pt_GT->GetNextAtt(pt_GT->ptTreeSt, pctNextGtNode, pctNextGtAtt);
				}

				(void)SF_InsertAtt(
					ptTreeSt,
					ptInsertNode,
					pt_GT->GetAttName(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttData(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttDataSize(pt_GT->ptTreeSt, pctNextGtAtt));
			}

			ui_GT_ChildNodeCount = pt_GT->GetChildNodeCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_ChildNodeCount; j++)
			{
				const JP_D_LinkedTree_T *pctNextGtChildNode = NULL;

				if(j == 0U)
				{
					pctNextGtChildNode = pt_GT->GetChildNode(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtChildNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtChildNode);
				}

				(void)SF_GraftNodeToChild(ptTreeSt, ptInsertNode, pt_GT, pctNextGtChildNode);
			}
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTree_T *SF_GraftNodeToChild(
	JP_D_LinkedTreeSt_T *ptTreeSt,
	JP_D_LinkedTree_T *ptNode,
	JP_D_LinkedTree_IF_T *pt_GT,
	const JP_D_LinkedTree_T *pct_GT_Node)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((ptTreeSt != NULL) && (ptNode != NULL))
	{
		uint32_t i;
		uint32_t j;
		uint32_t ui_GT_NodeCount;

		if(pct_GT_Node == NULL)
		{
			ui_GT_NodeCount = pt_GT->GetNodeCount(pt_GT->ptTreeSt);
		}
		else
		{
			ui_GT_NodeCount = 1U;
		}

		for(i = 0U; i < ui_GT_NodeCount; i++)
		{
			uint32_t ui_GT_AttCount;
			uint32_t ui_GT_ChildNodeCount;

			JP_D_LinkedTree_T *ptInsertNode;

			const JP_D_LinkedTree_T *pctNextGtNode = NULL;

			if(i == 0U)
			{
				if(pct_GT_Node == NULL)
				{
					pctNextGtNode = pt_GT->GetRootNode(pt_GT->ptTreeSt);
				}
				else
				{
					pctNextGtNode = pct_GT_Node;
				}
			}
			else
			{
				pctNextGtNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtNode);
			}

			ptInsertNode = SF_InsertNodeToChild(
				ptTreeSt,
				ptNode,
				pt_GT->GetNodeName(pt_GT->ptTreeSt, pctNextGtNode),
				pt_GT->GetNodeData(pt_GT->ptTreeSt, pctNextGtNode),
				pt_GT->GetNodeDataSize(pt_GT->ptTreeSt, pctNextGtNode));

			if(i == 0U)
			{
				ptRetNode = ptInsertNode;
			}

			ui_GT_AttCount = pt_GT->GetAttCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_AttCount; j++)
			{
				const JP_D_LinkedTreeAtt_T *pctNextGtAtt = NULL;

				if(j == 0U)
				{
					pctNextGtAtt = pt_GT->GetFirstAtt(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtAtt = pt_GT->GetNextAtt(pt_GT->ptTreeSt, pctNextGtNode, pctNextGtAtt);
				}

				(void)SF_InsertAtt(
					ptTreeSt,
					ptInsertNode,
					pt_GT->GetAttName(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttData(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttDataSize(pt_GT->ptTreeSt, pctNextGtAtt));
			}

			ui_GT_ChildNodeCount = pt_GT->GetChildNodeCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_ChildNodeCount; j++)
			{
				const JP_D_LinkedTree_T *pctNextGtChildNode = NULL;

				if(j == 0U)
				{
					pctNextGtChildNode = pt_GT->GetChildNode(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtChildNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtChildNode);
				}

				(void)SF_GraftNodeToChild(ptTreeSt, ptInsertNode, pt_GT, pctNextGtChildNode);
			}
		}
	}

	return ptRetNode;
}

static JP_D_LinkedTree_T *SF_GraftNodeToSibling(
	JP_D_LinkedTreeSt_T *ptTreeSt,
	JP_D_LinkedTree_T *ptBaseNode,
	JP_TreeInsertType_E eInsertType,
	JP_D_LinkedTree_IF_T *pt_GT,
	const JP_D_LinkedTree_T *pct_GT_Node)
{
	JP_D_LinkedTree_T *ptRetNode = NULL;

	if((ptTreeSt != NULL) && (ptBaseNode != NULL))
	{
		uint32_t i;
		uint32_t j;
		uint32_t ui_GT_NodeCount;

		if(pct_GT_Node == NULL)
		{
			ui_GT_NodeCount = pt_GT->GetNodeCount(pt_GT->ptTreeSt);
		}
		else
		{
			ui_GT_NodeCount = 1U;
		}

		for(i = 0U; i < ui_GT_NodeCount; i++)
		{
			uint32_t ui_GT_AttCount;
			uint32_t ui_GT_ChildNodeCount;

			JP_D_LinkedTree_T *ptInsertNode = NULL;

			const JP_D_LinkedTree_T *pctNextGtNode = NULL;

			if(i == 0U)
			{
				if(pct_GT_Node == NULL)
				{
					pctNextGtNode = pt_GT->GetRootNode(pt_GT->ptTreeSt);
				}
				else
				{
					pctNextGtNode = pct_GT_Node;
				}
			}
			else
			{
				pctNextGtNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtNode);
			}

			if(i == 0U)
			{
				ptInsertNode = SF_InsertNodeToSibling(
					ptTreeSt,
					ptBaseNode,
					eInsertType,
					pt_GT->GetNodeName(pt_GT->ptTreeSt, pctNextGtNode),
					pt_GT->GetNodeData(pt_GT->ptTreeSt, pctNextGtNode),
					pt_GT->GetNodeDataSize(pt_GT->ptTreeSt, pctNextGtNode));

				ptRetNode = ptInsertNode;
			}
			else
			{
				ptInsertNode = SF_InsertNodeToSibling(
					ptTreeSt,
					ptInsertNode,
					JP_TREE_INSERT_TYPE_AFTER,
					pt_GT->GetNodeName(pt_GT->ptTreeSt, pctNextGtNode),
					pt_GT->GetNodeData(pt_GT->ptTreeSt, pctNextGtNode),
					pt_GT->GetNodeDataSize(pt_GT->ptTreeSt, pctNextGtNode));
			}

			ui_GT_AttCount = pt_GT->GetAttCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_AttCount; j++)
			{
				const JP_D_LinkedTreeAtt_T *pctNextGtAtt = NULL;

				if(j == 0U)
				{
					pctNextGtAtt = pt_GT->GetFirstAtt(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtAtt = pt_GT->GetNextAtt(pt_GT->ptTreeSt, pctNextGtNode, pctNextGtAtt);
				}

				(void)SF_InsertAtt(
					ptTreeSt,
					ptInsertNode,
					pt_GT->GetAttName(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttData(pt_GT->ptTreeSt, pctNextGtAtt),
					pt_GT->GetAttDataSize(pt_GT->ptTreeSt, pctNextGtAtt));
			}

			ui_GT_ChildNodeCount = pt_GT->GetChildNodeCount(pt_GT->ptTreeSt, pctNextGtNode);

			for(j = 0U; j < ui_GT_ChildNodeCount; j++)
			{
				const JP_D_LinkedTree_T *pctNextGtChildNode = NULL;

				if(j == 0U)
				{
					pctNextGtChildNode = pt_GT->GetChildNode(pt_GT->ptTreeSt, pctNextGtNode);
				}
				else
				{
					pctNextGtChildNode = pt_GT->GetNextNode(pt_GT->ptTreeSt, pctNextGtChildNode);
				}

				(void)SF_GraftNodeToChild(ptTreeSt, ptInsertNode, pt_GT, pctNextGtChildNode);
			}
		}
	}

	return ptRetNode;
}

JP_D_LinkedTree_IF_T *JP_CreateTree(void)
{
	JP_D_LinkedTree_IF_T *pt_Tree_IF;

	pt_Tree_IF = (JP_D_LinkedTree_IF_T *)calloc(1UL, sizeof(JP_D_LinkedTree_IF_T));

	if(pt_Tree_IF != NULL)
	{
		pt_Tree_IF->ptTreeSt = JP_CreateList();

		if(pt_Tree_IF->ptTreeSt != NULL)
		{
			pt_Tree_IF->InsertNode 				= SF_InsertNode;
			pt_Tree_IF->InsertNodeToChild 		= SF_InsertNodeToChild;
			pt_Tree_IF->InsertNodeToSibling 	= SF_InsertNodeToSibling;
			pt_Tree_IF->DeleteNodeAll 			= SF_DeleteNodeAll;
			pt_Tree_IF->DeleteNode 				= SF_DeleteNode;
			pt_Tree_IF->DeleteNodeName 			= SF_DeleteNodeName;
			pt_Tree_IF->DeleteNodeData 			= SF_DeleteNodeData;
			pt_Tree_IF->GetNodeCount 			= SF_GetNodeCount;
			pt_Tree_IF->GetChildNodeCount 		= SF_GetChildNodeCount;
			pt_Tree_IF->HasParentNode 			= SF_HasParentNode;
			pt_Tree_IF->HasChildNode 			= SF_HasChildNode;
			pt_Tree_IF->GetRootNode 			= SF_GetRootNode;
			pt_Tree_IF->GetParentNode 			= SF_GetParentNode;
			pt_Tree_IF->GetChildNode 			= SF_GetChildNode;
			pt_Tree_IF->GetPrevNode 			= SF_GetPrevNode;
			pt_Tree_IF->GetNextNode 			= SF_GetNextNode;
			pt_Tree_IF->SetNodeName 			= SF_SetNodeName;
			pt_Tree_IF->SetNodeData 			= SF_SetNodeData;
			pt_Tree_IF->GetNodeName 			= SF_GetNodeName;
			pt_Tree_IF->GetNodeData 			= SF_GetNodeData;
			pt_Tree_IF->GetNodeDataSize 		= SF_GetNodeDataSize;
			pt_Tree_IF->FindRootNode 			= SF_FindRootNode;
			pt_Tree_IF->FindChildNode 			= SF_FindChildNode;
			pt_Tree_IF->FindPrevNode 			= SF_FindPrevNode;
			pt_Tree_IF->FindNextNode 			= SF_FindNextNode;
			pt_Tree_IF->InsertAtt 				= SF_InsertAtt;
			pt_Tree_IF->InsertAttTo 			= SF_InsertAttTo;
			pt_Tree_IF->DeleteAttAll 			= SF_DeleteAttAll;
			pt_Tree_IF->DeleteAtt 				= SF_DeleteAtt;
			pt_Tree_IF->DeleteAttName 			= SF_DeleteAttName;
			pt_Tree_IF->DeleteAttData 			= SF_DeleteAttData;
			pt_Tree_IF->GetAttCount 			= SF_GetAttCount;
			pt_Tree_IF->HasAtt 					= SF_HasAtt;
			pt_Tree_IF->GetFirstAtt 			= SF_GetFirstAtt;
			pt_Tree_IF->GetPrevAtt 				= SF_GetPrevAtt;
			pt_Tree_IF->GetNextAtt 				= SF_GetNextAtt;
			pt_Tree_IF->SetAttName 				= SF_SetAttName;
			pt_Tree_IF->SetAttData 				= SF_SetAttData;
			pt_Tree_IF->GetAttName 				= SF_GetAttName;
			pt_Tree_IF->GetAttData 				= SF_GetAttData;
			pt_Tree_IF->GetAttDataSize 			= SF_GetAttDataSize;
			pt_Tree_IF->FindAtt 				= SF_FindAtt;
			pt_Tree_IF->FindPrevAtt 			= SF_FindPrevAtt;
			pt_Tree_IF->FindNextAtt 			= SF_FindNextAtt;
			pt_Tree_IF->GraftNode 				= SF_GraftNode;
			pt_Tree_IF->GraftNodeToChild 		= SF_GraftNodeToChild;
			pt_Tree_IF->GraftNodeToSibling 		= SF_GraftNodeToSibling;
		}
		else
		{
			free(pt_Tree_IF);
			pt_Tree_IF = NULL;
		}
	}

	return pt_Tree_IF;
}

JP_Error_E JP_DestroyTree(JP_D_LinkedTree_IF_T *pt_IF)
{
	JP_Error_E eError = JP_Error_None;

	if(pt_IF != NULL)
	{
		if(pt_IF->ptTreeSt != NULL)
		{
			eError = SF_DeleteNodeAll(pt_IF->ptTreeSt);

			if(eError == JP_Error_None)
			{
				eError = JP_DestroyList(pt_IF->ptTreeSt);
			}
		}

		if(eError == JP_Error_None)
		{
			free(pt_IF);
		}
	}
	else
	{
		eError = JP_Error_BadParameter;
	}

	return eError;
}