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

#ifndef JP_SEMAPHORE_H_
#define JP_SEMAPHORE_H_

typedef struct JP_Semaphore_T JP_Semaphore_T;

JP_Error_E JP_ObtainSemaphore(JP_Semaphore_T *ptSemaphore, int iTimeout);
JP_Error_E JP_ReleaseSemaphore(JP_Semaphore_T *ptSemaphore);

JP_Semaphore_T *JP_CreateSemaphore(uint32_t uiInitialCount, uint32_t uiMaximumCount);
JP_Error_E JP_DestroySemaphore(JP_Semaphore_T *ptSemaphore);

#endif	// JP_SEMAPHORE_H_