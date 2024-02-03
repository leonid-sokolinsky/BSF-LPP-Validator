/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-bsf-Forwards.h (Problem Function Forwards)
Author: Leonid B. Sokolinsky 
==============================================================================*/
#include "Problem-bsfTypes.h"	// Predefined BSF types
#include "Problem-Types.h"		// Problem Types

void	Angles(PT_angles_T phi, int k);
void	CheckPoint(PT_vector_T p, int k);
bool	Conversion();
bool	LoadMatrixFormat();
bool	PointIn(PT_vector_T x, PT_vector_T a, PT_float_T b); // If the point belonges to the Halfspace <a,x> <= b?
double	ObjectiveF(PT_vector_T x);
void	SkipComments(FILE* stream);
void	Vector_Copy(PT_vector_T fromPoint, PT_vector_T toPoint);
double	Vector_DotProduct(PT_vector_T x, PT_vector_T y);
double	Vector_NormSquare(PT_vector_T x);
void	Vector_Subtraction(PT_vector_T x, PT_vector_T y, PT_vector_T z);
//====================== Macros ================================
#define PF_MIN(x,y) (x<y?x:y)
#define PF_MAX(x,y) (x>y?x:y)