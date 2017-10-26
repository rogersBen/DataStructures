 /*
 	Filename:			ass3.cpp
 	Student Name:		Benjamin Rogers
 	Student Number:		4503600
 	Login:				bjr342
 	File Description:	Djisktras Algortihm
 */



 #include<iostream>
 #include<fstream>
#include <cmath>


 using namespace std;


int numVert = 0;
int numEdges = 0;
const int infinity = -1;



verticeNode *vertices;
edgeNode *edges;
char source, destination;
//A is represented by its ASCII value-1
//coordinates[0][0] = x coordinate of node A
//coordinates[0][1] = y coordinate of node A
double** coordinates = 0; //2d array of doubles for coordinates
//costs[origin-1][destination-1] = weight of edge
double** costs = 0;
double* distances = 0;


void printAll();
int findSmallestIndex(double[], bool[]);
int findSmallestIndexCoord();
void aStar();
void modDijkstra(int, int);

 int main() {

 	fstream fin;
 	char filename[32];

 	cout << "Filename: ";
 	cin >> filename;



 	fin.open(filename);

 	if(fin.good()) {

 		fin >> numVert;
 		fin >> numEdges;

 		vertices = new verticeNode[numVert];
 		edges = new edgeNode[numEdges];

 		coordinates = new double*[numVert];


 		for(int i = 0; i < numVert; i++) {
 			//If using arrays
 			coordinates[i] = new double[2];

 			for(int j = 0; j < 2; j++) {
 				coordinates[i][j] = 0;
 			}
 		}

 		
 		for(int i = 0; i < numVert; i++) {
 			
 			//If using struct
 			fin >> vertices[i].label;
 			fin >> vertices[i].xCoord;
 			fin >> vertices[i].yCoord;
 			
 			int coord = (int(vertices[i].label)) - 96;
 			
 			//cout << vertices[i].label << " Ascii: " << coord << endl;

 			//coord[a][0] = x coordinate
 			//coord[a][1] = y coordinate
 			coordinates[coord-1][0] = vertices[i].xCoord;
 			coordinates[coord-1][1] = vertices[i].yCoord;
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
 			fin >> edges[i].origin;
 			fin >> edges[i].destination;
 			fin >> edges[i].weight;

 			int originCoord = (int(edges[i].origin)) - 96;
 			int destinationCoord = (int(edges[i].destination)) - 96;

 			//cout << edges[i].origin << " Ascii: " << originCoord << endl;
 			//cout << edges[i].destination << " Ascii: " << destinationCoord << endl;

 			//Set a value for the edge between two nodes
 			costs[originCoord-1][destinationCoord-1] = edges[i].weight; 
 			costs[destinationCoord-1][originCoord-1] = edges[i].weight;
 			
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
 	modDijkstra(start, end); 	

 	cout << "*************************" << endl;

 	for(int i = 0; i < numVert; i++) {
 		//Calculate the straight line distance
 		distances[i] = sqrt(pow(coordinates[i][0]-coordinates[end-1][0],2) + pow(coordinates[i][1]-coordinates[end-1][1],2));
 		/*
 		cout << distances[i] << endl;
 		if(distances[i] == 0) {
 			cout << "\nWhat the heck guys\n";
 			cout << "This is i: " << i << endl;
 			char c = (i+96);
 			cout << "i = " << c << endl; 
 			cout << coordinates[i][0] << endl;
 			cout << coordinates[end-1][0] << endl;
 			cout << coordinates[i][1] << endl;
 			cout << coordinates[end-1][1] << endl;
 			cout << "***********************" << endl;
 		}
 		*/
 	}

 	//dijkstra();




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


void printAll() {

	cout << "\nStructure: " << endl;
 	for(int i = 0; i < numVert; i++) {
 		cout << vertices[i].label << " " << vertices[i].xCoord << " " << vertices[i].yCoord << endl;
 	}

 	for(int i = 0; i < numEdges; i++) {
 		cout << edges[i].origin << " " << edges[i].destination << " " << edges[i].weight << endl;
 	}

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



int findSmallestIndexCoord() {

}

void aStar() {

}

void modDijkstra(int start, int end) {
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
	for(int i = x-1; i >= 0; i--)
		cout << path[i]+1 << " ";
	cout << endl;

	// Prints out visited nodes not included in the final path
	cout << "Additional nodes in the solution tree: ";
	for(int i = 0; i < numEdges; i++){
		if (additionalNodes[i]){
			cout << i+1 << " ";
			numAdditionalNodes++;
		}
	}
	cout << endl;
	
	cout << "Number of additional nodes in the solution tree: " << numAdditionalNodes << endl;
	
	// Deletes dynamic memory
	delete[] visited;
	delete[] additionalNodes;
	delete[] totalCostToNode;
	delete[] via;
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