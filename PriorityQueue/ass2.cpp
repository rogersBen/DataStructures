/*
	Student Name:		Benjamin Rogers
	Student Number:		4503600
	Student Login:		bjr342
	File Description:	CSCI203 Spring 2017 Ass 2
*/


#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char* argv[]) {
	
	//File object
	fstream fin;

	fin.open(argv[1]);

	if(fin.good()) {

	}else {		
		cerr << "ERROR OPENING FILE..." << endl;
		cerr << "TERMINATING PROGRAM";
		return -1;
	}

	return 0;
}