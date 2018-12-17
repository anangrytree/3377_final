#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <gsl/gsl_rng.h>

using namespace std;

int main(int args, char* argv[]) {
	int opterr = 0;
	int prob = -1;
	char* prob_string;
	bool probSet = false;
	int c;
	// loop through arguments
	while((c = getopt(args, argv, ":p:")) != -1) {
		switch(c) {
			case 'p':
				prob_string = optarg;
				probSet = true;
				break;
			case '?': // invalid argument settings
				if(optopt == 'p') {
					cerr << "The -p flag requires an argument with it" << "\n";
					return 0;
				} else {
					cerr << "An unrecognized flag was specified" << "\n";
					return 0;
				}
				break;
			case ':': // argument error
				cerr << "The -p flag must be set with an integer argument";
				return 0;
			
		}
	}
	
	// check if prob is set
	if(!probSet) {
		cerr << "Probability attribute must be set" << "\n";
		return 0;
	}

	// check if prob is an integer
	for(int i = 0; prob_string[i] != '\0'; i++) {
		if(!isdigit(prob_string[i])) {
			cerr << "Probability must be an integer\n";
			return 0;
		}
	}

	prob = atoi(prob_string); // convert prob to integer

	// check bounds of prob
	if(prob < 0 || prob > 100) {
		cerr << "Probability must be between 0 and 100, inclusive" << "\n";
		return 0;
	}

	// generate a random number
	gsl_rng * _gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);
	srand(time(NULL) ^ getpid());
	gsl_rng_set(_gsl_rng, rand());
	double random = 100 * gsl_rng_uniform(_gsl_rng);
	gsl_rng_free(_gsl_rng);

	if(random < prob)
		exit(0); // success
	else 
		exit(1); // failure

	gsl_rng_free(_gsl_rng);

	return 0;
}
