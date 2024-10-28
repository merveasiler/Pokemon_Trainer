#include "../../submission/pokemon.h"
#include "../../submission/region.h"
#include "../../submission/trainer.h"

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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp submission/trainer.cpp tests/test4/part3.cpp -o tests/test4/part3

#define EXIT_FAIL -1

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester(int groupId) {
	
	int i, success=0, result=0;
	int coordinates[1000][3];

	/* ******************************** */
	/*              PART 3              */
	/* ******************************** */

	switch(groupId) {
		case 1:
		{
			// trainer1
			int min1[3] = {0, 0, 0};
			int max1[3] = {7, 7, 7};

			Trainer t1("t1", Pokemon("x1", "grass", 25), min1, max1);

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
						t1.catchPokemon(*p);
					}	catch (exception& e) {}
				}
				myfile1.close();
			}

			// copy t1
			Trainer copy_t1(t1);
			ostringstream output;
			showPokemons(output, copy_t1);

			istringstream i_output(output.str());
			vector<string> output_p_names;
			copy(istream_iterator<string>(i_output), istream_iterator<string>(), back_inserter(output_p_names));

			if (output_p_names.size() == 51) {
				result = 1;
				break;
			}
			
			return EXIT_FAIL;
		}
		case 2:
		{
			// trainer2
			int min2[3] = {0, 0, 0};
			int max2[3] = {127, 127, 127};

			Trainer t2("t2", Pokemon("x2", "grass", 25), min2, max2);

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
						t2.catchPokemon(*p);
					}	catch (exception& e) {}
				}
				myfile2.close();
			}
		
			// copy t2
			Trainer copy_t2 = t2;
			ostringstream output;
			showPokemons(output, copy_t2);

			istringstream i_output(output.str());
			vector<string> output_p_names;
			copy(istream_iterator<string>(i_output), istream_iterator<string>(), back_inserter(output_p_names));

			if (output_p_names.size() == 1001) {
				result = 1;
				break;
			}
			
			return EXIT_FAIL;
		}
		case 3:
		{
			// region3
			int min3[3] = {0, 0, 0};
			int max3[3] = {63, 127, 0};

			Trainer t3("t3", Pokemon("x3", "grass", 25), min3, max3);

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
						t3.catchPokemon(*p);
					}	catch (exception& e) {}
				}
				myfile3.close();
			}

			// copy t3
			Trainer copy_t3 = t3;
			ostringstream output;
			showPokemons(output, copy_t3);

			istringstream i_output(output.str());
			vector<string> output_p_names;
			copy(istream_iterator<string>(i_output), istream_iterator<string>(), back_inserter(output_p_names));

			if (output_p_names.size() == 1001) {
				result = 1;
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
