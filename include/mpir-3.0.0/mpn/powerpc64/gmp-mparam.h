/* Generated by tuneup.c, 2014-03-24, gcc 4.7 */

#define MUL_KARATSUBA_THRESHOLD          14
#define MUL_TOOM3_THRESHOLD              97
#define MUL_TOOM4_THRESHOLD             143
#define MUL_TOOM8H_THRESHOLD            206

#define SQR_BASECASE_THRESHOLD            7
#define SQR_KARATSUBA_THRESHOLD          44
#define SQR_TOOM3_THRESHOLD             134
#define SQR_TOOM4_THRESHOLD             250
#define SQR_TOOM8_THRESHOLD             254

#define POWM_THRESHOLD                  984

#define DIVREM_1_NORM_THRESHOLD           0  /* always */
#define DIVREM_1_UNNORM_THRESHOLD         0  /* always */
#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define USE_PREINV_DIVREM_1               1
#define USE_PREINV_MOD_1                  1
#define DIVREM_2_THRESHOLD                0  /* always */
#define DIVEXACT_1_THRESHOLD              0  /* always */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always */
#define MOD_1_1_THRESHOLD                 7
#define MOD_1_2_THRESHOLD                 7
#define MOD_1_3_THRESHOLD                 7
#define DIVREM_HENSEL_QR_1_THRESHOLD      5
#define RSH_DIVREM_HENSEL_QR_1_THRESHOLD     20
#define DIVREM_EUCLID_HENSEL_THRESHOLD    177

#define MUL_FFT_FULL_THRESHOLD         4032

#define SQR_FFT_FULL_THRESHOLD         2016

#define MULLOW_BASECASE_THRESHOLD         0  /* always */
#define MULLOW_DC_THRESHOLD              53
#define MULLOW_MUL_THRESHOLD           2171

#define MULHIGH_BASECASE_THRESHOLD        4
#define MULHIGH_DC_THRESHOLD             59
#define MULHIGH_MUL_THRESHOLD          2171

#define MULMOD_2EXPM1_THRESHOLD          22

#define DC_DIV_QR_THRESHOLD              20
#define INV_DIV_QR_THRESHOLD           1894
#define INV_DIVAPPR_Q_N_THRESHOLD       300
#define DC_DIV_Q_THRESHOLD               27
#define INV_DIV_Q_THRESHOLD             923
#define DC_DIVAPPR_Q_THRESHOLD           20
#define INV_DIVAPPR_Q_THRESHOLD        7881
#define DC_BDIV_QR_THRESHOLD             72
#define DC_BDIV_Q_THRESHOLD              27

#define ROOTREM_THRESHOLD                 6

#define MATRIX22_STRASSEN_THRESHOLD      19
#define HGCD_THRESHOLD                  111
#define HGCD_APPR_THRESHOLD             136
#define HGCD_REDUCE_THRESHOLD          6852
#define GCD_DC_THRESHOLD                361
#define GCDEXT_DC_THRESHOLD             318
#define JACOBI_BASE_METHOD                2

#define GET_STR_DC_THRESHOLD             10
#define GET_STR_PRECOMPUTE_THRESHOLD     22
#define SET_STR_DC_THRESHOLD            882
#define SET_STR_PRECOMPUTE_THRESHOLD   2726

#define FAC_DSC_THRESHOLD               542
#define FAC_ODD_THRESHOLD                27

/* fft_tuning -- autogenerated by tune-fft */

#define FFT_TAB \
   { { 4, 3 }, { 3, 1 }, { 3, 2 }, { 2, 1 }, { 1, 0 } }

#define MULMOD_TAB \
   { 4, 3, 3, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 }

#define FFT_N_NUM 19

#define FFT_MULMOD_2EXPP1_CUTOFF 128


/* Tuneup completed successfully, took 186 seconds */

