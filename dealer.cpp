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
	bool probSet = false, verbose = false, fileset = false;
	while((c = getopt(args, argv, ":p:o:v")) != -1) { // parse CLI
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
				fileset = true;
				break;
			case '?': // CLI requirements not met
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
			case ':': // CLI error
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

	if(argv[optind] == NULL) { // trials not set
		cerr << "There are one or more issues with your input. Trials must be set as an integer at the end of the command\n";
		return 0;
	} else {
		trials_string = argv[optind];
	}

	if(!probSet) { // probability not set
		cerr << "There are one or more issues with your input. Probability attribute must be set as an integer with the -p flag preceding it \n";
		return 0;
	}

	// check if probability is an integer
	for(int i = 0; prob_string[i] != '\0'; i++) {
		if(!isdigit(prob_string[i])) {
			cerr << "There are one or more issues with your input. Probability attribute must be set as an integer with the -p flag preceding it\n";
			return 0;
		}
	}

	// check if trials is an integer
	for(int i = 0; trials_string[i] != '\0'; i++) {
		if(!isdigit(trials_string[i])) {
			cerr << "There are one or more issues with your input. Trials must be set as an integer at the end of the command\n";
			return 0;
		}
	}

	// convert trials and prob to integers
	trials = atoi(trials_string);
	prob = atoi(prob_string);

	// check if trials is within bounds
	if(trials < 0) { 
		cerr << "Trials must be a positive integer\n";
		return 0;
	}

	// check if prob is within bounds
	if(prob < 0 || prob > 100) {
		cerr << "Probability must be between 0 and 100, inclusive" << "\n";
		return 0;
	}

	// create an arg that can be passed to execl
	const char* prob_opt = "-p";
	char* fstring = new char[strlen(prob_string) + 3];
	strcpy(fstring, prob_opt);
	strcat(fstring, prob_string);
	pid_t pid;
	int status;

	int success = 0;

	for(int i = 0; i < trials; i++) {
		pid = fork(); // create child process
		if(pid == 0) {
			execl("hand", "hand", fstring, NULL); // execute hand in child
			exit(0);
		}
		pid_t wpid = wait(&status);
		if(verbose) { // output each child's results if verbose flag set
			cout << "PID " << wpid << " returned ";
			if(status == 0)
				cout << "success\n";
			else
				cout << "failure\n";
		}
		if(status == 0)
			success++;
	}

	// calculate final success/failure rates
	double successRate = 10000 * (success / (double)trials);
	double finalSuccess = ((int)successRate / 100.0);
	double finalFail = 100 - finalSuccess;

	// output results
	cout << "\nCreated " << trials << " processes\n";
	cout << "Success - " << finalSuccess << "%\n";
	cout << "Failure - " << finalFail << "%\n";
	
	// store results in struct
	struct data trialData;
	trialData.trials = trials;
	trialData.successRate = finalSuccess;
	trialData.failRate = finalFail;
	trialData.probability = prob;

	// output struct to output file that was set
	if(fileset) {
		ofstream output;
		output.open(filename, ios::binary | ios::app);
		output.write((char *)&trialData, sizeof(trialData));
		output.close();
	}
	return 0;
}
