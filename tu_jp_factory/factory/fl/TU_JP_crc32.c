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
#include <stdint.h>

#include "TU_JP_CommonSt.h"

#include "TU_JP_crc32.h"

#define POLYNOMIAL 0x04c11db7U

static JP_BOOL_E G_bMadeTable_CRC32 = JP_FALSE;
static uint32_t G_uiTable_CRC32[256];

static void SF_Make_CRC32_Table(void)
{
	uint32_t i, j;
	uint32_t uiAccum;

	for(i = 0U; i < 256U; i++)
	{
		uiAccum = (i << 24U);

		for(j = 0U; j < 8U; j++)
		{
			if((uiAccum & 0x80000000U) != 0U)
			{
				uiAccum = (uiAccum << 1U) ^ POLYNOMIAL;
			}
			else
			{
				uiAccum = (uiAccum << 1U);
			}
		}

		G_uiTable_CRC32[i] = uiAccum;
	}
}

static uint32_t SF_Calc_CRC(uint32_t uiAccum, const uint8_t *pucData, uint32_t uiDataSize)
{
	uint32_t i, j;

	for(j = 0U; j < uiDataSize; j++)
	{
		i = ((uiAccum >> 24U) ^ *pucData) & 0x000000FFU;

		pucData++;

		uiAccum = (uiAccum << 8U) ^ G_uiTable_CRC32[i];
	}

	return uiAccum;
}

JP_RetType_UInt32_T JP_CRC32(const uint8_t *pucData, uint32_t uiDataSize)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((pucData != NULL) && (uiDataSize > 0U))
	{
		if(G_bMadeTable_CRC32 == JP_FALSE)
		{
			SF_Make_CRC32_Table();
			G_bMadeTable_CRC32 = JP_TRUE;
		}

		tRetVal.uiRetVal = SF_Calc_CRC(0xFFFFFFFFU, pucData, uiDataSize);
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}