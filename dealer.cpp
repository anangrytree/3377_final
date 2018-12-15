#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

struct data
{
	int trials;
	int probability;
	double successRate;
	double failRate;
};

int main(int args, char* argv[]) {
	int opterr = 0, prob, c, trials = 0;
	char* prob_string;
	char* trials_string;
	char* filename;
	bool probSet = false, verbose = false;
	while((c = getopt(args, argv, ":p:o:v")) != -1) {
		switch(c) {
			case 'p':
				prob_string = optarg;
				probSet = true;
				break;
			case 'v':
				verbose = true;
				break;
			case 'o':
				filename = optarg;
				break;
			case '?':
				if(optopt == 'p') {
					cerr << "The -p flag requires an argument with it" << "\n";
					return 0;
				} else if(optopt == 'o') {
					cerr << "The -o flag requires an argument with it \n";
					return 0;
				} else {
					cerr << "An unrecognized argument  was specified" << "\n";
					return 0;
				}
				break;
			case ':':
				if(optopt == 'p') {
					cerr << "The -p flag must be set with an integer attribute\n";
					return 0;
				} else if(optopt == 'o') {
					cerr << "The -o flag must be set with a filename attribute\n";
					return 0;
				}
				break;
		}
	}

	if(argv[optind] == NULL) {
		cerr << "There are one or more issues with your input. Trials must be set as an integer at the end of the command\n";
		return 0;
	} else {
		trials_string = argv[optind];
	}

	if(!probSet) {
		cerr << "There are one or more issues with your input. Probability attribute must be set as an integer with the -p flag preceding it \n";
		return 0;
	}

	for(int i = 0; prob_string[i] != '\0'; i++) {
		if(!isdigit(prob_string[i])) {
			cerr << "There are one or more issues with your input. Probability attribute must be set as an integer with the -p flag preceding it\n";
			return 0;
		}
	}

	for(int i = 0; trials_string[i] != '\0'; i++) {
		if(!isdigit(trials_string[i])) {
			cerr << "There are one or more issues with your input. Trials must be set as an integer at the end of the command\n";
			return 0;
		}
	}

	trials = atoi(trials_string);
	prob = atoi(prob_string);

	if(trials < 0) {
		cerr << "Trials must be a positive integer\n";
		return 0;
	}

	if(prob < 0 || prob > 100) {
		cerr << "Probability must be between 0 and 100, inclusive" << "\n";
		return 0;
	}

	const char* prob_opt = "-p";
	char* fstring = new char[strlen(prob_string) + 3];
	strcpy(fstring, prob_opt);
	strcat(fstring, prob_string);
	pid_t pid;
	int status;

	int success = 0;

	for(int i = 0; i < trials; i++) {
		pid = fork();
		if(pid == 0) {
			execl("hand", "hand", fstring, NULL);
			exit(0);
		}
		pid_t wpid = wait(&status);
		if(verbose) {
			cout << "PID " << wpid << " returned ";
			if(status == 0)
				cout << "success\n";
			else
				cout << "failure\n";
		}
		if(status == 0)
			success++;
	}

	double successRate = 10000 * (success / (double)trials);
	double finalSuccess = ((int)successRate / 100.0);
	double finalFail = 100 - finalSuccess;

	cout << "\nCreated " << trials << " processes\n";
	cout << "Success - " << finalSuccess << "%\n";
	cout << "Failure - " << finalFail << "%\n";
	
	struct data trialData;
	trialData.trials = trials;
	trialData.successRate = finalSuccess;
	trialData.failRate = finalFail;
	trialData.probability = prob;

	ofstream output;
	output.open(filename, ios::binary | ios::app);
	output.write((char *)&trialData, sizeof(trialData));
	output.close();

	return 0;
}
