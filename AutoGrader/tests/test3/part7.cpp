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

// g++ -ansi -pedantic submission/pokemon.cpp submission/region.cpp submission/trainer.cpp tests/test3/part7.cpp -o tests/test3/part7

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
	/*             PART 7               */
	/* ******************************** */

	switch (groupId) {
		case 1:
		{
			// scanRegion-1
			int a1[3] = {0, 64, 0};
			int b1[3] = {15, 127, 0};
			Trainer t1("t1", Pokemon("y1", "electric", 10), a1, b1);
			t1.scanRegion(r);
			ostringstream output;
			showPokemons(output, t1);
			string t1_p_names[118] = {"y1", "p294", "p917", "p618", "p765", "p334", "p440", "p268", "p449", "p752", "p247", "p492",
			"p450", "p835", "p903", "p158", "p380", "p928", "p183", "p737", "p740", "p556", "p420", "p463", "p632", "p34", "p215", 				"p848", "p540", "p560", "p252", "p860", "p431", "p819", "p599", "p803", "p414", "p188", "p152", "p64", "p367", "p241",
			"p406", "p846", "p217", "p204", "p718", "p70", "p139", "p267", "p868", "p149", "p255", "p941", "p436", "p486", "p299",
			"p535", "p238", "p83", "p528", "p69", "p856", "p313", "p281", "p25", "p400", "p524", "p310", "p590", "p926", "p974",
			"p880", "p667", "p137", "p351", "p749", "p326", "p211", "p987", "p637", "p652", "p175", "p743", "p95", "p225", "p322",
			"p3", "p127", "p459", "p192", "p997", "p481", "p598", "p58", "p496", "p517", "p768", "p946", "p514", "p638", "p921",
			"p271", "p309", "p734", "p555", "p266", "p676", "p320", "p180", "p424", "p108", "p298", "p389", "p992", "p782", "p102",
			"p444"};

			if (compareTrainerPokemons(output, t1_p_names, 118))
				success = 2;
			break;
		}
		case 2:
		{
			// scanRegion-2a
			int a2_ta[3] = {16, 96, 0};
			int b2_ta[3] = {27, 119, 0};
			Trainer t2a("t2a", Pokemon("y2a", "electric", 10), a2_ta, b2_ta);
			t2a.scanRegion(r);
			ostringstream output_2a;
			showPokemons(output_2a, t2a);
			string t2a_p_names[41] = {"y2a", "p53", "p291", "p626", "p576", "p383", "p245", "p956", "p365", "p727", "p668",
			"p695", "p813", "p216", "p593", "p883", "p318", "p889", "p872", "p736", "p834", "p76", "p404", "p619", "p336", "p847",
			"p843", "p922", "p547", "p161", "p224", "p804", "p506", "p22", "p272", "p178", "p10", "p780", "p644", "p518", "p425"};
			if (compareTrainerPokemons(output_2a, t2a_p_names, 41))
				success++;

			// scanRegion-2b
			int a2_tb[3] = {20, 104, 0};
			int b2_tb[3] = {31, 127, 0};
			Trainer t2b("t2b", Pokemon("y2b", "electric", 10), a2_tb, b2_tb);
			t2b.scanRegion(r);
			ostringstream output_2b;
			showPokemons(output_2b, t2b);
			string t2b_p_names[20] = {"y2b", "p9", "p854", "p457", "p795", "p792", "p148", "p91", "p292", "p435", "p779", "p966",
			"p391", "p988", "p74", "p938", "p950", "p893", "p209", "p712"};
			if (compareTrainerPokemons(output_2b, t2b_p_names, 20))
				success++;
			break;
		}
		case 3:
		{
			// scanRegion-3
			int a5[3] = {48, 96, 0};
			int b5[3] = {63, 127, 0};
			Trainer t5("t5", Pokemon("y5", "electric", 10), a5, b5);
			t5.scanRegion(r);
			ostringstream output;
			showPokemons(output, t5);
			string t5_p_names[61] = {"y5", "p295", "p578", "p32", "p571", "p62", "p160", "p710", "p408", "p745", "p507", "p566",
			"p239", "p228", "p155", "p302", "p443", "p924", "p150", "p783", "p635", "p611", "p677", "p6", "p116", "p594", "p996",
			"p612", "p104", "p636", "p79", "p67", "p916", "p234", "p855", "p377", "p687", "p35", "p684", "p546", "p57", "p713",
			"p303", "p499", "p738", "p701", "p873", "p115", "p80", "p300", "p174", "p312", "p50", "p43", "p852", "p293", "p363",
			"p94", "p306", "p473", "p936"};
			if (compareTrainerPokemons(output, t5_p_names, 61))
				success++;
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
