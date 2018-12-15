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
	while((c = getopt(args, argv, ":p:")) != -1) {
		switch(c) {
			case 'p':
				prob_string = optarg;
				prob_set = true;
				break;
			case '?':
				if(optopt == 'p') {
					cerr << "The -p flag requires an argument with it \n";
					return 0;
				} else {
					cerr << "An unrecognized argument was specified \n";
					return 0;
				}
				break;
			case ':':
				if(optopt == 'o') {
					cerr << "The -p flag must be set with a filename attribute \n";
					return 0;
				}
				break;
		}
	}

	if(argv[optind] == NULL) {
		cerr << "There are one or more issues with your input. Output file must be set at the end of the command\n";
		return 0;
	} else {
		filename = argv[optind];
	}

	if(!prob_set) {
		cerr << "-p flag must be set \n";
		return 0;
	}

	for(int i = 0; prob_string[i] != '\0'; i++) {
		if(!isdigit(prob_string[i])) {
			cerr << "Probability must be an integer \n";
			return 0;
		}
	}

	prob = atoi(prob_string);	
	struct data trialData;
	bool found = false;

	cout << "Reading results from " << filename << "..." << endl << endl;
	cout << "Checking results for -p = " << prob << "..." << endl << endl;
	
	ifstream input(filename, ios::binary | ios::in);
	while(!input.fail()) {
		input.read((char *)&trialData, sizeof(struct data));
		if(trialData.probability == prob && !input.fail()) {
			found = true;
			cout << "Found " << trialData.trials << " trials." << endl << endl;
			cout << "Success - " << trialData.successRate << "%" << endl;
			cout << "Failure - " << trialData.failRate << "%" << endl;
		}
	}
	input.close();

	if(!found) {
		cout << "No results found" << endl;
	}

	return 0;
}
