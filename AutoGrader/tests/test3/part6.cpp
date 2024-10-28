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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test3/part6.cpp -o tests/test3/part6

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
	/*             PART 6               */
	/* ******************************** */

	switch (groupId) {
		case 1:
		{
			// crop-1
			success = 0;
			int a1[3] = {0, 64, 0};
			int b1[3] = {15, 127, 0};
			Region r1 = r.crop(a1, b1);
			if (r1.getPokemonCount(a1, b1) == 117) {
				Pokemon& crop_p = r1(14, 104, 0);
				if (crop_p.getName() == "p102")
					success++;
				delete &crop_p;
			}
			break;
		}
		case 2:
		{
			// crop-2
			int a2[3] = {16, 96, 0};
			int b2[3] = {31, 127, 0};
			Region r2 = r.crop(a2, b2);
			if (r2.getPokemonCount(a2, b2) == 67) {
				Pokemon& crop_p = r2(27, 125, 0);
				if (crop_p.getName() == "p712")
					success++;
				delete &crop_p;
			}
			break;
		}
		case 3:
		{
			// crop-3
			int a3[3] = {24, 64, 0};
			int b3[3] = {31, 95, 0};
			Region r3 = r.crop(a3, b3);
			if (r3.getPokemonCount(a3, b3) == 29) {
				Pokemon& crop_p = r3(29, 92, 0);
				if (crop_p.getName() == "p575")
					success += 2;
				delete &crop_p;
			}
			break;
		}
		case 4:
		{
			// crop-4
			int a4[3] = {32, 0, 0};
			int b4[3] = {47, 63, 0};
			Region r4 = r.crop(a4, b4);
			if (r4.getPokemonCount(a4, b4) == 132) {
				Pokemon& crop_p = r4(35, 32, 0);
				if (crop_p.getName() == "p739")
					success += 2;
				delete &crop_p;
			}
			break;
		}
		case 5:
		{
			// crop-5
			int a5[3] = {48, 96, 0};
			int b5[3] = {63, 127, 0};
			Region r5 = r.crop(a5, b5);
			if (r5.getPokemonCount(a5, b5) == 60) {
				Pokemon& crop_p = r5(57, 120, 0);
				if (crop_p.getName() == "p936")
					success++;
				delete &crop_p;
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
