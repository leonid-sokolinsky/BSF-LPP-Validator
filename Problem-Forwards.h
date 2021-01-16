/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-bsf-Forwards.h (Problem Function Forwards)
Author: Leonid B. Sokolinsky 
==============================================================================*/
#include "Problem-bsfTypes.h"	// Predefined BSF types
#include "Problem-Types.h"		// Problem Types

void Angles(PT_angles_T phi, int k);
void CheckPoint(PT_vector_T p, int k);
bool PointIn(PT_vector_T x, PT_vector_T a, PT_float_T b); // If the point belonges to the Halfspace <a,x> <= b?
double Vector_DotProduct(PT_vector_T x, PT_vector_T y);
//====================== Macros ================================
#define PF_MIN(x,y) (x<y?x:y)
#define PF_MAX(x,y) (x>y?x:y)