#include "../../submission/pokemon.h"
#include "../../submission/region.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>

#include <csignal>
#include <cstring>
#include <csetjmp>
#include <sys/time.h>
#include <sys/resource.h>

static jmp_buf  env;

using namespace std;

#define EXIT_FAIL -1

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test3/part8.cpp -o tests/test3/part8

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester(int groupId) {

	int i, success=0, result=0;

	/* ******************************** */
	/*              PART 1              */
	/* ******************************** */
	int min[3] = {0, 0, 0};
	int max[3] = {63, 127, 0};

	Region r(min, max);

	ifstream myfile("coordinates/coordinates3.txt");
	if (myfile.is_open()) {
		int x, y, z, id;
		char pokemonName[4];
		pokemonName[0] = 'p';
		
		for (id=0; id<1000; id++) {
			myfile >> x >> y >> z;
			sprintf(pokemonName+1, "%d", id);
			try {	// place pokemons
				r.placePokemon(Pokemon(pokemonName, "fire", 5), x, y, z);
			}
			catch (exception& e) {}
		}
		myfile.close();
	}

	/* ******************************** */
	/*             PART 8               */
	/* ******************************** */

	switch (groupId) {
		case 1:
		{
			// patch 1
			success = 0;
			int x1[3] = {20, 64, 0};
			int y1[3] = {27, 95, 0};
			Region patch1(x1, y1);
			patch1.placePokemon(Pokemon("m1", "water", 50), 20, 64, 0);	
			patch1.placePokemon(Pokemon("m2", "water", 50), 21, 68, 0);
			patch1.placePokemon(Pokemon("m3", "water", 50), 22, 72, 0);
			patch1.placePokemon(Pokemon("m4", "water", 50), 23, 76, 0);
			patch1.placePokemon(Pokemon("m5", "water", 50), 24, 80, 0);
			patch1.placePokemon(Pokemon("m6", "water", 50), 25, 84, 0);
			patch1.placePokemon(Pokemon("m7", "water", 50), 26, 88, 0);
			patch1.placePokemon(Pokemon("m8", "water", 50), 27, 92, 0);

			r.patch(patch1);
			if (r.getPokemonCount(x1, y1) == 8)	// 8
				success++;

			Pokemon& patch1_p1 = r(22, 72, 0);
			if (patch1_p1.getName().compare("m3") == 0)
				success++;
			delete &patch1_p1;

			try {
				Pokemon& patch1_p2 = r(24, 70, 0);
				delete &patch1_p2;
			}
			catch (exception& e) {
				if (((string)e.what()).compare("There does not exist any pokemon in the given coordinates!") == 0)
					success++;
			}

			break;
		}
		case 2:
		{
			// patch 2
			success = 0;
			int x2[3] = {32, 0, 0};
			int y2[3] = {63, 63, 0};
			Region patch2(x2, y2);
			patch2.placePokemon(Pokemon("m9", "water", 50), 33, 0, 0);	
			patch2.placePokemon(Pokemon("m10", "water", 50), 35, 5, 0);
			patch2.placePokemon(Pokemon("m11", "water", 50), 37, 10, 0);
			patch2.placePokemon(Pokemon("m12", "water", 50), 39, 15, 0);
			patch2.placePokemon(Pokemon("m13", "water", 50), 41, 20, 0);	
			patch2.placePokemon(Pokemon("m14", "water", 50), 44, 25, 0);
			patch2.placePokemon(Pokemon("m15", "water", 50), 46, 30, 0);
			patch2.placePokemon(Pokemon("m16", "water", 50), 48, 35, 0);
			patch2.placePokemon(Pokemon("m17", "water", 50), 50, 40, 0);	
			patch2.placePokemon(Pokemon("m18", "water", 50), 52, 45, 0);
			patch2.placePokemon(Pokemon("m19", "water", 50), 55, 50, 0);
			patch2.placePokemon(Pokemon("m20", "water", 50), 57, 55, 0);
			patch2.placePokemon(Pokemon("m21", "water", 50), 59, 60, 0);
			patch2.placePokemon(Pokemon("m22", "water", 50), 62, 62, 0);

			r.patch(patch2);
			if (r.getPokemonCount(x2, y2) == 14)	// 14
				success++;

			Pokemon& patch2_p1 = r(55, 50, 0);
			if (patch2_p1.getName().compare("m19") == 0)
				success++;
			delete &patch2_p1;

			try {
				Pokemon& patch2_p2 = r(41, 35, 0);
				delete &patch2_p2;
			}
			catch (exception& e) {
				if (((string)e.what()).compare("There does not exist any pokemon in the given coordinates!") == 0)
					success++;
			}

			break;
		}
		case 3:
		{
			// patch 3
			success = 0;
			int x3[3] = {56, 64, 0};
			int y3[3] = {63, 79, 0};
			Region patch3(x3, y3);

			r.patch(patch3);
			if (r.getPokemonCount(x3, y3) == 0)	// 0
				success += 2;

			try {
				Pokemon& patch2_p1 = r(60, 70, 0);
				delete &patch2_p1;
			}
			catch (exception& e) {
				if (((string)e.what()).compare("There does not exist any pokemon in the given coordinates!") == 0)
				success += 2;
			}

			break;
		}
		default:
			return EXIT_FAIL;
	}

	ofstream rfile("result.txt", ios::app);
	if (rfile.is_open()) {
		rfile << success << endl;
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
