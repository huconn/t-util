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

#ifndef JP_TYPE_CAST_H_
#define JP_TYPE_CAST_H_

#define P_CAST(tar, src) (void)memcpy((void*)&(tar), (void*)&(src), sizeof(void*))

JP_RetType_UInt32_T JP_TypeCast_Int16ToUInt32(int16_t sValue);
JP_RetType_UInt64_T JP_TypeCast_Int32ToUInt64(int32_t iValue);

JP_RetType_Int32_T JP_TypeCast_UInt64ToInt32(uint64_t ulValue);
JP_RetType_UInt32_T JP_TypeCast_UInt64ToUInt32(uint64_t ulValue);

#endif	// JP_TYPE_CAST_H_