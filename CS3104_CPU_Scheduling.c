#include <stdio.h>
#include <string.h>
#define MAX 6

typedef struct{
	int burst_time;
	int arrival_time;
    int priority_number;
}Process;

typedef struct{
	Process Processes[MAX];
	int count;
	int time_quantum;//Time Quantum or Time Slice
}ProcessList;

typedef struct{
	float average_waiting_time;
	float total_waiting_time;
}Result;

//Functions
void PrintProcessList(ProcessList PL);
void PrintResult(Result R);
int IsDone(int Done[], ProcessList PL);
int AllInQueue(int Queue[], ProcessList PL);
void Preemptive(ProcessList PL);
int LastProcess(int Done[], ProcessList PL);
void RoundRobin(ProcessList PL);
void FirstComeFirstServe(ProcessList PL);
void NonPreemptive(ProcessList PL);
void PriorityScheduling(ProcessList PL);
void InsertProcess(ProcessList*PL, Process P);//For debugging
void Greetings();
void Menu(ProcessList PL);
void InputProcess(ProcessList*PL);
//For the GANNT Chart
int PreemptiveGANNT(ProcessList PL);
int RoundRobinGANNT(ProcessList PL);
void FirstComeFirstServeGANNT(ProcessList PL);
//

int First=1;

void PrintProcessList(ProcessList PL){
    int i;
    printf("Legend:\n");
    printf("P# = Process Number\n");
    printf("BT = Burst Time\n");
    printf("AT = Arrival Time\n");
    printf("PN = Priority Number\n");
    printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240);
    printf("Your inputs:\n");
    printf("\nProcess Table\n");
    printf("--------------------\n");
    printf("%c%s%2c%s%3c%s%3c%s%3c\n",'|',"P#",'|',"BT",'|',"AT",'|',"PN",'|');
    for(i=0;i<PL.count;i++){
        printf("%c%d%3c%3d%2c%3d%2c%3d%2c\n",'|',i+1,'|',PL.Processes[i].burst_time,'|',PL.Processes[i].arrival_time,'|',PL.Processes[i].priority_number,'|');
        printf("--------------------\n");
    }
    printf("\n# of Processes: %d\n",PL.count);
    printf("Time Slice/Quantum: %d\n",PL.time_quantum);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n\n",240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240);
}

void PrintResult(Result R){
	printf("Total Waiting Time: %.2f m/s\n",R.total_waiting_time);
	printf("Average Waiting Time: %.2f m/s\n\n",R.average_waiting_time);
}

int IsDone(int Done[], ProcessList PL){
	int i,ctr;
	for(i=ctr=0;i<PL.count;i++){
		if(Done[i]){
			ctr++;
		}
	}
	return (ctr==PL.count)?1:0;
}

int AllInQueue(int Queue[], ProcessList PL){
    return IsDone(Queue,PL);
}

int PreemptiveGANNT(ProcessList PL){
    	
	Result R;
	
	R.average_waiting_time=R.total_waiting_time=0;
	
	int Done[PL.count],Queue[PL.count],Preemption[PL.count],Result[PL.count];
	
	int time=0,total=0,prev=0,firstTime=1,firstProcess=1,fp=0,x,min,w,line;
	
	for(x=line=0;x<PL.count;Done[x]=Queue[x]=Preemption[x]=Result[x]=0,x++){}

	printf("|");
	while(!IsDone(Done,PL)){
		for(x=0;x<PL.count;x++){
			if(!Queue[x]&&PL.Processes[x].arrival_time==time){
				Queue[x]=1;
				break;
			}
		}

		for(x=w=0,min=999;x<PL.count;x++){
			if(!Done[x]&&Queue[x]&&PL.Processes[x].burst_time&&PL.Processes[x].burst_time<=min){
				if(PL.Processes[x].burst_time==min){
					if(PL.Processes[x].arrival_time<PL.Processes[w].arrival_time){
						min=PL.Processes[x].burst_time; 
						w=x;
					}
				}else{
					min=PL.Processes[x].burst_time;
					w=x;
				}
			}
		}

		if(firstTime){
			firstTime=0;
			prev=w;
		}
		
		if(firstProcess){
			fp=w;
			firstProcess=0;
		}
	
		if(!Done[prev]){
			if(prev!=w){
				Preemption[prev]=total;
				prev=w;
			}else if(prev==w){
				Preemption[prev]=0;			
			}			
		}

		if(AllInQueue(Queue,PL)&&!Done[w]){
			Result[w]+=total;	
			total+=PL.Processes[w].burst_time;
			PL.Processes[w].burst_time-=PL.Processes[w].burst_time;	
			Done[w]=1;	
		}else{
			total++;
			PL.Processes[w].burst_time--;			
			if(PL.Processes[w].burst_time==0){
				Done[w]=1;
			}
		}
		line++;

		printf("P%1d%3c",w+1,'|');

		if(!Preemption[fp]){
			Result[fp]=0;
		}
			
		time++;
	}
	printf("\n");

	return line;
}

void Preemptive(ProcessList PL){
	
	Result R;
	
	R.average_waiting_time=R.total_waiting_time=0;
	
	int Done[PL.count],Queue[PL.count],Preemption[PL.count],Result[PL.count];
	
	int time=0,total=0,prev=0,firstTime=1,firstProcess=1,fp=0,x,min,w;
	
	for(x=0;x<PL.count;Done[x]=Queue[x]=Preemption[x]=Result[x]=0,x++){}
    printf("GANNT CHART\n\n");
	int line = PreemptiveGANNT(PL);
	for(x=0;x<line;x++){
		printf("-----");
	}
	printf("-\n");
	while(!IsDone(Done,PL)){
		printf("%2d",total);
        printf("%3c",' ');
		for(x=0;x<PL.count;x++){
			if(!Queue[x]&&PL.Processes[x].arrival_time==time){
				Queue[x]=1;
				break;
			}
		}

		for(x=w=0,min=999;x<PL.count;x++){
			if(!Done[x]&&Queue[x]&&PL.Processes[x].burst_time&&PL.Processes[x].burst_time<=min){
				if(PL.Processes[x].burst_time==min){
					if(PL.Processes[x].arrival_time<PL.Processes[w].arrival_time){
						min=PL.Processes[x].burst_time; 
						w=x;
					}
				}else{
					min=PL.Processes[x].burst_time;
					w=x;
				}
			}
		}

		if(firstTime){
			firstTime=0;
			prev=w;
		}
		
		if(firstProcess){
			fp=w;
			firstProcess=0;
		}
	
		if(!Done[prev]){
			if(prev!=w){
				Preemption[prev]=total;
				prev=w;
			}else if(prev==w){
				Preemption[prev]=0;			
			}			
		}

		if(AllInQueue(Queue,PL)&&!Done[w]){
			Result[w]+=total;
			
			total+=PL.Processes[w].burst_time;
			PL.Processes[w].burst_time-=PL.Processes[w].burst_time;	
			Done[w]=1;			
		}else{
			total++;
			PL.Processes[w].burst_time--;			
			if(PL.Processes[w].burst_time==0){
				Done[w]=1;
			}
		}

		if(!Preemption[fp]){
			Result[fp]=0;
		}
			
		time++;
	}

	printf("%d\n\n",total);
	for(x=0;x<PL.count;x++){
        if(Result[x]-PL.Processes[x].arrival_time-Preemption[x]>=0){
		    R.total_waiting_time+=Result[x]-PL.Processes[x].arrival_time-Preemption[x];
        }
	}
	R.average_waiting_time=R.total_waiting_time/PL.count;

    PrintResult(R);
}

int LastProcess(int Done[], ProcessList PL){
	int i,ctr,p;
	for(i=ctr=0;i<PL.count;i++){
		if(Done[i]){
			 ctr++;
		}
	};
	return (ctr==PL.count-1)?1:0;
}

int RoundRobinGANNT(ProcessList PL){
    int Done[PL.count], Preemptions[PL.count], Result[PL.count];

	int i,total,FinalProcess,x,p,line;

	for(i=total=FinalProcess=0;i<PL.count;i++){
		Done[i]=Preemptions[i]=Result[i]=0;
	}

    line=0;
    printf("|");
	while(!IsDone(Done,PL)){
		for(i=0;i<PL.count;i++){
			if(PL.Processes[i].burst_time){
				printf("P%1d%3c",i+1,'|');
                line++;
			}
			if(PL.Processes[i].burst_time>=PL.time_quantum&&!Done[i]){
				if(!FinalProcess){
				    Result[i]=total;
				}
				total+=PL.time_quantum;
				PL.Processes[i].burst_time-=PL.time_quantum;
			}else if(PL.Processes[i].burst_time<PL.time_quantum&&!Done[i]){
				if(!FinalProcess){
					Result[i]=total;
				}
				total+=PL.Processes[i].burst_time;
				PL.Processes[i].burst_time-=PL.Processes[i].burst_time;
			}

			if(!PL.Processes[i].burst_time){
				Done[i]=1;
			}else{
                if(!LastProcess(Done,PL)){
                    Preemptions[i]+=PL.time_quantum;
                }
            }
            if(LastProcess(Done,PL)&&!FinalProcess){
                    FinalProcess=1;
                    for(x=0;x<PL.count;x++){
                        if(!Done[x]){
                            p=x;
                            break;
                        }
                    }
                    Result[p]=total;
            }
		}
	}
    printf("\n");
    return line;
}

void RoundRobin(ProcessList PL){
	Result R;
    R.average_waiting_time=R.total_waiting_time=0;

	int Done[PL.count], Preemptions[PL.count], Result[PL.count];

	int i,total,FinalProcess,x,p;

	for(i=total=FinalProcess=0;i<PL.count;i++){
		Done[i]=Preemptions[i]=Result[i]=0;
	}
    printf("GANNT CHART\n\n");
    int line = RoundRobinGANNT(PL);
    for(i=0;i<line;i++){
        printf("%s","-----");
    }
    printf("%s","-");

    printf("\n");
	while(!IsDone(Done,PL)){
		for(i=0;i<PL.count;i++){
			if(PL.Processes[i].burst_time){
				printf("%2d",total);
                printf("%3c",' ');
			}
			if(PL.Processes[i].burst_time>=PL.time_quantum&&!Done[i]){
				if(!FinalProcess){
				    Result[i]=total;
				}
				total+=PL.time_quantum;
				PL.Processes[i].burst_time-=PL.time_quantum;
			}else if(PL.Processes[i].burst_time<PL.time_quantum&&!Done[i]){
				if(!FinalProcess){
					Result[i]=total;
				}
				total+=PL.Processes[i].burst_time;
				PL.Processes[i].burst_time-=PL.Processes[i].burst_time;
			}

			if(!PL.Processes[i].burst_time){
				Done[i]=1;
			}else{
                if(!LastProcess(Done,PL)){
                    Preemptions[i]+=PL.time_quantum;
                }
            }
            if(LastProcess(Done,PL)&&!FinalProcess){
                    FinalProcess=1;
                    for(x=0;x<PL.count;x++){
                        if(!Done[x]){
                            p=x;
                            break;
                        }
                    }
                    Result[p]=total;
            }
		}
	}

	printf("%d\n\n",total);

    for(i=0;i<PL.count;i++){
        R.total_waiting_time+=Result[i]-Preemptions[i];
    }
    R.average_waiting_time=R.total_waiting_time/PL.count;

    PrintResult(R);
}

void FirstComeFirstServeGANNT(ProcessList PL){
    Result R;

    R.average_waiting_time=R.total_waiting_time=0; 

    int WaitingTime[PL.count];

    int i,total;

    memset(WaitingTime,0,sizeof(WaitingTime));

    printf("|");
    for(i=total=0;i<PL.count;i++){
        printf("P%d%7c",i+1,'|');
        R.total_waiting_time+=WaitingTime[i]=total;
        total+=PL.Processes[i].burst_time;
    }
    printf("\n");
    for(i=0;i<PL.count;i++){
        printf("%s","---------");
    }
    printf("-\n");

    R.average_waiting_time=R.total_waiting_time/PL.count;
}

void FirstComeFirstServe(ProcessList PL){

    Result R;

    R.average_waiting_time=R.total_waiting_time=0; 

    int WaitingTime[PL.count];

    int i,total;

    printf("GANNT CHART\n\n");
    FirstComeFirstServeGANNT(PL);

    memset(WaitingTime,0,sizeof(WaitingTime));
    for(i=total=0;i<PL.count;i++){
        R.total_waiting_time+=WaitingTime[i]=total;
        total+=PL.Processes[i].burst_time;
        printf("%d",WaitingTime[i]);
        printf("%7c",' ');
    }
    printf("%d\n\n",total);

    R.average_waiting_time=R.total_waiting_time/PL.count;

    PrintResult(R);
}

void NonPreemptive(ProcessList PL){
    int i,j;
    Process temp;
    for(i=0;i<PL.count;i++){
        for(j=0;j<PL.count-1;j++){
            if(PL.Processes[j+1].burst_time<PL.Processes[j].burst_time){
                temp=PL.Processes[j];
                PL.Processes[j]=PL.Processes[j+1];
                PL.Processes[j+1]=temp;
            }
        }
    }
    FirstComeFirstServe(PL);
}

void PriorityScheduling(ProcessList PL){
    int i,j;
    Process temp;
    for(i=0;i<PL.count;i++){
        for(j=0;j<PL.count-1;j++){
            if(PL.Processes[j+1].priority_number<PL.Processes[j].priority_number){
                temp=PL.Processes[j];
                PL.Processes[j]=PL.Processes[j+1];
                PL.Processes[j+1]=temp;    
            }
        }
    }
    FirstComeFirstServe(PL);
}

void InsertProcess(ProcessList*PL,Process P){
	if(PL->count<MAX){
		PL->Processes[PL->count++]=P;
	}
}

void Greetings(){
    printf("\n[Welcome to the CPU Scheduling Simulator]\n\n");
    printf("============================RULES============================\n");
    printf("1. Strictly input whole numbers (0,1,2,...,Positive Infinity)\n");
    printf("2. At best, please input smaller numbers (0-100)\n");
    printf("3. Minimum number of processes should be 2, maximum of MAX(%d)\n",MAX);
    printf("4. MAX can be changed on line 3, MAX should be atleast 2\n");
    printf("5. The Arrival Time each of processes should be UNIQUE\n");
    printf("6. For the Priority Numbers:\n   Lowest Number = Highest Priority\n   Highest Number = Lowest Priority\n");
    printf("============================RULES============================\n\n");
}

void PrintMenu(){
    printf("%3c%c Press the number of your choice %c%c",254,254,254,254);
    printf("\n%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%cMENU%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    printf("%cCPU SCHEDULING ALGORITHMS:%13c\n",186,186);
    printf("%c1.)FIRST COME FIRST SERVE%14c\n",186);
    printf("%c2.)NONPREEMPTIVE SHORTEST JOB FIRST%4c\n",186,186);
    printf("%c3.)PREEMPTIVE SHORTEST JOB FIRST%7c\n",186,186);
    printf("%c4.)ROUND ROBIN%25c\n",186,186);
    printf("%c5.)PRIORITY SCHEDULING%17c\n",186,186);
    printf("%c6.)CHANGE PROCESS VALUES%15c\n",186,186);
    printf("%c7.)EXIT%32c\n",186,186);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
}

void Menu(ProcessList PL){
    char choice=' ';
    do{
        printf("Choice:");
        scanf("%s",&choice);
        if(choice=='1'){
            printf("\n1.)FIRST COME FIRST SERVE CPU SCHEDULING ALGORITHM\n\n");
            FirstComeFirstServe(PL);
        }else if(choice=='2'){
            printf("\n2.)NONPREEMPTIVE SHORTEST JOB FIRST CPU SCHEDULING ALGORITHM\n\n");
            NonPreemptive(PL);
        }else if(choice=='3'){
            printf("\n3.)PREEMPTIVE SHORTEST JOB FIRST CPU SCHEDULING ALGORITHM\n\n");
            Preemptive(PL);
        }else if(choice=='4'){
            printf("\n4.)ROUND ROBIN CPU SCHEDULING ALGORITHM\n\n");
            RoundRobin(PL);
        }else if(choice=='5'){
            printf("\n5.)PRIORITY SCHEDULING CPU SCHEDULING ALGORITHM\n\n");
            PriorityScheduling(PL);
        }else if(choice=='6'){
            printf("\nCHANGING PROCESS LIST VALUES...\n\n");
            InputProcess(&PL);
        }else{
            if(choice!='7'){
                printf("\nINVALID INPUT! TRY AGAIN\n\n");
            }
        }
    }while(choice!='7');
    printf("\n[Leaving the program]\n\n");
}

int CheckArrival(ProcessList PL, int AT, int ctr){
    int i;
    for(i=0;i<ctr&&PL.Processes[i].arrival_time!=AT;i++){}
    return (i==ctr)?1:0;
}

void InputProcess(ProcessList*PL){
    int i;
    do{
        printf("Number of  processes to input:");
        scanf("%d",&PL->count);
        if(PL->count<=1||PL->count>MAX){
            printf("\nNumber of processes cannot be less than or equal to 1,\nor greater than MAX(%d)\n\n",MAX);
        }
    }while(PL->count<=1||PL->count>MAX);

    do{
        printf("Time Slice/Quantum for all the processes:");
        scanf("%d",&PL->time_quantum);
        if(PL->time_quantum<=0){
            printf("\nThe Time Quantum/Slice value should be greater than 0\n\n");
        }
    }while(PL->time_quantum<=0);

    for(i=0;i<PL->count;i++){
        printf("\nVALUES FOR PROCESS #%d:\n",i+1);

        do{
            printf("BURST TIME:");
            scanf("%d",&PL->Processes[i].burst_time);
            if(PL->Processes[i].burst_time<=0){
                printf("\nBurst Time should be greater than 0\n\n");
            }
        }while(PL->Processes[i].burst_time<=0);

        int ArrivalCheck;

        do{
            do{
                printf("ARRIVAL TIME:");
                scanf("%d",&ArrivalCheck);
                if(ArrivalCheck<0||ArrivalCheck>20){
                    printf("\nArrival Time should be UNIQUE, greater than or equal to 0\nand less than or equal to 20\n\n");
                }
            }while(ArrivalCheck<0||ArrivalCheck>20);
            if(!CheckArrival(*PL,ArrivalCheck,i+1)){
                    printf("\nArrival Time should be UNIQUE, greater than or equal to 0\nand less than or equal to 20\n\n");
            }
        }while(!CheckArrival(*PL,ArrivalCheck,i+1));

        PL->Processes[i].arrival_time=ArrivalCheck;

        do{
            printf("PRIORITY NUMBER:");
            scanf("%d",&PL->Processes[i].priority_number);
            if(PL->Processes[i].priority_number<0){
                printf("\nPriority Number should be greater than or equal 0\n\n");
            }
        }while(PL->Processes[i].priority_number<0);
    }
    PrintProcessList(*PL);
    if(First){
        First=0;
        PrintMenu();
        Menu(*PL);
    }else{
        PrintMenu();
    }
}

int main(){
    ProcessList PL;
    Greetings();
    InputProcess(&PL);
    return 0;
}