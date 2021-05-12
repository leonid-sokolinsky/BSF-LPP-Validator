/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//=========================== Problem Parameters =========================
#define PP_MAX_N 50											// Maximal Space Dimension
#define PP_MAX_M (4 * PP_MAX_N + 1)							// Maximal number of inequalities
#define PP_D 41												// d - number of parallels excluding poles (d > 2, d is a odd number)
#define PP_MAX_K 10000000									// Maximal value of K
#define PP_RHO	1											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_EPS_OBJECTIVE 1E-9								// Precision of the comparison of the objective function values

//-------------------------- Input/Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 14
//#define PP_MATRIX_OUTPUT	// To output Matrix
#define PP_PATH "C:/TEMP/"
#define PP_LPP_FILE "lpp.txt"
#define PP_SOLUTION_FILE "solution.txt"
#define PP_TRACE_FILE "trace.txt"