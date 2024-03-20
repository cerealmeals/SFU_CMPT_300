#include "Structures_functions.h"
#include <stdio.h>

void Create_Proccess(int id, int prio, struct PCB* currently_running, List* queue){
    struct PCB* proccess = (struct PCB*)malloc(sizeof(PCB));

    proccess->ID = id;
    proccess->priority = prio;
    proccess->state = READY;

    if(currently_running.ID = 0){
        proccess->state = RUNNING;
        currently_running->ID = id;
        currently_running->priority = prio;
        printf("Proccess with ID: %d replace the init proccess and is now running\n", id);
    }
    else{
        int check = List_append(queue, proccess);
        if(check == 0){
            printf("Proccess with ID: %d was placed on the end the ready queue with priority %d\n", id, prio);
        }
        else{
            printf("Was not space to add the proccess with ID %d to the queue, remove other proccess before continuing", id);
            free(proccess);
        }
    }
}


void Kill_Proccess(int id, struct PCB* currently_running, List*high, List*norm, List*low){
    // search the Queue given
    List_first(high);
    List_first(norm);
    List_first(low);
    struct PCB* high_queue = List_search(high, compare, (void*)&id);
    struct PCB* norm_queue = List_search(norm, compare, (void*)&id);
    struct PCB* low_queue = List_search(low, compare, (void*)&id);

    if(high_queue == NULL && norm_queue == NULL && low_queue == NULL){
        //check currently running process
        if(currently_running->ID == id){
            // free the running process and get a new running process
            free(currently_running;)
            fill_in_running_with_next_process();
            printf("The process with ID %d was the running process\n
            it was killed and the new process is now running\n", id);
        }
        else{
            //user error no process with the ID
            printf("There is no process with ID %d\n
            Maybe it is blocked by a semaphor or send queue", id);
        }
    }
    else if(high_queue != NULL){
        List_remove(high);
        free(high_queue);
        printf("The process with ID %d was Killed\n
            It was in the high priority queue\n", id);

    }
    else if(norm_queue != NULL){
        List_remove(norm);
        free(norm_queue);
        printf("The process with ID %d was Killed\n
            It was in the normal priority queue\n", id);
    }
    else{
        List_remove(low);
        free(low_queue);
        printf("The process with ID %d was Killed\n
            It was in the low priority queue\n", id);
    }
}

bool compare(void* arg1, void* arg2){
    struct PCB* i = (struct PCB*)arg1;
    int j = *(int*)arg2;

    if(i->ID==j){
        return true;
    }
    else{
        return false;
    }
}

int Exit_Running_Proccess(struct PCB* currently_running, List*high, List*norm, List*low){
    if(currently_running->ID == 0){
        // end the program because the only process is the init process
        return 0;
    }
    else{
        free(currently_running);

    }
}

void fill_in_running_with_next_process(){

}