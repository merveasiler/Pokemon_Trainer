#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

// g++ -ansi -pedantic run_tests.cpp -o run_tests

const char* COMPILER[8][9];
const char* RUNNER[8][9];
int CASE_CMD[8][9];
bool MEMORY_CHECK[8][9];
int numberOfTests = 5;
int numberOfParts[8] = {-1, 7, 6, 8, 3, 1};
int max_points[8][9][5] = 
{
	{{},{},{},{},{}},
	{{}, {3}, {1}, {1}, {4}, {2, 2, 2}, {3, 3}, {3, 3}},
	{{}, {4}, {2}, {4}, {3}, {3}, {2}},
	{{}, {3}, {2}, {1}, {4}, {1}, {1, 1, 2, 2, 1}, {2, 2, 1}, {3, 3, 4}},
	{{}, {2, 2, 2}, {2, 2, 2}, {1, 1, 1}},
	{{}, {7}}
};
int points[8][9][5];
int penalties[8][9][5];

void printDetailedResults();
void init();
int readResult();
float checkMemoryLeaks(int testId, int partId, int caseId);
int checkComplexity();
int checkEmptyCells();

int main(int argc, char* argv[]) {

	init();

	char buffer[2048];
	int status = 0;
	int retval_compile_check = 0;
	int retval_run_check = 0;
	int i, j, k;
	float grade = 0;
	int STUDENT_ID;

	for (i=1; i<=numberOfTests; i++) {
		for (j=1; j<=numberOfParts[i]; j++) {
			sprintf(buffer, COMPILER[i][j], i, j, i, j);
			status = system(buffer);
			retval_compile_check = WEXITSTATUS(status);

			if (retval_compile_check == 0)
				for (k=1; k<=CASE_CMD[i][j]; k++) {
					sprintf(buffer, RUNNER[i][j], i, j, k);
					status = system(buffer);
					retval_run_check = WEXITSTATUS(status);

					if (retval_run_check == 0) {
						points[i][j][k-1] = readResult();
						if (MEMORY_CHECK[i][j])
							penalties[i][j][k-1] = checkMemoryLeaks(i, j, k);
						grade += points[i][j][k-1] + penalties[i][j][k-1];
					}
				}
		}
	}

	//printDetailedResults();
	
	if (grade > 30) {
		grade += checkComplexity();
		grade += checkEmptyCells();
	}
	sscanf(argv[1], "%d", &STUDENT_ID);
	fstream myfile("grades.txt", ios::app);;
	myfile << STUDENT_ID << " " << grade << endl;
	myfile.close();
	

	return 0;
}

void printDetailedResults() {

	int i, j, k;
	float grade = 0, penalty = 0;

	cout << endl << endl;
	for (i=1; i<=numberOfTests; i++) {
		for (j=1; j<=numberOfParts[i]; j++)
			for (k=1; k<=CASE_CMD[i][j]; k++) {
				grade += points[i][j][k-1];
				grade += penalties[i][j][k-1];

				if (points[i][j][k-1] < max_points[i][j][k-1]) {	
					cout << "You got " << points[i][j][k-1] << " out of " << max_points[i][j][k-1];
					cout << " points from test" << i << "/part" << j << "/case" << k << endl;
				}
				if (penalties[i][j][k-1] > 0) {
					cout << "PENALTY: " << penalties[i][j][k-1];
					cout << " point(s) cut-off was applied due to memory leaks from test";
					cout << i << "/part" << j << "/case" << k << endl;
				}
			}
	}

	if (grade > 30) {
		penalty = checkComplexity();
		grade += penalty;
		if (penalty < 0)
		     cout << "PENALTY: " << penalty << " point(s) cut-off was applied due to exceeding log(n) time complexity limit." << endl;

		penalty = checkEmptyCells();
		grade += penalty;
		if (penalty < 0)
			cout << "PENALTY: " << penalty << " point(s) cut-off was applied due to not deleting empty cells." << endl;
	}

	cout << "TOTAL GRADE: " << grade << endl;
}

void init() {
	
	const char* COMPILE_CMD_TYPE1 = "g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test%d/part%d.cpp -o temp/test%d/part%d";
	const char* COMPILE_CMD_TYPE2 = "g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp submission/trainer.cpp tests/test%d/part%d.cpp -o temp/test%d/part%d";
	const char* COMPILE_CMD_TYPE3 = "g++ -ansi -pedantic submission/pokemon.cpp tests/test%d/part%d.cpp -o temp/test%d/part%d";

	const char* RUN_CMD = "./temp/test%d/part%d %d";

	int i, j, k;

	for (i=1; i<=numberOfTests; i++) {
		for (j=1; j<=numberOfParts[i]; j++) {
			COMPILER[i][j] = COMPILE_CMD_TYPE1;
			RUNNER[i][j] = RUN_CMD;
			CASE_CMD[i][j] = 1;
			MEMORY_CHECK[i][j] = false;
		}
	}

	MEMORY_CHECK[1][4] = MEMORY_CHECK[1][5] = MEMORY_CHECK[1][6] = MEMORY_CHECK[1][7] = true;
	MEMORY_CHECK[2][3] = MEMORY_CHECK[2][4] = MEMORY_CHECK[2][5] = MEMORY_CHECK[2][6] = true;
	MEMORY_CHECK[3][4] = MEMORY_CHECK[3][6] = MEMORY_CHECK[3][7] = MEMORY_CHECK[3][8] = true;

	COMPILER[1][7] = COMPILER[2][6] = COMPILER[3][7] = COMPILER[4][3] = COMPILE_CMD_TYPE2;
	COMPILER[5][1] = COMPILE_CMD_TYPE3;

	CASE_CMD[1][5] = CASE_CMD[3][7] = CASE_CMD[3][8] = CASE_CMD[4][1] = CASE_CMD[4][2] = CASE_CMD[4][3] = 3;
	CASE_CMD[1][6] = CASE_CMD[1][7] = 2;
	CASE_CMD[3][6] = 5;

	for (i=1; i<=numberOfTests; i++) 
		for (j=1; j<=numberOfParts[i]; j++)
			for (k=1; k<=CASE_CMD[i][j]; k++) {
				points[i][j][k-1] = 0;
				penalties[i][j][k-1] = 0;
			}
}

int readResult() {

	int amount = 0;
	ifstream myfile("result.txt");
	if (myfile.is_open()) {
		myfile >> amount;
		myfile.close();
		remove("result.txt");
	}

	return amount;	
}

float checkMemoryLeaks(int testId, int partId, int caseId) {

	// memory leak detection
	const char* EXECUTABLE = "./temp/test%d/part%d %d";
	const char* VALGRIND_CMD = "valgrind --leak-check=full --error-exitcode=142 %s";
	char buffer[2048];
	char testName[25];
	int status = 0;
	int retval_memcheck = 0;
	float penalty = 0;
	
	sprintf(testName, EXECUTABLE, testId, partId, caseId);
	sprintf(buffer, VALGRIND_CMD, testName);
	status = system(buffer);
	retval_memcheck = WEXITSTATUS(status);

	/* retval_memcheck values:
	   142 : memory leak problem
	   127 : executable file does not exist
	   139 : executable file gives segmentation fault
	*/
	if (retval_memcheck == 142)
		penalty = -1.0/CASE_CMD[testId][partId];
	else if ( retval_memcheck == 0 || retval_memcheck == 127)
		penalty = 0;
	else
		penalty = -1.0/CASE_CMD[testId][partId];
	
	remove("result.txt");	
	return penalty;
}

int checkComplexity() {

	const char* COMPILE_CMD = "g++ -I/lib/boost_1_64_0/ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test6/part1.cpp -o temp/test6/part1";
	const char* RUN_CMD = "./temp/test6/part1";
	int status = 0;
	int retval_compile_check = 0;
	int retval_run_check = 0;

	status = system(COMPILE_CMD);
	retval_compile_check = WEXITSTATUS(status);
	if (retval_compile_check == 0) {
		status = system(RUN_CMD);
		retval_run_check = WEXITSTATUS(status);
		if (retval_run_check == 0)
			return readResult();
	}
	return 0;
}

int checkEmptyCells() {

	// (undeleted) empty cell detection
	const char* COMPILE_CMD = "g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test7/exp%d.cpp -o temp/test7/exp%d";
	const char* RUN_CMD = "valgrind --tool=massif --massif-out-file=temp/test7/massif_output_exp%d.txt ./temp/test7/exp%d";
	const char* MASSIF_OUTPUT_FILENAME = "temp/test7/massif_output_exp%d.txt";
	char buffer[2048];
	int status = 0;
	int retval_compile_check = 0;
	int retval_run_check = 0;
	int i, penalty = -4;
	int max_amount[3] = {-1, 0, 0};

	for (i=1; i<3; i++) {
		sprintf(buffer, COMPILE_CMD, i, i);
		status = system(buffer);
		retval_compile_check = WEXITSTATUS(status);

		if (retval_compile_check == 0) {
			sprintf(buffer, RUN_CMD, i, i);
			status = system(buffer);
			retval_run_check = WEXITSTATUS(status);
			if (retval_run_check == 0) {
				// read memory consumption records
				sprintf(buffer, MASSIF_OUTPUT_FILENAME, i);
				ifstream myfile(buffer);
				if (myfile.is_open()) {
					long amount = 0;
					string line;
					while(getline(myfile, line))
						if (line.find("mem_heap_B", 0) != string::npos) {
							sscanf(line.c_str()+11, "%ld", &amount);
							if (amount > max_amount[i])
								max_amount[i] = amount;
						}
					myfile.close();
				}
				continue;
			}
		}
		return 0;
	}
	cout << max_amount[1] << " " << max_amount[2] << endl;
	if (abs(max_amount[1] - max_amount[2]) <= 16)	// actually should be equal
			return 0;
	return penalty;
}

