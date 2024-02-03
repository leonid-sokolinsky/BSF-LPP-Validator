/*==============================================================================
Project: VaLiPro
Theme: LPP Solution Validator
Module: Problem-Parameters.h (Problem Parameters)
Prefix: PP
Author: Leonid B. Sokolinsky
==============================================================================*/

//-------------------------- Compilation Modes -----------------------
#define PP_DEBUG
#define PP_PATH "D:/OneDrive/_private/���������/BSF/BSF Target/Problems/"
//#define PP_PATH "Problems/"

//=========================== Problem Parameters =========================
#define PP_PROBLEM_NAME		"dataset2"
#define PP_M		10		// Number of equations (number of rows in *.mtx)
#define PP_N		13		// Number of variables (number of cols in *.mtx)
#define PP_INFINITY	1E+308	// Highest bound in *_hi.mtx

#define PP_MM (2*PP_M+2*PP_N)								// Maximal number of inequalities
#define PP_D 51												// d - number of parallels excluding poles (d > 2, d is an odd number)
#define PP_MAX_K 10000000									// Maximal value of K
#define PP_RHO	1E-5											// Radius of verification hypersphere (vicinity of solution)
#define PP_EPS_ZERO 1E-9									// Precision of the comparison with zero
#define PP_EPS_OBJECTIVE 1E-9								// Precision of the comparison of the objective function values

//-------------------------- Input/Outpoot Parameters ---------------------------
#define PP_OUTPUT_LIMIT	8	// Number of Elements to output
#define PP_SETW 9
#define PP_MATRIX_OUTPUT	// To output Matrix

//------------------------- Matrix format ----------------
#define PP_MTX_PREFIX			"lp_"
#define PP_MTX_POSTFIX_A		".mtx"
#define PP_MTX_POSTFIX_B		"_b.mtx"
#define PP_MTX_POSTFIX_LO		"_lo.mtx"
#define PP_MTX_POSTFIX_HI		"_hi.mtx"
#define PP_MTX_POSTFIX_C		"_c.mtx"
#define PP_MTX_POSTFIX_SO		"_so.mtx" // Solution