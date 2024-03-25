#include "Structures_functions.h"
#include <stdio.h>
#include <stdlib.h>


void Create_Proccess(int id, int prio,  PCB** currently_running, List* queue){
    PCB* proccess = (PCB*)malloc(sizeof(PCB));
    printf("withing create, address memory was allocated to: %d\n", proccess);
    proccess->ID = id;
    proccess->priority = prio;
    proccess->state = READY;

    if(((*currently_running)->ID) == 0){
        proccess->state = RUNNING;
        *currently_running = proccess;
        printf("Proccess with ID: %d replaced the init proccess and is now running\n", id);
        
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


int Kill_Proccess(int id, PCB* init, PCB** currently_running, List*high, List*norm, List*low){
    // search the Queue given
    List_first(high);
    List_first(norm);
    List_first(low);
    PCB* high_queue = List_search(high, compare, (void*)&id);
    PCB* norm_queue = List_search(norm, compare, (void*)&id);
    PCB* low_queue = List_search(low, compare, (void*)&id);

    if(high_queue == NULL && norm_queue == NULL && low_queue == NULL){
        //check currently running process
        if((((*currently_running)->ID) == id)&& 0 !=id){
            // free the running process and get a new running process
            free(*currently_running);
            printf("The process with ID %d was the running process\nit was killed and the new process is now running\n", id);
            fill_in_running_with_next_process(init, currently_running, high, norm, low);
            return 1;
        }
        else if(id == 0){ // 0 is the init process' ID
            if((0 == List_count(high)) && (0 == List_count(norm)) && (0 == List_count(low)) && (((*currently_running)->ID) == 0)){
                return 0;
            }
            else{
                printf("you tried killing the init process but there is other stuff still on other queue(s)\n");
                return 1;
            }
        }
        else{
            //user error no process with the ID
            printf("There is no process with ID %d\nMaybe it is blocked by a semaphor or send/receive queue\n", id);
            return 1;
        }
    }
    else if(high_queue != NULL){
        List_remove(high);
        free(high_queue);
        printf("The process with ID %d was Killed\nIt was in the high priority queue\n", id);
        return 1;

    }
    else if(norm_queue != NULL){
        List_remove(norm);
        free(norm_queue);
        printf("The process with ID %d was Killed\nIt was in the normal priority queue\n", id);
        return 1;
    }
    else{
        List_remove(low);
        free(low_queue);
        printf("The process with ID %d was Killed\nIt was in the low priority queue\n", id);
        return 1;
    }
}

bool compare(void* arg1, void* arg2){
    PCB* i = (PCB*)arg1;
    int j = *(int*)arg2;

    if(i->ID==j){
        return true;
    }
    else{
        return false;
    }
}

int Exit_Running_Proccess(PCB* init, PCB** currently_running, List*high, List*norm, List*low){
    if(((*currently_running)->ID) == 0){
        // end the program because the only process is the init process
        printf("Within Exit, you killed the init process\n");
        return 0;
    }
    else{
        printf("You killed the currently running process it had ID %d\n", ((*currently_running)->ID));
        printf("withing Exit, address memory trying to kill: %d\n", *currently_running);
        free(*currently_running);
        fill_in_running_with_next_process(init, currently_running, high, norm, low);
        return 1;
    }
}

void fill_in_running_with_next_process(PCB* init, PCB** currently_running, List*high, List*norm, List*low){
    /*  My theory here to prevent starvation is that every time you go through the entire high priority list
        a single normal priority process should get a turn. The same thing happens for
        normal top low priority.

        So if there is 2 process in the high prio, 3 in normal prio and 1 in low prio, and assuming no process is killed
        the low prio would have to wait for for (2*(3+1))+3 time quantums to have a turn.
        the patern would be "high1, high2, norm1, high1, high2, norm2, high1, high2, norm3, high1, high2, low1, high1, high2, norm1"
                                                                                        the patern repeat here /\
    */
    printf("Within Fill_in_running\n");
    // start with assuming you should use high priority
    static bool high_turn = 1, norm_turn = 1, low_turn = 1;
    static int track_high = 0, high_cap = 0, track_norm = 0, norm_cap = 0;
    // check who's turn it is, and if there is anything in that queue
    if(high_turn && (0 != List_count(high))){
        *currently_running = List_first(high);
        List_remove(high);
        (*currently_running)->state = RUNNING;
        printf("The new Running process had ID %d\nIt was taken for the high priority queue\n", ((*currently_running)->ID));
        track_high++;                       // count how many times you have taken the high priority queue
        if(track_high > high_cap){          // if you have gone through the entire list its normal priority turn
            track_high = 0;                 //reset the tracking
            high_cap = List_count(high);    // count the amount of high prio currently
            high_turn = 0;                  // given up your turn
            if(track_norm > norm_cap){      // check how many time normal has run
                track_norm = 0;             // if normal has run enough reset is tracking
                norm_cap = List_count(norm);//set a new cap
                norm_turn = 0;              // given of normals turn
            }else{
                norm_turn = 1;              // set normal's turn
            }
        }
    }
    else if(norm_turn && (0 != List_count(norm))){
        *currently_running = List_first(norm);
        List_remove(norm);
        printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", ((*currently_running)->ID));
        (*currently_running)->state = RUNNING;
        track_norm++;
        high_turn = 1;
    }
    else if( low_turn && (0 != List_count(low))) {
        *currently_running = List_first(low);
        List_remove(low);
        (*currently_running)->state = RUNNING;
        printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", ((*currently_running)->ID));
        high_turn = 1;
    }
    else{
        //weird stuff can happen if stuff is killed so check all the lists again
        
        //printf("Within fill_in_next,\nnumber of items on high queue %d\nnumber of items on normal queue %d\nnumber of items on low queue %d\n", List_count(high),List_count(norm), List_count(low));
        if(0 != List_count(high)){
            *currently_running = List_first(high);
            List_remove(high);
            (*currently_running)->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the high priority queue\n", ((*currently_running)->ID));
        }
        else if(0 != List_count(norm)){
            *currently_running = List_first(norm);
            List_remove(norm);
            (*currently_running)->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", ((*currently_running)->ID));
        }
        else if(0 != List_count(low)){
            *currently_running = List_first(low);
            List_remove(low);
            (*currently_running)->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", ((*currently_running)->ID));
        }
        else{ // then if all those fail there is nothing to do so set the init process to running
            *currently_running = init;
            (*currently_running)->state = RUNNING;
            printf("The new Running proces is the init process because there are no other process\n");
        }
    }

}

void Create_New_sem(Semaphore* semaphores, int sem_id, int value_for_mutex){
  
    if(semaphores[sem_id].initialized = 0){

        semaphores[sem_id].initialized = 1;
        semaphores[sem_id].mutex = value_for_mutex;
        semaphores[sem_id].blocked_on_this_semaphore = List_create();
        printf("You create a semaphore with ID %d\n", sem_id);
        
    }
    else{
        printf("You inputed %d, that semaphore is already initialized\n", sem_id);
    }

}



void P_sem(Semaphore* semaphores, int sem_id, PCB* init, PCB** currently_running, List*high, List*norm, List*low){
    if((*currently_running)->ID == 0){
        printf("This is the init process it can not call P_sem\n");
    }
    else if(semaphores[sem_id].initialized != 0){
        printf("called P_sem on process with ID %d\n", (*currently_running)->ID);
        semaphores[sem_id].mutex--;
        if(semaphores[sem_id].mutex < 0){
            (*currently_running)->state = BLOCKED;
            List_prepend(semaphores[sem_id].blocked_on_this_semaphore, *currently_running);
            fill_in_running_with_next_process(init, currently_running, high, norm, low);
            printf("Said process was blocked\n");
        }
        else{
            printf("Said process was not blocked\n");
        }
    }
    else{
        printf("P_sem - The semaphore with ID %d is not initialized\n", sem_id);
    }
}

void V_sem(Semaphore* semaphores, int sem_id, List*high, List*norm, List*low){
    if(semaphores[sem_id] != 0){
        semaphores[sem_id].mutex--;
        if(semaphores[sem_id].mutex <= 0){
            PCB* process = List_trim(semaphores[sem_id].blocked_on_this_semaphore);
            process->state = READY;
            if(process->priority == 0){
                List_append(high, &process);
                printf("The process with ID %D was unblocked on put on the high priority queue\n", process->ID);
            }
            else if(process->priority == 1){
                List_append(norm, &process);
                printf("The process with ID %D was unblocked on put on the normal priority queue\n", process->ID);
            }
            else{
                List_append(low, &process);
                printf("The process with ID %D was unblocked on put on the low priority queue\n", process->ID);
            }
        }
    }
    else{
        printf("V_sem - The semaphore with ID %d is not initialized\n", sem_id);
    }
}