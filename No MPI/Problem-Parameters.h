/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//=========================== Problem Parameters =========================
#define PP_MAX_N 50											// Maximal Space Dimension
#define PP_MAX_M (3 * PP_MAX_N + 1)							// Maximal number of inequalities
#define PP_D 61												// d - number of parallels excluding poles (d > 2, d is a odd number)
#define PP_MAX_K 655360										// Integer value of K
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_EPS_OBJECTIVE 1E-9								// Precision of the comparison of the objective function values
/*------------ begin of file -------------
m n
A_11 A_12 ... A_1n b_1
A_21 A_22 ... A_2n b_2
...
A_m1 A_m2 ... A_mn b_m
c_1 c_2 ... c_n
------------ end of file----------------*/

/*
------------ begin of file -------------
n
x_1 x_2 ... x_n
------------ end of file----------------
*/
//-------------------------- Input/Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 14
//#define PP_MATRIX_OUTPUT	// To output Matrix
#define PP_PATH "C:/TEMP/"