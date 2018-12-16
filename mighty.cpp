#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int args, char* argv[]) {
	if(argv[1] == NULL || argv[2] == NULL) {
		cerr << "Invalid arguments -- supply two filenames" << endl;
		return 0;
	}

	ifstream file1(argv[1]);
	if(!file1.is_open()) {
		cerr << "Input file supplied does not exist" << endl;
		return 0;
	}
	file1.close();

	int p[2];
	if(pipe(p) == -1)
		exit(1);
	pid_t pid = fork();
	if(pid == 0) {
		close(p[0]);
		dup2(p[1], 1);
		execl("/bin/cat", "cat", "-b", argv[1],  NULL);
		close(p[1]);
	} else {
		dup2(1, p[1]);
		ofstream output;
		output.open(argv[2], ios::out);
		char buffer;
		bool cond = true;
		while(read(p[0], &buffer, 1) != 0) {
			cout << buffer;
			output << buffer;
		}
		output.close();
		close(p[0]);
		wait(NULL);
	}
	return 0;
}
