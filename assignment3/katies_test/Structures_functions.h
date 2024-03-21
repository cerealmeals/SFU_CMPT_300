#include "list.h"
#define RUNNING 0
#define READY 1
#define BLOCKED 2

// Structures
typedef struct pcb PCB;
struct pcb{
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
extern PCB* initProcess;
extern PCB* runningProcess;

// 5 Queues, 3 ready queues with priority, 1 send and 1 receive
extern List* highPriorityQueue;
extern List* normPriorityQueue;
extern List* lowPriorityQueue;
extern List* sendQueue;
extern List* receiveQueue;

// Functions
int forkProcess();
void* nextRunningProcess();
int sendProcess(int pID, char* msg);
void receiveProcess();
int replyProcess(int pID, char* msg);
void procInfo(int pID);
void totalInfo();
