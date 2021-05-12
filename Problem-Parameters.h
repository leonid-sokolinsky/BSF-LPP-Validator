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
#define PP_N 5											// n - Space dimension (n > 2)
#define PP_NUM_OF_RND_INEQUALITIES (2 * PP_N)			// Number of random inequalities
#define PP_M (2*PP_N + PP_NUM_OF_RND_INEQUALITIES + 1)	// m - Total number of inequalities
#define PP_D 41											// d - number of parallels excluding poles (d > 2, d is a odd number)
#define PP_K (2*(PT_float_T)PP_D*pow(PP_D-1, PP_N-2))	// K - Total number of points on hypersphere (K < 2 147 483 647)
#define PP_RHO	1										// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9								// Precision of the comparison with zero
#define PP_EPS_OBJECTIVE 1E-6							// Precision of the comparison of the objective function values
//-------------------------- Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 14
//#define PP_MATRIX_OUTPUT	// To output Matrix
#define PP_PATH "C:/TEMP/"
#define PP_LPP_FILE "lpp.txt"
#define PP_SOLUTION_FILE "solution.txt"