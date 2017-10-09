/*
	Filename:			ass3.cpp
	Student Name:		Ben Rogers
	Student Number:		4503600
	File Description:	CSCI203 Assignment 3 Dijkstras Algorithm
*/


#include<iostream>
#include<fstream>


using namespace std;

struct vert {
	char label;
	int x;
	int y;
};

struct edge {
	char start;
	char end;
	int weight;
};

int numVertices = 0;
int numEdges = 0;

edge *edges;
vert *vertices;
char startVert;
char endVert;

void print();
void shortestPath();
void secondShortest();

int main() {
	
	char filename[32];

	cout << "Filename: ";
	cin >> filename;

	fstream fin;

	fin.open(filename);

	if(fin.good()) {

		fin >> numVertices;
		fin >> numEdges;

		vertices = new vert[numVertices];
		edges = new edge[numEdges];

		//Read in vertices
		for(int i = 0; i < numVertices; i++) {
			fin >> vertices[i].label;
			fin >> vertices[i].x;
			fin >> vertices[i].y;
		}

		//Read in edges
		for(int i = 0; i < numEdges; i++) {
			fin >> edges[i].start;
			fin >> edges[i].end;
			fin >> edges[i].weight;
		}

		//Read in start and goal vertex
		fin >> startVert;
		fin >> endVert;


	}else {
		cerr << "ERROR OPENING FILE..." << endl;
		cerr << "TERMINATING.." << endl;
		return -1;
	}

	fin.close();

	//shortestPath();
	//secondShortest();

	print();


	//Delete memory
	delete vertices;
	delete edges;

	return 0;
}


void print() {
	cout << "NumVertices: " << numVertices << endl;
	cout << "NumEdges: " << numEdges << endl;
	for(int i = 0; i < numVertices; i++) {
		cout << vertices[i].label << "\t";
		cout << vertices[i].x << "\t";
		cout << vertices[i].y << endl;
	}

	for(int i = 0; i < numEdges; i++) {
		cout << edges[i].start << "\t";
		cout << edges[i].end << "\t";
		cout << edges[i].weight << endl;
	}

	cout << "Start: " << startVert << endl;
	cout << "End: " << endVert << endl;
}

void shortestPath() {

}

void secondShortest() {

}