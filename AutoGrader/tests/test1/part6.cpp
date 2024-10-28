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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test1/part6.cpp -o tests/test1/part6

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester(int groupId) {

	int i, success=0, result=0;

	/* ******************************** */
	/*              PART 1              */
	/* ******************************** */
	int min[3] = {0, 0, 0};
	int max[3] = {7, 7, 7};

	Region r(min, max);

	ifstream myfile("coordinates/coordinates1.txt");
	if (myfile.is_open()) {
		int x, y, z, id;
		char pokemonName[3];
		pokemonName[0] = 'p';
		
		for (id=0; id<50; id++) {
			myfile >> x >> y >> z;
			sprintf(pokemonName+1, "%d", id);
			try {	// place pokemons
				r.placePokemon(Pokemon(pokemonName, "fire", 5), x, y, z);
			}
			catch(exception& e) {}
		}
		myfile.close();
	}

	/* ******************************** */
	/*              PART 6              */
	/* ******************************** */

	switch (groupId) {
		case 1:
		{
			// patch-1
			int x1[3] = {0, 0, 4};
			int y1[3] = {3, 3, 7};
			success = 0;
			Region patch1(x1, y1);

			patch1.placePokemon(Pokemon("m1", "water", 50), 0, 1, 5);	
			patch1.placePokemon(Pokemon("m2", "water", 50), 1, 2, 6);
			patch1.placePokemon(Pokemon("m3", "water", 50), 2, 3, 7);

			r.patch(patch1);
			if (r.getPokemonCount(x1, y1) == 3)	// 3
				success++;

			Pokemon& patch1_p1 = r(1, 2, 6);
			if (patch1_p1.getName().compare("m2") == 0)
				success++;
			delete &patch1_p1;

			try {
				Pokemon& patch1_p2 = r(0, 3, 4);
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
			// patch-2
			int x2[3] = {4, 2, 2};
			int y2[3] = {5, 3, 3};
			success = 0;
			Region patch2(x2, y2);

			patch2.placePokemon(Pokemon("m4", "water", 50), 5, 2, 3);	
			patch2.placePokemon(Pokemon("m5", "water", 50), 4, 3, 2);
			patch2.placePokemon(Pokemon("m6", "water", 50), 5, 3, 2);

			r.patch(patch2);
			if (r.getPokemonCount(x2, y2) == 3)	// 3
				success++;

			Pokemon& patch2_p1 = r(5, 3, 2);
			if (patch2_p1.getName().compare("m6") == 0)
				success++;
			delete &patch2_p1;

			try {
				Pokemon& patch2_p2 = r(4, 3, 3);
				delete &patch2_p2;
			}
			catch (exception& e) {
				if (((string)e.what()).compare("There does not exist any pokemon in the given coordinates!") == 0)
					success++;
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

int main(int argc, char* argv[]){

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
