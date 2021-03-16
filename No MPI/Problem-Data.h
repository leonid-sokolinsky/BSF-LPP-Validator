/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Data.h (Problem Data)
Prefix: PD
Author: Leonid B. Sokolinsky
==============================================================================*/
#include "Problem-Types.h"			// Problem Parameters 
using namespace std;
//========================== Problem variables ====================================
static int		PD_n;		// Space dimension
static int		PD_m;		// Number of inequalities
static int		PD_K;		// Total number of points on hypersphere (must be < 2 147 483 647)	
static PT_float_T PD_objF_x; // Value of objective function in the solution point
static PT_float_T PD_objF_pMax = (-FLT_MAX); // Maximal value of objective function
static bool PD_firstTime = true;
static bool PD_solutionIsOk;
//========================== Problem data structures ==============================
static PT_matrix_T PD_A;	// Matrix of the system Ax <= b
static PT_column_T PD_b;	// Column of the constant terms of the system Ax <= b
static PT_vector_T PD_c;	// Coefficients of the objective function <c,x>
static PT_vector_T PD_x;	// Solution to check
static PT_vector_T PD_pMax;	// Correct solution if PD_x failed
static PT_vector_T PD_p;	// Current approximation

//========================== Files ================================================
static string PD_lppFile; /* LPP file in the following format:
------------ begin of file -------------
m n
A_11 A_12 ... A_1n b_1
A_21 A_22 ... A_2n b_2
...
A_m1 A_m2 ... A_mn b_m
c_1 c_2 ... c_n
------------ end of file----------------*/

static string PD_solutionFile;/* LPP solution file in the following format:
------------ begin of file -------------
n
x_1 x_2 ... x_n
------------ end of file----------------*/
static string PD_traceFile;