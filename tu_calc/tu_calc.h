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

#ifndef TCC_CALC_H
#define TCC_CALC_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define VERIFY_ARRAY_INDEX(index, max) ((((index) < 0) || ((index) >= (max))) ? false : true)
size_t popcount(uintmax_t num);
#define PRECISION(umax_value) (popcount(umax_value))
#define TCC_CALC_DONOTHING()

unsigned char tu_calc_uc_cov_ui(unsigned int input1);

int tu_calc_i_add_ii(int input1, int input2);
int tu_calc_i_sub_ii(int input1, int input2);
int tu_calc_i_mul_ii(int input1, int input2);
int tu_calc_i_mul_uiui(unsigned int input1, unsigned int input2);
int tu_calc_i_mul_iii(int input1, int input2, int input3);
int tu_calc_i_mul_id(int input1, double input2);
long tu_calc_l_mul_dd(double input1, double input2);
int tu_calc_i_div_ii(int input1, int input2);
int tu_calc_i_div_id(int input1, double input2);
int tu_calc_i_cov_ui(unsigned int input1);
int tu_calc_i_cov_l(long input1);
int tu_calc_i_cov_ul(unsigned long input1);
int tu_calc_i_leftshift_ii(int input1, int input2);
int tu_calc_i_rightshift_ii(int input1, int input2);

unsigned int tu_calc_ui_add_uiui(unsigned int input1, unsigned int input2);
unsigned int tu_calc_ui_sub_uiui(unsigned int input1, unsigned int input2);
unsigned int tu_calc_ui_mul_uiui(unsigned int input1, unsigned int input2);
unsigned int tu_calc_ui_cov_i(int input1);
unsigned int tu_calc_ui_cov_l(long input1);
unsigned int tu_calc_ui_cov_ul(unsigned long input1);

unsigned char *tu_calc_puc_buffer_puci(unsigned char *input1, int input2);
unsigned char *tu_calc_puc_buffer_pucui(unsigned char *input1, unsigned int input2);
unsigned char *tu_calc_puc_buffer_pucul(unsigned char *input1, unsigned long input2);
unsigned char *tu_calc_puc_buffer_pucl(unsigned char *input1, long input2);

void *tu_calc_p_buffer_pucl(void *input1, long input2);

long tu_calc_l_add_ll(long input1, long input2);
long tu_calc_l_add_lul(long input1, unsigned long input2);
long tu_calc_l_sub_ll(long input1, long input2);
long tu_calc_l_mul_ll(long input1, long input2);
long tu_calc_l_mul_lll(long input1, long input2, long input3);
long tu_calc_l_mul_lul(long input1, unsigned long input2);
long tu_calc_l_mul_ulul(unsigned long input1, unsigned long input2);
long tu_calc_l_div_ll(long input1, long input2);
long tu_calc_l_cov_ul(unsigned long input1);
long tu_calc_l_cov_i(int input1);
long tu_calc_l_cov_f(float input1);
long tu_calc_l_cov_d(double input1);
long tu_calc_l_rightshift_li(long input1, int input2);
long tu_calc_l_leftshift_li(long input1, int input2);

long long tu_calc_ll_mul_ll_ll(long long input1, long long input2);
long long tu_calc_ll_add_ll_ll(long long input1, long long input2);
long long tu_calc_ll_sub_ll_ll(long long input1, long long input2);

unsigned long long tu_calc_ull_cov_ll(long long input1);

unsigned long tu_calc_ul_add_ulul(unsigned long input1, unsigned long input2);
unsigned long tu_calc_ul_sub_ulul(unsigned long input1, unsigned long input2);
unsigned long tu_calc_ul_mul_ulul(unsigned long input1, unsigned long input2);
unsigned long tu_calc_ul_div_ulul(unsigned long input1, unsigned long input2);
unsigned long tu_calc_ul_cov_l(long input1);
unsigned long tu_calc_ul_cov_ui(unsigned int input1);
unsigned long tu_calc_ul_cov_i(int input1);
unsigned long tu_calc_ul_cov_ll(long long input1);

float tu_calc_f_cov_d(double input1);

double tu_calc_d_div_ii(int input1, int input2);
float tu_calc_f_div_uiui(unsigned int input1, unsigned int input2);

int tu_calc_atoi(const char *buff);

unsigned int tu_calc_leftshift_unsigned_type(unsigned int input1, unsigned int input2);
unsigned int tu_calc_rightshift_unsigned_type(unsigned int input1, unsigned int input2);
double tu_calc_pow(double input1, double input2);


#ifdef __cplusplus
}
#endif

#endif // TCC_CALC_H