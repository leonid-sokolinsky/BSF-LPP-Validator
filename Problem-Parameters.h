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
#define PP_N 6												// n - Space dimension (n > 2)
#define PP_NUM_OF_NATURAL_INEQUALITIES (PP_N)				// Number of natural inequalities for random LPP
#define PP_M (2*PP_N + PP_NUM_OF_NATURAL_INEQUALITIES + 1)	// m - Total number of inequalities
#define PP_D 12												// d - Density of points per 2-D sphere (d > 3)
#define PP_K (PP_D*pow(PP_D/2 - 1,PP_N - 2))				// K - Total number of points on hypersphere (K < 2 147 483 647)
#define PP_K_INT 7500										// Integer value of K
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_LPP_FILE "D:/OneDrive/Data/rnd006-0.txt"			// LPP data file in the following format:
/*------------ begin of file -------------
m n
A_11 A_12 ... A_1n b_1
A_21 A_22 ... A_2n b_2
...
A_m1 A_m2 ... A_mn b_m
c_1 c_2 ... c_n
------------ end of file----------------*/

//#define PP_SOLUTION_FILE "D:/OneDrive/Data/rndSolution.txt"			// LPP solution file in the following format:
#define PP_SOLUTION_FILE "D:/OneDrive/Data/rndSolution006-0.txt"
/*
------------ begin of file -------------
n
x_1 x_2 ... x_n
------------ end of file----------------
*/
//-------------------------- Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
//#define PP_SETW 14
#define PP_SETW 20
//#define PP_MATRIX_OUTPUT	// To output Matrix