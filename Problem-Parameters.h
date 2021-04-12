/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//-------------------------- Compilation Modes -----------------------
//#define PP_DEBUG

//=========================== Problem Parameters =========================
#define PP_N 5												// n - Space dimension (n > 2)
#define PP_NUM_OF_RND_INEQUALITIES (PP_N)					// Number of random inequalities
#define PP_M (2*PP_N + PP_NUM_OF_RND_INEQUALITIES + 1)		// m - Total number of inequalities
#define PP_D 17												// d - number of parallels excluding poles (d > 2, d is a odd number)
#define PP_K (int)(2*PP_D*pow(PP_D - 1,PP_N - 2))				// K - Total number of points on hypersphere (K < 2 147 483 647)
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_EPS_OBJECTIVE 1E-6								// Precision of the comparison of the objective function values
#define PP_LPP_FILE "lpp.txt"			// LPP data file in the following format:
/*------------ begin of file -------------
m n
A_11 A_12 ... A_1n b_1
A_21 A_22 ... A_2n b_2
...
A_m1 A_m2 ... A_mn b_m
c_1 c_2 ... c_n
------------ end of file----------------*/

#define PP_SOLUTION_FILE "solution.txt" // LPP solution file in the following format:
/*
------------ begin of file -------------
n
x_1 x_2 ... x_n
------------ end of file----------------
*/
//-------------------------- Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 14
#define PP_MATRIX_OUTPUT	// To output Matrix