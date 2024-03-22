#include "Structures_functions.h"
#include <stdio.h>
#include <stdlib.h>

void Create_Proccess(int id, int prio,  PCB* currently_running, List* queue){
    PCB* proccess = (PCB*)malloc(sizeof(PCB));

    proccess->ID = id;
    proccess->priority = prio;
    proccess->state = READY;

    if(currently_running->ID == 0){
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


int Kill_Proccess(int id, PCB init, PCB* currently_running, List*high, List*norm, List*low){
    // search the Queue given
    List_first(high);
    List_first(norm);
    List_first(low);
    PCB* high_queue = List_search(high, compare, (void*)&id);
    PCB* norm_queue = List_search(norm, compare, (void*)&id);
    PCB* low_queue = List_search(low, compare, (void*)&id);

    if(high_queue == NULL && norm_queue == NULL && low_queue == NULL){
        //check currently running process
        if(currently_running->ID == id){
            // free the running process and get a new running process
            free(currently_running);
            fill_in_running_with_next_process(init, currently_running, high, norm, low);
            printf("The process with ID %d was the running process\nit was killed and the new process is now running\n", id);
            return 1;
        }
        else if(id == 0){ // 0 is the init process' ID
            if((0 == List_count(high)) && (0 == List_count(norm)) && (0 == List_count(low))){
                return 0;
            }
            else{
                printf("you tried killing the init process but there is other stuff still on other queues\n");
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

int Exit_Running_Proccess(PCB init, PCB* currently_running, List*high, List*norm, List*low){
    if(currently_running->ID == 0){
        // end the program because the only process is the init process
        return 0;
    }
    else{
        free(currently_running);
        fill_in_running_with_next_process(init, currently_running, high, norm, low);
        return 1;
    }
}

void fill_in_running_with_next_process(PCB init, PCB* currently_running, List*high, List*norm, List*low){
    /*  My theory here to prevent starvation is that every time you go through the entire high priority list
        a single normal priority process should get a turn. The same thing happens for
        normal top low priority.

        So if there is 2 process in the high prio, 3 in normal prio and 1 in low prio, and assuming no process is killed
        the low prio would have to wait for for (2*(3+1))+3 time quantums to have a turn.
        the patern would be "high1, high2, norm1, high1, high2, norm2, high1, high2, norm3, high1, high2, low1, high1, high2, norm1"
                                                                                        the patern repeat here /\
    */

    // start with assuming you should use high priority
    static bool high_turn = 1, norm_turn = 1, low_turn = 1;
    static int track_high = 0, high_cap = 0, track_norm = 0, norm_cap = 0;
    // check who's turn it is, and if there is anything in that queue
    if(high_turn && (0 != List_count(high))){
        currently_running = List_first(high);
        List_remove(high);
        currently_running->state = RUNNING;

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
        currently_running = List_first(norm);
        List_remove(norm);
        currently_running->state = RUNNING;
        track_norm++;
        high_turn = 1;
    }
    else if( low_turn && (0 != List_count(low))) {
        currently_running = List_first(low);
        List_remove(low);
        currently_running->state = RUNNING;
        high_turn = 1;
    }
    else{
        //weird stuff can happen if stuff is killed so check all the list again
        if(0 != List_count(high)){
            currently_running = List_first(high);
            List_remove(high);
            currently_running->state = RUNNING;
        }
        else if(0 != List_count(norm)){
            currently_running = List_first(norm);
            List_remove(norm);
            currently_running->state = RUNNING;
        }
        else if(0 != List_count(low)){
            currently_running = List_first(low);
            List_remove(low);
            currently_running->state = RUNNING;
        }

        // then if all those fail there is nothing to do so set the init process to running
        currently_running = &init;
        currently_running->state = RUNNING;
    }

}

void Create_New_sem(Semaphore* semaphores, int sem_id, int count){
  
    if(sem_id > count){
        //error checking
        if(sem_id > 4){
            printf("You inputed %d, please give input a number between 0 and 4\n", sem_id);
            return;
        }

        // malloc new space for the list of blocked process'

        // copy old space into
    }
    else{
        printf("You inputed %d, that semaphore is already initialized\n", sem_id);
        return;
    }

}