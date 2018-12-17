#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int args, char* argv[]) {
	// check if two filenames are supplied
	if(argv[1] == NULL || argv[2] == NULL) {
		cerr << "Invalid arguments -- supply two filenames" << endl;
		return 0;
	}
	
	// check validity of input file
	ifstream file1(argv[1]);
	if(!file1.is_open()) {
		cerr << "Input file supplied does not exist" << endl;
		return 0;
	}
	file1.close();

	int p[2];
	if(pipe(p) == -1) // create pipe
		exit(1);
	pid_t pid = fork(); // create child
	if(pid == 0) {
		close(p[0]); // close read in child
		dup2(p[1], 1); // write to pipe
		execl("/bin/cat", "cat", "-b", argv[1],  NULL); // write cat to pipe
		close(p[1]); // close write
	} else {
		dup2(1, p[1]); // close write pipe, open STDOUT
		ofstream output;
		output.open(argv[2], ios::out); // open file for output
		char buffer;
		bool cond = true;
		// loop through pipe stream until no more input
		while(read(p[0], &buffer, 1) != 0) {
			cout << buffer; // output pipe contents to STDOUT
			output << buffer; // output pipe contents to file
		}
		output.close(); // close file
		close(p[0]);
		wait(NULL);
	}
	return 0;
}
