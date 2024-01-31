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

#ifndef TCC_CSV_REPORT_H
#define TCC_CSV_REPORT_H


#ifdef __cplusplus
extern "C" {
#endif


#define TUCR_MAX_SHORT_STR_LEN 	(64)


typedef struct
{
	char strReportFileName[TUCR_MAX_SHORT_STR_LEN];
	char strDept[TUCR_MAX_SHORT_STR_LEN];
	char strModule[TUCR_MAX_SHORT_STR_LEN];
} TUCR_Init_T;

typedef struct
{
	char strMainCat[TUCR_MAX_SHORT_STR_LEN];
	char strSubCat[TUCR_MAX_SHORT_STR_LEN];
	int iIsFailed;
} TUCR_Set_T;

typedef struct
{
	char strReportFileName[TUCR_MAX_SHORT_STR_LEN];

	char strTestStart[TUCR_MAX_SHORT_STR_LEN];
	char strTestEnd[TUCR_MAX_SHORT_STR_LEN];

	int iTotalTestCnt;
	int iFailCnt;
	int iPassCnt;
} TUCR_Handle_T;


TUCR_Handle_T *tucr_init(TUCR_Init_T *ptReportInit);
void tucr_deinit(TUCR_Handle_T *ptHandle);
int tucr_set_a_test_case_result(TUCR_Handle_T *ptHandle, TUCR_Set_T *ptReportSet);


#ifdef __cplusplus
}
#endif

#endif // TCC_CSV_REPORT_H