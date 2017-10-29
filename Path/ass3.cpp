  /*
 	Filename:			ass3.cpp
 	Student Name:		Benjamin Rogers
 	Student Number:		4503600
 	Login:				bjr342
 	File Description:	Djisktras Algortihm
 */



#include<iostream>
#include<fstream>
#include<cmath>
#include<time.h>

using namespace std;


int numVert = 0;

const int infinity = -1;
int pass = 1;

 	
int numEdges = 0;

char source, destination;
//A is represented by its ASCII value-1
//coordinates[0][0] = x coordinate of node A
//coordinates[0][1] = y coordinate of node A
double** coordinates = 0; //2d array of doubles for coordinates
//costs[origin-1][destination-1] = weight of edge
double** costs = 0;
double* distances = 0;


void printAll(int, double**);
int findSmallestIndex(double[], bool[]);
int modDijkstra(double**, int, int);

void secondFastest(int, int, bool*, int);
void second(int*, int, int);

 int main() {



 	fstream fin;
 	char filename[32];

 	cout << "Filename: ";
 	cin >> filename;

 	bool debug = false;



 	fin.open(filename);

 	if(fin.good()) {

 		fin >> numVert;
 		fin >> numEdges;

 		
 		coordinates = new double*[numVert];


 		for(int i = 0; i < numVert; i++) {
 			//If using arrays
 			coordinates[i] = new double[2];

 			for(int j = 0; j < 2; j++) {
 				coordinates[i][j] = 0;
 			}
 		}

 		
 		for(int i = 0; i < numVert; i++) {
 			char label;
 			fin >> label;
 			
 			int coord = (int(label)) - 96;
 			fin >> coordinates[coord-1][0];
 			fin >> coordinates[coord-1][1];

 			if(debug) {
 				cout << "Coord: " << coord << endl;
 				cout << "Coordinates[" << coord-1 << "][0]: " << coordinates[coord-1][0] << endl;
 				cout << "Coordinates[" << coord-1 << "][1]: " << coordinates[coord-1][1] << endl;
 			}
 			
 		}
 		
 		costs = new double*[numEdges];

 		for(int i = 0; i < numEdges; i++) {
 			costs[i] = new double[numEdges];
 			for(int j = 0; j < numEdges; j++) {
 				costs[i][j] = infinity;
 			}
 		}

 		
 		//Read in costs of edges
 		for(int i = 0; i < numEdges; i++) {
 			char origin, destination;
 			int weight;

 			fin >> origin;
 			fin >> destination;
 			fin >> weight;

 			int originCoord = (int(origin)) - 96;
 			int destinationCoord = (int(destination)) - 96;

 			if(debug) {
 				cout << origin << " Ascii: " << originCoord << endl;
 				cout << destination << " Ascii: " << destinationCoord << endl;
 			}


 			//Set a value for the edge between two nodes
 			costs[originCoord-1][destinationCoord-1] = weight; 
 			costs[destinationCoord-1][originCoord-1] = weight;
 			
 		}

 		fin >> source;
 		fin >> destination;
 		

 	}else {
 		cerr << "ERROR OPENING FILE...";
 		cerr << "TERMINATING..";
 		return -1;
 	}

 	//printAll();

 	fin.close();

 	distances = new double[numVert];

	int start = (int(source)-96);
 	int end = (int(destination)-96);

 	int shortest = modDijkstra(costs, start, end); 


 	cout << "Shortest: " << shortest << endl;


 	//Delete Dynamic memory
 	for(int i = 0; i < numVert; i++) {
 		delete[] coordinates[i];
 	}
 	delete[] coordinates;

 	for(int i = 0; i < numEdges; i++) {
 		delete[] costs[i];
 	}
 	delete[] costs;

 	delete[] distances;

 	return 0;
 }


void printAll(int numEdges, double** costs) {


 	cout << "Source: " << source << endl;
 	cout << "Destination: " << destination << endl;

 	cout << "*****************************" << endl;

 	cout << "Arrays: " << endl;

 	for(int i = 0; i < numVert; i++) {
 		for(int j = 0; j < 2; j++) {
 			cout << "Coordinates[" << i << "][" << j << "]: " << coordinates[i][j] << endl;
 		}
 	}

 	for(int i = 0; i < numEdges; i++) {
 		cout << costs[i][0] << endl;
 		for(int j = 0; j < numEdges; j++) {
 			//cout << costs[i][j] << endl;
 		}
 	}
 }


int modDijkstra(double **costs, int start, int end) {

	bool *visited = new bool[numEdges];				// Stores visited nodes 
	bool *additionalNodes = new bool[numEdges];		// Stores visited nodes not included in the final path
	int numAdditionalNodes = 0;
	double *totalCostToNode = new double[numEdges];	// Stores the total cost to each node number - 1
	int *via = new int[numEdges]; 						//node numbers according to the node in which reached that node by the shortest path recorded
	double *path = new double[numEdges]; 				// Stores the path to be later printed in reverse
	int nextNode = 0, source = 0;
	start--;		// Sets start so it can be used correctly as an array index
	end--;			// Sets end so it can be used correctly as an array index
	
	// Initalise all values to false and infinity
	for (int i=0;i < numEdges; i++)
	{
		visited[i] = false;
		additionalNodes[i] = false;
		totalCostToNode[i] = infinity;
		via[i] = infinity;
		path[i] = infinity;
	}
	totalCostToNode[start] = 0;		// Start nodes cost is 0
	
	nextNode = start;
	
	while(!visited[end]){
		for (int i=0; i < numEdges; i ++){
			if (costs[nextNode][i] != infinity && !visited[i]){		// If there's a node that hasn't been tranversed 	
				if ((totalCostToNode[nextNode] + costs[nextNode][i] < totalCostToNode[i]) || (totalCostToNode[i] == infinity)){		// If the new total cost of the path found is greater than the total path found
					totalCostToNode[i] = totalCostToNode[nextNode] + costs[nextNode][i];
					via[i] = nextNode;
					additionalNodes[i] = true;
				}
			}
		}
		visited[nextNode] = true;
		nextNode = findSmallestIndex(totalCostToNode, visited);		// Finds the next node with the smallest total cost
	}


	
	cout << "Length of shortest path: " << totalCostToNode[end] << endl;
	
	int x = 0;
	source = end;
	
	do{
		path[x] = source;					// Stores the path to be later printed in reverse
		additionalNodes[source] = false;	// Unflags values that have already been read from solution tree
		source = via[source];
		x++;
	}while(source != start);
	
	// Prints out the path in reverse
	cout << "Shortest path: " << start + 1 << " ";

	for(int i = x-1; i >= 0; i--) {
		cout << path[i]+1 << " ";
	}
	cout << endl;



	// Prints out visited nodes not included in the final path
	cout << "Additional nodes in the solution tree: ";
	for(int i = 0; i < numEdges; i++){
		if (additionalNodes[i]){
			cout << i+1 << " ";
			numAdditionalNodes++;	//Count number of additional nodes in solution tree
		}
	}
	cout << endl;


	cout << "\nNumber of additional nodes in the solution tree: " << numAdditionalNodes << endl;


	

//////////////////////////////////////////////////////////////////////////////////////////////////////////

	int* extraNodes = new int[numAdditionalNodes];

	//Store additional nodes for second fastest path
	for(int i = 0; i < numEdges; i++){
		if (additionalNodes[i]) {
			extraNodes[i] = i+1;
		}else if(i == start) {
			extraNodes[i] = start+1;
		}else if(i == end) {
			extraNodes[i] = end+1;
		}
	}


	additionalNodes[start] = start+1;
	additionalNodes[end] = end+1;
	



 	int *adjacentToStart = new int[numVert];
 	int countStart  = 0;

 	int tempNode = start;
 	for(int i = 0; i < numEdges; i ++) {
 		if(costs[tempNode][i] > infinity) {
 			adjacentToStart[countStart] = i;
 			//cout << "Cost: " <<  costs[tempNode][i] << " From " << start << " to " << i << endl;
 			countStart++;
 		}
 	}



 	cout << endl;

 	int *adjacentToEnd = new int[numVert];
 	int countEnd = 0;


 	int tempNodeB = end;
 	for(int i = 0; i < numEdges; i ++) {
 		if(costs[tempNodeB][i] > infinity) {
 			adjacentToEnd[countEnd] = i;
 			cout << "Cost: " <<  costs[tempNodeB][i] << " From " << end << " to " << i << endl;
 			countEnd++;
 		}
 	}
	
	/*
	cout << "\nThese nodes are adjacent to end: " << endl;
	for(int i = 0; i < numVert; i ++) {
		if(adjacentToEnd[i] > 0 && adjacentToEnd[i] < 10) {
			cout << adjacentToEnd[i] << endl;
		}
	}
	*/
 	if(pass == 1) {
 		second(adjacentToStart, start+1, end+1);
 		pass++;		
 	}

	
	// Deletes dynamic memory
	delete[] visited;
	delete[] additionalNodes;
	delete[] totalCostToNode;
	delete[] via;


	return totalCostToNode[end];
}

void secondFastest(int start, int end, bool* additionalNodes, int numAdditionalNodes) {

}

void second(int* adjacentTo, int start, int end) {
	cout << endl << "*******************************************" << endl;
	cout << start << endl << end << endl;

	cout << "\nThese nodes are adjacent to start: " << endl;
 	for(int i = 0; i < numVert; i ++) {
 		if(adjacentTo[i] > 0) {
 			cout << adjacentTo[i] << " " << i << endl;
 		}
 	}

 	
 	int second = modDijkstra(costs, adjacentTo[1], end);
}



int findSmallestIndex(double integers[], bool visited[]) {
	int smallestIndex = infinity;

	for(int i = 0; i < numEdges; i++) {
		if (!visited[i] && integers[i] != infinity){
			if (smallestIndex == infinity) // If its the first found
				smallestIndex = i;
				
			if (integers[i] < integers[smallestIndex])
						smallestIndex = i;	
		}
	}

	return smallestIndex;	
}

