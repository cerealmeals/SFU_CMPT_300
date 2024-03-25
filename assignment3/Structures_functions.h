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
};

typedef struct Semaphore_s Semaphore;
struct Semaphore_s{
    int mutex;
    List* blocked_on_this_semaphore;
    bool initialized;
};


void Create_Proccess(int id, int prio, PCB** currently_running, List*queue);

int Kill_Proccess(int id, PCB* init, PCB** currently_running, List*high, List*norm, List*low);

int Exit_Running_Proccess(PCB* init, PCB** currently_running, List*high, List*norm, List*low);

bool compare(void* arg1, void* arg2);

void fill_in_running_with_next_process(PCB* init, PCB** currently_running, List*high, List*norm, List*low);

void Create_New_sem(Semaphore* semaphores, int sem_id, int value_for_mutex);

void P_sem(Semaphore* semaphores, int sem_id, PCB* init, PCB** currently_running, List*high, List*norm, List*low);

void V_sem(Semaphore* semaphores, int sem_id, List*high, List*norm, List*low);