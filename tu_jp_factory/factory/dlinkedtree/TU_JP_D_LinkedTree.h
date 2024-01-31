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

#ifndef JP_D_LINKED_TREE_H_
#define JP_D_LINKED_TREE_H_

typedef enum
{
	JP_TREE_INSERT_TYPE_BEFORE = 0,
	JP_TREE_INSERT_TYPE_AFTER
} JP_TreeInsertType_E;

typedef struct JP_D_LinkedListItem_T JP_D_LinkedTreeAtt_T;
typedef struct JP_D_LinkedList_T JP_D_LinkedTree_T;
typedef struct JP_D_LinkedList_IF_T JP_D_LinkedTreeSt_T;

typedef struct JP_D_LinkedTree_IF_T JP_D_LinkedTree_IF_T;

struct JP_D_LinkedTree_IF_T
{
	JP_D_LinkedTreeSt_T *ptTreeSt;

	// Node Operations
	JP_D_LinkedTree_T *(*InsertNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const char *pcstrName, void *pvData, int iDataSize);

	JP_D_LinkedTree_T *(*InsertNodeToChild)(
		const JP_D_LinkedTreeSt_T *pctTreeSt,
		JP_D_LinkedTree_T *ptNode,
		const char *pcstrName,
		void *pvData,
		int iDataSize);

	JP_D_LinkedTree_T *(*InsertNodeToSibling)(
		const JP_D_LinkedTreeSt_T *pctTreeSt,
		JP_D_LinkedTree_T *ptBaseNode,
		JP_TreeInsertType_E eInsertType,
		const char *pcstrName,
		void *pvData,
		int iDataSize);

	JP_Error_E (*DeleteNodeAll)(JP_D_LinkedTreeSt_T *ptTreeSt);
	JP_Error_E (*DeleteNode)(JP_D_LinkedTreeSt_T *ptTreeSt, JP_D_LinkedTree_T *ptNode);

	JP_Error_E (*DeleteNodeName)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTree_T *ptNode);
	JP_Error_E (*DeleteNodeData)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	// Node Attributes
	uint32_t (*GetNodeCount)(const JP_D_LinkedTreeSt_T *pctTreeSt);
	uint32_t (*GetChildNodeCount)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_BOOL_E (*HasParentNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_BOOL_E (*HasChildNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_D_LinkedTree_T *(*GetRootNode)(const JP_D_LinkedTreeSt_T *pctTreeSt);
	JP_D_LinkedTree_T *(*GetParentNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_D_LinkedTree_T *(*GetChildNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_D_LinkedTree_T *(*GetPrevNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_D_LinkedTree_T *(*GetNextNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_Error_E (*SetNodeName)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTree_T *ptNode, const char *pcstrName);
	JP_Error_E (*SetNodeData)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, void *pvData, int iDataSize);
	char *(*GetNodeName)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	void *(*GetNodeData)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	int (*GetNodeDataSize)(const JP_D_LinkedTreeSt_T *cptTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_D_LinkedTree_T *(*FindRootNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const char *pcstrName);
	JP_D_LinkedTree_T *(*FindChildNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName);
	JP_D_LinkedTree_T *(*FindPrevNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName);
	JP_D_LinkedTree_T *(*FindNextNode)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName);

	// Att Operations
	JP_D_LinkedTreeAtt_T *(*InsertAtt)(
		const JP_D_LinkedTreeSt_T *pctTreeSt, 
		const JP_D_LinkedTree_T *pctNode, 
		const char *pcstrName, 
		void *pvData, 
		int iDataSize);

	JP_D_LinkedTreeAtt_T *(*InsertAttTo)(
		const JP_D_LinkedTreeSt_T *pctTreeSt, 
		JP_D_LinkedTreeAtt_T *ptBaseAtt, 
		JP_TreeInsertType_E eInsertType, 
		const char *pcstrName, 
		void *pvData, 
		int iDataSize);

	JP_Error_E (*DeleteAttAll)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_Error_E (*DeleteAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt);

	JP_Error_E (*DeleteAttName)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt);
	JP_Error_E (*DeleteAttData)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt);

	// Att Attributes
	uint32_t (*GetAttCount)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_BOOL_E (*HasAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);

	JP_D_LinkedTreeAtt_T *(*GetFirstAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode);
	JP_D_LinkedTreeAtt_T *(*GetPrevAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt);
	JP_D_LinkedTreeAtt_T *(*GetNextAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt);

	JP_Error_E (*SetAttName)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt, const char *pcstrName);
	JP_Error_E (*SetAttData)(const JP_D_LinkedTreeSt_T *pctTreeSt, JP_D_LinkedTreeAtt_T *ptAtt, void *pvData, int iDataSize);
	char *(*GetAttName)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt);
	void *(*GetAttData)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt);
	int (*GetAttDataSize)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTreeAtt_T *pctAtt);

	JP_D_LinkedTreeAtt_T *(*FindAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const char *pcstrName);
	JP_D_LinkedTreeAtt_T *(*FindPrevAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt, const char *pcstrName);
	JP_D_LinkedTreeAtt_T *(*FindNextAtt)(const JP_D_LinkedTreeSt_T *pctTreeSt, const JP_D_LinkedTree_T *pctNode, const JP_D_LinkedTreeAtt_T *pctAtt, const char *pcstrName);

	// Graft
	JP_D_LinkedTree_T *(*GraftNode)(JP_D_LinkedTreeSt_T *ptTreeSt, JP_D_LinkedTree_IF_T *pt_GT, const JP_D_LinkedTree_T *pct_GT_Node);

	JP_D_LinkedTree_T *(*GraftNodeToChild)(
		JP_D_LinkedTreeSt_T *ptTreeSt,
		JP_D_LinkedTree_T *ptNode,
		JP_D_LinkedTree_IF_T *pt_GT,
		const JP_D_LinkedTree_T *pct_GT_Node);

	JP_D_LinkedTree_T *(*GraftNodeToSibling)(
		JP_D_LinkedTreeSt_T *ptTreeSt,
		JP_D_LinkedTree_T *ptBaseNode,
		JP_TreeInsertType_E eInsertType,
		JP_D_LinkedTree_IF_T *pt_GT,
		const JP_D_LinkedTree_T *pct_GT_Node);

};

JP_D_LinkedTree_IF_T *JP_CreateTree(void);
JP_Error_E JP_DestroyTree(JP_D_LinkedTree_IF_T *pt_IF);

#endif	// JP_D_LINKED_TREE_H_