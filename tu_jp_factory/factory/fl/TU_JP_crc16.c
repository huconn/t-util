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

#include "TU_JP_crc16.h"
#define POLYNOMIAL ((unsigned short)0x1021u)
#define TABLE_SIZE (256u)

static JP_BOOL_E G_bMadeTable_CRC16 = JP_FALSE;
static uint16_t G_uiTable_CRC16[256];

static void SF_Make_CRC16_Table(void)
{
	uint16_t i,j;
	uint16_t crc;
	for (i = 0u; i < TABLE_SIZE; ++i)
	{
		crc = i << 8u;
		for (j = 0u; j < 8u; ++j)
		{
			if((crc & 0x8000u) != 0u)
			{
				crc = ((crc << 1u) ^ POLYNOMIAL) & 0xFFFFu;
			}
			else
			{
				crc = crc << 1u;
			}
		}
		G_uiTable_CRC16[i] = crc;
	}
}

JP_RetType_UInt16_T JP_CRC16(const uint8_t *pucData, uint32_t uiDataSize)
{
	JP_RetType_UInt16_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.usRetVal = 0;

	if((pucData != NULL) && (uiDataSize > 0U))
	{
		if(G_bMadeTable_CRC16 == JP_FALSE)
		{
			SF_Make_CRC16_Table();
			G_bMadeTable_CRC16 = JP_TRUE;
		}

		while(uiDataSize > 0U)
		{
			uint16_t usExp1;
			uint16_t usExp2;
			uint32_t uiIndex;

			usExp1 = tRetVal.usRetVal >> 8;
			usExp2 = usExp1 ^ *pucData;

			uiIndex = (uint32_t)usExp2 & 0x000000FFU;

			usExp1 = tRetVal.usRetVal << 8;

			tRetVal.usRetVal = usExp1 ^ G_uiTable_CRC16[uiIndex];

			pucData++;

			uiDataSize--;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}