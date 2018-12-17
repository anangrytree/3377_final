#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

struct data
{
	int trials;
	int probability;
	double successRate;
	double failRate;
};

int main(int args, char* argv[]) {
	int opterr = 0, c, prob;
	char* filename;
	char* prob_string;
	bool prob_set = false;
	// loop through arguments
	while((c = getopt(args, argv, ":p:")) != -1) {
		switch(c) {
			case 'p':
				prob_string = optarg;
				prob_set = true;
				break;
			case '?': // invalid argument settings
				if(optopt == 'p') {
					cerr << "The -p flag requires an argument with it \n";
					return 0;
				} else {
					cerr << "An unrecognized argument was specified \n";
					return 0;
				}
				break;
			case ':': // error in args
				if(optopt == 'o') {
					cerr << "The -p flag must be set with a filename attribute \n";
					return 0;
				}
				break;
		}
	}

	// output file not set
	if(argv[optind] == NULL) {
		cerr << "There are one or more issues with your input. Output file must be set at the end of the command\n";
		return 0;
	} else {
		filename = argv[optind];
	}

	// prob not set
	if(!prob_set) {
		cerr << "-p flag must be set \n";
		return 0;
	}

	// check if prob is an integer
	for(int i = 0; prob_string[i] != '\0'; i++) {
		if(!isdigit(prob_string[i])) {
			cerr << "Probability must be an integer \n";
			return 0;
		}
	}

	prob = atoi(prob_string); // convert prob to integer

	// check if prob within bounds
	if(prob < 0 || prob > 100) {
		cerr << "Probability must be between 0 and 100 inclusive" << endl;
		return 0;
	}
	
	struct data trialData;
	bool found = false;

	// output info
	cout << "Reading results from " << filename << "..." << endl << endl;
	cout << "Checking results for -p = " << prob << "..." << endl << endl;
	
	ifstream input(filename, ios::binary | ios::in);
	// read info from binary file until it fails
	while(!input.fail()) {
		input.read((char *)&trialData, sizeof(struct data));
		if(trialData.probability == prob && !input.fail()) { // check if info matches query
			found = true;
			cout << "Found " << trialData.trials << " trials." << endl << endl;
			cout << "Success - " << trialData.successRate << "%" << endl;
			cout << "Failure - " << trialData.failRate << "%" << endl;
		}
	}
	input.close();

	if(!found) { // no results found
		cout << "No results found" << endl;
	}

	return 0;
}
