#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <cmath>

using namespace std;

int main() {

	double x, y, deviation;
	int hits = 0, i;

	gsl_rng * _gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);
	srand(time(NULL) ^ getpid());
	gsl_rng_set(_gsl_rng, rand());
	for(i = 1; i < 1000; i++) {
		x = gsl_rng_uniform(_gsl_rng); // generate random coordinates
		y = gsl_rng_uniform(_gsl_rng);
	
		double distance = sqrt(pow(x, 2) + pow(y, 2)); // calculate distance from center
		if(distance <= 1) { // inside circle
			hits++; // register a hit
		}

		deviation += pow((4.0 * hits / i) - 3.1415926535, 2); // calculate deviation

		cout << "Point #" << i << " ";
		cout << (4.0 * hits) / i << endl; // output estimation for every hit (for time-tests)
	}
	gsl_rng_free(_gsl_rng);

	// output final stats if program is not timed out
	cout << endl;
	cout << "Approximation: " << 4.0 * hits / i << endl;
	cout << "Deviation: " << sqrt(deviation / i) << endl;
	cout << "% error: " << 100 * abs((4.0 * hits / i) - 3.1415926535) / 3.1415926535 << endl;
	return 0;
}
