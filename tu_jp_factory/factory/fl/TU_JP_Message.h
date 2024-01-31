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

#ifndef JP_MESSAGE_H_
#define JP_MESSAGE_H_

// JP_MSG_OPT
#define JP_MSG_OPT_REMOVE_DUPLICATED	0x00000001U
#define JP_MSG_OPT_PUSH_FIRST			0x00000002U

typedef struct JP_MessageSt_T JP_MessageSt_T;

typedef struct
{
	int iParam;
	void *pvParam;

	uint32_t uiMessage;
} JP_Message_T;

typedef int32_t (*pfnMessageHandler)(const JP_Message_T *ptMessage);
typedef void (*pfnMessageFlushHandler)(const JP_Message_T *ptMessage);

typedef struct
{
	JP_MessageSt_T *ptMessageSt;
	unsigned int (*GetMessageCount)(const JP_MessageSt_T *pctMessageSt);
	JP_RetType_Int32_T (*SendMessage)(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int iParam, void *pvParam);
	JP_Error_E (*PostMessage)(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int iParam, void *pvParam);
	JP_Error_E (*PostMessageEx)(const JP_MessageSt_T *pctMessageSt, uint32_t uiMessage, int iParam, void *pvParam, uint32_t uiOption);
	void (*FlushMessage)(const JP_MessageSt_T *pctMessageSt);
} JP_Message_IF_T;

JP_Message_IF_T *JP_CreateMessage(pfnMessageHandler fnMessageHandler, pfnMessageFlushHandler fnMessageFlushHandler);
JP_Error_E JP_DestroyMessage(JP_Message_IF_T *pt_IF);

#endif	// JP_MESSAGE_H_