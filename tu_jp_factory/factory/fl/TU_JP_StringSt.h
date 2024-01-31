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
 *	JP_StringSt.h
 * \brief
 *
 * \author
 *	Name 	: Jang Won Seok
 *	E-mail	: highwire77@gmail.com
 ************************************************************************
 */
#ifndef __JP_STRING_ST_H__
#define __JP_STRING_ST_H__

typedef void *JP_STRING_ST_H;

typedef struct tag_JP_StringSt_T
{
	char *pstrBuffer;
} JP_StringSt_T;

typedef struct tag_JP_String_IF_T
{
	JP_STRING_ST_H hStringSt;

	void (*SetAt)(JP_STRING_ST_H hStringSt, int iIndex, char chChar);

	char *(*GetBuffer)(JP_STRING_ST_H hStringSt);
	int (*GetLength)(JP_STRING_ST_H hStringSt);
	char (*GetAt)(JP_STRING_ST_H hStringSt, int iIndex);

	int (*IsEmpty)(JP_STRING_ST_H hStringSt);
	void (*Empty)(JP_STRING_ST_H hStringSt);

	void (*Format)(JP_STRING_ST_H hStringSt, const char *pcstrFormat, ...);
	void (*AddCh)(JP_STRING_ST_H hStringSt, char chChar);
	void (*AddStr)(JP_STRING_ST_H hStringSt, const char *pcstrFormat, ...);
	int (*InsertCh)(JP_STRING_ST_H hStringSt, int iIndex, char chChar);
	int (*InsertStr)(JP_STRING_ST_H hStringSt, int iIndex, const char *pcstrString);

	int (*Delete)(JP_STRING_ST_H hStringSt, int iIndex, int iCount);
	int (*Remove)(JP_STRING_ST_H hStringSt, char chChar);

	int (*Compare)(JP_STRING_ST_H hStringSt, const char *pcstrString);

	char *(*MidByBuf)(JP_STRING_ST_H hStringSt, int iFirst);
	char *(*MidToByBuf)(JP_STRING_ST_H hStringSt, int iFirst, int iCount);
	char *(*LeftByBuf)(JP_STRING_ST_H hStringSt, int iCount);
	char *(*RightByBuf)(JP_STRING_ST_H hStringSt, int iCount);

	struct tag_JP_String_IF_T *(*MidByIns)(JP_STRING_ST_H hStringSt, int iFirst);
	struct tag_JP_String_IF_T *(*MidToByIns)(JP_STRING_ST_H hStringSt, int iFirst, int iCount);
	struct tag_JP_String_IF_T *(*LeftByIns)(JP_STRING_ST_H hStringSt, int iCount);
	struct tag_JP_String_IF_T *(*RightByIns)(JP_STRING_ST_H hStringSt, int iCount);

	int (*FindCh)(JP_STRING_ST_H hStringSt, char chChar);
	int (*FindStr)(JP_STRING_ST_H hStringSt, const char *pcstrString);
	int (*FindChFrom)(JP_STRING_ST_H hStringSt, char chChar, int iStart);
	int (*FindStrFrom)(JP_STRING_ST_H hStringSt, const char *pcstrString, int iStart);
	int (*ReverseFind)(JP_STRING_ST_H hStringSt, char chChar);
	int (*FindOneOf)(JP_STRING_ST_H hStringSt, const char *pcstrString);
} JP_String_IF_T;

#endif	// __JP_STRING_ST_H__
