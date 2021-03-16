/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//-------------------------- Compilation Modes -----------------------
#define PP_DEBUG

//=========================== Problem Parameters =========================
#define PP_MAX_N 10											// Maximal Space Dimension
#define PP_MAX_M (3 * PP_MAX_N + 1)							// Maximal number of inequalities
#define PP_D 10												// d - Density of points per 2-D sphere (d > 3)
#define PP_MAX_K 655360										// Integer value of K
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
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
//-------------------------- Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 14
//#define PP_MATRIX_OUTPUT	// To output Matrix