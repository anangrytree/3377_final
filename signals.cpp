#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <gsl/gsl_rng.h>

using namespace std;

pid_t pid = fork();

void handler(int signal_num) {
	cout << "Child received signal " << signal_num << endl;
}

void parenthand(int signal_num) {
	int status;
	cout << "Parent received signal " << signal_num << endl;
	kill(pid, SIGKILL);
	waitpid(pid, &status, 0);
	cout << "Parent is done and now exiting" << endl;
	exit(1);
}

int main(int args, char* argv[]) {
	if(pid == 0) { // child	
		signal(1, handler);
		signal(2, handler);
		signal(3, handler);
		signal(4, handler);
		signal(5, handler);
		signal(6, handler);
		signal(7, handler);
		signal(8, handler);
		signal(9, handler);
		signal(10, handler);
		signal(11, handler);
		signal(12, handler);
		signal(13, handler);
		signal(14, handler);
		signal(15, handler);
		signal(16, handler);
		signal(17, handler);
		signal(18, handler);
		signal(19, handler);
		signal(20, handler);
		signal(21, handler);
		signal(22, handler);
		signal(23, handler);
		signal(24, handler);
		signal(25, handler);
		signal(26, handler);
		signal(27, handler);
		signal(28, handler);
		signal(29, handler);
		signal(30, handler);
		signal(31, handler);

		struct timespec wt = {0, 500000000L};
		while(true) {
			cout << "Child waiting" << endl;
			nanosleep(&wt, NULL);
		}
	} else {
		signal(1, parenthand);
		signal(2, parenthand);
		signal(3, parenthand);
		signal(4, parenthand);
		signal(5, parenthand);
		signal(6, parenthand);
		signal(7, parenthand);
		signal(8, parenthand);
		signal(9, parenthand);
		signal(10, parenthand);
		signal(11, parenthand);
		signal(12, parenthand);
		signal(13, parenthand);
		signal(14, parenthand);
		signal(15, parenthand);
		signal(16, parenthand);
		signal(17, parenthand);
		signal(18, parenthand);
		signal(19, parenthand);
		signal(20, parenthand);
		signal(21, parenthand);
		signal(22, parenthand);
		signal(23, parenthand);
		signal(24, parenthand);
		signal(25, parenthand);
		signal(26, parenthand);
		signal(27, parenthand);
		signal(28, parenthand);
		signal(29, parenthand);
		signal(30, parenthand);
		signal(31, parenthand);

		struct timespec wt = {0, 500000000L};
	
		gsl_rng * _gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);
		srand(time(NULL) ^ getpid());
		gsl_rng_set(_gsl_rng, rand());

		sleep(1);
		while(true) {
			int random = gsl_rng_uniform_int(_gsl_rng, 30);
			random++;
			cout << "Sending signal " << random << " to child" << endl;
			kill(pid, random);
			nanosleep(&wt, NULL);
		}
		
		gsl_rng_free(_gsl_rng);

	}
	return 0;
}
