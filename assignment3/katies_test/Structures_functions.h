#include "list.h"
#define RUNNING 0
#define READY 1
#define BLOCKED 2

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

int forkProcess();
int sendProcess(int pID, char* msg);
void receiveProcess();
int replyProcess(int pID, char* msg);
void procInfo(int pID);
void totalInfo();
