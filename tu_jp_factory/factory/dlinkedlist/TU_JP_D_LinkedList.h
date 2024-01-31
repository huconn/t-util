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

#ifndef JP_D_LINKED_LIST_H_
#define JP_D_LINKED_LIST_H_

typedef enum
{
	JP_LIST_INSERT_TYPE_BEFORE = 0,
	JP_LIST_INSERT_TYPE_AFTER
} JP_ListInsertType_E;

typedef struct JP_D_LinkedListItem_T JP_D_LinkedListItem_T;
typedef struct JP_D_LinkedList_T JP_D_LinkedList_T;
typedef struct JP_D_LinkedListSt_T JP_D_LinkedListSt_T;

typedef struct JP_D_LinkedList_IF_T JP_D_LinkedList_IF_T;

struct JP_D_LinkedList_IF_T
{
	JP_D_LinkedListSt_T *ptListSt;

	// List Operations
	JP_D_LinkedList_T *(*InsertList)(const JP_D_LinkedListSt_T *pctListSt, const char *pcstrName, void *pvData, int iDataSize);

	JP_D_LinkedList_T *(*InsertListTo)(
		JP_D_LinkedList_T *ptBaseList,
		JP_ListInsertType_E eInsertType,
		const char *pcstrName,
		void *pvData,
		int iDataSize);

	JP_Error_E (*DeleteListAll)(const JP_D_LinkedListSt_T *pctListSt);
	JP_Error_E (*DeleteList)(JP_D_LinkedList_T *ptList);

	JP_Error_E (*DeleteListName)(JP_D_LinkedList_T *ptList);
	JP_Error_E (*DeleteListData)(JP_D_LinkedList_T *ptList);

	// List Attributes
	uint32_t (*GetListCount)(const JP_D_LinkedListSt_T *pctListSt);

	JP_D_LinkedList_T *(*GetFirstList)(const JP_D_LinkedListSt_T *pctListSt);
	JP_D_LinkedList_T *(*GetPrevList)(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList);
	JP_D_LinkedList_T *(*GetNextList)(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList);

	JP_Error_E (*SetListName)(JP_D_LinkedList_T *ptList, const char *pcstrName);
	JP_Error_E (*SetListData)(JP_D_LinkedList_T *ptList, void *pData, int iDataSize);
	char *(*GetListName)(const JP_D_LinkedList_T *pctList);
	void *(*GetListData)(const JP_D_LinkedList_T *pctList);
	int (*GetListDataSize)(const JP_D_LinkedList_T *pctList);

	JP_D_LinkedList_T *(*FindList)(const JP_D_LinkedListSt_T *pctListSt, const char *pcstrName);
	JP_D_LinkedList_T *(*FindPrevList)(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList, const char *pcstrName);
	JP_D_LinkedList_T *(*FindNextList)(const JP_D_LinkedListSt_T *pctListSt, const JP_D_LinkedList_T *pctList, const char *pcstrName);

	// Item Operations
	JP_D_LinkedListItem_T *(*InsertItem)(const JP_D_LinkedList_T *pctList, const char *pcstrName, void *pvData, int iDataSize);

	JP_D_LinkedListItem_T *(*InsertItemTo)(
		JP_D_LinkedListItem_T *ptBaseItem,
		JP_ListInsertType_E eInsertType,
		const char *pcstrName,
		void *pvData,
		int iDataSize);

	JP_Error_E (*DeleteItemAll)(const JP_D_LinkedList_T *pctList);
	JP_Error_E (*DeleteItem)(JP_D_LinkedListItem_T *ptItem);

	JP_Error_E (*DeleteItemName)(JP_D_LinkedListItem_T *ptItem);
	JP_Error_E (*DeleteItemData)(JP_D_LinkedListItem_T *ptItem);

	// Item Attributes
	uint32_t (*GetItemCount)(const JP_D_LinkedList_T *pctList);

	JP_D_LinkedListItem_T *(*GetFirstItem)(const JP_D_LinkedList_T *pctList);
	JP_D_LinkedListItem_T *(*GetPrevItem)(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem);
	JP_D_LinkedListItem_T *(*GetNextItem)(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem);

	JP_Error_E (*SetItemName)(JP_D_LinkedListItem_T *ptItem, const char *pcstrName);
	JP_Error_E (*SetItemData)(JP_D_LinkedListItem_T *ptItem, void *pData, int iDataSize);
	char *(*GetItemName)(const JP_D_LinkedListItem_T *pctItem);
	void *(*GetItemData)(const JP_D_LinkedListItem_T *pctItem);
	int (*GetItemDataSize)(const JP_D_LinkedListItem_T *pctItem);

	JP_D_LinkedListItem_T *(*FindItem)(const JP_D_LinkedList_T *pctList, const char *pcstrName);
	JP_D_LinkedListItem_T *(*FindPrevItem)(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem, const char *pcstrName);
	JP_D_LinkedListItem_T *(*FindNextItem)(const JP_D_LinkedList_T *pctList, const JP_D_LinkedListItem_T *pctItem, const char *pcstrName);

	// Graft
	JP_D_LinkedList_T *(*GraftList)(
		const JP_D_LinkedList_IF_T *pct_GL,
		const JP_D_LinkedList_T *pctGlList);

	JP_D_LinkedList_T *(*GraftListTo)(
		JP_D_LinkedList_T *ptBaseList,
		JP_ListInsertType_E eInsertType,
		const JP_D_LinkedList_IF_T *pt_GL,
		const JP_D_LinkedList_T *pctGlList);
};

JP_D_LinkedList_IF_T *JP_CreateList(void);
JP_Error_E JP_DestroyList(JP_D_LinkedList_IF_T *ptList_IF);

#endif	// JP_D_LINKED_LIST_H_