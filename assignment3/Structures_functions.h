#include "list.h"
#define RUNNING 0
#define READY 1
#define BLOCKED 2

typedef struct PCB_s PCB;
struct PCB_s{
    int ID;
    int priority;
    int state;
    char msg[40];
    int sending_ID;
};

typedef struct Semaphore_s Semaphore;
struct Semaphore_s{
    int mutex;
    List* blocked_on_this_semaphore;
    bool initialized;
};


// Sam's functions
void Create_Process(int prio, PCB** currently_running, List*queue, int count);

int Kill_Process(int id, PCB* init, PCB** currently_running, List*high, List*norm, List*low, List*send, List*receive, Semaphore* semaphores);

int Exit_Running_Process(PCB* init, PCB** currently_running, List*high, List*norm, List*low, List*send, List*receive, Semaphore* semaphores);

bool compare(void* arg1, void* arg2);

bool compare_send_ID(void* arg1, void* arg2);

bool are_send_receive_and_semaphore_queues_are_empty(List*send, List*receive, Semaphore* semaphores);

void fill_in_running_with_next_process(PCB* init, PCB** currently_running, List*high, List*norm, List*low);

void Create_New_sem(Semaphore* semaphores, int sem_id, int value_for_mutex);

void P_sem(Semaphore* semaphores, int sem_id, PCB* init, PCB** currently_running, List*high, List*norm, List*low);

void V_sem(Semaphore* semaphores, int sem_id, PCB** currently_running, List*high, List*norm, List*low);

//Katie's functions
int forkProcess(PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, int count);

int sendProcess(int pID, char* msg, PCB* initProcess,PCB** runningProcess,List* sendQueue, List* receiveQueue, List* highPriorityQueue, List* normPriorityQueue,List* lowPriorityQueue, Semaphore* semaphores);

int receiveProcess(PCB** runningProcess, List*receiveQueue, List* sendQueue);

int replyProcess(int pID, char* msg, List* sendQueue,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue);

int procInfo(int pID, PCB* initProcess, PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue, Semaphore* semaphores);

void totalInfo(PCB* initProcess, PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue, Semaphore* semaphores);
