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
void PC_bsf_Init(bool* success) {

	if (PP_D % 2 == 0) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout
			<< "PP_D = " << PP_D << " must be an odd number!" << endl;
		*success = false;
		return;
	}

	if (PP_D < 3) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout
			<< "PP_D = " << PP_D << " must be greater than " << 2 << ".\n";
		*success = false;
		return;
	}

	PD_problemName = PP_PROBLEM_NAME;
	*success = LoadMatrixFormat();
	if (*success == false)
		return;

	if (PD_n < 3) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout
			<< "Dimension PD_n = " << PD_n << " must be greater than " << 2 << ".\n";
		*success = false;
		return;
	}

	double k = (2 * PP_D * pow(PP_D - 1, PD_n - 2));
	if (k > PP_MAX_K) {
		if (BSF_sv_mpiRank == BSF_sv_mpiMaster)
			cout
			<< "Number of points on hypersphere K = " << k << " must be < " << PP_MAX_K
			<< "\nYou need to decrease the PP_D parameter in Problem-Parameters.h" << endl;
		*success = false;
		return;
	}

	PD_K = (int)(k);

	PD_objF_x = ObjectiveF(PD_x);

	*success = true;
}

void PC_bsf_SetListSize(int* listSize) {
	*listSize = PD_K;
}

void PC_bsf_CopyParameter(PT_bsf_parameter_T parameterIn, PT_bsf_parameter_T* parameterOutP) {
	// Nothing to do with stuff!
}

void PC_bsf_MapF(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T* reduceElem, int* success) {	// For Job 0
	PT_float_T objF_p;
	int k = BSF_sv_addressOffset * PD_n + BSF_sv_numberInSublist;

	CheckPoint(PD_p, k);
	for (int j = 0; j < PD_n; j++)
		PD_p[j] += PD_x[j];

	/* Debug **
	if (BSF_sv_mpiRank == 0) {
		cout << "PC_bsf_MapF: ";
		for (int j = 0; j < PD_n; j++)
			cout << setw(PP_SETW) << PD_p[j];
	} /* End debug */

	for (int i = 0; i < PD_m; i++) {

		/* Debug **
		if (BSF_sv_mpiRank == 0)
			if (!PointIn(PD_p, PD_A[i], PD_b[i])) {
				cout << "\tPoint NOT in!" << endl;
				system("pause");
			}
		/* End debug */

		if (!PointIn(PD_p, PD_A[i], PD_b[i])) {
			*success = false;
			return;
		}
	}

	objF_p = ObjectiveF(PD_p);
	if (objF_p < PD_objF_x + PP_EPS_OBJECTIVE)
		reduceElem->ok = true;
	else {
		reduceElem->ok = false;
		if (objF_p > PD_objF_pMax) {
			Vector_Copy(PD_p, PD_pMax);
			PD_objF_pMax = objF_p;
		}
		/* Debug **
		if (BSF_sv_mpiRank == 0) {
			if (fabs(objF_p - PD_objF_x) >= PP_EPS_ZERO && objF_p + PP_EPS_ZERO >= PD_objF_x) {
				cout << setw(PP_SETW) << "F(p)=" << objF_p << "\t>\tF(x)=" << PD_objF_x << endl;
				system("pause");
			}
		} /* End debug */
	}
}

void PC_bsf_MapF_1(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_1* reduceElem, int* success) {// For Job 1
	// Not used.
}

void PC_bsf_MapF_2(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_2* reduceElem, int* success) {// For Job 2
	// Not used.
}

void PC_bsf_MapF_3(PT_bsf_mapElem_T* mapElem, PT_bsf_reduceElem_T_3* reduceElem, int* success) {// For Job 3
	// Not used.
}

void PC_bsf_ReduceF(PT_bsf_reduceElem_T* x, PT_bsf_reduceElem_T* y, PT_bsf_reduceElem_T* z) {			// For Job 0
	z->ok = x->ok && y->ok;
}

void PC_bsf_ReduceF_1(PT_bsf_reduceElem_T_1* x, PT_bsf_reduceElem_T_1* y, PT_bsf_reduceElem_T_1* z) {	// For Job 1
	// Not used.
}

void PC_bsf_ReduceF_2(PT_bsf_reduceElem_T_2* x, PT_bsf_reduceElem_T_2* y, PT_bsf_reduceElem_T_2* z) {	// For Job 2
	// Not used.
}

void PC_bsf_ReduceF_3(PT_bsf_reduceElem_T_3* x, PT_bsf_reduceElem_T_3* y, PT_bsf_reduceElem_T_3* z) {	// For Job 3
	// Not used.
}

void PC_bsf_ProcessResults(		// For Job 0
	PT_bsf_reduceElem_T* reduceResult,
	int reduceCounter,
	PT_bsf_parameter_T* parameter,
	int* nextJob,
	bool* exit
) {
	if (PD_firstTime) {
		PD_solutionIsOk = reduceResult->ok;
		if (PD_solutionIsOk) {
			*exit = true;
			return;
		}
	}

	PT_float_T diff = fabs(PD_objF_pMax - PD_objF_x);
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
	// Not used.
}

void PC_bsf_ProcessResults_2(	// For Job 2
	PT_bsf_reduceElem_T_2* reduceResult,
	int reduceCounter,
	PT_bsf_parameter_T* parameter,
	int* nextJob,
	bool* exit
) {
	// Not used.
}

void PC_bsf_ProcessResults_3(	// For Job 3
	PT_bsf_reduceElem_T_3* reduceResult,
	int reduceCounter,
	PT_bsf_parameter_T* parameter,
	int* nextJob,
	bool* exit
) {
	// Not used.
}

void PC_bsf_JobDispatcher(
	PT_bsf_parameter_T* parameter, // Current Approximation
	int* job,
	bool* exit,
	double t
) {
	// Not used.
}

void PC_bsf_ParametersOutput(PT_bsf_parameter_T parameter) {
	cout << "=================================================== Problem parameters ====================================================" << endl;
	cout << "Problem name: " << PD_problemName << endl;
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
	// Not used

}

void PC_bsf_IterOutput_2(PT_bsf_reduceElem_T_2* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 2
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;
	// Not used

}

void PC_bsf_IterOutput_3(PT_bsf_reduceElem_T_3* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double elapsedTime, int jobCase) {	// For Job 3
	cout << "------------------ " << BSF_sv_iterCounter << " ------------------" << endl;
	// Not used

}

void PC_bsf_ProblemOutput(PT_bsf_reduceElem_T* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter, double t) {	// For Job 0
	cout << "=============================================" << endl;
	cout << "Points in polytope: " << round((PT_float_T)reduceCounter * 100 / (PT_float_T)PD_K) << "%.\n";
	if (PD_solutionIsOk) {
		cout << "The solution is correct!\n";
	}
	else {
		cout << "The solution is NOT correct!\n";
		cout << setprecision(16);
		cout << "Correct solution:   p = ";
		for (int j = 0; j < PF_MIN(PP_OUTPUT_LIMIT, PD_n); j++) cout << setw(PP_SETW) << PD_x[j] << "\t";
		if (PP_OUTPUT_LIMIT < PD_n) cout << "	..." << setw(PP_SETW) << PD_x[PD_n - 1];
		PD_objF_x = ObjectiveF(PD_x);
		cout << "\t F(p) = " << PD_objF_x << endl;
	}
}

void PC_bsf_ProblemOutput_1(PT_bsf_reduceElem_T_1* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 1
	// Not used
}

void PC_bsf_ProblemOutput_2(PT_bsf_reduceElem_T_2* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 2
	// Not used
}

void PC_bsf_ProblemOutput_3(PT_bsf_reduceElem_T_3* reduceResult, int reduceCounter, PT_bsf_parameter_T parameter,
	double t) {	// For Job 3
	// Not used
}

void PC_bsf_SetInitParameter(PT_bsf_parameter_T* parameter) {

}

void PC_bsf_SetMapListElem(PT_bsf_mapElem_T* elem, int i) {
	// Nothing to do with stuff!
}

// MPI
// MPI
// MPI
// MPI
// MPI
// MPI

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
static bool LoadMatrixFormat() {
	int nor,	// Number of matrix rows
		noc,	// Number of matrix columns
		non,	// Number of non-zero elements
		noe;	// Number of equations
	const char* mtxFile;
	FILE* stream;// Input stream
	char str[80] = { '\0' };
	char* chr = str;

	//--------------- Reading A ------------------
	PD_MTX_File_A = PP_PATH;
	PD_MTX_File_A += PP_MTX_PREFIX;
	PD_MTX_File_A += PD_problemName;
	PD_MTX_File_A += PP_MTX_POSTFIX_A;
	mtxFile = PD_MTX_File_A.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d%d", &nor, &noc, &non) < 3) {
		//
		cout
			<< "Unexpected end of file " << mtxFile << endl;
		return false;
	}

	if (nor >= noc) {
		//
		cout
			<< "Number of rows m = " << nor << " must be < " << "Number of columns n = " << noc << "\n";
		return false;
	}

	if (noc != PP_N) {
		//
		cout
			<< "Invalid input data: PP_N must be = " << noc << "\n";
		return false;
	}

	if (nor != PP_M) {
		//
		cout
			<< "Invalid input data: PP_M must be = " << nor << "\n";
		return false;
	}

	PD_m = noe = nor;
	PD_n = noc;

	if (2 * nor + noc > PP_MM) {
		//
		cout
			<< "Invalid input data: number of inequalities m = " << 2 * nor + noc
			<< " must be < PP_MM + 1 =" << PP_MM + 1 << "\n";
		return false;
	}

	for (int k = 0; k < non; k++) {
		int i, j;

		if (fscanf(stream, "%d%d%s", &i, &j, str) < 3) {
			//	
			cout
				<< "Unexpected end of file'" << mtxFile << "'." << endl;
			return false;
		}

		i -= 1;
		j -= 1;
		if (i < 0) {
			//
			cout
				<< "Negative row index in'" << mtxFile << "'.\n" << endl;
			return false;
		}
		if (j < 0) {
			//
			cout
				<< "Negative column index in'" << mtxFile << "'.\n" << endl;
			return false;
		}
		PD_A[i][j] = strtod(str, &chr);
	}

	/*debug*
	for (int i = 0; i < PD_m; i++) {
		for (int j = 0; j < PD_n; j++)
			cout << PD_A[i][j] << " ";
		cout << endl;
	}
	/*end debug*/

	fclose(stream);

	//--------------- Reading b ------------------
	PD_MTX_File_b = PP_PATH;
	PD_MTX_File_b += PP_MTX_PREFIX;
	PD_MTX_File_b += PD_problemName;
	PD_MTX_File_b += PP_MTX_POSTFIX_B;
	mtxFile = PD_MTX_File_b.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d", &nor, &noc) < 2) {
		//
		cout
			<< "Unexpected end of file'" << mtxFile << "'." << endl;
		return false;
	}
	if (noe != nor) {
		//
		cout
			<< "Incorrect number of rows in'" << mtxFile << "'.\n";
		return false;
	}
	if (noc != 1) {
		//
		cout
			<< "Incorrect number of columnws in'" << mtxFile << "'.\n";
		return false;
	}

	for (int i = 0; i < noe; i++) {
		if (fscanf(stream, "%s", str) < 1) {
			//
			cout
				<< "Unexpected end of file '" << mtxFile << "'." << endl;
			return false;
		}
		PD_b[i] = strtod(str, &chr);
	}
	fclose(stream);

	/*debug*
	for (int i = 0; i < PD_m; i++)
		cout << PD_b[i] << endl;
	/*end debug*/

	//--------------- Reading lo ------------------
	PD_MTX_File_lo = PP_PATH;
	PD_MTX_File_lo += PP_MTX_PREFIX;
	PD_MTX_File_lo += PD_problemName;
	PD_MTX_File_lo += PP_MTX_POSTFIX_LO;
	mtxFile = PD_MTX_File_lo.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d", &nor, &noc) < 2) {
		//
		cout
			<< "Unexpected end of file'" << mtxFile << "'." << endl;
		return false;
	}
	if (nor != PD_n) {
		//
		cout
			<< "Incorrect number of rows in'" << mtxFile << "'.\n";
		return false;
	}
	if (noc != 1) {
		//
		cout
			<< "Incorrect number of columnws in'" << mtxFile << "'.\n";
		return false;
	}

	for (int j = 0; j < PD_n; j++) {
		if (fscanf(stream, "%s", str) < 1) {
			//
			cout
				<< "Unexpected end of file '" << mtxFile << "'." << endl;
			return false;
		}
		PD_lo[j] = strtod(str, &chr);
	}

	fclose(stream);

	//--------------- Reading c ------------------
	PD_MTX_File_c = PP_PATH;
	PD_MTX_File_c += PP_MTX_PREFIX;
	PD_MTX_File_c += PD_problemName;
	PD_MTX_File_c += PP_MTX_POSTFIX_C;
	mtxFile = PD_MTX_File_c.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d", &nor, &noc) < 2) {
		//
		cout
			<< "Unexpected end of file'" << mtxFile << "'." << endl;
		return false;
	}
	if (nor != PD_n) {
		//
		cout
			<< "Incorrect number of rows in'" << mtxFile << "'.\n";
		return false;
	}
	if (noc != 1) {
		//
		cout
			<< "Incorrect number of columnws in'" << mtxFile << "'.\n";
		return false;
	}

	for (int j = 0; j < PD_n; j++) {
		if (fscanf(stream, "%s", str) < 0) {
			//
			cout
				<< "Unexpected end of file" << endl;
			return false;
		}
		PD_c[j] = -strtod(str, &chr);
	}
	fclose(stream);

	/*debug*
	for (int i = 0; i < PD_n; i++)
		cout << PD_c[i] << " ";
	cout << endl;
	/*end debug*/

	//--------------- Reading hi ------------------
	PD_MTX_File_hi = PP_PATH;
	PD_MTX_File_hi += PP_MTX_PREFIX;
	PD_MTX_File_hi += PD_problemName;
	PD_MTX_File_hi += PP_MTX_POSTFIX_HI;
	mtxFile = PD_MTX_File_hi.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d", &nor, &noc) < 2) {
		//
		cout
			<< "Unexpected end of file'" << mtxFile << "'." << endl;
		return false;
	}
	if (nor != PD_n) {
		//
		cout
			<< "Incorrect number of rows in'" << mtxFile << "'.\n";
		return false;
	}
	if (noc != 1) {
		//
		cout
			<< "Incorrect number of columnws in'" << mtxFile << "'.\n";
		return false;
	}

	for (int j = 0; j < PD_n; j++) {
		if (fscanf(stream, "%s", str) < 1) {
			//
			cout
				<< "Unexpected end of file '" << mtxFile << "'." << endl;
			return false;
		}
		PD_hi[j] = strtod(str, &chr);
	}
	fclose(stream);

	bool error = !Conversion();
	if (error) return false;

	//--------------- Reading so ------------------
	PD_MTX_File_so = PP_PATH;
	PD_MTX_File_so += PP_MTX_PREFIX;
	PD_MTX_File_so += PD_problemName;
	PD_MTX_File_so += PP_MTX_POSTFIX_SO;
	mtxFile = PD_MTX_File_so.c_str();
	stream = fopen(mtxFile, "r+b");

	if (stream == NULL) {
		//
		cout
			<< "Failure of opening file '" << mtxFile << "'.\n";
		return false;
	}

	SkipComments(stream);
	if (fscanf(stream, "%d%d", &nor, &noc) < 2) {
		//
		cout
			<< "Unexpected end of file'" << mtxFile << "'." << endl;
		return false;
	}
	if (nor != PD_n) {
		//
		cout
			<< "Incorrect number of rows in'" << mtxFile << "'.\n";
		return false;
	}
	if (noc != 1) {
		//
		cout
			<< "Incorrect number of columnws in'" << mtxFile << "'.\n";
		return false;
	}

	for (int j = 0; j < PD_n; j++) {
		if (fscanf(stream, "%s", str) < 0) {
			//
			cout
				<< "Unexpected end of file" << endl;
			return false;
		}
		PD_x[j] = strtod(str, &chr);
	}
	fclose(stream);
	return true;
}

static bool Conversion() { // Transformation to inequalities & dimensionality reduction
	static PT_float_T fvA[PP_MM]; // Free variable coefficients
	static bool Flag[PP_N];		// Flags of free variables to delete
	static int fvEqI;	// Inequality index of free variable
	static bool single;

	for (int jc = 0; jc < PD_n; jc++)
		if (PD_c[jc] == 0 && PD_hi[jc] == PP_INFINITY && PD_lo[jc] == 0)
			Flag[jc] = true;

	for (int jc = 0; jc < PD_n; jc++) {
		if (!Flag[jc])
			continue;
		for (int i = 0; i < PD_m; i++) { // Find corresponding equation
			if (PD_A[i][jc] == 0)
				continue;
			single = true;
			for (int ii = i + 1; ii < PD_m; ii++) // Vertical uniqueness
				if (PD_A[ii][jc] != 0) {
					single = false;
					break;
				}
			if (single)
				fvEqI = i;
			break;
		}

		if (!single) {
			Flag[jc] = false;
		}
		else
			fvA[fvEqI] = PD_A[fvEqI][jc];
	}

	/*debug*
	for (int j = 0; j < PD_n; j++)
		cout << Flag[j] << " ";
	cout << endl;
	/*end debug*/

	/*debug*
	for (int i = 0; i < PD_m; i++)
		cout << fvA[i] << endl;
	/*end debug*/


	static bool PD_delete[PP_MM]; // Rows to delete
	PT_float_T s;

	for (int i = 0; i < PD_m; i++) { // Check inconsistent end degenerate equation
		s = 0;
		for (int j = 0; j < PD_n; j++)
			s += fabs(PD_A[i][j]);
		if (s == 0) {
			if (PD_b[i] != 0) {
				//
				cout
					<< "Inconsistent equation " << i << ": " << s << " = " << PD_b[i] << endl;
				return false;
				PD_delete[i] = true;
			}
		}
	}

	for (int i = 0; i < PD_m; i++) { // Removing degenerate equations
		if (!PD_delete[i]) continue;
		for (int ii = i; ii < PD_m - 1; ii++) {  // Remove null equation
			for (int j = 0; j < PD_n; j++)
				PD_A[ii][j] = PD_A[ii + 1][j];
			fvA[ii] = fvA[ii + 1];
			PD_b[ii] = PD_b[ii + 1];
		}
		PD_m--;
	}

	for (int jc = 0; jc < PD_n; jc++) { // Delete free variables
		if (!Flag[jc])
			continue;
		for (int j = jc; j < PD_n - 1; j++) { // Delete column
			PD_c[j] = PD_c[j + 1];
			PD_lo[j] = PD_lo[j + 1];
			PD_hi[j] = PD_hi[j + 1];
			Flag[j] = Flag[j + 1];
			for (int i = 0; i < PD_m; i++)
				PD_A[i][j] = PD_A[i][j + 1];
		}

		PD_n--;
		jc--;
		for (int i = 0; i < PD_m; i++)
			PD_A[i][PD_n] = 0;
		PD_c[PD_n] = 0;
		PD_lo[PD_n] = 0;
		PD_hi[PD_n] = 0;
	}

	/*debug*
	for (int i = 0; i < PD_m; i++) {
		cout << i << ")\t";
		for (int j = 0; j < PD_n; j++)
			cout << PD_A[i][j] << " ";
		cout << endl;
	}
	cout << "----------------------------------------\n";
	/*end debug*/

	int m = PD_m;
	for (int i = 0; i < m; i++) { // Conversion to inequalities

		if (fvA[i] == 0) { // Equation without free variable => adding inequality.
			for (int j = 0; j < PD_n; j++)
				PD_A[PD_m][j] = -PD_A[i][j];
			PD_b[PD_m] = -PD_b[i];
			PD_m++;
			assert(PD_m <= PP_MM);
		}
		else {
			if (fvA[i] < 0) { // Free variable is negative => change sign to opposite.
				for (int j = 0; j < PD_n; j++)
					PD_A[i][j] = -PD_A[i][j];
				PD_b[i] = -PD_b[i];
			}
		}
	}

	/*debug*
	for (int i = 0; i < PD_m; i++) {
		cout << i << ")\t";
		for (int j = 0; j < PD_n; j++)
			cout << PD_A[i][j] << " ";
		cout << endl;
	}
	cout << "----------------------------------------\n";
	/*end debug*/

	for (int i = 0; i < PD_m; i++) // Remove negative sign for zero value
		for (int j = 0; j < PD_n; j++)
			if (PD_A[i][j] == 0)
				PD_A[i][j] = 0;

	for (int i = 0; i < PD_n; i++) { // Adding lower bound conditions
		for (int j = 0; j < PD_n; j++)
			PD_A[i + PD_m][j] = 0;
		PD_A[i + PD_m][i] = -1;
		if (PD_lo[i] == 0)
			PD_b[i + PD_m] = 0;
		else
			PD_b[i + PD_m] = -PD_lo[i];
	}
	PD_m += PD_n;
	assert(PD_m <= PP_MM);

	for (int i = 0; i < PD_n; i++) { // Adding higher bound conditions
		if (PD_hi[i] != PP_INFINITY) {
			for (int j = 0; j < PD_n; j++)
				PD_A[PD_m][j] = 0;
			PD_A[PD_m][i] = 1;
			PD_b[PD_m] = PD_hi[i];
			PD_m++;
			assert(PD_m <= PP_MM);
		}
	}

	/*debug*
	for (int i = 0; i < PD_m; i++) {
		cout << i << ")\t";
		for (int j = 0; j < PD_n; j++)
			cout << PD_A[i][j] << " ";
		cout << endl;
	}
	cout << "----------------------------------------\n";
	/*end debug*/

	/*debug*
	for (int j = 0; j < PD_n; j++)
		cout << PD_c[j] << endl;
	/*end debug*/

	return true;
}

inline void SkipComments(FILE* stream) {
	fpos_t pos;	// Position in the input stream
	int res;
	res = fscanf(stream, "\n");
	fgetpos(stream, &pos);
	while (getc(stream) == '%') {
		while (getc(stream) != 10);
		res = fscanf(stream, "\n");
		fgetpos(stream, &pos);
	};
	fsetpos(stream, &pos);
}

static void CheckPoint(PT_vector_T p, int k) {
	PT_float_T fac = 1;

	Angles(PD_phi, k);

	p[0] = PP_RHO * cos(PD_phi[0]);
	for (int j = 1; j < PD_n - 2; j++) {
		fac *= sin(PD_phi[j - 1]);
		p[j] = PP_RHO * cos(PD_phi[j]) * fac;
	}

	fac *= sin(PD_phi[PD_n - 3]);
	p[PD_n - 2] = PP_RHO * sin(PD_phi[PD_n - 2]) * fac;
	p[PD_n - 1] = PP_RHO * cos(PD_phi[PD_n - 2]) * fac;

	/* Debug **
	if (BSF_sv_mpiRank == 0) {
		cout << "CheckPoint("<< k << "):";
		for (int j = 0; j < PD_n; j++)
			cout << setw(PP_SETW) << p[j];
		cout << endl;
		system("pause");
	} /* End debug */
}

static void Angles(PT_angles_T phi, int k) {
	int P = (int)pow(PP_D - 1, PD_n - 2);
	PT_float_T varphi = 3.1415926536 / PP_D;
	int u[PP_N];

	u[PD_n - 2] = k / P;
	u[PD_n - 1] = u[PD_n - 2];
	k %= P;
	for (int j = PD_n - 3; j >= 0; j--) {
		P /= PP_D - 1;
		u[j] = k / P + 1;
		k %= P;
	}
	for (int j = 0; j < PD_n - 1; j++) {
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

inline PT_float_T Vector_DotProduct(PT_vector_T x, PT_vector_T y) {
	PT_float_T s = 0;
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

inline PT_float_T Vector_NormSquare(PT_vector_T x) {
	PT_float_T sum = 0;

	for (int j = 0; j < PD_n; j++) {
		sum += x[j] * x[j];
	}
	return sum;
}

inline PT_float_T ObjectiveF(PT_vector_T x) {
	PT_float_T s = 0;
	for (int j = 0; j < PD_n; j++)
		s += PD_c[j] * x[j];
	return s;
}