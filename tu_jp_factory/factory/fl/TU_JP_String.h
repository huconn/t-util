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

#ifndef JP_STRING_H_
#define JP_STRING_H_

typedef struct JP_String_T JP_String_T;

typedef struct JP_String_IF_T JP_String_IF_T;

struct JP_String_IF_T
{
	JP_String_T *ptString;

	JP_Error_E (*SetAt)(const JP_String_T *pctString, uint32_t uiIndex, char chChar);

	char *(*GetBuffer)(const JP_String_T *pctString);
	uint32_t (*GetLength)(const JP_String_T *pctString);
	char (*GetAt)(const JP_String_T *pctString, uint32_t uiIndex);

	JP_BOOL_E (*IsEmpty)(const JP_String_T *pctString);
	JP_Error_E (*Empty)(JP_String_T *ptString);

	JP_Error_E (*Format)(JP_String_T *ptString, const char *pcstrFormat, ...);

	JP_Error_E (*AddCh)(JP_String_T *ptString, char cChar);
	JP_Error_E (*AddStr)(JP_String_T *ptString, const char *pcstrFormat, ...);

	JP_Error_E (*InsertCh)(JP_String_T *ptString, uint32_t uiIndex, char chChar);
	JP_Error_E (*InsertStr)(JP_String_T *ptString, uint32_t uiIndex, const char *pcstrString);

	JP_Error_E (*Delete)(JP_String_T *ptString, uint32_t uiIndex, uint32_t uiCount);
	JP_Error_E (*Remove)(JP_String_T *ptString, char chChar);

	int32_t (*Compare)(const JP_String_T *pctString, const char *pcstrString);

	char *(*MidByBuf)(const JP_String_T *pctString, uint32_t uiFirst);
	char *(*MidToByBuf)(const JP_String_T *pctString, uint32_t uiFirst, uint32_t uiCount);
	char *(*LeftByBuf)(const JP_String_T *pctString, uint32_t uiCount);
	char *(*RightByBuf)(const JP_String_T *pctString, uint32_t uiCount);

	JP_String_IF_T *(*MidByIns)(const JP_String_T *pctString, uint32_t uiFirst);
	JP_String_IF_T *(*MidToByIns)(const JP_String_T *pctString, uint32_t uiFirst, uint32_t uiCount);
	JP_String_IF_T *(*LeftByIns)(const JP_String_T *pctString, uint32_t uiCount);
	JP_String_IF_T *(*RightByIns)(const JP_String_T *pctString, uint32_t uiCount);

	JP_RetType_UInt32_T (*FindCh)(const JP_String_T *pctString, char chChar);
	JP_RetType_UInt32_T (*FindStr)(const JP_String_T *pctString, const char *pcstrString);
	JP_RetType_UInt32_T (*FindChFrom)(const JP_String_T *pctString, char chChar, uint32_t uiStart);
	JP_RetType_UInt32_T (*FindStrFrom)(const JP_String_T *pctString, const char *pcstrString, uint32_t uiStart);
	JP_RetType_UInt32_T (*ReverseFind)(const JP_String_T *pctString, char chChar);
	JP_RetType_UInt32_T (*FindOneOf)(const JP_String_T *pctString, const char *pcstrString);
};

JP_String_IF_T *JP_CreateString(const char *pcstrString);
JP_Error_E JP_DestroyString(JP_String_IF_T *ptString_IF);

#endif	// JP_STRING_H_