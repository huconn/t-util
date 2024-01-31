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

#ifndef JP_COMMON_ST_H_
#define JP_COMMON_ST_H_

#include <stdint.h>

typedef enum
{
	JP_FALSE = 0,
	JP_TRUE
} JP_BOOL_E;

typedef enum
{
	JP_Error_None = 0, 	
	JP_Error_BadParameter, 	
	JP_Error_InsufficientResources, 
	JP_Error_ReleaseResource, 
	JP_Error_Overflow, 
	JP_Error_Signal, 
	JP_Error_Timeout, 
	JP_Error_Wait, 
	JP_Error_TimeWait, 
	JP_Error_GetTime, 
	JP_Error_QueueFull, 
	JP_Error_QueueEmpty, 
	JP_Error_EndOfStream, 
	JP_Error_TypeCasting, 
	JP_Error_NotEnabled, 
	JP_Error_BufferEmpty, 
	JP_Error_NotFound, 
	JP_Error_Undefined
} JP_Error_E;

typedef struct
{
	JP_Error_E eError;
	JP_BOOL_E bRetVal;
} JP_RetType_Bool_T;

typedef struct
{
	JP_Error_E eError;
	char cRetVal;
} JP_RetType_Char_T;

typedef struct
{
	JP_Error_E eError;
	double dRetVal;
} JP_RetType_Double_T;

typedef struct
{
	JP_Error_E eError;
	int32_t iRetVal;
} JP_RetType_Int32_T;

typedef struct
{
	JP_Error_E eError;
	uint16_t usRetVal;
} JP_RetType_UInt16_T;

typedef struct
{
	JP_Error_E eError;
	uint32_t uiRetVal;
} JP_RetType_UInt32_T;

typedef struct
{
	JP_Error_E eError;
	uint64_t ulRetVal;
} JP_RetType_UInt64_T;

typedef struct
{
	JP_Error_E eError;
	char *pcRetVal;
} JP_RetType_pChar_T;

#endif	// JP_COMMON_ST_H_