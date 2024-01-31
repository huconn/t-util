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

#ifndef JP_TIMER_H_
#define JP_TIMER_H_

typedef struct JP_Timer_T JP_Timer_T;

typedef void (*pfnTimerCallBack)(uint32_t uiTimer_ID, void *pvUserPrivate);

typedef struct
{
	JP_Timer_T *hTimer;

	JP_Error_E (*EnableTimer)(JP_Timer_T *hTimer, uint32_t uiTimer_ID, int32_t iMilliSecond, pfnTimerCallBack fnTimerCallBack, void *pvUserPrivate);
	JP_Error_E (*DisableTimer)(JP_Timer_T *hTimer);
} JP_Timer_IF_T;

JP_Timer_IF_T *JP_CreateTimer(void);
JP_Error_E JP_DestroyTimer(JP_Timer_IF_T *ptTimer_IF);

#endif	// JP_TIMER_H_