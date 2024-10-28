#include "../../submission/pokemon.h"
#include "../../submission/region.h"
#include "../../submission/trainer.h"

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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp submission/trainer.cpp tests/test2/part6.cpp -o tests/test2/part6

bool compareTrainerPokemons(ostringstream& output, string trainer_p_names[], int numberOfPokemons);
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
				success++;
			}
			catch(exception& e) {}
		}
		myfile.close();
	}

	/* ******************************** */
	/*              PART 6              */
	/* ******************************** */

	// scanRegion
	ostringstream output;
	success = 0;
	int e[3] = {34, 0, 69};
	int f[3] = {50, 10, 119};

	Trainer t("t0", Pokemon("y0", "electric", 10), e, f);
	t.scanRegion(r);
	showPokemons(output, t);

	string trainer_p_names[7] = {"p141", "p171", "p334", "p601", "p823", "p85", "y0"};
	if (compareTrainerPokemons(output, trainer_p_names, 7))
		success++;

	try {
		Pokemon& caught_p = r(43, 3, 86);	// is caught?
		delete &caught_p;
	}
	catch(exception& e) {
		if (((string)e.what()).compare("There does not exist any pokemon in the given coordinates!") == 0)
			success++;
	}
	
	if (success == 1)
		result = 1;
	else if(success == 2)
		result = 2;
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

bool compareTrainerPokemons(ostringstream& output, string trainer_p_names[], int numberOfPokemons) {
	int i;
	istringstream i_output(output.str());
	vector<string> output_p_names;

	copy(istream_iterator<string>(i_output), istream_iterator<string>(), back_inserter(output_p_names));

	if (output_p_names.size() == numberOfPokemons) {
		sort(trainer_p_names, trainer_p_names+numberOfPokemons);
		sort(output_p_names.begin(), output_p_names.end());
		for (i=0; i<numberOfPokemons; i++)
			if (trainer_p_names[i].compare(output_p_names[i]) != 0)
				return false;
		return true;
	}
	return false;
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
