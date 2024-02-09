/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator (No MPI)
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
static PT_float_T PD_objF_pMax = (-DBL_MAX); // Maximal value of objective function
static bool PD_firstTime = true;
static bool PD_solutionIsOk;
//========================== Problem data structures ==============================
static PT_matrix_T PD_A;	// Matrix of the system Ax <= b
static PT_column_T PD_b;	// Column of the constant terms of the system Ax <= b
static PT_vector_T PD_c;	// Coefficients of the objective function <c,x>
static PT_vector_T PD_hi;	// Higher bound
static PT_vector_T PD_lo;	// Lower bound
static PT_vector_T PD_x;	// Solution to check
static PT_vector_T PD_pMax;	// Correct solution if PD_x failed
static PT_angles_T PD_phi;	// Checkpoint angles in the radial coordinate system
static PT_vector_T PD_p;	// Checkpoint coordinates
//========================== Input/Output ====================================
static string PD_problemName;
static string PD_solutionFile;
static string PD_MTX_File_tr;
static FILE* PD_traceStream;
//========================== Matrix format ====================================
// nor - number of rows; noc - number of columns; non - number of non-zero values
static string PD_MTX_File_A; /* File of matrix A (only non-zero elements):
------------ begin of file -------------
nor noc non // nor=m (number of inequalities); noc=n (dimension); non - number of non-zero values
i_1 j_1 A_{{i_1}{j_1}} // i_1 - index of row; j_1 - index of column
...
i_k j_k A_{{i_k}{j_k}}
------------ end of file----------------*/
static string PD_MTX_File_b; /* File of column b:
------------ begin of file -------------
nor noc // nor=m (number of inequalities); noc=1
b_1
...
b_{nor}
------------ end of file----------------*/
static string PD_MTX_File_lo; /* File of variable lower bounds:
------------ begin of file -------------
nor noc // nor=n (dimension); noc=1
lo_1
...
lo_{nor}
------------ end of file----------------*/
static string PD_MTX_File_hi; /* File of variable higher bounds:
------------ begin of file -------------
nor noc // nor=n (dimension); noc=1
lo_1
...
lo_{nor}
------------ end of file----------------*/
static string PD_MTX_File_c; /* File of variable higher bounds:
------------ begin of file -------------
nor noc // nor=n (dimension); noc=1
c_1
...
c_{nor}
------------ end of file----------------*/
static string PD_MTX_File_so;/* Solution file in the following format:
------------ begin of file -------------
nor noc // nor=n (dimension); noc=1
x_1
...
x_{nor}
------------ end of file----------------*/