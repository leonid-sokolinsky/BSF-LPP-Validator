/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-Data.h (Problem Data)
Prefix: PD
Author: Leonid B. Sokolinsky
==============================================================================*/
#include "Problem-Types.h"			// Problem Parameters 

//========================== Problem variables ====================================
static PT_float_T PD_objF_x; // Value of objective function in the solution point
//========================== Problem data structures ==============================
static PT_matrix_T PD_A;	// Matrix of the system Ax <= b
static PT_column_T PD_b;	// Column of the constant terms of the system Ax <= b
static PT_vector_T PD_c;	// Coefficients of the objective function <c,x>
static PT_vector_T PD_x;	// Solution to check