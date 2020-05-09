#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct ProcessCtrBlk{
    int pid;
    int priority;
//    int arrival_time;
    int agingtime;
    int burst_time;
    char state;
    struct ProcessCtrBlk* link;
}PCB;

typedef struct  Scheduling_Queue{
    int PCBnum;
    PCB* headerNode;
    PCB* tailNode;
}Queue;
//
int generation_times = 0; 
int terminate_process = 0;
PCB* generate_process(Queue* schedulQ, int n, int index){
    PCB* newprocess = NULL;
    //Queue* schedulQ;
    //schedulQ = &schedul[index];
    int num = 0;
    if(n == 2){//1~4까지 난수에 대해 25%확률로 프로세스 생성 
    //srand((unsigned int)time(NULL));
    int randomCPU_burstT = rand()%500 +1;
    int randpriority = rand()%100 +1;
    int randimPid = rand()%10 +1;
    newprocess = (PCB*)malloc(sizeof(PCB));
        if(newprocess != NULL){
            newprocess->burst_time = randomCPU_burstT; // 난수 수행시간 지정  
            newprocess->priority = randpriority; // 난수  우선순위 지정 
            newprocess->pid = randimPid;
            newprocess->state = 'R';//ready
            newprocess->link = NULL;//
            if( (schedulQ + index)->PCBnum==0){//Q가 비었을때 
                (schedulQ + index)->headerNode = newprocess;//디큐 시 헤드먼저
                (schedulQ + index)->tailNode = newprocess;//인큐 시 꼬리로 
            }
            else{
              (schedulQ + index)->tailNode->link = newprocess;//꼬리노드에 있는 프로세스 링크에 연결 
               //(schedulQ + index)->tailNode = newprocess;
            }
            printf("%d",randomCPU_burstT);
            printf("new process generated!\n");
            printf("PCBNUM is %d\n",(schedulQ + index)->PCBnum);
            printf("Insert index is %d\n",index);
            
            (schedulQ + index)->PCBnum++;
            
            //printf("Arrival time : %d\n",randonArrivalT);
            //avtime++;
        }
        else{
            printf("memory allocation fail");
            return NULL;
        }
        ++generation_times;
    }
    return newprocess;    
}

//우선순위 1번째 
int R_R(Queue* schedul, int timeqauntum, int agingtime, int contextswitching){
    int pcbnum = (schedul+3)->PCBnum;
    int i,j,k,m = 0;
    int agtime = agingtime;
    int exectime = 0;
    int waitingtime = 0;
    int qauntumT = timeqauntum;
    int cwT = contextswitching;
    //Queue* schedulptr = &schedul[3];
    PCB* processptr = NULL;
    processptr = (schedul+3)->headerNode; // waiting타임, agingtime, context switching
    while((schedul+3)->headerNode != NULL){
       // for(i=0;i<pcbnum;i++){
            int remainBT = (schedul+3)->headerNode->burst_time; //타임퀀텀만큼 bursttime을 감소시킬 변수 
            for(j=0;j<(schedul+3)->headerNode->burst_time;j++){
            // --remainBT;
                if(j>=timeqauntum){
                    remainBT = remainBT - j;// BT가 퀀텀을 넘어서면  실행횟수(j)만큼 감소한 BT 저장
                    cwT = cwT+cwT;//context switching 시간 추가 
                    ++waitingtime;
                    break;
                }            
                ++exectime;
            }
            if(remainBT == (schedul+3)->headerNode->burst_time){// 프로세스 BT가 타임퀀텀보다 작은경우 
                if(processptr->link == NULL){ // 제거하려는 노드의 링크가 NULL -> 제일 끝일때 
                    (schedul+3)->headerNode = NULL; //큐의 헤더를 NULL 
                    printf("# context switches = %d\n",cwT);
                    
                }
                processptr->link = processptr;
                (schedul+3)->PCBnum--;
                free(processptr);
                ++terminate_process;
                processptr = (schedul+3)->headerNode;
            }
            else{
                (schedul+3)->headerNode->burst_time = remainBT;
                if(waitingtime >= agtime){
                    (schedul+2)->tailNode = processptr; 
                    break; // 에이징타임 초과에 따른 다른 큐 이동 PQ로 
                }
                else processptr->link = processptr;//다음노드로 
            }
      //  }
    }
    return exectime;
}

//우선순위 2번째 // non-premptive 만 작성
int PQ(Queue *schedul, int preemptive, int agingtime, int contextswitching){
    int i,j,k = 0;
    int exectime = 0;
    int waitingtime = 0;
    int cwT = contextswitching;
    int pcbnum = (schedul+2)->PCBnum;
    PCB* processptr = NULL;
    processptr = (schedul+2)->headerNode;
    int priority[pcbnum];
        for(i=0;i<pcbnum;i++){
            priority[i] = processptr->priority;
            processptr = processptr->link;// 큐 내에 있는 프로세스의 bursttime을 들어온 순서대로 배열에 저장
        }
        for(k=0;k<pcbnum;k++){
            for(j=k+1;j<pcbnum;j++){
                if(priority[k]>priority[j]){
                    int temp = priority[k];// bursttime 배열을 내림차순으로 정렬 
                    priority[k] = priority[j];
                    priority[j] = temp;
                }
            }
        }
        processptr = (schedul+2)->headerNode;//메소드 내에서 선언된 processptr 포인터 변수를 다시 큐 헤더노드로 지정 
        for(int l=0;l<pcbnum;l++){
            while(priority[l] != processptr->priority){
                processptr = processptr->link;
                ++waitingtime;
            } 
                for(int a=0;a<processptr->burst_time;a++) ++exectime; // burst time 이 작은 순서대로 exectime  증가 
                    processptr->link = processptr;//현재 노드(프로세스)를 삭제하기 위해 다음노드 헤드를 이전 노드 링크로 연결 
                    free(processptr);//현재 노드 메모리 해제 
                    ++terminate_process;
                    (schedul+2)->PCBnum--;
                    processptr = (schedul+2)->headerNode;//다시 재지정
                    if(waitingtime >= agingtime) break;
                    cwT = cwT+cwT;
                    printf("# context switches = %d\n",cwT);
        }
        if(waitingtime >= agingtime){//aging time 을 넘기면 다음 우선순위 큐로 이동 
            (schedul+1)->tailNode = processptr; //SJF로
        }
        else return exectime;
}

//우선순위 3번째 // non-premptive 만 작성 
int SJF(Queue *schedul, int preemptive, int agingtime, int contextswitching){
    int pcbnum = (schedul+1)->PCBnum;
    int i,j,k,m = 0;    
    int exectime = 0;
    int waitingtime = 0;
    int cwT = contextswitching;
    int burstime[pcbnum];
    PCB* processptr = NULL;
    processptr = (schedul+1)->headerNode;
    for(i=0;i<pcbnum;i++){
        burstime[i] = processptr->burst_time;
        processptr = processptr->link;// 큐 내에 있는 프로세스의 bursttime을 들어온 순서대로 배열에 저장
    }
    for(k=0;k<pcbnum;k++){
        for(j=k+1;j<pcbnum;j++){
            if(burstime[k]>burstime[j]){
                int temp = burstime[k];// bursttime 배열을 내림차순으로 정렬 
                burstime[k] = burstime[j];
                burstime[j] = temp;
            }
        }
    }
    processptr = (schedul+1)->headerNode;//메소드 내에서 선언된 processptr 포인터 변수를 다시 큐 헤더노드로 지정 
    for(int l=0;l<pcbnum;l++){
        while(burstime[l] != processptr->burst_time){
            processptr = processptr->link;
            ++waitingtime;// 큐를 돌면서 일치하지 않으면 대기시간 증가 
        }
            for(int a=0;a<processptr->burst_time;a++) ++exectime; // burst time 이 작은 순서대로 exectime  증가 
                processptr->link = processptr;//현재 노드(프로세스)를 삭제하기 위해 다음노드 헤드를 이전 노드 링크로 연결 
                free(processptr);//현재 노드 메모리 해제 
                ++terminate_process;
                (schedul+1)->PCBnum--;
                processptr = (schedul+1)->headerNode;//다시 재지정
                if(waitingtime >= agingtime) break;
                cwT = cwT+cwT;
                printf("# context switches = %d\n",cwT);
       }
    if(waitingtime >= agingtime){//aging time 을 넘기면 다음 우선순위 큐로 이동 
         schedul->tailNode = processptr; //FCFS로
    }
    else return exectime;
}


//우선순위 4번째 
int FCFS(Queue *schedul, int agingtime/*,int estimatetime*/){
    int i,j,k =0;
    int exectime = 0;
    int pcbNum = schedul->PCBnum;
    if(pcbNum>0){
        for(i=0;i<pcbNum;i++){//FCFS 큐에 들어온 프로세스의 갯수
            PCB* processptr = NULL;
            processptr = schedul->headerNode;
            k = processptr->burst_time;
            for(j=0;j<k;j++){
                ++exectime;
            }
            schedul->headerNode = schedul->headerNode->link;
            processptr->link = NULL;
            schedul->PCBnum--;
            free(processptr);
            ++terminate_process;
            if(i == pcbNum) schedul->tailNode = NULL;
        }
        return exectime;
    }
    else{
        printf("No process in this queue");
        return 0;
    } 
}



int main(){ //난수로 랜덤하게 발생하기 보다는 매 사이클마다 발생시켜서 큐에 넣는게 어떨까 
    int timecycle, timeslot, cswitht;
    int agingtime[4];
    int alpha_eff[4];
    int initialtime[4];
    int process_exetime[4];
    int sjf_preemption; //dummy
    int pq_preemption; //dummy
    int randomNum;
    int tcount;
    int exectimesum = 0;
    int initialselect = 0;
    
    printf("Enter the time cycles                  : ");
    scanf("%d",&timecycle);
    printf("Enter the value of time slot for RR    : ");
    scanf("%d",&timeslot);
    printf("Enter the context switching time       : ");
    scanf("%d",&cswitht);
    printf("SJF with pre-emption (1-yes/0-no)      :");
    scanf("%d",&sjf_preemption);
    printf("PQ with pre-emption (1-yes/0-no)       : ");
    scanf("%d",&pq_preemption);
    //Alpha-eff
    printf("Enter the alpha co-eff for RR          : ");
    scanf("%d",&alpha_eff[0]);
    printf("Enter the alpha co-eff for SJF         : ");
    scanf("%d",&alpha_eff[1]);
    printf("Enter the alpha co-eff for PQ          : ");
    scanf("%d",&alpha_eff[2]);
    printf("Enter the alpha co-eff for FPFS        : ");
    scanf("%d",&alpha_eff[3]);
    // aging time
    printf("Enter the aging time for RR            : ");
    scanf("%d",&agingtime[0]);
    printf("Enter the aging time for SJF           : ");
    scanf("%d",&agingtime[1]);
    printf("Enter the aging time for PQ            : ");
    scanf("%d",&agingtime[2]);
    printf("Enter the aging time for FIFO          : ");
    scanf("%d",&agingtime[3]);
    //initial estimate time
    printf("Enter the initial estimated time for RR: ");
    scanf("%d",&initialtime[0]);
    printf("Enter the initial estimated time for SJF: ");
    scanf("%d",&initialtime[1]);
    printf("Enter the initial estimated time for PQ: ");
    scanf("%d",&initialtime[2]);
    printf("Enter the initial estimated time for FPFS: ");
    scanf("%d",&initialtime[3]);
    for(int j=0;j<4;j++) printf("agingtime is : %d\n ",agingtime[j]);
    //각 알고리즘 별 에이징 타임과 최초 예측동작시간을 비교해서 
    //에이징 타임이 예측 동작시간보다 작은알고리즘 들 중 우선순위가 높은 알고리즘을 채택하여 프로세스 할당
    /*schedul[0] = FCFS, 
      schedul[1] = SJF, 
      schedul[2] = PQ, 
      schedul[3] = R_R
    */
    Queue* schedul;//idea form stackoverflow 배열로선언하면 오류남 
    schedul = (Queue*)malloc(4*sizeof(Queue));//각 알고리즘 레디큐 메모리 할당 
    memset(schedul,0,(4*sizeof(Queue)));//pcbnum=0 초기화
    int q_selectindex = 0;
    for(int i=0; i<timecycle; i++){//타임사이클
        randomNum = rand()%4;//1 ~ 4 까지 난수발생 
        for(int j=0;j<4;j++){
            if(initialtime[j]>=agingtime[j]){
                initialselect = alpha_eff[q_selectindex];
                for(q_selectindex=j;q_selectindex<4;q_selectindex++){
                    if(alpha_eff[q_selectindex]<initialselect){
                        initialselect = alpha_eff[q_selectindex];
                        break;
                    }
                }
                //generate_process(schedul,randomNum,j);// 프로세스 랜덤생성 함수 
            }
        }
        generate_process(schedul,randomNum,q_selectindex);
        //generate_process(schedul,randomNum,3);
        initialtime[q_selectindex] = initialselect;//?

        if((schedul+3)->PCBnum != 0){
            int exetime = R_R(schedul, timeslot, agingtime[0], cswitht);
            printf("Process appended to RR queue\n");
            printf("Process execution time= %d\n",exetime);
            initialtime[0] = exetime;
            process_exetime[0] = process_exetime[0] + exetime;
        } 
        else if((schedul+2)->PCBnum != 0){
            int exetime = PQ(schedul, 0, agingtime[1], cswitht);
            printf("Process appended to PRQ queue\n");
            printf("Process execution time= %d\n",exetime);
            initialtime[1] = exetime;
            process_exetime[1] = process_exetime[1] + exetime;
        } 
        else if((schedul+1)->PCBnum != 0){
            int exetime = SJF(schedul, 0,agingtime[2], cswitht);
            printf("Process appended to SJF queue\n");
            printf("Process execution time= %d\n",exetime);
            initialtime[2] = exetime;
            process_exetime[2] = process_exetime[2] + exetime;
        } 
        else if((schedul+0)->PCBnum != 0){
            int exetime = FCFS(schedul,agingtime[3]);
            printf("Process appended to FCFS queue\n");
            printf("Process execution time= %d\n",exetime);
            initialtime[3] = exetime;
            process_exetime[3] = process_exetime[3] + exetime;
        } 
        printf("# of jobs in RR = %d\n",(schedul+3)->PCBnum);
        printf("\n");
        printf("# of jobs in SJF = %d\n",(schedul+2)->PCBnum);
        printf("\n");
        printf("# of jobs in PRQ = %d\n",(schedul+1)->PCBnum);
        printf("\n");
        printf("# of jobs in FCFS = %d\n",(schedul+0)->PCBnum);
        printf("\n");///////////////////////////미완성 
        printf("#context switches = %d\n");
        printf("\n");
        printf("#Total process generated = %d\n", generation_times);
        printf("\n");
        printf("#Total process completed = %d\n", terminate_process);

        //R_R(schedul,timeslot,100, 1);
        //printf("%d\n",schedul->PCBnum);
        printf("-----------------------------------------------\n");
    }
    printf("SUMMARY\n");

    printf("AVERAGE WAITING TIME = %d\n");
    printf("AVERAGE TURNAROUND TIME = %d \n");
    printf("CPU UTILIZATION = %d\n");
    printf("MAXIMUM TURNARAUND TIME = %d\n");
    printf("MAXIMUM WAIT TIME = %d\n");
    printf("CPU THROUGHPUT = %d\n");


    printf("-----------------------------------------------\n");

    
}