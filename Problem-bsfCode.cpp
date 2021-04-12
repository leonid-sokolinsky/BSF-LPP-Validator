/*==============================================================================
Project: LiFe
Theme: LPP Solution Validator
Module: Problem-bsfCode.cpp (Problem-dependent Code)
Prefix: PC
Author: Leonid B. Sokolinsky

==============================================================================*/
#include "Problem-Data.h"			// Problem Types 
#include "Problem-Forwards.h"		// Problem Function Forwards
#include "Problem-bsfParameters.h"	// BSF-skeleton parameters
#include "BSF-SkeletonVariables.h"	// Skeleton Variables
using namespace std;

//----------------------- Predefined problem-dependent functions -----------------
void PC_bsf_Init(bool *success) {
	FILE* stream;
	float buf; int m, n;

	if (PP_K > 2147483647) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Total number of points on hypersphere PP_K = " << PP_K << " must be less than " << 2147483647 << ".\n";
		*success = false; return;
	}
	if (PP_N < 3) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Dimension PP_N = " << PP_N << " must be greater than " << 2 << ".\n";
		*success = false; return;
	}
	if (PP_D % 2 == 0) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Density PP_D = " << PP_D << " must be must be a odd number!" << endl;
		*success = false; return;
	}
	if (PP_D < 3) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Density PP_D = " << PP_D << " must be greater than " << 2 << ".\n";
		*success = false; return;
	}

	// ------------- Load LPP data -------------------
	stream = fopen(PP_LPP_FILE, "r");
	if (stream == NULL) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Failure of opening file '" << PP_LPP_FILE << "'.\n";
		*success = false; return;
	}	

	fscanf(stream, "%d%d", &m, &n);
	if (n != PP_N || m != PP_M) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Error in input data '" << PP_LPP_FILE << "': PP_N != n and/or PP_M != m (PP_N = "
			<< PP_N << ", n = " << n << "; PP_M = " << PP_M << ", m = " << m << ").\n";
		*success = false; return;
	}

	for (int i = 0; i < PP_M; i++) {
		for (int j = 0; j < PP_N; j++) {
			fscanf(stream, "%f", &buf);
			PD_A[i][j] = buf;
		}
		fscanf(stream, "%f", &buf);
		PD_b[i] = buf;
	}

	for (int j = 0; j < PP_N; j++) {
		fscanf(stream, "%f", &buf);
		PD_c[j] = buf;
	}
	fclose(stream);

	// --------------- Load solution ---------------
	stream = fopen(PP_SOLUTION_FILE, "r");
	if (stream == NULL) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Failure of opening file '" << PP_SOLUTION_FILE << "'.\n";
		*success = false; return;
	}

	fscanf(stream, "%d", &n);
	if (n != PP_N) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Error in input data '" << PP_SOLUTION_FILE << "': PP_N != n (PP_N = " << PP_N << ", n = " << n << ").\n";
		*success = false; return;
	}

	for (int j = 0; j < PP_N; j++) {
		fscanf(stream, "%f", &buf);
		PD_x[j] = buf;
	}

	fclose(stream);

	/*if (BSF_sv_mpiRank == 0) {
		int counter = 0;
		for (int i = 0; i < PP_M; i++)
			if (!PointIn(PD_x, PD_A[i], PD_b[i])) {
				cout << "\tPoint x = (";
				for (int j = 0; j < PP_N; j++)
					cout << setw(PP_SETW) << PD_x[j];
				cout << ") does not satisfies to the following inequality!: ";
				for (int j = 0; j < PP_N; j++)
					cout << setw(PP_SETW) << PD_A[i][j];
				cout << "\t<=" << setw(PP_SETW) << PD_b[i] << endl;
				*success = false; return;
			}
	}/**/

	PD_objF_x = ObjectiveF(PD_x);
	Vector_Copy(PD_x, PD_p);

	*success = true;
}

void PC_bsf_SetListSize(int *listSize) {
	*listSize = (int)PP_K;
}

void PC_bsf_CopyParameter(PT_bsf_parameter_T parameterIn, PT_bsf_parameter_T* parameterOutP) {
	// Nothing to do with stuff!
}

void PC_bsf_MapF(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T* reduceElem, int *success) {	// For Job 0
	static PT_vector_T p;	// Checkpoint coordinates
	PT_float_T objF_p;

	CheckPoint(p, BSF_sv_addressOffset * PP_N + BSF_sv_numberInSublist);
	for (int j = 0; j < PP_N; j++)
		p[j] += PD_x[j];

	/* Debug *//*
	if (BSF_sv_mpiRank == 0) {
		cout << "PC_bsf_MapF: ";
		for (int j = 0; j < PP_N; j++) 
			cout << setw(PP_SETW) << p[j];
	} /* End debug */

	for (int i = 0; i < PP_M; i++) {

		/* Debug *//*
		if (BSF_sv_mpiRank == 0)
			if (!PointIn(p, PD_A[i], PD_b[i])) {
				cout << "\tPoint NOT in!" << endl;
				//system("pause");
			}
		/* End debug */

		if (!PointIn(p, PD_A[i], PD_b[i])) {
			*success = false;
			return;
		}
	}

	objF_p = ObjectiveF(p);
	if (objF_p < PD_objF_x + PP_EPS_OBJECTIVE)
		reduceElem->ok = true;
	else {
		reduceElem->ok = false;
		if (objF_p > PD_objF_pMax) {
			Vector_Copy(p, PD_pMax);
			PD_objF_pMax = objF_p;
		}
			/* Debug *//*
			if (BSF_sv_mpiRank == 0) {
				if (fabs(objF_p - PD_objF_x) >= PP_EPS_ZERO && objF_p + PP_EPS_ZERO >= PD_objF_x) {
					cout << setw(PP_SETW) << "F(p)=" << objF_p << "\t>\tF(x)=" << PD_objF_x << endl;
					system("pause");
				}
			} /* End debug */
	}
}

void PC_bsf_MapF_1(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_1* reduceElem, int* success) {// For Job 1
	// Optional filling. Do not delete!
}

void PC_bsf_MapF_2(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_2* reduceElem, int* success) {// For Job 2
	// Optional filling. Do not delete!
}

void PC_bsf_MapF_3(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_3* reduceElem, int* success) {// For Job 3
	// Optional filling. Do not delete!
}

void PC_bsf_ReduceF(PT_bsf_reduceElem_T* x, PT_bsf_reduceElem_T* y, PT_bsf_reduceElem_T* z) {			// For Job 0
	z->ok = x->ok && y->ok;
}

void PC_bsf_ReduceF_1(PT_bsf_reduceElem_T_1* x, PT_bsf_reduceElem_T_1* y, PT_bsf_reduceElem_T_1* z) {	// For Job 1
	// Optional filling. Do not delete!
}

void PC_bsf_ReduceF_2(PT_bsf_reduceElem_T_2* x, PT_bsf_reduceElem_T_2* y, PT_bsf_reduceElem_T_2* z) {	// For Job 2
	// Optional filling. Do not delete!
}

void PC_bsf_ReduceF_3(PT_bsf_reduceElem_T_3* x, PT_bsf_reduceElem_T_3* y, PT_bsf_reduceElem_T_3* z) {	// For Job 3
	// Optional filling. Do not delete!
}

void PC_bsf_ProcessResults(		// For Job 0
	PT_bsf_reduceElem_T* reduceResult,
	int reduceCounter, 
	PT_bsf_parameter_T* parameter, 
	int *nextJob,
	bool *exit 
) {
	if (PD_firstTime) {
		PD_solutionIsOk = reduceResult->ok;
		if (PD_solutionIsOk) {
			*exit = true;
			return;
		}
	}

	double diff = fabs(PD_objF_pMax - PD_objF_x);
	if (diff < PP_EPS_ZERO) {
		*exit = true;
		return;
	}
	Vector_Copy(PD_pMax, PD_x);
	PD_objF_x = PD_objF_pMax;
	PD_firstTime = false;
}

void PC_bsf_ProcessResults_1(	// For Job 1	
	PT_bsf_reduceElem_T_1* reduceResult,
	int reduceCounter, 
	PT_bsf_parameter_T* parameter, 
	int* nextJob,
	bool* exit 
) {
	// Optional filling. Do not delete!
}

void PC_bsf_ProcessResults_2(	// For Job 2
	PT_bsf_reduceElem_T_2* reduceResult,
	int reduceCounter, 
	PT_bsf_parameter_T* parameter, 
	int* nextJob,
	bool* exit 
	) {
	// Optional filling. Do not delete!
}

void PC_bsf_ProcessResults_3(	// For Job 3
	PT_bsf_reduceElem_T_3* reduceResult,
	int reduceCounter, 
	PT_bsf_parameter_T* parameter, 
	int* nextJob,
	bool* exit 
	) {
	// Optional filling. Do not delete!
}

void PC_bsf_JobDispatcher(
	PT_bsf_parameter_T* parameter, // Current Approximation
	int* job,
	bool* exit
) {
	// Optional filling. Do not delete!
}

void PC_bsf_ParametersOutput(PT_bsf_parameter_T parameter) {
	cout << "=================================================== Problem parameters ====================================================" << endl;
	cout << "Number of Workers: " << BSF_sv_numOfWorkers << endl;
#ifdef PP_BSF_OMP
#ifdef PP_BSF_NUM_THREADS
	cout << "Number of Threads: " << PP_BSF_NUM_THREADS << endl;
#else
	cout << "Number of Threads: " << omp_get_num_procs() << endl;
#endif // PP_BSF_NUM_THREADS
#else
	cout << "OpenMP is turned off!" << endl;
#endif // PP_BSF_OMP

	cout << "Dimension: N = " << PP_N << endl;
	cout << "Number of Constraints: M = " << PP_M << endl;
	cout << "Density of Checkpoints per 2-D sphere: D = " << PP_D << endl;
	cout << "Total Number of Checkpoints: K = " << PP_K << endl;
#ifdef PP_MATRIX_OUTPUT
	cout << "------- Matrix A & Column b -------\n";
	for (int i = 0; i < PP_M; i++) {
		for (int j = 0; j < PP_N; j++)
			cout << setw(PP_SETW) << PD_A[i][j];
		cout << "\t<=" << setw(PP_SETW) << PD_b[i] << endl;
	};
#endif // PP_MATRIX_OUTPUT
	
	cout << "Objective Function: c = "; 
	for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PP_N); j++) cout << setw(PP_SETW) << PD_c[j];
	cout << (PP_OUTPUT_LIMIT < PP_N ? "	..." : "") << endl;
	cout << "Solution to Check:  x = ";
	for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PP_N); j++) cout << setw(PP_SETW) << PD_x[j];
	if (PP_OUTPUT_LIMIT < PP_N) cout << "	..." << setw(PP_SETW) << PD_x[PP_N - 1];
	cout << endl;
	cout << "Value of Objective Function in Point x = " << PD_objF_x << endl;
	cout << "-------------------------------------------" << endl;
}

void PC_bsf_IterOutput(PT_bsf_reduceElem_T* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 0
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;

}

void PC_bsf_IterOutput_1(PT_bsf_reduceElem_T_1* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 1
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;
	// Optional filling. Do not delete!

}

void PC_bsf_IterOutput_2(PT_bsf_reduceElem_T_2* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 2
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;
	// Optional filling. Do not delete!

}

void PC_bsf_IterOutput_3(PT_bsf_reduceElem_T_3* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 3
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;
	// Optional filling. Do not delete!

}

void PC_bsf_ProblemOutput(PT_bsf_reduceElem_T* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter, double t) {	// For Job 0
	cout << "=============================================" << endl;
	cout << "Time: " << t << endl;
	cout << "Points in polytope: " << round((double)reduceCounter*100 / (double)PP_K) << "%.\n";
	if (PD_solutionIsOk)
		cout << "The solution is correct!\n";
	else {
		cout << "The solution is NOT correct!\n";
		cout << "Correct solution:   p = ";
		for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PP_N); j++) cout << setw(PP_SETW) << PD_x[j];
		if (PP_OUTPUT_LIMIT < PP_N) cout << "	..." << setw(PP_SETW) << PD_x[PP_N - 1];
		cout << endl;
	}
}

void PC_bsf_ProblemOutput_1(PT_bsf_reduceElem_T_1* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 1
	// Optional filling. Do not delete!
}

void PC_bsf_ProblemOutput_2(PT_bsf_reduceElem_T_2* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 2
	// Optional filling. Do not delete!
}

void PC_bsf_ProblemOutput_3(PT_bsf_reduceElem_T_3* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 3
	// Optional filling. Do not delete!
}

void PC_bsf_SetInitParameter(PT_bsf_parameter_T* parameter) {

}

void PC_bsf_SetMapListElem(PT_bsf_mapElem_T *elem, int i) {
	// Nothing to do with stuff!
}

//----------------------- Assigning Values to BSF-skeleton Variables (Do not modify!) -----------------------
void PC_bsfAssignAddressOffset(int value) { BSF_sv_addressOffset = value; }
void PC_bsfAssignIterCounter(int value) { BSF_sv_iterCounter = value; }
void PC_bsfAssignJobCase(int value) { BSF_sv_jobCase = value; }
void PC_bsfAssignMpiMaster(int value) { BSF_sv_mpiMaster = value; }
void PC_bsfAssignMpiRank(int value) { BSF_sv_mpiRank = value; }
void PC_bsfAssignNumberInSublist(int value) { BSF_sv_numberInSublist = value; }
void PC_bsfAssignNumOfWorkers(int value) { BSF_sv_numOfWorkers = value; }
void PC_bsfAssignParameter(PT_bsf_parameter_T parameter) { PC_bsf_CopyParameter(parameter, &BSF_sv_parameter); }
void PC_bsfAssignSublistLength(int value) { BSF_sv_sublistLength = value; }

//----------------------------- User functions -----------------------------
static void CheckPoint(PT_vector_T p, int k) {
	static PT_angles_T phi;	// Checkpoint angles in the radial coordinate system
	PT_float_T fac = 1;

	Angles(phi, k);

	p[0] = PP_RHO * cos(phi[0]);
	for (int j = 1; j < PP_N - 2; j++) {
		fac *= sin(phi[j - 1]);
		p[j] = PP_RHO * cos(phi[j]) * fac;
	}

	fac *= sin(phi[PP_N - 3]);
	p[PP_N - 2] = PP_RHO * sin(phi[PP_N - 2]) * fac;
	p[PP_N - 1] = PP_RHO * cos(phi[PP_N - 2]) * fac;

	/* Debug *//*
	if (BSF_sv_mpiRank == 0) {
		cout << "CheckPoint("<< k << "):";
		for (int j = 0; j < PP_N; j++)
			cout << setw(PP_SETW) << p[j];
		//system("pause");
	} /* End debug */
}

static void Angles(PT_angles_T phi, int k) {
	int P = (int)pow(PP_D - 1,PP_N - 2);
	PT_float_T varphi = 3.1415926536 / PP_D;
	int u[PP_N];

	u[PP_N - 2] = k / P;
	u[PP_N - 1] = u[PP_N - 2];
	k %= P;
	for (int j = PP_N - 3; j >= 0; j--) {
		P /= PP_D - 1;
		u[j] = k / P + 1;
		k %= P;
	}
	for (int j = 0; j < PP_N - 1; j++) {
		phi[j] = varphi * (PT_float_T)(u[j]);
	}
}

inline bool PointIn(PT_vector_T x, PT_vector_T a, PT_float_T b) { // If the point belonges to the Halfspace <a,x> <= b
	PT_float_T dotProduct_a_x = Vector_DotProduct(a, x);

	if (dotProduct_a_x < b + PP_EPS_ZERO)
		return true;
	else
		return false;
}

inline double Vector_DotProduct(PT_vector_T x, PT_vector_T y) {
	double s = 0;
	for (int j = 0; j < PP_N; j++)
		s += x[j] * y[j];
	return s;
}

inline void Vector_Copy(PT_vector_T fromPoint, PT_vector_T toPoint) { // toPoint = fromPoint
	for (int j = 0; j < PP_N; j++) {
		toPoint[j] = fromPoint[j];
	}
}

inline void Vector_Subtraction(PT_vector_T x, PT_vector_T y, PT_vector_T z) {  // z = x - y
	for (int j = 0; j < PP_N; j++) {
		z[j] = x[j] - y[j];
	}
}

inline double Vector_NormSquare(PT_vector_T x) {
	double sum = 0;

	for (int j = 0; j < PP_N; j++) {
		sum += x[j] * x[j];
	}
	return sum;
}

inline double ObjectiveF(PT_vector_T x) {
	double s = 0;
	for (int j = 0; j < PP_N; j++)
		s += PD_c[j] * x[j];
	return s;
}