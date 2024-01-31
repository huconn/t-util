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

#ifndef JP_ARITHMETIC_OPERATION_H_
#define JP_ARITHMETIC_OPERATION_H_

JP_Error_E JP_Add_Int32(const uint32_t uiOperandCnt, const int32_t *piOperands, int32_t *piResult);
JP_Error_E JP_Add_Int64(const uint32_t uiOperandCnt, const int64_t *plOperands, int64_t *plResult);

JP_Error_E JP_Add_UInt8(const uint32_t uiOperandCnt, const uint8_t *pucOperands, uint8_t *pucResult);
JP_Error_E JP_Add_UInt32(const uint32_t uiOperandCnt, const uint32_t *puiOperands, uint32_t *puiResult);
JP_Error_E JP_Add_UInt64(const uint32_t uiOperandCnt, const uint64_t *pulOperands, uint64_t *pulResult);

JP_Error_E JP_Subtract_Int64(const uint32_t uiOperandCnt, const int64_t *plOperands, int64_t *plResult);

JP_Error_E JP_Subtract_UInt32(const uint32_t uiOperandCnt, const uint32_t *puiOperands, uint32_t *puiResult);

JP_Error_E JP_Multiply_Int32(const uint32_t uiOperandCnt, const int32_t *piOperands, int32_t *piResult);
int64_t JP_Multiply_Int64(int64_t input1, int64_t input2);
int64_t JP_Divide_Int64(int64_t input1, int64_t input2);

#endif  // JP_ARITHMETIC_OPERATION_H_