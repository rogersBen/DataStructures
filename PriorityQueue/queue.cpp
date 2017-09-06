using namespace std;

#include <iostream> 
#include <fstream> 

struct event //server
	{
	float event_time,event_duration;
	int event_type;
};

struct request //customer
	{
	float arrival_time,service_time;
	request*next;
};

int n_serv;
int n_heap;
int n_idle;
event**events;
float now,then;
int arrivals;

request**head;
request**tail;
int*qlen;

void sift_down(void);
void swap(int,int);
void sift_up(void);

int main()
	{
	
	char fname[40];
	ifstream infile;
	
	cerr<<"Enter the input file name: ";
	cin>>fname;
	
	//For each pass
	for(int pass= 0;pass<2;pass++)
		{
		
		infile.open(fname);
		infile>>n_serv;
		cout << n_serv << "This\n";
		events= new event*[n_serv+1];
		for(int i= 0;i<=n_serv;i++)
			{
			events[i]= new event;
			events[i]->event_type= i;
			events[i]->event_time= 0.0;
			events[i]->event_duration= 0.0;
			cout << events[i]->event_time << " " << events[i]->event_duration <<endl;
			
			
		}
		infile>>events[0]->event_time>>events[0]->event_duration;

		arrivals= 1;
		n_heap= 1;
		n_idle= n_serv;
		now= then= 0.0;
		
		//If single queue
		if(pass==0)
		{
			head= new request*;
			tail= new request*;
			qlen= new int;
			head[0]= tail[0]= 0;
			qlen[0]= 0;
		}
		else
			{
			head= new request*[n_serv];
			tail= new request*[n_serv];
			qlen= new int[n_serv];
			for(int i= 0;i<n_serv;i++)
				{
				head[i]= tail[i]= 0;
				qlen[i]= 0;
			}
		}
		
		//Stats
		float*tq= new float[n_serv];
		int*served= new int[n_serv];
		int*queued= new int[n_serv];
		int*qmax= new int[n_serv];
		float*busy= new float[n_serv];
		//44
		//Accumulate service statistics
		if(pass==0)
			{
			//First run is single queue 
			tq[0]= 0;
		}
		else
			{
			for(int s= 0;s<n_serv;s++)
				{
				//Initialize
				tq[s]= 0;
				queued[s]= 0;
				qmax[s]= 0;
			}
		}
		for(int s= 0;s<n_serv;s++)
			{
				//still initialize
			served[s]= 0;
			busy[s]= 0.0;
		}
		
		while(n_heap> 0)
			{
			//20
			//Set time to current event
			then= now;
			now= events[0]->event_time;
			
#			ifdef debug
			cout<<endl<<"Debug:"<<endl;
			cout<<"It is now: "<<now<<endl;
#			endif
			
			
#			ifdef debug
			//Process event
			if(events[0]->event_type==0)
				{
				cout<<"Event is arrival "<<arrivals<<endl;
			}
			else
				{
				cout<<"Event is a departure from server "<<events[0]->event_type
				<<endl;
			}
#			endif
			
#			ifdef debug
			cout<<"Heap has "<<n_heap<<" events on it."<<endl;
			for(int d= 0;d<n_heap;d++)
				{
				cout<<"Heap["<<d<<"] is of type "<<events[d]->event_type
				<<" and contains "<<events[d]->event_time<<" : "
				<<events[d]->event_duration<<endl;
			}
			cout<<"Idle list has "<<n_idle<<" events on it"<<endl;
			for(int d= n_heap;d<=n_serv;d++)
				{
				cout<<"Idle["<<d<<"] is of type "<<events[d]->event_type
				<<endl;
			}
#			endif
			
#			ifdef debug
			if(pass==0)
				{
				if(qlen[0]==0)
				cout<<"Queue is empty"<<endl;
				else
				cout<<"Queue contains "<<qlen[0]<<" items"<<endl;
				cout<<"  Next item in queue arrived at "<<head[0]->arrival_time
				<<" and will take "<<head[0]->service_time<<" to serve"
				<<endl;
			}
			else
				{
				for(int d= 0;d<n_serv;d++)
					{
					if(qlen[d]==0)
					cout<<"Queue "<<d+1<<" is empty"<<endl;
					else
					cout<<"Queue "<<d+1<<" contains "<<qlen[d]<<" items"
					<<endl;
					cout<<"  Next item in queue arrived at "
					<<head[d]->arrival_time<<" and will take "
					<<head[d]->service_time<<" to serve"<<endl;
				}
			}
#			endif
			
			//20
			if(events[0]->event_type==0)
				//If arrival
				{
				//22
				float service_duration= events[0]->event_duration;
				infile>>events[0]->event_time>>events[0]->event_duration;
				if(events[0]->event_time!=0.0)
					{
					arrivals++;
					//Arrival is valid
					sift_down();
					
				}
				else
					{
					
					n_heap--;
					swap(0,n_heap);
					if(n_heap> 0)sift_down();
					swap(n_heap,n_serv);
					
				}
				
				if(n_idle==0)
				{
					//
					int q;
					if(pass==0)
						{
							
						q= 0;
					}
					else
						{
							//Find the right q value
							//Find the shortest qLength and set it to q
						q= 0;
						for(int i= 1;i<n_serv;i++)
							{
							if(qlen[i]<qlen[q])q= i;
						}
						
					}
					//Really add the request to the queue
					
					request*r= new request;
					r->arrival_time= now;
					r->service_time= service_duration;
					r->next= 0;
					
					//if the Q is empty i need to create the Q
					//Otherwise simply add a request on the end
					if(head[q]==0)
						{
						head[q]= r;
					}
					else
						{
						tail[q]->next= r;
					}
					tail[q]= r;
					qlen[q]++;
					//Do queueing stats accumulation
					queued[q]++;
					if(qmax[q]<qlen[q])qmax[q]= qlen[q];
					
				}
				else
					{
					//make another server busy
					n_heap++;
					n_idle--;
					events[n_heap-1]->event_time= now+service_duration;
					events[n_heap-1]->event_duration= service_duration;
					sift_up();
					
				}
				
			}
			else //Do a service event 39
				
				//If service
			{
				//Add a service to the appropriate server
				served[events[0]->event_type-1]++;
				busy[events[0]->event_type-1]+= events[0]->event_duration;
				
				if(pass==0&&qlen[0]!=0)
					{
					int i= 0;
					//Keep server busy
					request*todo= head[i];
					head[i]= head[i]->next;
					qlen[i]--;
					events[0]->event_duration= todo->service_time;
					events[0]->event_time= now+todo->service_time;
					delete todo;
					sift_down();
					
				}
				else if(pass==1&&qlen[events[0]->event_type-1]!=0)
					{
					int i= events[0]->event_type-1;
					//Keep server busy
					request*todo= head[i];
					head[i]= head[i]->next;
					qlen[i]--;
					events[0]->event_duration= todo->service_time;
					events[0]->event_time= now+todo->service_time;
					delete todo;
					sift_down();
					
				}
				else
					{
					//Make server idle
					n_heap--;
					n_idle++;
					if(n_heap> 0)
						{
						swap(0,n_heap);
						sift_down();
					}
					
				}
				
			}
			
			//Statistics 2
			if(pass==0)
				{
				tq[0]= tq[0]+(now-then)*qlen[0];
			}
			else
				{
				for(int s= 0;s<n_serv;s++)
					{
					tq[s]= tq[s]+(now-then)*qlen[s];
				}
			}
			
		}
		
		cout<<endl
		<<"********************************************************************"
		<<endl;
		
		if(pass==0) //which pass 48
			{
			cout<<endl<<"Single queue simulation with "<<n_serv<<
			" servers:"<<endl<<endl;
		}
		else
			{
			cout<<endl<<"Multiple queue simulation with "<<n_serv<<
			" servers:"<<endl<<endl;
		}
		//Common stats
		cout<<"  Total Run time = "<<now<<endl;
		cout<<"  Total Number Served = "<<arrivals<<endl<<endl;
		for(int s= 0;s<n_serv;s++)
			{
			cout<<"  Server "<<s+1<<" served "<<served[s]<<
			" customers"<<endl;
			cout<<"      and was busy for a total of "<<busy[s]<<endl;
			cout<<"      and was idle for a total of "<<now-busy[s]<<endl
			<<endl;
		}
		
		if(pass==0)
			{
			//Single queue stats
			cout<<"  A total of "<<queued[0]<<" customers were queued"
			<<endl;
			cout<<"    with a maximum of "<<qmax[0]<<
			" queued at any one time"<<endl<<endl;
			cout<<"  Average time in queue (all customers) = "<<tq[0]/arrivals
			<<endl;
			cout<<"  Average time in queue (queued customers) = "<<
			tq[0]/queued[0]<<endl;
			cout<<"  Average length of queue = "<<tq[0]/now<<endl;
			
		}
		else
			{
			//Multiple queue stats
			int totqueued= 0;
			for(int s= 0;s<n_serv;s++)
				{
				totqueued+= queued[s];
			}
			cout<<"  A total of "<<totqueued<<
			" customers were queued: of these:-"<<endl<<endl;
			for(int s= 0;s<n_serv;s++)
				{
				cout<<"    "<<queued[s]<<" customers spent time in queue "
				<<s<<endl;
				cout<<"    with a maximum of "<<qmax[s]<<
				" queued at any one time"<<endl<<endl;
			}
			for(int s= 0;s<n_serv;s++)
				{
				cout<<"  Average time in queue for server "<<s+1<<
				" (all customers for this server) = "
				<<tq[s]/served[s]<<endl;
				if(queued[s]==0)
					{
					cout<<"  Average time in queue for server "<<s+1<<
					" (queued customers for this server) = "<<0<<endl;
				}
				else
					{
					cout<<"  Average time in queue for server "<<s+1<<
					" (queued customers for this server) = "<<tq[s]/queued[s]
					<<endl;
				}
				cout<<"  Average length of queue for server "<<s+1<<" = "<<
				tq[s]/now<<endl<<endl;
			}
			cout<<endl<<
			"********************************************************************"<<
			endl<<endl;
			
		}
		
		infile.close();
		for(int i= 0;i<=n_serv;i++)
			{
			delete events[i];
		}
		delete events;
		
		delete head;
		delete tail;
		delete qlen;
		
	}
	
	return 0;
}

void sift_down()
	{
	int j,k;
	k= 0;
	do
		{
		j= k;
		if(2*j+1<n_heap&&events[2*j+1]->event_time<events[k]->event_time)
		k= 2*j+1;
		if(2*j+2<n_heap&&events[2*j+2]->event_time<events[k]->event_time)
		k= 2*j+2;
		swap(j,k);
	}
	while(j!=k);
	
	return;
}

void swap(int i,int j)
	{
	event*temp;
	if(i!=j)
		{
		temp= events[i];
		events[i]= events[j];
		events[j]= temp;
	}
	return;
}

void sift_up()
	{
	int j,k;
	
	j= n_heap-1;
	do
		{
		k= j;
		if(j> 0&&events[j]->event_time<events[(j-1)/2]->event_time)
		k= (j-1)/2;
		swap(j,k);
	}
	while(j!=k);
	return;
}

