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


void Create_Proccess(int id, int prio, struct PCB* currently_running, List*queue);

void Kill_Proccess(int id, struct PCB* currently_running, List*high, List*norm, List*low);

void Exit_Running_Proccess(struct PCB* currently_running, List*high, List*norm, List*low);

bool compare(void* arg1, void* arg2);

void fill_in_running_with_next_process();