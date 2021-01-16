/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-bsfTypes.h (Predefined Problem-depended BSF Types)
Prefix: PT_bsf
Author: Leonid B. Sokolinsky
==============================================================================*/
#pragma once
#include "Problem-Types.h"		// Problem Types 

//=========================== BSF Types =========================
struct PT_bsf_parameter_T {		// Order parameters
	char stuff;
};

struct PT_bsf_mapElem_T {		// Type of map-list elements
	char stuff;
};

struct PT_bsf_reduceElem_T {	// Type of reduce-list elements for Job 0 (default)	
	bool ok;
};

struct PT_bsf_reduceElem_T_1 {	// Type of reduce-list elements for Job 1
	// Optional filling. Do not delete!
};

struct PT_bsf_reduceElem_T_2 {	// Type of reduce-list elements for Job 2
	// Optional filling. Do not delete!
};

struct PT_bsf_reduceElem_T_3 {	// Type of reduce-list elements for Job 3
	// Optional filling. Do not delete!
};