#include "../../submission/pokemon.h"
#include "../../submission/region.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#include <cstdlib>
#include <boost/random.hpp>
#include <boost/nondet_random.hpp>

#include <csignal>
#include <cstring>
#include <csetjmp>
#include <sys/time.h>
#include <sys/resource.h>

static jmp_buf  env;

using namespace std;

// g++ -I/lib/boost_1_64_0/ -ansi -pedantic submission/pokemon.cpp submission/region.cpp tests/test6/part1.cpp -o tests/test6/part1

int random_pt(int max);
void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester() {

	// computational complexity of placePokemon()

	int i, j, pt, penalty = 0;
	int micro = pow(10,6);
	int size1 = pow(2,7) - 1;
	int size2 = pow(2,14) - 1;
	int size3 = pow(2,21) - 1;
	int size4 = pow(2,28) - 1;

	int min[3] = {0, 0, 0};
	int max1[3] = {size1, size1, size1};
	int max2[3] = {size2, size2, size2};
	int max3[3] = {size3, size3, size3};
	int max4[3] = {size4, size4, size4};

	Pokemon pokemon("pokemon", "flying", 10);

	clock_t begin;
	double small_dur, big_dur, huge_dur, epic_dur;
	double times[100][3];

	for (i=0; i<100; i++) {
		// small_dur
		begin = clock();
		for (j=0; j<128; j++) {
			pt = random_pt(max1[0]);
			Region small(min, max1);
			small.placePokemon(pokemon, pt, pt, pt);
		}

		small_dur = (((double)(clock() - begin)) * micro) / CLOCKS_PER_SEC;

		// big_dur
		begin = clock();
		for (j=0; j<128; j++) {
			pt = random_pt(max2[0]);
			Region big(min, max2);
			big.placePokemon(pokemon, pt, pt, pt);
		}

		big_dur = (((double)(clock() - begin)) * micro) / CLOCKS_PER_SEC;

		// huge_dur
		begin = clock();
		for (j=0; j<128; j++) {
			pt = random_pt(max3[0]);
			Region huge(min, max3);
			huge.placePokemon(pokemon, pt, pt, pt);
		}

		huge_dur = (((double)(clock() - begin)) * micro) / CLOCKS_PER_SEC;

		// epic_dur
		begin = clock();
		for (j=0; j<128; j++) {
			pt = random_pt(max4[0]);
			Region epic(min, max4);
			epic.placePokemon(pokemon, pt, pt, pt);
		}

		epic_dur = (((double)(clock() - begin)) * micro) / CLOCKS_PER_SEC;


		times[i][0] = big_dur / small_dur;
		times[i][1] = huge_dur / big_dur;
		times[i][2] = epic_dur / huge_dur;
	}

	double avg[3] = {0, 0, 0};
	for (i=0; i<100; i++) {
		for (j=0; j<3; j++)
			avg[j] += times[i][j];
	}
	for (j=0; j<3; j++)
		avg[j] = avg[j] / 100;

	double perfect[3] = {2.0 / 1.0, 3.0 / 2.0, 4.0 / 3.0};
	
	double diff1 = abs(avg[0] - perfect[0]);
	if (diff1 > 0.3)
		penalty = penalty - 2;

	double diff2 = abs(avg[1] - perfect[1]);
	if (diff2 > 0.25)
		penalty = penalty - 2;

	double diff3 = abs(avg[2] - perfect[2]);
	if (diff3 > 0.18)
		penalty = penalty - 2;

	ofstream rfile("result.txt", ios::app);
	if (rfile.is_open()) {
		rfile << penalty << endl;
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
	rl.rlim_cur = 60;
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

int random_pt(int max) {
	static boost::random::mt19937 twstr(0);
	boost::random::uniform_int_distribution<int> dist(0, max);
	return dist(twstr);
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
