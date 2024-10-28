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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test1/part5.cpp -o tests/test1/part5

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
	/*              PART 5              */
	/* ******************************** */

	switch (groupId) {
		case 1:
		{
			// crop-1
			int a1[3] = {0, 4, 0};
			int b1[3] = {3, 7, 3};
			success = 0;
	
			Region r1 = r.crop(a1, b1);
			if (r1.getPokemonCount(a1, b1) == 9)	// 9
				success++;

			Pokemon& crop_p = r1(1, 5, 3);
			if (crop_p.getName().compare("p39") == 0)
				success++;
			delete &crop_p;
			break;
		}
		case 2:
		{
			// crop-2
			int a2[3] = {4, 0, 4};
			int b2[3] = {5, 3, 7};
			success = 0;
	
			Region r2 = r.crop(a2, b2);
			if (r2.getPokemonCount(a2, b2) == 4)	// 4
				success++;

			Pokemon& crop_p = r2(4, 3, 5);
			if (crop_p.getName().compare("p41") == 0)
				success++;
			delete &crop_p;
			break;
		}
		case 3:
		{
			// crop-3
			int a3[3] = {6, 4, 0};
			int b3[3] = {7, 5, 3};
			success = 0;
	
			Region r3 = r.crop(a3, b3);
			if (r3.getPokemonCount(a3, b3) == 1)	// 1
				success++;

			Pokemon& crop_p = r3(6, 4, 2);
			if (crop_p.getName().compare("p4") == 0)
				success++;
			delete &crop_p;
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
