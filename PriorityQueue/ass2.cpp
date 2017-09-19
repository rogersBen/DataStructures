
/*
	Student Name:		Benjamin Rogers
	Student Number:		4503600
	Student Login:		bjr342
	File Description:	CSCI203 Spring 2017 Ass 2
*/

#include <iostream> 
#include <fstream> 

//Server
struct event {
	float eventTime;	//When the event will occur
	float eventDuration;	//How long the event will take
	int eventType; //0 is arrival, 1 completion of first server, 2 completion of second server etc
	float efficiency;
};

//Customer
struct request {
	float arrivalTime;
	float serviceTime;
	request*next; //Linked list
};

int numServers;
int numHeap;
int numIdle;	//Number of idle servers
event**events;	//Contains pointers to both active events and idle ones
float now,then;	//Time variables
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

	//Open file
	fin.open(argv[1]);

	//Get number of servers
	fin >> numServers;
	cout << "Number of servers: " << numServers << endl;

	//Allocate resources for events
	events = new event*[numServers+1];

	//Initialise servers
	for(int i = 0; i <= numServers;i++) {
		//Read efficeincy of each server
		events[i] = new event;
		events[i]->eventType = i;
		events[i]->eventTime = 0.0;
		events[i]->eventDuration = 0.0;
		fin >> events[i]->efficiency;
		//cout << events[i]->eventTime << " " << events[i]->eventDuration <<endl;
	}

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
	float*timeQ = new float[numServers];
	int*served = new int[numServers];
	int*queued = new int[numServers];
	int*maxQ = new int[numServers];
	float*busy = new float[numServers];

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
		
	while(numHeap > 0) {
		//Set time to current event
		then = now;
		now = events[0]->eventTime;

		/*
		cout << endl << "Debug" << endl;
		cout << "It is now: " << now << endl;
		if(events[0]->event_type==0) {
			cout << "Event is arrival " << arrivals << endl;
		} else {
			cout << "Event is a departure from server " << events[0]->event_type << endl;
		}
		
		cout<<"Heap has "<< numHeap <<" events on it."<<endl;
		
		for(int d= 0; d<numHeap;d++)
			{
			cout << "Heap["<< d <<"] is of type "<< events[d]->event_type
			<<" and contains "<< events[d]->event_time <<" : "
			<< events[d]->event_duration << endl;
		}
		
		cout<<"Idle list has "<< numIdle <<" events on it"<<endl;
		
		for(int d= numHeap;d<numServers;d++)
			{
			cout<<"Idle["<<d<<"] is of type "<<events[d]->event_type <<endl;
		}
	
		for(int d= 0; d < numServers;d++) {
			if(qlen[d]==0) {
				cout<<"Queue "<<d+1<<" is empty"<<endl;					
			} else {
				cout<<"Queue "<<d+1<<" contains "<<qlen[d]<<" items" <<endl;
				cout<<"  Next item in queue arrived at " <<head[d]->arrival_time<<" and will take " <<head[d]->service_time<<" to serve"<<endl;					
			}
		}
		*/
		
		
		//If arrival
		if(events[0]->eventType == 0) {

			float serviceDuration = events[0]->eventDuration;

			fin >> events[0]->eventTime >> events[0]->eventDuration;

			//Not final arrival
			if(events[0]->eventTime != 0.0) {
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
				if(maxQ[q] < qLength[q])maxQ[q] = qLength[q];
				
			}
			else {
				//make another server busy
				numHeap++;
				numIdle--;
				events[numHeap-1]->eventTime = now + serviceDuration;
				events[numHeap-1]->eventDuration = serviceDuration;
				siftUp();
			}
			
		} else {
			//Do a service event

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
	
	cout << endl << "**********************************************************" << endl;

	cout << endl << "Multiple queue simulation with " << numServers << " servers:" << endl << endl;
		
	
	//Stats
	cout <<"  Total Run time = "<< now <<endl;
	cout <<"  Total Number Served = "<< arrivals << endl << endl;
	for(int s = 0;s < numServers; s++) {
		cout <<"  Server "<< s+1 <<" served "<< served[s] <<	" customers" << endl;
		cout <<"      and was busy for a total of "<< busy[s] << endl;
		cout <<"      and was idle for a total of "<< now-busy[s] << endl << endl;
	}
	//Multiple queue stats
	int totalQueued = 0;
	for(int s = 0; s < numServers; s++) {
		totalQueued+= queued[s];
	}

	cout << "A total of " << totalQueued << " customers were queued: of these:-" << endl << endl;

	for(int s= 0;s<numServers;s++) {

		cout << "	" << queued[s] << "	customers spent time in queue " << s << endl;
		cout << "	with a maximum of " << maxQ[s] << " queued at any one time" << endl << endl;
	}
	for(int s= 0;s<numServers;s++) {

		cout << "	Average time in queue for server " << s+1 << " (all customers for this server) = " <<timeQ[s]/served[s] << endl;

		if(queued[s] == 0) {
			cout << "	Average time in queue for server " << s+1 << " (queued customers for this server) = " << 0 << endl;
		}else {
			cout << "	Average time in queue for server " << s+1 << " (queued customers for this server) = " << timeQ[s]/queued[s] << endl;
		}
		cout << "	Average length of queue for server " << s+1 << " = " << timeQ[s]/now << endl << endl;

	}
	cout << endl << "********************************************************************"<< endl << endl;
		
	
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