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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>

#include <tu_csv_report.h>


#define TUCR_DONOTHING(fmt, ...) 	;
#define TUCR_CAST(tar, src) 		(void)memcpy((void*)&(tar), (void*)&(src), sizeof(void*))

#define TUCR_TITLE_DEPT				"Testing Department"
#define TUCR_TITLE_MODULE			"Module Name"
#define TUCR_TITLE_START_DATE		"Test Start Date"
#define TUCR_TITLE_END_DATE			"Test End Date"
#define TUCR_TITLE_TOTAL_RESULT		"Total Result"
#define TUCR_TITLE_TEST_NUMBER		"NUM"
#define TUCR_TITLE_TEST_RESULT		"Result"
#define TUCR_TITLE_MAIN_CATEGORY	"MAIN Category"
#define TUCR_TITLE_SUB_CATEGORY		"SUB Category"
#define TUCR_TITLE_TOTAL 			"TOTAL"
#define TUCR_TITLE_PASS 			"PASS"
#define TUCR_TITLE_FAIL 			"FAIL"

#define TUCR_STR_EMPTY	 			" "
#define TUCR_STR_PASS 				"PASS"
#define TUCR_STR_FAIL 				"FAIL"

#define TUCR_MIN_STR_LEN 			(2)
#define TUCR_MAX_CSV_LINE_LEN 		(4)
#define TUCR_MAX_STR_LEN 			(TUCR_MAX_SHORT_STR_LEN * TUCR_MAX_CSV_LINE_LEN)
#define TUCR_MAX_CSV_STR_LEN 		(TUCR_MAX_STR_LEN * 2)


static void tucr_write_test_title(TUCR_Handle_T *ptHandle, TUCR_Init_T *ptReportInit);
static void tucr_get_time_info_str(TUCR_Handle_T *ptHandle, char strTime[TUCR_MAX_SHORT_STR_LEN]);
static void tucr_write_a_result(TUCR_Handle_T *ptHandle, char strMainCat[TUCR_MAX_SHORT_STR_LEN], char strSubCat[TUCR_MAX_SHORT_STR_LEN], int iIsPass);
static void tucr_write_result_cnt(TUCR_Handle_T *ptHandle, char strTitle[TUCR_MAX_SHORT_STR_LEN], int iCnt);
static void tucr_write_a_line(TUCR_Handle_T *ptHandle, bool bIsNewFile, char str0[TUCR_MAX_SHORT_STR_LEN], char str1[TUCR_MAX_SHORT_STR_LEN], char str2[TUCR_MAX_SHORT_STR_LEN], char str3[TUCR_MAX_SHORT_STR_LEN]);
static void tucr_write_empty_line(TUCR_Handle_T *ptHandle, char strFile[TUCR_MAX_CSV_STR_LEN]);
static void tucr_write_str_to_file(TUCR_Handle_T *ptReport, bool bIsNewFile, char strContent[TUCR_MAX_CSV_LINE_LEN][TUCR_MAX_SHORT_STR_LEN]);


TUCR_Handle_T *tucr_init(TUCR_Init_T *ptReportInit)
{
	TUCR_Handle_T *ptHandle = NULL;

	if (ptReportInit != NULL) {
		void *ptTmpPtr = g_malloc0(sizeof(TUCR_Handle_T));

		if (ptTmpPtr != NULL) {
			TUCR_CAST(ptHandle, ptTmpPtr);
		}

		if ((strlen(ptReportInit->strReportFileName) > 0U) && (strlen(ptReportInit->strDept) > 0U) && (strlen(ptReportInit->strModule) > 0U)) {
			(void)strncpy(ptHandle->strReportFileName, ptReportInit->strReportFileName, (TUCR_MAX_SHORT_STR_LEN - 1));
			tucr_get_time_info_str(ptHandle, ptHandle->strTestStart);
			tucr_write_test_title(ptHandle, ptReportInit);
		} else {
			(void)g_printf("[%s(%d)] ERROR: %s \n", __func__, __LINE__, ptHandle->strReportFileName);
			tucr_deinit(ptHandle);
		}
	}

	return ptHandle;
}

void tucr_deinit(TUCR_Handle_T *ptHandle)
{
    if (ptHandle != NULL) {
		tucr_get_time_info_str(ptHandle, ptHandle->strTestEnd);

		tucr_write_empty_line(ptHandle, ptHandle->strReportFileName);
		tucr_write_a_line(ptHandle, false, TUCR_TITLE_START_DATE, ptHandle->strTestStart, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
		tucr_write_a_line(ptHandle, false, TUCR_TITLE_END_DATE, ptHandle->strTestEnd, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
		tucr_write_empty_line(ptHandle, ptHandle->strReportFileName);

		tucr_write_a_line(ptHandle, false, TUCR_TITLE_TOTAL_RESULT, TUCR_STR_EMPTY, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
		tucr_write_result_cnt(ptHandle, TUCR_TITLE_TOTAL, ptHandle->iTotalTestCnt);
		tucr_write_result_cnt(ptHandle, TUCR_TITLE_PASS, ptHandle->iPassCnt);
		tucr_write_result_cnt(ptHandle, TUCR_TITLE_FAIL, ptHandle->iFailCnt);

		g_free(ptHandle);
	}
}

int tucr_set_a_test_case_result(TUCR_Handle_T *ptHandle, TUCR_Set_T *ptReportSet)
{
	int ret = -1;

	if ((ptHandle != NULL) && (ptReportSet != NULL)) {
		int iIsPass = 0;

		if (ptReportSet->iIsFailed == 0) {
			iIsPass = 1;
		}

		tucr_write_a_result(ptHandle, ptReportSet->strMainCat, ptReportSet->strSubCat, iIsPass);

		ptHandle->iTotalTestCnt++;

		if (ptReportSet->iIsFailed == 0) {
			ptHandle->iPassCnt++;
		} else {
			ptHandle->iFailCnt++;
		}
	}

	return ret;
}

static void tucr_write_test_title(TUCR_Handle_T *ptHandle, TUCR_Init_T *ptReportInit)
{
	if (ptHandle != NULL) {
		tucr_write_a_line(ptHandle, true, TUCR_TITLE_DEPT, ptReportInit->strDept, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
		tucr_write_a_line(ptHandle, false, TUCR_TITLE_MODULE, ptReportInit->strModule, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
		tucr_write_empty_line(ptHandle, ptHandle->strReportFileName);
		tucr_write_a_line(ptHandle, false, TUCR_TITLE_TEST_NUMBER, TUCR_TITLE_MAIN_CATEGORY, TUCR_TITLE_SUB_CATEGORY, TUCR_TITLE_TEST_RESULT);
	}
}

static void tucr_get_time_info_str(TUCR_Handle_T *ptHandle, char strTime[TUCR_MAX_SHORT_STR_LEN])
{
	if (ptHandle != NULL) {
		time_t tTime = time(NULL);
		struct tm tmTime = *localtime(&tTime);

		(void)g_snprintf(strTime, TUCR_MAX_SHORT_STR_LEN, "%d-%d-%d %d:%d:%d",
				(tmTime.tm_year + 1900), (tmTime.tm_mon + 1), tmTime.tm_mday,
				tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
	}
}

static void tucr_write_empty_line(TUCR_Handle_T *ptHandle, char strFile[TUCR_MAX_CSV_STR_LEN])
{
	tucr_write_a_line(ptHandle, false, TUCR_STR_EMPTY, TUCR_STR_EMPTY, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
}

static void tucr_write_a_result(TUCR_Handle_T *ptHandle, char strMainCat[TUCR_MAX_SHORT_STR_LEN], char strSubCat[TUCR_MAX_SHORT_STR_LEN], int iIsPass)
{
	if (ptHandle != NULL) {
		int iTestNum = ptHandle->iTotalTestCnt + 1;

		char strResult[TUCR_MAX_SHORT_STR_LEN] = "";
		char strTestNum[TUCR_MAX_SHORT_STR_LEN] = "";

		if (iIsPass == 1) {
			(void)strncpy(strResult, TUCR_STR_PASS, (TUCR_MAX_SHORT_STR_LEN - 1));
		} else {
			(void)strncpy(strResult, TUCR_STR_FAIL, (TUCR_MAX_SHORT_STR_LEN - 1));
		}

		(void)g_snprintf(strTestNum, TUCR_MAX_SHORT_STR_LEN, "%d", iTestNum);

		tucr_write_a_line(ptHandle, false, strTestNum, strMainCat, strSubCat, strResult);
	}
}

static void tucr_write_result_cnt(TUCR_Handle_T *ptHandle, char strTitle[TUCR_MAX_SHORT_STR_LEN], int iCnt)
{
	if (ptHandle != NULL) {
		char strResult[TUCR_MAX_SHORT_STR_LEN] = "";
		(void)g_snprintf(strResult, TUCR_MAX_SHORT_STR_LEN, "%d", iCnt);

		tucr_write_a_line(ptHandle, false, strTitle, strResult, TUCR_STR_EMPTY, TUCR_STR_EMPTY);
	}
}

static void tucr_write_a_line(TUCR_Handle_T *ptHandle, bool bIsNewFile,
	char str0[TUCR_MAX_SHORT_STR_LEN], char str1[TUCR_MAX_SHORT_STR_LEN], char str2[TUCR_MAX_SHORT_STR_LEN], char str3[TUCR_MAX_SHORT_STR_LEN])
{
	if (ptHandle != NULL) {
		char strContent[TUCR_MAX_CSV_LINE_LEN][TUCR_MAX_SHORT_STR_LEN];

		(void)strncpy(strContent[0], str0, (TUCR_MAX_SHORT_STR_LEN - 1));
		(void)strncpy(strContent[1], str1, (TUCR_MAX_SHORT_STR_LEN - 1));
		(void)strncpy(strContent[2], str2, (TUCR_MAX_SHORT_STR_LEN - 1));
		(void)strncpy(strContent[3], str3, (TUCR_MAX_SHORT_STR_LEN - 1));

		tucr_write_str_to_file(ptHandle, bIsNewFile, strContent);
	}
}

static void tucr_write_str_to_file(TUCR_Handle_T *ptHandle, bool bIsNewFile, char strContent[TUCR_MAX_CSV_LINE_LEN][TUCR_MAX_SHORT_STR_LEN])
{
	if (ptHandle != NULL) {
		char strLine[TUCR_MAX_STR_LEN] = "";

		(void)g_snprintf(strLine, TUCR_MAX_STR_LEN, "%s, %s, %s, %s", strContent[0], strContent[1], strContent[2], strContent[3]);

		if (strlen(strLine) > TUCR_MIN_STR_LEN) {
			char strSystemCmd[TUCR_MAX_CSV_STR_LEN] = "";

			if (bIsNewFile == true) {
				(void)g_snprintf(strSystemCmd, TUCR_MAX_CSV_STR_LEN, "echo \"%s\" > %s", strLine, ptHandle->strReportFileName);
			} else {
				(void)g_snprintf(strSystemCmd, TUCR_MAX_CSV_STR_LEN, "echo \"%s\" >> %s", strLine, ptHandle->strReportFileName);
			}

			(void)system(strSystemCmd);
		}
	}
}