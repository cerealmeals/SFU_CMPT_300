#include "list.h"
#define RUNNING 0
#define READY 1
#define BLOCKED 2

// Structures
typeof struct PCB pcb
struct PCB{
    int ID;
    int priority;
    int state;
    char msg[40];
};

struct semaphore{
    int mutex;
    List* blocked_on_this_semaphore;
};

// Processes
PCB* initProcess;
PCB* runningProcess;

// 5 Queues, 3 ready queues with priority, 1 send and 1 receive
List* highPriorityQueue;
List* normPriorityQueue;
List* lowPriorityQueue;
List* sendQueue;
List* receiveQueue;

// Functions
int forkProcess();
int sendProcess(int pID, char* msg);
void receiveProcess();
int replyProcess(int pID, char* msg);
void procInfo(int pID);
void totalInfo();
