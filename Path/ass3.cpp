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
int globalEnd;


void printAll(int, double**);
int findSmallestIndex(double[], bool[]);
int modDijkstra(double**, int, int, bool, int);


int second(int*, int, int, int, int, int&);

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

 	globalEnd = end;


 	int shortest = modDijkstra(costs, start, end, true, 1); 


 	//cout << "Shortest: " << shortest << endl;


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


int modDijkstra(double **costs, int start, int end, bool first, int isEnd) {

	//cout << "This is start: " << start << endl << "This is end: " << end << endl;

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


	if(first || pass ==2) {

		if(pass == 1) {
			cout << "Length of shortest path: " << totalCostToNode[end] << endl;
		}else {
			cout << ": Length of second shortest path" << endl;

		}
	
		
		int x = 0;
		source = end;

		
		do{
			path[x] = source;					// Stores the path to be later printed in reverse
			additionalNodes[source] = false;	// Unflags values that have already been read from solution tree
			source = via[source];
			x++;
		}while(source != start);
		
		// Prints out the path in reverse
		if(pass == 1) {
			cout << "Shortest path: " << start + 1 << " ";

			for(int i = x-1; i >= 0; i--) {
				cout << path[i]+1 << " ";
			}
			cout << endl << endl;

		}else {
			cout << "Shortest path: " << start + 1 << " ";
			for(int i = x-1; i >= 0; i--) {
				cout << path[i]+1 << " ";
			}

			if(isEnd == 3) {
				//cout << "This should be the last:";
				cout  << globalEnd << endl;
			}
			cout << endl;
		}


	}
	
	
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////


	int secondStart = 0;
	int secondEnd = 0;


	if(first) {
	 	int *adjacentToStart = new int[numVert];
	 	int countStart  = 0;
	 	int node = 0;

	 	//Initialize
	 	for(int i = 0; i < numVert; i ++) {
	 		adjacentToStart[i] = infinity;
	 	}

	 	//Grab nodes connected to start
	 	for(int i = 0; i < numEdges; i ++) {
	 		if(costs[start][i] > infinity) {

	 			adjacentToStart[countStart] = i;
	 			countStart++;
	 		}
	 	}

	 	secondStart = second(adjacentToStart, start, end, countStart, totalCostToNode[end], node);

	 	//cout << "Start: " << secondStart << " node: " << node << endl;
	
		int *adjacentToEnd = new int[numVert];
	 	int countEnd  = 0;

	 	//Initialize
	 	for(int i = 0; i < numVert; i ++) {
	 		adjacentToEnd[i] = infinity;
	 	}

	 	//Grab nodes connected to end
	 	for(int i = 0; i < numEdges; i ++) {
	 		if(costs[end][i] > infinity) {

	 			adjacentToEnd[countEnd] = i;
	 			countEnd++;
	 		}
	 	}


	 	secondEnd = second(adjacentToEnd, start, end, countEnd, totalCostToNode[end], node);

	 	//cout << "End: " << secondEnd << " node " << node << endl;

	 	pass++;

	 	if(secondStart < secondEnd) {
			//Start is second shortest
	 		cout << node << secondStart;

	 		modDijkstra(costs, node, end, false, 2);

		} else {
			//End is second shortest
			cout << secondEnd;
			//if 3 print end node
			//cout << "Start: " << start << " " << node << endl;
			modDijkstra(costs, start+1, node, false, 3);

		}

	 	delete[] adjacentToStart;
		delete[] adjacentToEnd;

		
	}




	
	// Deletes dynamic memory
	delete[] visited;
	delete[] additionalNodes;
	delete[] totalCostToNode;
	delete[] via;



	return totalCostToNode[end];
}



int second(int* adjacentTo, int start, int end, int count, int totalCostToNode, int &node) {

	int newPath = 10000;
	int finalNode = 0;

 		for(int i = 0; i < count; i ++) {

	 		if(adjacentTo[i] > 0 && adjacentTo[i] < numVert) {

	 			//Find route
	 			//index is nothing, value is the node travelled to from origin
	 			int temp = modDijkstra(costs, adjacentTo[i], end, false, 1);

	 			if(temp < newPath) {
	 				newPath = temp;
	 				node = adjacentTo[i]+1;
	 				
	 			}
	 	
	 		}
	 	}

	 	//cost = newPath, node = finalNode
	 	//cout << "This is newPath: " << newPath+totalCostToNode << " at this node: " << finalNode<<  endl;
	 	//cout << "Path: " << start << " " << finalNode << " path " << end << endl;

	 	return (newPath+totalCostToNode);


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

