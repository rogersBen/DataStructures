
/*
	Student Name:		Benjamin Rogers
	Student Number:		4503600
	Student Login:		bjr342
	File Description:	CSCI203 Spring 2017 Ass 2
*/

#include <iostream> 
#include <fstream> 
#include <iomanip>

//Because each event has efficiency attached to it everytime the time is updated
//this will need to be considered. For example if server i with efficiency ei serves
//customer j with a service time tj the actual time taken to server the customer is
// ei*tj
//if server 5 has an efficiency of 1.2 and customer 7 has a service time of 5.0 then
//server 5 takes a time of 6.0 to serve customer 7

//Server
struct event {
	double eventTime;	//When the event will occur
	double eventDuration;	//How long the event will take
	int eventType; //0 is arrival, 1 completion of first server, 2 completion of second server etc
	double efficiency;
};

//Customer
struct request {
	double arrivalTime;
	double serviceTime;
	request*next; //Linked list
};

int numServers;
int numHeap;
int numIdle;	//Number of idle servers
event**events;	//Contains pointers to both active events and idle ones
double now,then;	//Time variables
int arrivals;	//Number of arrivals

request**head;	//Head of linked list
request**tail;	//Tail of linked list
int*qLength;	//Length of queue

void siftDown(void);
void swap(int,int);
void siftUp(void);

using namespace std;

int main(int argc, char* argv[]) {
	
	ifstream fin;

	char filename[32];


	cout << "Filename: ";
	cin >> filename;

	//Open file
	fin.open(filename);

	if(!fin.good()) {
		cerr << "ERROR OPENING FILE" << endl;
		cerr << "TERMINATING" << endl;
	} else {
		
	//Get number of servers
	fin >> numServers;

	//Allocate resources for events
	events = new event*[numServers+1];

	//Initialise servers
	for(int i = 0; i <= numServers;i++) {
		events[i] = new event;
		events[i]->eventType = i;
		events[i]->eventTime = 0.0;
		events[i]->eventDuration = 0.0;
	}

	//Read in efficiency
	for(int i = 0; i < numServers; i++) {
		fin >> events[i]->efficiency;
		//cout << i << " " << events[i]->efficiency << endl;
	}

	events[numServers]->efficiency = events[numServers-1]->efficiency;

	//Read in first arrival
	fin >> events[0]->eventTime >> events[0]->eventDuration;

	arrivals = 1;
	numHeap = 1;
	numIdle = numServers;
	now = then = 0.0;
	

	//Initialize queues
	head = new request*[numServers];
	tail = new request*[numServers];
	qLength = new int[numServers];

	for(int i = 0; i < numServers; i++) {
		head[i] = tail[i] = 0;
		qLength[i ]= 0;
	}
	
	
	//Variables for statistics collection
	double*timeQ = new double[numServers];
	int*served = new int[numServers];
	int*queued = new int[numServers];
	int*maxQ = new int[numServers];
	double*busy = new double[numServers];
	int maxQueueLength = 0;
	double avgQTime = 0.0;
	double avgQLength = 0.0;

	//Initialize statistics variables
	for(int s = 0; s < numServers; s++) {
		timeQ[s]= 0;
		queued[s]= 0;
		maxQ[s]= 0;
	}
	for(int s = 0; s < numServers;s++) {
		served[s]= 0;
		busy[s]= 0.0;
	}
	
	//While event priority queue not empty	
	while(numHeap > 0) {
		//Set time to current event
		then = now;

		now = events[0]->eventTime; 

		/*
		cout << endl << "Test" << endl;
		cout << "Time: " << now << endl;
		if(events[0]->eventType==0) {
			cout << "arrival " << arrivals << endl;
		} else {
			cout << "complete payment " << events[0]->eventType << endl;
		}
		
		cout<<"numHeap: "<< numHeap <<endl;
		
		for(int d = 0; d<numHeap;d++)
		{
			cout << "Heap["<< d <<"] is of type "<< events[d]->eventType
			<<" and contains "<< events[d]->eventTime <<" : "
			<< events[d]->eventDuration << endl;
		}
		
		cout<<"Idle list has "<< numIdle <<" events on it"<<endl;
		
		for(int d= numHeap;d<numServers;d++)
			{
			cout<<"Idle["<<d<<"] is of type "<<events[d]->eventType <<endl;
		}
	
		for(int d= 0; d < numServers;d++) {
			if(qLength[d]==0) {
				cout<<"Queue "<<d+1<<" is empty"<<endl;					
			} else {
				cout<<"Queue "<<d+1<<" contains "<<qLength[d]<<" items" <<endl;
				cout<<"  Next item in queue arrived at " <<head[d]->arrivalTime<<" and will take " <<head[d]->serviceTime<<" to serve"<<endl;					
			}
		}
		
		char cont;
		//cout << "\nContinue? ";
		//cin >> cont;
		*/
		
		
		
		
		//If arrival
		if(events[0]->eventType == 0) {

			double serviceDuration = (events[0]->eventDuration * events[0]->efficiency);

			fin >> events[0]->eventTime >> events[0]->eventDuration;



			//Not final arrival
			if(!fin.eof()) {
				//Arrival is valid
				arrivals++;
				siftDown(); //siftDown to ensure heap property is maintained
			} else {
				//No more arrivals
				//Make arrival event idle
				numHeap--;
				swap(0,numHeap);

				if(numHeap> 0)siftDown(); //Restore heap property

				swap(numHeap,numServers); //Swap arrival event with last server
			}
			
			//If no idle servers
			if(numIdle == 0) {
				
				int q;

				//Find the right q value
				//Find the shortest qLengthgth and set it to q
				q = 0;
				for(int i = 1; i < numServers; i++) {
					if(qLength[i] < qLength[q])q = i;
				}
					
				
				//Add the request to the queue
				request*r = new request;
				r->arrivalTime = now;
				r->serviceTime = serviceDuration;
				r->next = 0;
				
				//If the queue is empty need to create the Q
				//Otherwise simply add a request on the end
				if(head[q] == 0) {
					head[q] = r;
				}
				else {
					tail[q]->next = r;
				}
				tail[q] = r;
				qLength[q]++;

				//Do queueing stats accumulation
				queued[q]++;
				if(maxQ[q] < qLength[q]) {
					maxQ[q] = qLength[q];
				}
				
			}
			else {
				//idle serve available
				//make another server busy
				numHeap++;

				numIdle--;
				if(numHeap >= numServers) {
					numHeap - numServers;
				}
				//cout << " B" << numHeap << endl;
				events[numHeap-1]->eventTime = now + serviceDuration;
				events[numHeap-1]->eventDuration = serviceDuration;
				siftUp();
			}
			
		} else {
			//Otherwise must be a customer complete payment event

			//Add a service to the appropriate server
			served[events[0]->eventType-1]++;
			busy[events[0]->eventType-1]+= events[0]->eventDuration;
			

			if(qLength[events[0]->eventType-1] != 0) {
				int i = events[0]->eventType-1;
				//Keep server busy
				//Remove the request from queue[i] 
				request*todo = head[i];
				head[i] = head[i]->next;
				qLength[i]--;
				//Update event at top of heap
				events[0]->eventDuration = todo->serviceTime;
				events[0]->eventTime = now + todo->serviceTime;
				delete todo;
				siftDown(); //Restore the heap property	
			} else {
				//Make server idle
				numHeap--;
				numIdle++; //Increase number of idle servers
				if(numHeap> 0) {
					swap(0,numHeap); //Swap with last event
					siftDown(); //Rebalance heap
				}	
			}	
		}

		
		//Statistics 
		for(int s= 0; s<numServers;s++) {
			timeQ[s]= timeQ[s]+(now-then)*qLength[s];
		}
		
		
	}

	/*
	//Debug efficiency
	cout << "\n*********\n";
	for(int i = 0; i < numServers; i++) {
		cout << i << " " << events[i]->efficiency << endl;
	}
	*/

	//Number of people in the queues
	for(int s = 0; s < numServers; s++) {

		if(maxQ[s] > maxQueueLength) {
			maxQueueLength = maxQ[s];
		}	
	}

	//Time spent in queues
	for(int s = 0; s < numServers; s++) {
		avgQLength += timeQ[s]/now;
		avgQTime += timeQ[s]/queued[s];
	}

	avgQTime = avgQTime / numServers;
	avgQLength = avgQLength / numServers;
	
	//Stats
	cout << "Number of servers: " << numServers << endl;
	cout << "Number of customers served: "<< arrivals << endl;	
	cout << "Time last customer completed service: "<< setprecision(5) << now << endl;
	cout << "Greatest length reached by the queue: " << maxQueueLength << endl;
	cout << "Average length of the queue: "  << avgQLength << endl;
	cout << "Average customer waiting time: " << avgQTime << endl << endl;
	cout << "Checkout \t Priority \t CustomersServed \t IdleTime"<< endl;

	//Each server statistics
	for(int s = 0;s < numServers; s++) {
		cout << s;
		cout << " \t\t " << events[s]->efficiency; 
		cout << " \t " << served[s]; 
		cout << " \t\t\t " << now-busy[s] << endl;
	}

	cout << endl;

	/*

	//Number of people in the queues
	for(int s = 0; s < numServers; s++) {
		cout<<"    "<<queued[s]<<" customers spent time in queue "
		<<s<<endl;
		cout<<"    with a maximum of "<<maxQ[s]<<
		" queued at any one time"<<endl<<endl;

		if(maxQ[s] > maxQueueLength) {
			maxQueueLength = maxQ[s];
		}	
	}

	cout << endl;



	//Time spent in queues
	for(int s = 0; s < numServers; s++) {
			cout<<"  Average time in queue for server "<< 
			s+1 << " (all customers for this server) = "<<
			timeQ[s]/served[s] << endl;

			cout<<"  Average time in queue for server "<<s+1<<
				" (queued customers for this server) = "<<timeQ[s]/queued[s]
				<<endl;
			

			cout<<"  Average length of queue for server "<<s+1<<" = "<<
				timeQ[s]/now<<endl<<endl;

			avgQLength += timeQ[s]/now;
			avgQTime += timeQ[s]/queued[s];
	}

	avgQTime = avgQTime / numServers;
	avgQLength = avgQLength / numServers;

	*/


	cout << endl;
	
	}

	
	//Close file and deallocate resources
	fin.close();
	for(int i = 0;i <= numServers;i++) {
		delete events[i];
	}
	delete events;
	
	delete head;
	delete tail;
	delete qLength;
	
	return 0;
}

//Re-establishes the heap property of the first numHeap elements of the events array by filtering top event down
//This will loop until the element is in the right place
//This is achieved by comparing the event_time of a member of the heap with the values of children
//Then if necessary swapping it with the smallest event_time
//Repeated with new position of parent event until no sap occurs or it reaches a leaf
//The heap is sorted in ascending order
void siftDown() {
	int j,k;
	k = 0;
	do {
		j = k;
		if( 2*j+1 < numHeap && events[2*j+1]->eventTime<events[k]->eventTime)
		k = 2*j+1;
		if( 2*j+2 < numHeap && events[2*j+2]->eventTime<events[k]->eventTime)
		k = 2*j+2;
		swap(j,k);
	} while(j!=k);
	
	return;
}

//Basic swap function
void swap(int i,int j) {
	event*temp;
	if(i != j) {
		temp= events[i];
		events[i]= events[j];
		events[j]= temp;
	}
	return;
}

//Repeatedly compare the new service event to its parent and swap until its in the right place
void siftUp() {
	int j,k;
	
	j = numHeap-1;
	do {
		k = j;
		if(j > 0 && events[j]->eventTime<events[(j-1)/2]->eventTime)
		k = (j-1)/2;
		swap(j,k);
	} while(j!=k);
	return;
}