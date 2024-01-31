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
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>

#include <glib.h>
#include <glib/gprintf.h>

#include <tu_calc.h>


size_t popcount(uintmax_t num)
{
	size_t precision = 0U;
	while (num != 0U)
	{
		if ((num % 2U) == 1U)
		{
			precision = tu_calc_ul_add_ulul(precision, 1UL);
		}
		num >>= 1;
	}
	return precision;
}

double tu_calc_pow(double input1, double input2)
{
	bool bIsError = false;
	double result;

	if (((input1 == 0.0f) && (islessequal(input2, 0.0) == 1)) || (isless(input1, 0.0) == 1)) {
		bIsError = true;
	}

	(void)feclearexcept(FE_ALL_EXCEPT);
	errno = 0;
	result = pow(input1, input2);

	if ((result == +HUGE_VAL) || (result == -HUGE_VAL)){
		bIsError = true;
	}

	errno = 0;
	if (fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW) != 0){
		bIsError = true;
	}

	if (bIsError == true) {
		result = 0.0;
	}

	return result;
}

unsigned int tu_calc_leftshift_unsigned_type(unsigned int input1, unsigned int input2)
{
	unsigned int result = 0U;
	if (input2 < PRECISION(UINT32_MAX))
	{
		result = input1 << input2;
	}
	return result;
}

unsigned int tu_calc_rightshift_unsigned_type(unsigned int input1, unsigned int input2)
{
	unsigned int result = 0U;
	if (input2 < PRECISION(UINT32_MAX))
	{
		result = input1 >> input2;
	}
	return result;
}

unsigned char tu_calc_uc_cov_ui(unsigned int input1)
{
	unsigned char ret = 0U;

	if (input1 < 255U /*UINT8_MAX*/) {
		ret = (unsigned char)input1;
	}

	return ret;
}

int tu_calc_i_sub_ii(int input1, int input2)
{
	int ret = 0;

	(void)__builtin_ssub_overflow(input1, input2, &ret);

	return ret;
}

int tu_calc_i_add_ii(int input1, int input2)
{
	int ret = 0;

	(void)__builtin_sadd_overflow(input1, input2, &ret);

	return ret;
}

int tu_calc_i_mul_ii(int input1, int input2)
{
	int ret = 0;

	(void)__builtin_smul_overflow(input1, input2, &ret);

	return ret;
}

int tu_calc_i_mul_uiui(unsigned int input1, unsigned int input2)
{
	int ret = 0;
	int tempCalc1 = 0, tempCalc2 = 0;

	if (input1 < (UINT32_MAX / 2U)) {
		tempCalc1 = (int)input1;
	}

	if (input2 < (UINT32_MAX / 2U)) {
		tempCalc2 = (int)input2;
	}

	(void)__builtin_smul_overflow(tempCalc1, tempCalc2, &ret);

	return ret;
}

int tu_calc_i_mul_iii(int input1, int input2, int input3)
{
	int ret = 0;

	(void)__builtin_smul_overflow(input1, input2, &ret);
	(void)__builtin_smul_overflow(ret, input3, &ret);

	return ret;
}

int tu_calc_i_mul_id(int input1, double input2)
{
	int ret = 0;
	double calc = 0.0F;

	calc = (double)input1 * input2;

	if (calc < (double)INT_MAX) {
		ret = (int)calc;
	}

	return ret;
}

long tu_calc_l_mul_dd(double input1, double input2)
{
	long ret = 0;
	double calc = 0.0F;

	calc = input1 * input2;

	if (calc < (double)INT32_MAX) {
		ret = (long)calc;
	}

	return ret;
}

int tu_calc_i_div_ii(int input1, int input2)
{
	int ret = 0;

	if (input2 > 0) {
		ret = input1 / input2;
	}

	return ret;
}

int tu_calc_i_div_id(int input1, double input2)
{
	int ret = 0;
	double tempCalc = 0.0F;

	if (input2 > 0.0F) {
		tempCalc = (double)input1 / input2;

		if (tempCalc < (double)INT_MAX) {
			ret = (int)tempCalc;
		}
	}

	return ret;
}

int tu_calc_i_cov_ui(unsigned int input1)
{
	int ret = 0;

	if (input1 < (UINT32_MAX / 2U)) {
		ret = (int)input1;
	}

	return ret;
}

int tu_calc_i_cov_l(long input1)
{
	int ret = 0;

	if (input1 < (INT32_MAX)) {
		ret = (int)input1;
	}

	return ret;
}

int tu_calc_i_cov_ul(unsigned long input1)
{
	int ret = 0;

	if (input1 < (UINT32_MAX / 2U)) {
		ret = (int)input1;
	}

	return ret;
}

int tu_calc_i_leftshift_ii(int input1, int input2)
{
	int ret = 0;
	unsigned int tempRes = 0, tempValue1 = 0, tempValue2 = 0;

	if (input1 >= 0) {
		tempValue1 = (unsigned int)input1;
	}

	if (input2 >= 0) {
		tempValue2 = (unsigned int)input2;
	}

	tempRes = tempValue1 << tempValue2;

	if (tempRes < (UINT32_MAX / 2U)) {
		ret = (int)tempRes;
	}

	return ret;
}

int tu_calc_i_rightshift_ii(int input1, int input2)
{
	int ret = 0;
	unsigned int tempRes = 0, tempValue1 = 0, tempValue2 = 0;

	if (input1 >= 0) {
		tempValue1 = (unsigned int)input1;
	}

	if (input2 >= 0) {
		tempValue2 = (unsigned int)input2;
	}

	tempRes = tempValue1 >> tempValue2;

	if (tempRes < (UINT32_MAX / 2U)) {
		ret = (int)tempRes;
	}

	return ret;
}

unsigned int tu_calc_ui_add_uiui(unsigned int input1, unsigned int input2)
{
	unsigned int ret = 0;

	(void)__builtin_uadd_overflow(input1, input2, &ret);

	return ret;
}

unsigned int tu_calc_ui_sub_uiui(unsigned int input1, unsigned int input2)
{
	unsigned int ret = 0;

	if (input1 >= input2) {
		ret = input1 - input2;
	}

	return ret;
}

unsigned int tu_calc_ui_mul_uiui(unsigned int input1, unsigned int input2)
{
	unsigned int ret = 0;

	(void)__builtin_umul_overflow(input1, input2, &ret);

	return ret;
}

unsigned int tu_calc_ui_cov_i(int input1)
{
	unsigned int ret = 0;

	if (input1 >= 0) {
		ret = (unsigned int)input1;
	}

	return ret;
}

unsigned long long tu_calc_ull_cov_ll(long long input1)
{
	unsigned long long ret = 0;
	if (input1 >= 0)
	{
		ret = (unsigned long long)input1;
	}
	return ret;
}

unsigned int tu_calc_ui_cov_ul(unsigned long input1)
{
	unsigned int ret = 0;

	if (input1 < UINT32_MAX) {
		ret = (unsigned int)input1;
	}

	return ret;
}

unsigned int tu_calc_ui_cov_l(long input1)
{
	unsigned int ret = 0;
	long maxVal = INT32_MAX;

	maxVal = maxVal * 2L;

	if (input1 < maxVal) {
		ret = (unsigned int)input1;
	}

	return ret;
}

unsigned long tu_calc_ul_cov_ui(unsigned int input1)
{
	unsigned long ret = 0U;

	(void)memcpy(&ret, &input1, sizeof(unsigned int));

	return ret;
}

unsigned long tu_calc_ul_cov_i(int input1)
{
	unsigned long ret = 0U;

	if (input1 >= 0) {
		ret = (unsigned long)input1;
	}

	return ret;
}

unsigned char *tu_calc_puc_buffer_puci(unsigned char *input1, int input2)
{
	unsigned char *ret = NULL;

	if (input1 != NULL) {
		ret = input1;

		if (input2 > 0) {
			for (int i = 0; i < input2; i++) {
				ret++;
			}
		}
	}

	return ret;
}

unsigned char *tu_calc_puc_buffer_pucl(unsigned char *input1, long input2)
{
	unsigned char *ret = NULL;

	if (input1 != NULL) {
		ret = input1;

		if (input2 > 0) {
			for (long i = 0; i < input2; i++) {
				ret++;
			}
		}
	}

	return ret;
}

unsigned char *tu_calc_puc_buffer_pucui(unsigned char *input1, unsigned int input2)
{
	unsigned char *ret = NULL;

	if (input1 != NULL) {
		ret = input1;

		if (input2 > 0UL) {
			for (unsigned int i = 0U; i < input2; i++) {
				ret++;
			}
		}
	}

	return ret;
}

unsigned char *tu_calc_puc_buffer_pucul(unsigned char *input1, unsigned long input2)
{
	unsigned char *ret = NULL;

	if (input1 != NULL) {
		ret = input1;

		if (input2 > 0UL) {
			for (unsigned long i = 0UL; i < input2; i++) {
				ret++;
			}
		}
	}

	return ret;
}

void *tu_calc_p_buffer_pucl(void *input1, long input2)
{
	void *ret = NULL;

	if (input1 != NULL) {
		ret = input1;

		if (input2 > 0) {
			for (long i = 0; i < input2; i++) {
				ret++;
			}
		}
	}

	return ret;
}

long long tu_calc_ll_mul_ll_ll(long long input1, long long input2)
{
	long long ret = 0;

	(void)__builtin_smulll_overflow(input1, input2, &ret);

	return ret;
}

long long tu_calc_ll_add_ll_ll(long long input1, long long input2)
{
	long long ret = 0;

	(void)__builtin_saddll_overflow(input1, input2, &ret);

	return ret;
}

long long tu_calc_ll_sub_ll_ll(long long input1, long long input2)
{
	long long ret = 0;

	(void)__builtin_ssubll_overflow(input1, input2, &ret);

	return ret;
}

long tu_calc_l_add_ll(long input1, long input2)
{
	long ret = 0;

	(void)__builtin_saddl_overflow(input1, input2, &ret);

	return ret;
}

long tu_calc_l_add_lul(long input1, unsigned long input2)
{
	long ret = 0;
	long tempCalc = tu_calc_l_cov_ul(input2);

	(void)__builtin_saddl_overflow(input1, tempCalc, &ret);

	return ret;
}

long tu_calc_l_mul_ll(long input1, long input2)
{
	long ret = 0;

	(void)__builtin_smull_overflow(input1, input2, &ret);

	return ret;
}

long tu_calc_l_mul_lul(long input1, unsigned long input2)
{
	long ret = 0;
	long tempCalc = tu_calc_l_cov_ul(input2);

	(void)__builtin_smull_overflow(input1, tempCalc, &ret);

	return ret;
}

long tu_calc_l_mul_ulul(unsigned long input1, unsigned long input2)
{
	unsigned long tempCalc = 0U;

	(void)__builtin_umull_overflow(input1, input2, &tempCalc);

	return tu_calc_l_cov_ul(tempCalc);
}

long tu_calc_l_mul_lll(long input1, long input2, long input3)
{
	long ret = 0;

	(void)__builtin_smull_overflow(input1, input2, &ret);
	(void)__builtin_smull_overflow(ret, input3, &ret);

	return ret;
}

long tu_calc_l_div_ll(long input1, long input2)
{
	long ret = 0;

	if (input2 > 0) {
		ret = input1 / input2;
	}

	return ret;
}

long tu_calc_l_sub_ll(long input1, long input2)
{
	long ret = 0;

	(void)__builtin_ssubl_overflow(input1, input2, &ret);

	return ret;
}

long tu_calc_l_cov_ul(unsigned long input1)
{
	long ret = 0;

	if (input1 < (UINT64_MAX / 2U)) {
		ret = (long)input1;
	}

	return ret;
}

long tu_calc_l_cov_i(int input1)
{
	long ret = 0;

	(void)memcpy(&ret, &input1, sizeof(int));

	return ret;
}

long tu_calc_l_cov_f(float input1)
{
	long ret = 0L;

	if (input1 != 0.0f) {
		ret = (long)input1;
	} else {
		ret = 0L;
	}

	return ret;
}

long tu_calc_l_cov_d(double input1)
{
	float tmpCalc = tu_calc_f_cov_d(input1);
	long ret = tu_calc_l_cov_f(tmpCalc);

	return ret;
}

long tu_calc_l_rightshift_li(long input1, int input2)
{
	long ret = 0;
	unsigned long tempRes = 0, tempValue1 = 0, tempValue2 = 0;

	if (input1 >= 0) {
		tempValue1 = (unsigned long)input1;
	}

	if (input2 >= 0) {
		tempValue2 = (unsigned long)input2;
	}

	tempRes = tempValue1 >> tempValue2;

	if (tempRes < (UINT64_MAX / 2U)) {
		ret = (long)tempRes;
	}

	return ret;
}

long tu_calc_l_leftshift_li(long input1, int input2)
{
	long ret = 0;
	unsigned long tempRes = 0, tempValue1 = 0, tempValue2 = 0;

	if (input1 >= 0) {
		tempValue1 = (unsigned long)input1;
	}

	if (input2 >= 0) {
		tempValue2 = (unsigned long)input2;
	}

	tempRes = tempValue1 << tempValue2;

	if (tempRes < (UINT64_MAX / 2U)) {
		ret = (long)tempRes;
	}

	return ret;
}

unsigned long tu_calc_ul_add_ulul(unsigned long input1, unsigned long input2)
{
	unsigned long ret = 0;

	(void)__builtin_uaddl_overflow(input1, input2, &ret);

	return ret;
}

unsigned long tu_calc_ul_sub_ulul(unsigned long input1, unsigned long input2)
{
	unsigned long ret = 0;

	if (input1 >= input2) {
		ret = input1 - input2;
	}

	return ret;
}

unsigned long tu_calc_ul_mul_ulul(unsigned long input1, unsigned long input2)
{
	unsigned long ret = 0;

	(void)__builtin_umull_overflow(input1, input2, &ret);

	return ret;
}

unsigned long tu_calc_ul_div_ulul(unsigned long input1, unsigned long input2)
{
	unsigned long ret = 0;

	if (input2 > 0UL) {
		ret = input1 / input2;
	}

	return ret;
}

unsigned long tu_calc_ul_cov_l(long input1)
{
	unsigned long ret = 0;

	if (input1 >= 0) {
		ret = (unsigned long)input1;
	}

	return ret;
}

unsigned long tu_calc_ul_cov_ll(long long input1)
{
	unsigned long ret = 0;

	if (input1 >= 0L) {
		ret = (unsigned long)input1;
	}

	return ret;
}

float tu_calc_f_cov_d(double input1)
{
	float ret = 1.0f;

	if ((input1 != 0.0f) &&
		((isnan(input1) > 0) || (isgreater(fabs(input1), FLT_MAX) > 0) || (isless(fabs(input1), FLT_MIN) > 0))) {
		ret = -1.0f;
	} else {
		ret = (float)input1;
	}

	return ret;
}

double tu_calc_d_div_ii(int input1, int input2)
{
	double ret = 0.0F;

	if (input2 > 0) {
		ret = (double)input1 / (double)input2;
	}

	return ret;
}

float tu_calc_f_div_uiui(unsigned int input1, unsigned int input2)
{
	float ret = 0.0F;

	double tmpRet = 0.0F;
	double tmpI1 = (double)input1;
	double tmpI2 = (double)input2;

	if (tmpI2 > 0.0F) {
		tmpRet = tmpI1 / tmpI2;
		ret = tu_calc_f_cov_d(tmpRet);
	}

	return ret;
}

int tu_calc_atoi(const char *buff)
{
	char *end;
	int ret = 0;

	errno = 0;
	const long sl = strtol(buff, &end, 10);
	if ((sl != 0) && (errno == EINVAL)) {
		(void)g_printf("%s: failed to strtol %ld", __func__, sl);
		ret = -EINVAL;
	} else if (((sl == LONG_MAX) || (sl == LONG_MIN)) && (errno == ERANGE)) {
		(void)g_printf("%s: failed to strtol (outside of range) %ld", __func__, sl);
		ret = -EINVAL;
	} else if (errno != 0) {
		(void)g_printf("%s: failed to strtol %ld", __func__, sl);
		ret = -EINVAL;
	} else if (*end != '\0') {
		(void)g_printf("%s: failed to strtol %ld", __func__, sl);
		ret = -EINVAL;
	} else if (end == buff) {
		(void)g_printf("%s: failed to strtol %ld", __func__, sl);
		ret = -EINVAL;
	} else {
		// none
	}

	if (ret == 0) {
		if ((sl > INT_MAX) || (sl < INT_MIN)) {
			(void)g_printf("%s: outside of range for integer type", __func__);
		} else {
			ret = (int)sl;
		}
	}

	return ret;
}