#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <math.h>

using namespace std;

int main() {

	double x, y;
	int hits = 0;

	for(int i = 0; i < 10; i++) {
		gsl_rng * _gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);
		srand(time(NULL) ^ getpid());
		gsl_rng_set(_gsl_rng, rand());
		x = gsl_rng_uniform(_gsl_rng);
		y = gsl_rng_uniform(_gsl_rng);
		gsl_rng_free(_gsl_rng);
	
		double distance = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2));
		if(distance <= 1) {
			hits++;
		}

		cout << 4.0 * hits / i << endl;
	}
	return 0;
}
