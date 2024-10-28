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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test2/part5.cpp -o tests/test2/part5

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester() {

	int i, success=0, result=0;

	/* ******************************** */
	/*              PART 1              */
	/* ******************************** */
	int min[3] = {0, 0, 0};
	int max[3] = {127, 127, 127};

	Region r(min, max);

	ifstream myfile("coordinates/coordinates2.txt");
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
			catch(exception& e) {}
		}
		myfile.close();
	}

	/* ******************************** */
	/*              PART 5              */
	/* ******************************** */

	// patch
	int a[3] = {32, 0, 64};
	int b[3] = {63, 31, 127};
	int c[3] = {32, 0, 64};
	int d[3] = {63, 31, 95};
	success = 0;
	
	Region r3(c, d);
	r3.placePokemon(Pokemon("x0", "water", 50), 35, 2, 77);	
	r3.placePokemon(Pokemon("x1", "water", 50), 60, 6, 78);
	r3.placePokemon(Pokemon("x2", "water", 50), 40, 20, 80);
	r3.placePokemon(Pokemon("x3", "water", 50), 43, 3, 86);

	r.patch(r3);
	if (r.getPokemonCount(c, d) == 4)	// 4
		success++;
	if (r.getPokemonCount(a, b) == 26)	// 26
		success++;
	
	int patch_cells[2][3] = {{35, 2, 77}, {60, 6, 78}};
	string patch_p_names[2] = {"x0", "x1"};
	for (i=0; i<2; i++) {
		try {
			Pokemon& patch_p = r(patch_cells[i][0], patch_cells[i][1], patch_cells[i][2]);
			if (patch_p.getName().compare(patch_p_names[i]) == 0)
				success++;
			delete &patch_p;
		}
		catch (exception& e) {}
	}

	if (success > 1) {
		if (success < 3)
			result = 1;
		else if (success < 4)
			result = 2;
		else
			result = 3;
	}
	else
		return EXIT_FAIL;

	ofstream rfile("result.txt", ios::app);
	if (rfile.is_open()) {
		rfile << result << endl;
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

	return tester();
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
