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

#ifndef JP_CONSOLE_H_
#define JP_CONSOLE_H_

typedef struct JP_ConsoleSt_T JP_ConsoleSt_T;

typedef void (*pfn_Console_CallBack)(unsigned char *pucBuffer, void *pvPrivate);

typedef struct
{
	JP_ConsoleSt_T *ptConsoleSt;

	JP_Error_E (*Start)(JP_ConsoleSt_T *ptConsoleSt);
	JP_Error_E (*Stop)(JP_ConsoleSt_T *ptConsoleSt);

	uint32_t (*Divide)(unsigned char *pucBuffer, char **ppucArgs, uint32_t uiArgsMax);
} JP_Console_IF_T;

JP_Console_IF_T *JP_CreateConsole(const char *pcstrDev, pfn_Console_CallBack fnCallBack, void *pvPrivate);
JP_Error_E JP_DestroyConsole(JP_Console_IF_T *pt_IF);

#endif	// JP_CONSOLE_H_