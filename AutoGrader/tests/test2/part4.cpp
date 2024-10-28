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
	/*              PART 4              */
	/* ******************************** */
	// crop
	int a[3] = {32, 0, 64};
	int b[3] = {63, 31, 127};
	success = 0;
	
	Region r2 = r.crop(a, b);
	if (r2.getPokemonCount(a, b) == 39)	// 39
		success++;
	
	int crop_cells[2][3] = {{62, 1, 126}, {59, 23, 99}};
	string crop_p_names[2] = {"p24", "p492"};
	for (i=0; i<2; i++) {
		try {
			Pokemon& crop_p = r2(crop_cells[i][0], crop_cells[i][1], crop_cells[i][2]);
			if (crop_p.getName().compare(crop_p_names[i]) == 0)
				success++;
			delete &crop_p;
		}
		catch (exception& e) {}
	}

	if (success > 0) {
		if (success < 3)
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
