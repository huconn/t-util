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
#include <limits.h>
#include <stdint.h>

#include "TU_JP_CommonSt.h"
#include "TU_JP_ArithmeticOperation.h"

JP_Error_E JP_Add_Int32(const uint32_t uiOperandCnt, const int32_t *piOperands, int32_t *piResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (piOperands != NULL) && (piResult != NULL))
	{
		uint32_t i;

		*piResult = piOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			int32_t iOperand_A = *piResult;
			int32_t iOperand_B = piOperands[i];

			if(((iOperand_B > 0) && (iOperand_A > (INT_MAX - iOperand_B)))
				|| ((iOperand_B < 0) && (iOperand_A < (INT_MIN - iOperand_B))))
			{
				*piResult = 0;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*piResult = iOperand_A + iOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Add_Int64(const uint32_t uiOperandCnt, const int64_t *plOperands, int64_t *plResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (plOperands != NULL) && (plResult != NULL))
	{
		uint32_t i;

		*plResult = plOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			int64_t lOperand_A = *plResult;
			int64_t lOperand_B = plOperands[i];

			if(((lOperand_B > 0) && (lOperand_A > (INT64_MAX - lOperand_B)))
				|| ((lOperand_B < 0) && (lOperand_A < (INT64_MAX - lOperand_B))))
			{
				*plResult = 0;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*plResult = lOperand_A + lOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Add_UInt8(const uint32_t uiOperandCnt, const uint8_t *pucOperands, uint8_t *pucResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (pucOperands != NULL) && (pucResult != NULL))
	{
		uint32_t i;

		*pucResult = pucOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			uint8_t ucOperand_A = *pucResult;
			uint8_t ucOperand_B = pucOperands[i];

			if((255U/*UINT8_MAX*/ - ucOperand_A) < ucOperand_B)
			{
				*pucResult = 0U;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*pucResult = ucOperand_A + ucOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Add_UInt32(const uint32_t uiOperandCnt, const uint32_t *puiOperands, uint32_t *puiResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (puiOperands != NULL) && (puiResult != NULL))
	{
		uint32_t i;

		*puiResult = puiOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			uint32_t uiOperand_A = *puiResult;
			uint32_t uiOperand_B = puiOperands[i];

			if((UINT32_MAX - uiOperand_A) < uiOperand_B)
			{
				*puiResult = 0U;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*puiResult = uiOperand_A + uiOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Add_UInt64(const uint32_t uiOperandCnt, const uint64_t *pulOperands, uint64_t *pulResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (pulOperands != NULL) && (pulResult != NULL))
	{
		uint32_t i;

		*pulResult = pulOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			uint64_t ulOperand_A = *pulResult;
			uint64_t ulOperand_B = pulOperands[i];

			if((UINT64_MAX - ulOperand_A) < ulOperand_B)
			{
				*pulResult = 0UL;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*pulResult = ulOperand_A + ulOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Subtract_Int64(const uint32_t uiOperandCnt, const int64_t *plOperands, int64_t *plResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (plOperands != NULL) && (plResult != NULL))
	{
		uint32_t i;

		*plResult = plOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			int64_t lOperand_A = *plResult;
			int64_t lOperand_B = plOperands[i];

			if(((lOperand_B > 0) && (lOperand_A < (INT64_MIN + lOperand_B)))
				|| ((lOperand_B < 0) && (lOperand_A > (INT64_MAX + lOperand_B))))
			{
				*plResult = 0;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*plResult = lOperand_A - lOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Subtract_UInt32(const uint32_t uiOperandCnt, const uint32_t *puiOperands, uint32_t *puiResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (puiOperands != NULL) && (puiResult != NULL))
	{
		uint32_t i;

		*puiResult = puiOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			uint32_t uiOperand_A = *puiResult;
			uint32_t uiOperand_B = puiOperands[i];

			if(uiOperand_A < uiOperand_B)
			{
				*puiResult = 0;

				eRetErr = JP_Error_Overflow;
				break;
			}
			else
			{
				*puiResult = uiOperand_A - uiOperand_B;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_Multiply_Int32(const uint32_t uiOperandCnt, const int32_t *piOperands, int32_t *piResult)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((uiOperandCnt > 1U) && (piOperands != NULL) && (piResult != NULL))
	{
		uint32_t i;

		*piResult = piOperands[0];

		for(i = 1U; i < uiOperandCnt; i++)
		{
			int32_t iOperand_A = *piResult;
			int32_t iOperand_B = piOperands[i];

			if(iOperand_A > 0)
			{
				if(iOperand_B > 0)
				{
					if(iOperand_A > (INT32_MAX / iOperand_B))
					{
						eRetErr = JP_Error_Overflow;
					}
				}
				else
				{
					if(iOperand_B < (INT32_MIN / iOperand_A))
					{
						eRetErr = JP_Error_Overflow;
					}
				}
			}
			else
			{
				if(iOperand_B > 0)
				{
					if(iOperand_A < (INT32_MIN / iOperand_B))
					{
						eRetErr = JP_Error_Overflow;
					}
				}
				else
				{
					if((iOperand_A != 0) && (iOperand_B < (INT32_MAX / iOperand_A)))
					{
						eRetErr = JP_Error_Overflow;
					}
				}
			}

			if(eRetErr == JP_Error_None)
			{
				*piResult = iOperand_A * iOperand_B;
			}
			else
			{
				*piResult = 0;
				break;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

int64_t JP_Multiply_Int64(int64_t input1, int64_t input2)
{
	int64_t ret = 0;

	(void)__builtin_smull_overflow(input1, input2, &ret);

	return ret;
}

int64_t JP_Divide_Int64(int64_t input1, int64_t input2)
{
	int64_t ret = 0;

	if (input2 > 0) {
		ret = input1 / input2;
	}
	return ret;
}

