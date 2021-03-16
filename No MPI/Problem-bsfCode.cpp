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
	float buf;

	if (PP_D % 2 == 1) {
		cout << "Density PP_D = " << PP_D << " must be must be a even number!" << endl;
		*success = false;
		system("pause");
		return;
	}

	if (PP_D < 4) {
		cout << "Density PP_D = " << PP_D << " must be greater than " << 3 << ".\n";
		*success = false;
		system("pause");
		return;
	}

	// ------------- Load LPP data -------------------
	/*cout << "Enter LPP file name: ";
	cin >> PD_lppFile;
	const char* lppFile = PD_lppFile.c_str();/**/
	const char* lppFile = "lpp.txt";
		stream = fopen(lppFile, "r");
	if (stream == NULL) {
		cout << "Failure of opening file '" << lppFile << "'.\n";
		*success = false;
		system("pause");
		return;
	}

	fscanf(stream, "%d%d", &PD_m, &PD_n);

	if (PD_n > PP_MAX_N) {
		cout << "Invalid input data: Space dimension n = " << PD_n << " must be < " << PP_MAX_N + 1 << "\n";
		*success = false;
		system("pause");
		return;
	}

	if (PD_m > PP_MAX_M) {
		cout << "Invalid input data: Number of inequalities m = " << PD_m << " must be < " << PP_MAX_M + 1 << "\n";
		*success = false;
		system("pause");
		return;
	}

	if (PD_n < 3) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Dimension PD_n = " << PD_n << " must be greater than " << 2 << ".\n";
		*success = false; 
		system("pause");
		return;
	}

	PD_K = (int)(PP_D * pow(PP_D / 2 - 1, PD_n - 2));

	if (PD_K > PP_MAX_K) {
		cout << "Invalid input data: Number of points on hypersphere K = " << PD_K << " must be < " << PP_MAX_K + 1 << "\n";
		*success = false;
		system("pause");
		return;
	}

	for (int i = 0; i < PD_m; i++) {
		for (int j = 0; j < PD_n; j++) {
			fscanf(stream, "%f", &buf);
			PD_A[i][j] = buf;
		}
		fscanf(stream, "%f", &buf);
		PD_b[i] = buf;
	}

	for (int j = 0; j < PD_n; j++) {
		fscanf(stream, "%f", &buf);
		PD_c[j] = buf;
	}
	fclose(stream);

	// --------------- Load solution ---------------
	/*cout << "Enter solution file name: ";
	cin >> PD_solutionFile;
	const char* solutionFile = PD_solutionFile.c_str();/**/
	const char* solutionFile = "solution.txt";
	stream = fopen(solutionFile, "r");
	if (stream == NULL) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Failure of opening file '" << PD_solutionFile << "'.\n";
		*success = false; return;
	}

	int n;
	fscanf(stream, "%d", &n);
	if (n != PD_n) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout << "Error in input data '" << PD_solutionFile << "': PD_n != n (PD_n = " << PD_n << ", n = " << n << ").\n";
		*success = false; return;
	}

	for (int j = 0; j < PD_n; j++) {
		fscanf(stream, "%f", &buf);
		PD_x[j] = buf;
	}

	fclose(stream);

	/*if (BSF_sv_mpiRank == 0) {
		int counter = 0;
		for (int i = 0; i < PD_m; i++)
			if (!PointIn(PD_x, PD_A[i], PD_b[i])) {
				cout << "\tPoint x = (";
				for (int j = 0; j < PD_n; j++)
					cout << setw(PP_SETW) << PD_x[j];
				cout << ") does not satisfies to the following inequality!: ";
				for (int j = 0; j < PD_n; j++)
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
	*listSize = (int)PD_K;
}

void PC_bsf_CopyParameter(PT_bsf_parameter_T parameterIn, PT_bsf_parameter_T* parameterOutP) {
	// Nothing to do with stuff!
}

void PC_bsf_MapF(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T* reduceElem, int *success) {	// For Job 0
	static PT_vector_T p;	// Checkpoint coordinates
	PT_float_T objF_p;

	CheckPoint(p, BSF_sv_addressOffset * PD_n + BSF_sv_numberInSublist);
	for (int j = 0; j < PD_n; j++)
		p[j] += PD_x[j];

	/* Debug *//*
	if (BSF_sv_mpiRank == 0) {
		cout << "PC_bsf_MapF: ";
		for (int j = 0; j < PD_n; j++) 
			cout << setw(PP_SETW) << p[j];
	} /* End debug */

	for (int i = 0; i < PD_m; i++) {

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
	if (objF_p < PD_objF_x)
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
/*	cout << "Number of Workers: " << BSF_sv_numOfWorkers << endl;
#ifdef PP_BSF_OMP
#ifdef PP_BSF_NUM_THREADS
	cout << "Number of Threads: " << PP_BSF_NUM_THREADS << endl;
#else
	cout << "Number of Threads: " << omp_get_num_procs() << endl;
#endif // PP_BSF_NUM_THREADS
#else
	cout << "OpenMP is turned off!" << endl;
#endif // PP_BSF_OMP

	cout << "Dimension: N = " << PD_n << endl;
	cout << "Number of Constraints: M = " << PD_m << endl;
	cout << "Density of Checkpoints per 2-D sphere: D = " << PP_D << endl;
	cout << "Total Number of Checkpoints: K = " << PD_K << endl;
#ifdef PP_MATRIX_OUTPUT
	cout << "------- Matrix A & Column b -------\n";
	for (int i = 0; i < PD_m; i++) {
		for (int j = 0; j < PD_n; j++)
			cout << setw(PP_SETW) << PD_A[i][j];
		cout << "\t<=" << setw(PP_SETW) << PD_b[i] << endl;
	};
#endif // PP_MATRIX_OUTPUT
	
	cout << "Objective Function: c = "; 
	for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PD_n); j++) cout << setw(PP_SETW) << PD_c[j];
	cout << (PP_OUTPUT_LIMIT < PD_n ? "	..." : "") << endl;
	cout << "Solution to Check:  x = ";
	for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PD_n); j++) cout << setw(PP_SETW) << PD_x[j];
	if (PP_OUTPUT_LIMIT < PD_n) cout << "	..." << setw(PP_SETW) << PD_x[PD_n - 1];
	cout << endl;
	cout << "Value of Objective Function in Point x = " << PD_objF_x << endl;
	cout << "-------------------------------------------" << endl;/**/
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
	cout << "Points in polytope: " << round((double)reduceCounter*100 / (double)PD_K) << "%.\n";
	if (PD_solutionIsOk) {
		cout << "The solution is correct!\n";
		// Copy trace.txt to *.trc
		string fileName = to_string(PD_n) + "_" + to_string((int)PD_A[2 * PD_n + 1][0]);
		ifstream inTraceFile("trace.txt");
		PD_traceFile = fileName + ".trc";
		const char* traceFile = PD_traceFile.c_str();
		ofstream outTraceFile(traceFile);
		outTraceFile << inTraceFile.rdbuf();
		// Copy lpp.txt to *.lpp
		ifstream inLppFile("lpp.txt");
		PD_traceFile = fileName + ".lpp";
		const char* lppFile = PD_traceFile.c_str();
		ofstream outLppFile(lppFile);
		outLppFile << inLppFile.rdbuf();
	} else {
		cout << "The solution is NOT correct!\n";
		cout << "Correct solution:   p = ";
		for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PD_n); j++) cout << setw(PP_SETW) << PD_x[j];
		if (PP_OUTPUT_LIMIT < PD_n) cout << "	..." << setw(PP_SETW) << PD_x[PD_n - 1];
		cout << endl;
	}
	//system("pause");
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

#define CHECK_ZERO(x) 	(fabs(x)>PP_EPS_ZERO?x:0)	
	Angles(phi, k);

	p[0] = PP_RHO * cos(phi[0]); p[0] = CHECK_ZERO(p[0]);
	if (fabs(p[0]) < PP_EPS_ZERO) p[0] = 0;
	for (int j = 1; j < PD_n - 2; j++) {
		fac *= sin(phi[j - 1]);
		p[j] = PP_RHO * cos(phi[j]) * fac; p[j] = CHECK_ZERO(p[j]);
	}

	fac *= sin(phi[PD_n - 3]);
	p[PD_n - 2] = PP_RHO * sin(phi[PD_n - 2]) * fac; p[PD_n - 2] = CHECK_ZERO(p[PD_n - 2]);
	p[PD_n - 1] = PP_RHO * cos(phi[PD_n - 2]) * fac; p[PD_n - 1] = CHECK_ZERO(p[PD_n - 1]);

	/* Debug *//*
	if (BSF_sv_mpiRank == 0) {
		cout << "CheckPoint("<< k << "):";
		for (int j = 0; j < PD_n; j++)
			cout << setw(PP_SETW) << p[j];
		//system("pause");
	} /* End debug */
}

static void Angles(PT_angles_T phi, int k) {
	int P = (int)pow(PP_D / 2 - 1,PD_n - 2);
	PT_float_T fac = 6.283185307179586476925286766559 / PP_D;
	int v[PP_MAX_N - 1];

	v[PD_n - 2] = k / P;
	k %= P;
	for (int j = PD_n - 3; j > -1; j--) {
		P /= PP_D / 2 - 1;
		v[j] = k / P + 1;
		k %= P;
	}
	for (int j = 0; j < PD_n - 1; j++) {
		phi[j] = fac * (PT_float_T)(v[j]);
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
	for (int j = 0; j < PD_n; j++)
		s += x[j] * y[j];
	return s;
}

inline void Vector_Copy(PT_vector_T fromPoint, PT_vector_T toPoint) { // toPoint = fromPoint
	for (int j = 0; j < PD_n; j++) {
		toPoint[j] = fromPoint[j];
	}
}

inline void Vector_Subtraction(PT_vector_T x, PT_vector_T y, PT_vector_T z) {  // z = x - y
	for (int j = 0; j < PD_n; j++) {
		z[j] = x[j] - y[j];
	}
}

inline double Vector_NormSquare(PT_vector_T x) {
	double sum = 0;

	for (int j = 0; j < PD_n; j++) {
		sum += x[j] * x[j];
	}
	return sum;
}

inline double ObjectiveF(PT_vector_T x) {
	double s = 0;
	for (int j = 0; j < PD_n; j++)
		s += PD_c[j] * x[j];
	return s;
}