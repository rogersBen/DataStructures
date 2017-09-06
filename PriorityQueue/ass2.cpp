/*
	Student Name:		Benjamin Rogers
	Student Number:		4503600
	Student Login:		bjr342
	File Description:	CSCI203 Spring 2017 Ass 2
*/


#include <iostream>
#include <fstream>

struct event {
	float event_time;	//When the event will occur
	float event_duration;	//How long the event will take
	int event_type;	//0 is arrival, 1 completion of first server, 2 completion of second server etc
	float efficiency;
};

//
struct request {
	float arrival_time;
	float service_time;
	request*next;	//Linked list
};

void siftDown();
void swap(int, int);
void siftUp();

event**events;	//Contains pointers to both active events and idle ones
request**head;	
request**tail;
int*qlen;
int numHeap;
int numIdle;
float now, then;
int numServers;	//Overall array length is numServers+1
int arrivals;

using namespace std;


int main(int argc, char* argv[]) {

	bool first = false;
	
	//File object
	fstream fin;
	char next[30];

	//Open file
	fin.open(argv[1]);

	if(fin.good()) {
		
		//Get number of servers
		fin >> numServers;
		cout << "Number of servers: " << numServers << endl;

		events= new event*[numServers+1];

		//Initialise servers
		for(int i = 0; i < numServers; i++) {
			//Efficiency of each server
			events[i] = new event;
			fin >> events[i]->efficiency;
			events[i]->event_time = 0.0;
			events[i]->event_duration = 0.0;
			cout << events[i]->efficiency << " " << events[i]->event_time << " " << events[i]->event_duration <<endl;
			cout << "Continue? :";
			cin >> next;
		}

		arrivals = 1;
		numHeap = 1;
		numIdle = numServers;
		now= then= 0.0;	//Timer variables

		//If pass == 0
		if(first) {
			head= new request*;
			tail= new request*;
			qlen= new int;
			head[0]= tail[0]= 0;
			qlen[0]= 0;	
		}else {

			head= new request*[numServers];
			tail= new request*[numServers];
			qlen= new int[numServers];

			for(int i= 0;i<numServers;i++)
				{
				head[i]= tail[i]= 0;
				qlen[i]= 0;
			}
		}


		float*timeQ= new float[numServers];
		int*served= new int[numServers];
		int*queued= new int[numServers];
		int*qmax= new int[numServers];
		float*busy= new float[numServers];

		//If pass == 0
		if(first) {
			timeQ[0]= 0;			
		} else {
			for(int s = 0; s<numServers;s++) {
				timeQ[s] = 0;
				queued[s] = 0;
				qmax[s] = 0;
			}
		}


		for(int s= 0;s<numServers;s++) {
			served[s]= 0;
			busy[s]= 0.0;
		}

		while(numHeap > 0) {
			then = now;
			now = events[0]->event_time;

			cout << endl << "Debug" << endl;
			cout << "It is now: " << now << endl;

			if(events[0]->event_type==0) {
				cout << "Event is arrival " << arrivals << endl;
			} else {
				cout << "Event is a departure from server " << events[0]->event_type << endl;
			}
			
			cout<<"Heap has "<< numHeap <<" events on it."<<endl;
			
			for(int d= 0;d<numHeap;d++)
				{
				cout<<"Heap["<<d<<"] is of type "<<events[d]->event_type
				<<" and contains "<<events[d]->event_time<<" : "
				<<events[d]->event_duration<<endl;
			}
			
			cout<<"Idle list has "<<numIdle<<" events on it"<<endl;
			
			for(int d= numHeap;d<=numServers;d++)
				{
				cout<<"Idle["<<d<<"] is of type "<<events[d]->event_type <<endl;
			}

			if(first) {

			}else {
				for(int d= 0;d<numServers;d++) {
					
					if(qlen[d]==0) {
						cout<<"Queue "<<d+1<<" is empty"<<endl;					
					} else {
						cout<<"Queue "<<d+1<<" contains "<<qlen[d]<<" items" <<endl;
						cout<<"  Next item in queue arrived at " <<head[d]->arrival_time<<" and will take " <<head[d]->service_time<<" to serve"<<endl;					
					}

				}
			}

			if(events[0]->event_type==0) {
				//If arrival
				float serviceDuration = events[0]->event_duration;
				fin >> events[0]->event_time >> events[0]->event_duration;

				if(events[0]->event_time != 0.0) {
					arrivals++;
					//Arrival is valid
					siftDown();
				} else {
					numHeap--;
					swap(0, numHeap);

					if(numHeap > 0) {
						siftDown();
					}
					swap(numHeap, numServers);
				}

				if(numIdle==0) {
					int q;
					if(first) {

					}else {
						//Find the right q value
						//Find th shortest qLength and set it to q
						q = 0;
						for(int i=1; i<numServers; i++) {
							if(qlen[i] < qlen[q]) {
								q = i;
							}
						}
					}
					//Really add the request to the queue
					request*r= new request;
					r->arrival_time= now;
					r->service_time= serviceDuration;
					r->next= 0;

					//If the queue is empty need to create the Q
					//Otherwise simply add a request on the end
					if(head[q]==0) {
						head[q] = r;
					} else {
						tail[q]->next = r;
					}
					tail[q] = r;
					qlen[q]++;

					//Do queueing stats accumulation
					queued[q]++;
					if(qmax[q] < qlen[q]) {
						qmax[q] = qlen[q];
					}
					

				} else {
					//Make another server busy
					numHeap++;
					numIdle--;
					events[numHeap-1]->event_time = now+serviceDuration;
					events[numHeap-1]->event_duration = serviceDuration;
					siftUp();
				}
			} else {
				//Do a service event

				//Add a service to the appropriate server
				served[events[0]->event_type-1]++;
				busy[events[0]->event_type-1]+= events[0]->event_duration;

				if(first && qlen[0]!= 0) {
					int i = 0;
					//Keep server busy
					request*todo = head[i];
					head[i] = head[i]->next;
					qlen[i]--;
					events[0]->event_duration= todo->service_time;
					events[0]->event_time= now+todo->service_time;
					delete todo;
					siftDown();
				} else if (!first && qlen[events[0]->event_type-1] != 0) {
					int i = events[0]->event_type-1;
					//Keep server busy
					request*todo= head[i];
					head[i]= head[i]->next;
					qlen[i]--;
					events[0]->event_duration= todo->service_time;
					events[0]->event_time= now+todo->service_time;
					delete todo;
					siftDown();

				} else {
					numHeap--;
					numIdle++;
					if(numHeap > 0) {
						swap(0, numHeap);
						siftDown();
					}
				}
			}

			//Statistics
			if(first) {

			} else {
				for(int s =0; s <numServers; s++) {
					timeQ[s] = timeQ[s] + (now-then) * qlen[s];
				}
			}
		}

		//
		cout << endl << "**********************************************************" << endl;

		if(first) {

		} else {
			cout << endl << "Multiple queue simulation with " << numServers << " servers:" <<endl << endl;
		}

		//Common stats
		cout << " Total Run Time = " << now << endl;
		cout << " Total Number Served = " << arrivals << endl << endl;
		for(int s = 0; s <numServers;s++) {
			cout << "Server " << s+1 << " served " << served[s] << " customers" << endl;
			cout << "		and was busy for a total of " << busy[s] << endl;
			cout << "		and was idle for a total of " << now-busy[s] << endl;
		}

		if(first) {

		}else {
			int totalQueued = 0;
			for(int s = 0; s <numServers;s++) {
				totalQueued+= queued[s];
			}

			cout << "A total of " << totalQueued << " customers were queued: of these:-" << endl << endl;

			for(int s = 0; s<numServers; s++) {
				cout << "	" << queued[s] << "	customers spent time in queue " << s << endl;
				cout << "	with a maximum of " << qmax[s] << " queued at any one time" << endl << endl;
			}

			for(int s = 0; s<numServers; s++) {
				cout << "	Average time in queue for server " << s+1 << " (all customers for this server) = " <<timeQ[s]/served[s] << endl;
				if(queued[s] == 0) {
					cout << "	Average time in queue for server " << s+1 << " (queued customers for this server) = " << 0 << endl;
				}else {
					cout << "	Average time in queue for server " << s+1 << " (queued customers for this server) = " << timeQ[s]/queued[s] << endl;
				}
				cout << "	Average length of queue for server " << s+1 << " = " << timeQ[s]/now << endl << endl;
			}

			cout << "\n*******************************************************************\n";

		}
	}else {		
		cerr << "ERROR OPENING FILE..." << endl;
		cerr << "TERMINATING PROGRAM";
		return -1;
	}

	fin.close();
	for(int i = 0; i <=numServers; i++) {
		delete events[i];
	}
	delete events;
	delete head;
	delete tail;
	delete qlen;

	return 0;
}

void siftDown() {
	int j,k;
	k = 0;
	do {
		j= k;
		if(2 * j+1 < numHeap && events[2*j+1]->event_time < events[k]->event_time) {
			k = 2*j+1;			
		}
		if(2 * j+2 < numHeap && events[2*j+2]->event_time < events[k]->event_time) {
			k= 2*j+2;
			swap(j,k);			
		}
	} while(j != k);
	
	return;
}

void swap(int i, int j) {
	event* temp;
	if(i != j) {
		temp = events[i];
		events[i] = events[j];
		events[j] = temp;
	}

	return;
}

void siftUp() {
	int j, k;

	j = numHeap-1;

	do {
		k = j;
		if(j > 0 && events[j]->event_time < events[(j-1)/2]->event_time) {
			k = (j-1)/2;
			swap(j,k);
		}
	}while(j != k);

	return;
}