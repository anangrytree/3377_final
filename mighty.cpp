#include <iostream>

using namespace std;

int main(int args, char* argv[]) {
	if(argv[1] == NULL || argv[2] == NULL) {
		cerr << "Invalid arguments -- supply two filenames \n" << endl;
		return 0;
	}

	cout << argv[1] << endl;
	cout << argv[2] << endl;
	
	return 0;
}
