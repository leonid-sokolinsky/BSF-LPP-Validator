/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-bsfParameters.h
Prefix: PP_BSF
Author: Leonid B. Sokolinsky
==============================================================================*/

//=========================== Skeleton Parameters =========================
#define PP_MAX_MPI_SIZE 500			// Defines the maximum possible number of MPI processes
//#define PP_BSF_ITER_OUTPUT		// If it is defined then Iteration Output is performed
#define PP_BSF_TRACE_COUNT 1		// Each PP_BSF_TRACE_COUNT-th iteration to be outputted
#define PP_BSF_MAX_JOB_CASE 0		// Defines the maximum number of activities (jobs) in workflow minus 1

//--------------------------- OpenMP Parameters ---------------------------
// OpenMP mode is impossible!

//--------------- BSF Lists parameters (For "No MPI" only) ----------------
#include "Problem-Parameters.h"
#define PP_BSF_MAP_LIST_LENGTH		PP_MAX_K
#define PP_BSF_REDUCE_LIST_LENGTH	PP_MAX_K
#define PP_BSF_REDUCE_LIST_1_LENGTH	1
#define PP_BSF_REDUCE_LIST_2_LENGTH	1
#define PP_BSF_REDUCE_LIST_3_LENGTH	1