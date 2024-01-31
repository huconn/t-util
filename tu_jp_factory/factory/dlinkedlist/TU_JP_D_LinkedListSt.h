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

/*!
 ************************************************************************
 * \file
 *	JP_D_LinkedListSt.h
 * \brief
 *
 * \author
 *	Name 	: Jang Won Seok
 *	E-mail	: highwire77@gmail.com
 ************************************************************************
 */
#ifndef __JP_D_LINKED_LIST_ST_H__
#define __JP_D_LINKED_LIST_ST_H__

typedef void *JP_D_LINKED_LIST_ST_H;
typedef void *JP_D_LinkedList_T*;

typedef enum tag_JP_ListInsertType_E
{
	JP_LIST_INSERT_TYPE_BEFORE = 0,
	JP_LIST_INSERT_TYPE_AFTER
} JP_ListInsertType_E;

typedef struct tag_JP_D_LinkedListItem_T
{
	char *pstrName;
	void *pvData;
	int iDataSize;

	struct tag_JP_D_LinkedListItem_T *ptPrev;
	struct tag_JP_D_LinkedListItem_T *ptNext;
} JP_D_LinkedListItem_T, *p_JP_D_LinkedListItem_T;

typedef struct tag_JP_D_LinkedList_T
{
	char *pstrName;
	void *pvData;
	int iDataSize;

	JP_D_LinkedListItem_T *ptHead;
	JP_D_LinkedListItem_T *ptTail;

	struct tag_JP_D_LinkedList_T *ptPrev;
	struct tag_JP_D_LinkedList_T *ptNext;
} JP_D_LinkedList_T;

typedef struct tag_JP_D_LinkedListSt_T
{
	JP_D_LinkedList_T *ptHead;
	JP_D_LinkedList_T *ptTail;
} JP_D_LinkedListSt_T;

typedef struct tag_JP_D_LinkedList_IF_T
{
	JP_D_LINKED_LIST_ST_H hListSt;

	// List Operations
	JP_D_LinkedList_T* (*InsertList)(JP_D_LINKED_LIST_ST_H hListSt, char *pstrName, void *pvData, int iDataSize);

	JP_D_LinkedList_T* (*InsertListTo)(
		JP_D_LINKED_LIST_ST_H hListSt,
		JP_D_LinkedList_T* hBaseList,
		JP_ListInsertType_E eInsertType,
		char *pstrName,
		void *pvData,
		int iDataSize);

	int (*RemoveAll)(JP_D_LINKED_LIST_ST_H hListSt);
	int (*DeleteList)(JP_D_LinkedList_T* hList);

	int (*DeleteListName)(JP_D_LinkedList_T* hList);
	int (*DeleteListData)(JP_D_LinkedList_T* hList);

	// List Attributes
	int (*GetListCount)(JP_D_LINKED_LIST_ST_H hListSt);

	JP_D_LinkedList_T* (*GetFirstList)(JP_D_LINKED_LIST_ST_H hListSt);
	JP_D_LinkedList_T* (*GetPrevList)(JP_D_LINKED_LIST_ST_H hListSt, JP_D_LinkedList_T* hList);
	JP_D_LinkedList_T* (*GetNextList)(JP_D_LINKED_LIST_ST_H hListSt, JP_D_LinkedList_T* hList);

	int (*SetListName)(JP_D_LinkedList_T* hList, char *pstrName);
	int (*SetListData)(JP_D_LinkedList_T* hList, void *pData, int iDataSize);
	char *(*GetListName)(JP_D_LinkedList_T* hList);
	void *(*GetListData)(JP_D_LinkedList_T* hList);
	int (*GetListDataSize)(JP_D_LinkedList_T* hList);

	JP_D_LinkedList_T* (*FindList)(JP_D_LINKED_LIST_ST_H hListSt, char *pstrName);
	JP_D_LinkedList_T* (*FindPrevList)(JP_D_LINKED_LIST_ST_H hListSt, JP_D_LinkedList_T* hList, char *pstrName);
	JP_D_LinkedList_T* (*FindNextList)(JP_D_LINKED_LIST_ST_H hListSt, JP_D_LinkedList_T* hList, char *pstrName);

	// Item Operations
	JP_D_LinkedList_T* (*InsertItem)(JP_D_LinkedList_T* hList, char *pstrName, void *pvData, int iDataSize);

	JP_D_LinkedList_T* (*InsertItemTo)(
		JP_D_LinkedList_T* hBaseItem,
		JP_ListInsertType_E eInsertType,
		char *pstrName,
		void *pvData,
		int iDataSize);

	int (*DeleteItemAll)(JP_D_LinkedList_T* hList);
	int (*DeleteItem)(JP_D_LinkedList_T* hItem);

	int (*DeleteItemName)(JP_D_LinkedList_T* hItem);
	int (*DeleteItemData)(JP_D_LinkedList_T* hItem);

	// Item Attributes
	int (*GetItemCount)(JP_D_LinkedList_T* hList);

	JP_D_LinkedList_T* (*GetFirstItem)(JP_D_LinkedList_T* hList);
	JP_D_LinkedList_T* (*GetPrevItem)(JP_D_LinkedList_T* hList, JP_D_LinkedList_T* hItem);
	JP_D_LinkedList_T* (*GetNextItem)(JP_D_LinkedList_T* hList, JP_D_LinkedList_T* hItem);

	int (*SetItemName)(JP_D_LinkedList_T* hItem, char *pstrName);
	int (*SetItemData)(JP_D_LinkedList_T* hItem, void *pData, int iDataSize);
	char *(*GetItemName)(JP_D_LinkedList_T* hItem);
	void *(*GetItemData)(JP_D_LinkedList_T* hItem);
	int (*GetItemDataSize)(JP_D_LinkedList_T* hItem);

	JP_D_LinkedList_T* (*FindItem)(JP_D_LinkedList_T* hList, char *pstrName);
	JP_D_LinkedList_T* (*FindPrevItem)(JP_D_LinkedList_T* hList, JP_D_LinkedList_T* hItem, char *pstrName);
	JP_D_LinkedList_T* (*FindNextItem)(JP_D_LinkedList_T* hList, JP_D_LinkedList_T* hItem, char *pstrName);

	// Graft
	JP_D_LinkedList_T* (*Graft)(
		JP_D_LINKED_LIST_ST_H hListSt,
		struct tag_JP_D_LinkedList_IF_T *pt_GL,
		JP_D_LinkedList_T* hGlList);

	JP_D_LinkedList_T* (*GraftTo)(
		JP_D_LINKED_LIST_ST_H hListSt,
		JP_D_LinkedList_T* hBaseList,
		JP_ListInsertType_E eInsertType,
		struct tag_JP_D_LinkedList_IF_T *pt_GL,
		JP_D_LinkedList_T* hGlList);

} JP_D_LinkedList_IF_T;

#endif	// __JP_D_LINKED_LIST_ST_H__