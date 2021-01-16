/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//=========================== Problem Parameters =========================
//#define PP_N 19												// n - Space dimension (n > 2)
#define PP_N 8												// n - Space dimension (n > 2)
//#define PP_NUM_OF_NATURAL_INEQUALITIES 1					// Number of natural inequalities for model LPP
#define PP_NUM_OF_NATURAL_INEQUALITIES (PP_N*5)				// Number of natural inequalities for random LPP
#define PP_M (2*PP_N + PP_NUM_OF_NATURAL_INEQUALITIES)		// m - Total number of inequalities
//#define PP_D 8												// d - Density of points per 2-D sphere (d > 3)
#define PP_D 8												// d - Density of points per 2-D sphere (d > 3)
#define PP_K (PP_D*pow(PP_D/2 - 1,PP_N - 2))				// K - Total number of points on hypersphere (K < 2 147 483 647)
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_LPP_FILE "C:/TEMP/rndLpp.txt"					// LPP data file in the following format:
/*
------------ begin of file -------------
m n
A_11 A_12 ... A_1n b_1
A_21 A_22 ... A_2n b_2
...
A_m1 A_m2 ... A_mn b_m
c_1 c_2 ... c_n
------------ end of file----------------
*/
#define PP_SOLUTION_FILE "C:/TEMP/rndSolution.txt"			// LPP solution file in the following format:
/*
------------ begin of file -------------
n
x_1 x_2 ... x_n
------------ end of file----------------
*/
//-------------------------- Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
//#define PP_MATRIX_OUTPUT	// To output Matrix