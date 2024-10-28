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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp submission/trainer.cpp tests/test1/part7.cpp -o tests/test1/part7

bool compareTrainerPokemons(ostringstream& output, string trainer_p_names[], int numberOfPokemons);
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
	/*              PART 7              */
	/* ******************************** */
	
	switch (groupId) {
		case 1:
		{
			// scanRegion-1
			int k1[3] = {0, 0, 0};
			int m1[3] = {1, 2, 3};
			Trainer t1("t1", Pokemon("y1", "electric", 10), k1, m1);
			t1.scanRegion(r);
			ostringstream output;
			showPokemons(output, t1);
			string t1_p_names[4] = {"y1", "p31", "p12", "p44"};
			if (compareTrainerPokemons(output, t1_p_names, 4)) {
				result = 3;
				break;
			}
			return EXIT_FAIL;
		}
		case 2:
		{
			// scanRegion-2
			int k2[3] = {4, 4, 4};
			int m2[3] = {7, 7, 7};
			Trainer t2("t2", Pokemon("y2", "electric", 10), k2, m2);
			t2.scanRegion(r);
			ostringstream output;
			showPokemons(output, t2);
			string t2_p_names[3] = {"y2", "p30", "p32"};
			if (compareTrainerPokemons(output, t2_p_names, 3)) {
				result = 3;
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
