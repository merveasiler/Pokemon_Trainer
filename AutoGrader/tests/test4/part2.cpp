#include "../../submission/pokemon.h"
#include "../../submission/region.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include <csignal>
#include <cstring>
#include <csetjmp>
#include <sys/time.h>
#include <sys/resource.h>

static jmp_buf  env;

using namespace std;

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test4/part2.cpp -o tests/test4/part2

#define EXIT_FAIL -1

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester(int groupId) {
	
	int i, success=0, result=0;
	int coordinates[1000][3];

	/* ******************************** */
	/*              PART 2              */
	/* ******************************** */

	switch(groupId) {
		case 1:
		{
			// region1
			int min1[3] = {0, 0, 0};
			int max1[3] = {7, 7, 7};

			Region r1(min1, max1);

			ifstream myfile1("coordinates/coordinates1.txt");
			if (myfile1.is_open()) {
				int id;
				char pokemonName[3];
				pokemonName[0] = 'p';
		
				for (id=0; id<50; id++) {
					myfile1 >> coordinates[id][0] >> coordinates[id][1] >> coordinates[id][2];
					sprintf(pokemonName+1, "%d", id);
					Pokemon* p = new Pokemon(pokemonName, "fire", 5);
					try {
						r1.placePokemon(*p, coordinates[id][0], coordinates[id][1], coordinates[id][2]);
					}	catch (exception& e) {}
				}
				myfile1.close();
			}

			Region copy_r1 = r1;
			int cells[2][3] = {{5, 0, 1}, {3, 3, 3}};
			for (i=0; i<2; i++) {
				Pokemon& p = copy_r1(cells[i][0], cells[i][1], cells[i][2]);
				delete &p;
			}
			
			// assign r1
			success = 0;
			Region assgn_r1(min1, max1);
			assgn_r1 = copy_r1;
			if (assgn_r1.getPokemonCount(min1, max1) == 48)	// 48
				success++;

			int cells2[3][3] = {{2, 5, 5}, {0, 4, 0}, {1, 0, 1}};
			string pnames[3] = {"p8", "p33", "p44"};
			for (i=0; i<3; i++) {
				Pokemon& p = copy_r1(cells2[i][0], cells2[i][1], cells2[i][2]);
				if (p.getName().compare(pnames[i]) == 0)
					success++;
				delete &p;
			}

			if (success > 0) {
				if (success < 4)
					result = 1;
				else
					result = 2;
				break;
			}
			return EXIT_FAIL;
		}
		case 2:
		{
			// region2
			int min2[3] = {0, 0, 0};
			int max2[3] = {127, 127, 127};

			Region r2(min2, max2);

			ifstream myfile2("coordinates/coordinates2.txt");
			if (myfile2.is_open()) {
				int id;
				char pokemonName[4];
				pokemonName[0] = 'p';
		
				for (id=0; id<1000; id++) {
					myfile2 >> coordinates[id][0] >> coordinates[id][1] >> coordinates[id][2];
					sprintf(pokemonName+1, "%d", id);
					Pokemon* p = new Pokemon(pokemonName, "fire", 5);
					try {
						r2.placePokemon(*p, coordinates[id][0], coordinates[id][1], coordinates[id][2]);
					}	catch (exception& e) {}
				}
				myfile2.close();
			}
		
			Region copy_r2 = r2;
			int cells[2][3] = {{125, 127, 98}, {6, 74, 9}};
			for (i=0; i<2; i++) {
				Pokemon& p = copy_r2(cells[i][0], cells[i][1], cells[i][2]);
				delete &p;
			}

			// assign r2
			success = 0;
			Region assgn_r2(min2, max2);
			assgn_r2 = copy_r2;
			if (assgn_r2.getPokemonCount(min2, max2) == 998)	// 998
				success++;

			int cells2[3][3] = {{100, 69, 20}, {27, 27, 7}, {14, 99, 0}};
			string pnames[3] = {"p50", "p480", "p841"};
			for (i=0; i<3; i++) {
				Pokemon& p = copy_r2(cells2[i][0], cells2[i][1], cells2[i][2]);
				if (p.getName().compare(pnames[i]) == 0)
					success++;
				delete &p;
			}

			if (success > 0) {
				if (success < 4)
					result = 1;
				else
					result = 2;
				break;
			}
			return EXIT_FAIL;
		}
		case 3:
		{
			// region3
			int min3[3] = {0, 0, 0};
			int max3[3] = {63, 127, 0};

			Region r3(min3, max3);

			ifstream myfile3("coordinates/coordinates3.txt");
			if (myfile3.is_open()) {
				int id;
				char pokemonName[4];
				pokemonName[0] = 'p';
		
				for (id=0; id<1000; id++) {
					myfile3 >> coordinates[id][0] >> coordinates[id][1] >> coordinates[id][2];
					sprintf(pokemonName+1, "%d", id);
					Pokemon* p = new Pokemon(pokemonName, "fire", 5);
					try {
						r3.placePokemon(*p, coordinates[id][0], coordinates[id][1], coordinates[id][2]);
					}	catch (exception& e) {}
				}
				myfile3.close();
			}
		
			Region copy_r3 = r3;
			int cells[2][3] = {{0, 48, 0}, {24, 101, 0}};
			for (i=0; i<2; i++) {
				Pokemon& p = copy_r3(cells[i][0], cells[i][1], cells[i][2]);
				delete &p;
			}

			// assign r3
			success = 0;
			Region assgn_r3(min3, max3);
			assgn_r3 = copy_r3;
			if (assgn_r3.getPokemonCount(min3, max3) == 998)	// 998
				success++;

			int cells2[3][3] = {{60, 20, 0}, {0, 121, 0}, {13, 100, 0}};
			string pnames[3] = {"p55", "p431", "p921"};
			for (i=0; i<3; i++) {
				Pokemon& p = copy_r3(cells2[i][0], cells2[i][1], cells2[i][2]);
				if (p.getName().compare(pnames[i]) == 0)
					success++;
				delete &p;
			}

			if (success > 0) {
				if (success < 4)
					result = 1;
				else
					result = 2;
				break;
			}
			return EXIT_FAIL;
		}
		default:
			return EXIT_FAIL;
	}

	ofstream rfile("result.txt", ios::app);
	if (rfile.is_open()) {
		rfile << result << endl;
		rfile.close();
	}
	return 0;
}

int main(int argc, char* argv[]) {

	// signal handlers
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags   = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);

	struct sigaction sa_sigabort;
	memset(&sa_sigabort, 0, sizeof(struct sigaction));
	sigemptyset(&sa_sigabort.sa_mask);
	sa_sigabort.sa_sigaction = segfault_sigaction_sigabort;
	sa_sigabort.sa_flags   = SA_SIGINFO;
	sigaction(SIGABRT, &sa_sigabort, NULL);

	struct rlimit rl;
	getrlimit (RLIMIT_CPU, &rl);
	rl.rlim_cur = 30;
	setrlimit(RLIMIT_CPU, &rl);

	struct sigaction sa_sigcpukill;
	memset(&sa_sigcpukill, 0, sizeof(struct sigaction));
	sigemptyset(&sa_sigcpukill.sa_mask);
	sa_sigcpukill.sa_sigaction = segfault_sigaction_sigcpukill;
	sa_sigcpukill.sa_flags   = SA_SIGINFO;
	sigaction(SIGXCPU, &sa_sigcpukill, NULL);

	setjmp(env);

	return tester(atoi(argv[1]));
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg) {
	cout << "Comment :=>> Caught segfault at address " << si->si_addr << ".\n";
	cout << "Comment :=>> FAIL\n\n";
	exit(-1);
}
    
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg) {
	cout << "Comment :=>> Received SIGABRT at address " << si->si_addr << ".\n";
	cout << "Comment :=>> There may be a memory problem in your code (e.g. double free or invalid pointer)\n";
	cout << "Comment :=>> FAIL\n\n";
	exit(-1);
}

void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg) {
	cout << "Comment :=>> Received SIGXCPU\n";
	cout << "Comment :=>> Your code did not finish in a reasonable amount of time and got killed\n";
	cout << "Comment :=>> FAIL\n\n";
	exit(-1);
}
