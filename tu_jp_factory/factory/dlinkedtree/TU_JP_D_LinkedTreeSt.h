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
 *	JP_D_LinkedTreeSt.h
 * \brief
 *
 * \author
 *	Name 	: Jang Won Seok
 *	E-mail	: highwire77@gmail.com
 ************************************************************************
 */
#ifndef __JP_D_LINKED_TREE_ST_H__
#define __JP_D_LINKED_TREE_ST_H__

typedef void *JP_D_LINKED_TREE_ST_H;
typedef void *JP_D_LinkedTree_T*;

typedef enum tag_JP_TreeInsertType_E
{
	JP_TREE_INSERT_TYPE_BEFORE = 0,
	JP_TREE_INSERT_TYPE_AFTER
} JP_TreeInsertType_E;

typedef struct tag_JP_D_LinkedTreeData_T
{
	void *pvData;
	int iDataSize;

	JP_D_LinkedTree_T* hPrev;
	JP_D_LinkedTree_T* hNext;
} JP_D_LinkedTreeData_T;

typedef struct tag_JP_D_LinkedTree_IF_T
{
	JP_D_LINKED_TREE_ST_H hTreeSt;

	// Node Operations
	JP_D_LinkedTree_T* (*InsertNode)(JP_D_LINKED_TREE_ST_H hTreeSt, char *pstrName, void *pvData, int iDataSize);

	JP_D_LinkedTree_T* (*InsertNodeToChild)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hNode,
		char *pstrName,
		void *pvData,
		int iDataSize);

	JP_D_LinkedTree_T* (*InsertNodeToSibling)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hBaseNode,
		JP_TreeInsertType_E eInsertType,
		char *pstrName,
		void *pvData,
		int iDataSize);

	void (*RemoveAll)(JP_D_LINKED_TREE_ST_H hTreeSt);
	void (*DeleteNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	void (*DeleteNodeName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	void (*DeleteNodeData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	// Node Attributes
	int (*GetNodeCount)(JP_D_LINKED_TREE_ST_H hTreeSt);
	int (*GetChildNodeCount)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	int (*HasParentNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	int (*HasChildNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	JP_D_LinkedTree_T* (*GetRootNode)(JP_D_LINKED_TREE_ST_H hTreeSt);
	JP_D_LinkedTree_T* (*GetParentNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	JP_D_LinkedTree_T* (*GetChildNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	JP_D_LinkedTree_T* (*GetPrevNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	JP_D_LinkedTree_T* (*GetNextNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	int (*SetNodeName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, char *pstrName);
	int (*SetNodeData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, void *pvData, int iDataSize);
	char *(*GetNodeName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	void *(*GetNodeData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	int (*GetNodeDataSize)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	JP_D_LinkedTree_T* (*FindRootNode)(JP_D_LINKED_TREE_ST_H hTreeSt, char *pstrName);
	JP_D_LinkedTree_T* (*FindChildNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, char *pstrName);
	JP_D_LinkedTree_T* (*FindPrevNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, char *pstrName);
	JP_D_LinkedTree_T* (*FindNextNode)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, char *pstrName);

	// Att Operations
	JP_D_LinkedTree_T* (*InsertAtt)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hNode,
		char *pstrName,
		void *pvData,
		int iDataSize);

	JP_D_LinkedTree_T* (*InsertAttTo)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hBaseAtt,
		JP_TreeInsertType_E eInsertType,
		char *pstrName,
		void *pvData,
		int iDataSize);

	void (*DeleteAttAll)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	void (*DeleteAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);

	void (*DeleteAttName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);
	void (*DeleteAttData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);

	// Att Attributes
	int (*GetAttCount)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	int (*HasAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);

	JP_D_LinkedTree_T* (*GetFirstAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode);
	JP_D_LinkedTree_T* (*GetPrevAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, JP_D_LinkedTree_T* hAtt);
	JP_D_LinkedTree_T* (*GetNextAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, JP_D_LinkedTree_T* hAtt);

	int (*SetAttName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt, char *pstrName);
	int (*SetAttData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt, void *pvData, int iDataSize);
	char *(*GetAttName)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);
	void *(*GetAttData)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);
	int (*GetAttDataSize)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hAtt);

	JP_D_LinkedTree_T* (*FindAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, char *pstrName);
	JP_D_LinkedTree_T* (*FindPrevAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, JP_D_LinkedTree_T* hAtt, char *pstrName);
	JP_D_LinkedTree_T* (*FindNextAtt)(JP_D_LINKED_TREE_ST_H hTreeSt, JP_D_LinkedTree_T* hNode, JP_D_LinkedTree_T* hAtt, char *pstrName);

	// Graft
	JP_D_LinkedTree_T* (*Graft)(JP_D_LINKED_TREE_ST_H hTreeSt, struct tag_JP_D_LinkedTree_IF_T *pt_GT, JP_D_LinkedTree_T* h_GT_Node);

	JP_D_LinkedTree_T* (*GraftToChild)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hNode,
		struct tag_JP_D_LinkedTree_IF_T *pt_GT,
		JP_D_LinkedTree_T* h_GT_Node);

	JP_D_LinkedTree_T* (*GraftToSibling)(
		JP_D_LINKED_TREE_ST_H hTreeSt,
		JP_D_LinkedTree_T* hBaseNode,
		JP_TreeInsertType_E eInsertType,
		struct tag_JP_D_LinkedTree_IF_T *pt_GT,
		JP_D_LinkedTree_T* h_GT_Node);

} JP_D_LinkedTree_IF_T;

#endif	// __JP_D_LINKED_TREE_ST_H__

