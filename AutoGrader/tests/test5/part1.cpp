#include "../../submission/pokemon.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <csignal>
#include <cstring>
#include <csetjmp>
#include <sys/time.h>
#include <sys/resource.h>

static jmp_buf  env;

using namespace std;

#define EXIT_FAIL -1

// g++ -ansi -pedantic submission/pokemon.cpp tests/test5/part1.cpp -o tests/test5/part1

void segfault_sigaction(int signal, siginfo_t *si, void *arg);  
void segfault_sigaction_sigabort(int signal, siginfo_t *si, void *arg);
void segfault_sigaction_sigcpukill(int signal, siginfo_t *si, void *arg);

int tester() {

	// Pokemon battles & evolutions
	int min[3] = {0, 0, 0};
	int max[3] = {7, 7, 7};
	int success=0, result=0;

	Pokemon* x1 = new Pokemon("x1", "electric", 3);
	Pokemon* x2 = new Pokemon("x2", "fire", 3);
	Pokemon* x3 = new Pokemon("x3", "water", 3);
	Pokemon* x4 = new Pokemon("x4", "grass", 3);
	Pokemon* x5 = new Pokemon("x5", "flying", 3);

	Pokemon* y1 = new Pokemon("y1", "fire", 3);
	Pokemon* y2 = new Pokemon("y2", "water", 3);
	Pokemon* y3 = new Pokemon("y3", "grass", 3);
	Pokemon* y4 = new Pokemon("y4", "flying", 3);
	Pokemon* y5 = new Pokemon("y5", "electric", 3);

	// ROUND-1
	if ( ((*x1) & (*y1)).getName() == "y1" )
		success++;
	if ( ((*x2) & (*y2)).getName() == "y2" )
		success++;
	if ( ((*x3) & (*y3)).getName() == "y3" )
		success++;
	if ( ((*x4) & (*y4)).getName() == "y4" )
		success++;
	if ( ((*x5) & (*y5)).getName() == "y5" )
		success++;
	
	if ( (*x1) >> Pokemon("evolved_x1", "electric", 6) == 0 )
		success++;

	// ROUND-2
	if ( ((*x1) & (*y2)).getName() == "x1" )
		success++;
	if ( ((*x2) & (*y3)).getName() == "x2" )
		success++;
	if ( ((*x3) & (*y4)).getName() == "x3" )
		success++;
	if ( ((*x4) & (*y5)).getName() == "x4" )
		success++;
	if ( ((*x5) & (*y1)).getName() == "x5" )
		success++;

	if ( (*y3) >> Pokemon("evolved_y3", "grass", 4) == 0 )
		success++;

	// ROUND-3
	if ( ((*x1) & (*y3)).getName() == "y3" )
		success++;
	if ( ((*x2) & (*y4)).getName() == "y4" )
		success++;
	if ( ((*x3) & (*y5)).getName() == "y5" )
		success++;
	if ( ((*x4) & (*y1)).getName() == "y1" )
		success++;
	if ( ((*x5) & (*y2)).getName() == "y2" )
		success++;

	if ( (*x1) >> Pokemon("evolved_x1", "electric", 6) )
		success++;

	if ( (*y3) >> Pokemon("evolved_y3", "grass", 4) )
		success++;

	if ( (*x5) >> Pokemon("evolved_x5", "flying", 10) )
		success++;

	delete x1;	delete y1;
	delete x2;	delete y2;
	delete x3;	delete y3;
	delete x4;	delete y4;
	delete x5;	delete y5;

	if (success >= 10)
	{
		if (success <= 12)
			result = 3;
		else if (success <= 15)
			result = 5;
		else if (success <= 18)
			result = 6;
		else
			result = 7;
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
