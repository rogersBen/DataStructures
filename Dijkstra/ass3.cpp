/*
	Filename:			ass3.cpp
	Student Name:		Ben Rogers
	Student Number:		4503600
	File Description:	CSCI203 Assignment 3 Dijkstras Algorithm
*/


#include<iostream>
#include<fstream>


using namespace std;

int numVertices = 0;
int numEdges = 0;


int main() {
	
	char filename[32];

	cout << "Filename: ";
	cin >> filename;

	fstream fin;

	fin.open(filename);

	if(fin.good()) {

		fin >> numVertices;

		fin >> numEdges;

		cout << numVertices << " " << numEdges;

	}else {
		cerr << "ERROR OPENING FILE..." << endl;
		cerr << "TERMINATING.." << endl;
		return -1;
	}




	return 0;
}