#include "Structures_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Create_Process(int prio,  PCB** currently_running, List* prio_queue, int* id){
    PCB* process = (PCB*)malloc(sizeof(PCB));
    
    process->ID = *id;
    process->priority = prio;
    process->state = READY;
    process->sending_ID = 0;
    (*id)++;

    if(((*currently_running)->ID) == 0){
        process->state = RUNNING;
        *currently_running = process;
        printf("Process with ID: %d replaced the init process and is now running\n", process->ID);
        
    }
    else{
        int check = List_append(prio_queue, process);
        if(check == 0){
            printf("Process with ID: %d was placed on the end the ready queue with priority %d\n", process->ID, prio);
        }
        else{
            printf("Was not space to add the process with ID %d to the queue, remove other process before continuing", process->ID);
            free(process);
        }
    }
}

// Copy the currently running process and put it on the ready
// Q corresponding to the original process' priority.
// Attempting to Fork the "init" process (see below) should fail.
// Return pID on success, -1 on failure 
int forkProcess(PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, int* id)
{
    // if "init" process, return -1
    if ((*runningProcess)->ID == 0)
    {
        printf("**The running process is the initial process.**\n");
        return -1;
    }

    // copy current process
    PCB* copy_process = NULL;
    copy_process = (PCB*)malloc(sizeof(*runningProcess));
    copy_process->ID = *id;
    copy_process->priority = (*runningProcess)->priority;
    copy_process->state = (*runningProcess)->state;
    copy_process->sending_ID = (*runningProcess)->sending_ID;
    strcpy(copy_process->msg, ((*runningProcess)->msg));
    (*id)++;

    // put it into the ready queue
    if (copy_process->priority == 0)
    {
        List_append(highPriorityQueue, (void*)copy_process);
    }
    else if (copy_process->priority == 1)
    {
        List_append(normPriorityQueue, (void*)copy_process);
    }
    else if(copy_process->priority == 2)
    {
        List_append(lowPriorityQueue, (void*)copy_process);
    }


    printf("**within fork, the forked process priority is %d.**\n", copy_process->priority);
    return copy_process->ID;
}


int Kill_Process(int id, PCB* init, PCB** currently_running, List*high, List*norm, List*low, List*send, List*receive, Semaphore* semaphores){
    // search the Queue given
    List_first(high);
    List_first(norm);
    List_first(low);
    List_first(send);
    List_first(receive);
    PCB* process_from_high_queue = List_search(high, compare, (void*)&id);
    PCB* process_from_norm_queue = List_search(norm, compare, (void*)&id);
    PCB* process_from_low_queue = List_search(low, compare, (void*)&id);
    PCB* process_from_send_queue = List_search(send, compare, (void*)&id);
    PCB* process_from_receive_queue = List_search(receive, compare, (void*)&id);

    if(process_from_high_queue == NULL && process_from_norm_queue == NULL && process_from_low_queue == NULL && process_from_receive_queue == NULL && process_from_send_queue == NULL){
        //check currently running process
        if((((*currently_running)->ID) == id)&& 0 !=id){
            // free the running process and get a new running process
            free(*currently_running);
            printf("The process with ID %d was the running process\nit was killed and the new process is now running\n", id);
            fill_in_running_with_next_process(init, currently_running, high, norm, low);
            return 1;
        }
        else if(id == 0){ // 0 is the init process' ID
            if((0 == List_count(high)) && (0 == List_count(norm)) && (0 == List_count(low)) && (((*currently_running)->ID) == 0) && are_send_receive_and_semaphore_queues_are_empty(send, receive, semaphores)){
                return 0;
            }
            else{
                printf("you tried killing the init process but there is other stuff still on other queue(s)\n");
                return 1;
            }
        }
        else{
            //user error no process with the ID
            printf("There is no process with ID %d\nMaybe it is blocked by a semaphor, because you can't kill those\n", id);
            return 1;
        }
    }
    else if(process_from_high_queue != NULL){
        List_remove(high);
        free(process_from_high_queue);
        printf("The process with ID %d was Killed\nIt was in the high priority queue\n", id);
        return 1;

    }
    else if(process_from_norm_queue != NULL){
        List_remove(norm);
        free(process_from_norm_queue);
        printf("The process with ID %d was Killed\nIt was in the normal priority queue\n", id);
        return 1;
    }
    else if(process_from_low_queue != NULL){
        List_remove(low);
        free(process_from_low_queue);
        printf("The process with ID %d was Killed\nIt was in the low priority queue\n", id);
        return 1;
    }
    else if(process_from_send_queue != NULL){
        List_remove(send);
        free(process_from_send_queue);
        printf("The process with ID %d was Killed\nIt was in the send queue\n", id);
        return 1;
    }
    else{
        List_remove(receive);
        free(process_from_receive_queue);
        printf("The process with ID %d was Killed\nIt was in the receive queue\n", id);
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


int Exit_Running_Process(PCB* init, PCB** currently_running, List*high, List*norm, List*low, List*send, List*receive, Semaphore* semaphores){
    if(((*currently_running)->ID) == 0){
        if(are_send_receive_and_semaphore_queues_are_empty(send, receive, semaphores)){ 
            // end the program because the only process is the init process
            printf("Within Exit, you killed the init process\n");
            return 0;
        }
        else{   
            printf("You trying killing the init process while there are process blocked\nThese process could be on send, recerve or semaphore queues\n");
            return 1;
        }
    }
    else{
        printf("You killed the currently running process it had ID %d\n", ((*currently_running)->ID));
        free(*currently_running);
        fill_in_running_with_next_process(init, currently_running, high, norm, low);
        return 1;
    }
}

bool are_send_receive_and_semaphore_queues_are_empty(List*send, List*receive, Semaphore* semaphores){
    if(List_count(send) != 0){
        return 0;
    }
    if(List_count(receive) != 0){
        return 0;
    }
    for(int i = 0; i < 5; i++){
        if(semaphores[i].initialized == 1){
            if(List_count(semaphores[i].blocked_on_this_semaphore) != 0){
                return 0;
            }
        }
    }
    return 1;
}

void fill_in_running_with_next_process(PCB* init, PCB** currently_running, List*high, List*norm, List*low){
    /*  My theory here to prevent starvation is that every time you go through the entire high priority list
        a single normal priority process should get a turn. The same thing happens for
        normal top low priority.

        So if there is 2 process in the high prio, 3 in normal prio and 1 in low prio, and assuming no process is killed
        the low prio would have to wait for for (2*(3+1))+3 time quantums to have a turn.
        the patern would be "high1, high2, norm1, high1, high2, norm2, high1, high2, norm3, high1, high2, low1, high1, high2, norm1"
                                                                                        the patern repeats here /\
    */

    // start with assuming you should use high priority
    static bool high_turn = 1, norm_turn = 1, low_turn = 1;
    static int track_high = 0, high_cap = 0, track_norm = 0, norm_cap = 0;
    high_cap = List_count(high);
    norm_cap = List_count(norm);

    // check who's turn it is, and if there is anything in that queue
    if(high_turn && (0 != List_count(high))){
        *currently_running = List_first(high);
        List_remove(high);
        (*currently_running)->state = RUNNING;
        printf("The new Running process had ID %d\nIt was taken for the high priority queue\n", ((*currently_running)->ID));
        printf("The message within that process is, %s\n", ((*currently_running)->msg));
        track_high++;                       // count how many times you have taken the high priority queue
        if(track_high >= high_cap){          // if you have gone through the entire list its normal priority turn
            track_high = 0;                 // reset the tracking
            high_cap = List_count(high);    // count the amount in high prio currently
            high_turn = 0;                  // given up high's turn
            if(track_norm >= norm_cap){      // check how many time normal has run
                track_norm = 0;             // if normal has run enough reset is tracking
                norm_cap = List_count(norm);// count the amount in normal prio currently
                norm_turn = 0;              // given up normals turn
            }else{
                norm_turn = 1;              // set normal's turn
            }
        }
    }
    else if(norm_turn && (0 != List_count(norm))){
        *currently_running = List_first(norm);
        List_remove(norm);
        printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", ((*currently_running)->ID));
        printf("The message within that process is, %s\n", ((*currently_running)->msg));
        (*currently_running)->state = RUNNING;
        track_norm++;
        high_turn = 1;
    }
    else if(low_turn && (0 != List_count(low))) {
        *currently_running = List_first(low);
        List_remove(low);
        (*currently_running)->state = RUNNING;
        printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", ((*currently_running)->ID));
        printf("The message within that process is, %s\n", ((*currently_running)->msg));
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
            printf("The message within that process is, %s\n", ((*currently_running)->msg));
        }

        else if(0 != List_count(norm)){
            *currently_running = List_first(norm);
            List_remove(norm);
            (*currently_running)->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", ((*currently_running)->ID));
            printf("The message within that process is, %s\n", ((*currently_running)->msg));
        }

        else if(0 != List_count(low)){
            *currently_running = List_first(low);
            List_remove(low);
            (*currently_running)->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", ((*currently_running)->ID));
            printf("The message within that process is, %s\n", ((*currently_running)->msg));
        }

        else{ // then if all those fail there is nothing to do so set the init process to running
            *currently_running = init;
            (*currently_running)->state = RUNNING;
            printf("The new Running proces is the init process because there are no other process\n");
        }
    }

}

// send a message to another process - block until reply
// report scheduling information,  and reply source and text (once
// reply arrives)
// Return 0 on success, -1 on failure, 1 if the init process was the calling process
int sendProcess(int pID, char* msg, PCB* initProcess, PCB** runningProcess ,List* sendQueue, List* receiveQueue, List* highPriorityQueue, List* normPriorityQueue,List* lowPriorityQueue, Semaphore* semaphores)
{
    // printf("The id of running process is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);

    // make sure not using the init process
    if((*runningProcess)->ID != 0)
    {
        List_first(receiveQueue);
        PCB* process_from_receive_queue = List_search(receiveQueue, compare, (void*)&pID);
        strcpy((*runningProcess)->msg, msg);

        if(process_from_receive_queue != NULL){
            // found on the receive queue
            // print message into said process and take off receive queue
            // then block yourself waiting for reply

            strcpy(process_from_receive_queue->msg, msg);
            List_remove(receiveQueue);
            if(process_from_receive_queue->priority == 0){
                List_append(highPriorityQueue, process_from_receive_queue);
            }
            else if(process_from_receive_queue->priority == 1){
                List_append(normPriorityQueue, process_from_receive_queue);
            }
            else{
                List_append(lowPriorityQueue, process_from_receive_queue);
            }
            (*runningProcess)->state = BLOCKED;
            List_append(sendQueue, *runningProcess);
            printf("The message to be sent is: %s \n", process_from_receive_queue->msg);
            printf("The id of receiver process is: %d, and it was place on the queue with priority: %d.\n", process_from_receive_queue->ID, process_from_receive_queue->priority);

            return 0;
        }
        else{
            // check all other queue to make sure the process exists
            bool Found_ID = false;
            List_first(highPriorityQueue);
            PCB* process_from_queue = List_search(highPriorityQueue, compare, (void*)&pID);

            if(process_from_queue != NULL){ // check high prio queue
                Found_ID = true;
            }
            if(!Found_ID){  // check normal prio queue
                List_first(normPriorityQueue);
                process_from_queue = List_search(normPriorityQueue, compare, (void*)&pID);
                if(process_from_queue != NULL){ 
                    Found_ID = true;
                }
            }
            if(!Found_ID){  // check low prio queue
                List_first(lowPriorityQueue);
                process_from_queue = List_search(lowPriorityQueue, compare, (void*)&pID);
                if(process_from_queue != NULL){
                    Found_ID = true;
                }
            }
            if(!Found_ID){  // check send queue
                List_first(sendQueue);
                process_from_queue = List_search(sendQueue, compare, (void*)&pID);
                if(process_from_queue != NULL){
                    Found_ID = true;
                }
            }
            for(int i = 0; (i < 5) && (!Found_ID); i++){ // check all semaphore queues

                if(semaphores[i].initialized != 0){
                    List_first(semaphores[i].blocked_on_this_semaphore);
                    process_from_queue = List_search(semaphores[i].blocked_on_this_semaphore, compare, (void*)&pID);
                    if(process_from_queue != NULL){
                        Found_ID = true;
                    }
                }  
            }

            // Did you find the process?
            if(Found_ID){
                // block yourself and wait for the process your sending to, to call receive
                (*runningProcess)->state = BLOCKED;
                (*runningProcess)->sending_ID = pID;
                List_append(sendQueue, *runningProcess);
                printf("The message to be sent is: %s \n", msg);
                printf("The process wasn't on the receiving queue so the running process was blocked\n");
                return 0;
            }
            else{
                return -1;
            }
        }
    }
    else{
        return 1;
    }

}


// receive a message - block until one arrives
// report: scheduling information and (once msg is received) the
// message text and source of message
int receiveProcess(PCB** runningProcess, List*receiveQueue, List* sendQueue)
{   

    if((*runningProcess)->ID != 0)
    {
        // search the send queue for a process trying to send a message to your ID
        List_first(sendQueue);
        PCB* process_from_send_queue = List_search(sendQueue, compare_send_ID, (void*)&((*runningProcess)->ID));

        if(process_from_send_queue == NULL){
            // did not find a process that has already sent
            // block your self so you can wait until one sends to you
            printf("Did not find a process on the send queue for sending to the ID %d\nBlocking currently running process until it gets sent a message\n", ((*runningProcess)->ID));
            (*runningProcess)->state = BLOCKED;
            List_append(receiveQueue, *runningProcess);
            return -1;
        }
        else{
            // Found a process that has sent to you
            // Print it's message
            // Contiune executing

            printf("Found process with ID %d sending the message, %s", process_from_send_queue->ID, process_from_send_queue->msg);
            return 0;
        }   
    }
    else{
        printf("The running process is the init process it cannot receive since it can not be blocked");
        return 0;
    }
}

bool compare_send_ID(void* arg1, void* arg2){
    PCB* i = (PCB*)arg1;
    int j = *(int*)arg2;

    if(i->sending_ID==j){
        return true;
    }
    else{
        return false;
    }
}

// unblocks sender and delivers reply 
// Return 0 on success, -1 on failure 
int replyProcess(int pID, char* msg, List* sendQueue ,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue)
{
    // find sender
    PCB* sender_process;
    PCB* current = List_first(sendQueue);
    
    bool ifFound = false;

    while (current != NULL)
    {
        if (current->ID != pID)
        {
            current = List_next(sendQueue);
        }
        else
        {
            // unblock it
            sender_process = current;
            
            List_remove(sendQueue);
            
            current = NULL;
            ifFound = true;
            
        }
    }

    if (ifFound)
    {
        // put it in the ready queue
        
        sender_process->state = READY;
        if (sender_process->priority == 0)
        {
            List_append(highPriorityQueue, sender_process);
        }
        else if (sender_process->priority == 1)
        {
            List_append(normPriorityQueue, sender_process);
        }
        else if (sender_process->priority == 2)
        {
            List_append(lowPriorityQueue, sender_process);
        }

        strcpy(sender_process->msg, msg);
        printf("The ID of process it replied to is: %d\n", sender_process->ID);    
        printf("The message replied is: %s\n", sender_process->msg);
        return 0;
    }

    return -1;
}

void Create_New_sem(Semaphore* semaphores, int sem_id, int value_for_mutex){
  
    if(semaphores[sem_id].initialized == 0){

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
        printf("P_sem was called on process with ID %d\n", (*currently_running)->ID);
        semaphores[sem_id].mutex--;
        if(semaphores[sem_id].mutex < 0){
            (*currently_running)->state = BLOCKED;
            List_prepend(semaphores[sem_id].blocked_on_this_semaphore, *currently_running);
            printf("Said process was blocked\n");
            fill_in_running_with_next_process(init, currently_running, high, norm, low);
        }
        else{
            printf("Said process was not blocked\n");
        }
    }
    else{
        printf("P_sem - The semaphore with ID %d is not initialized\n", sem_id);
    }
}

void V_sem(Semaphore* semaphores, int sem_id, PCB** currently_running, List*high, List*norm, List*low){
    if(semaphores[sem_id].initialized != 0){
        semaphores[sem_id].mutex++;
        if(semaphores[sem_id].mutex <= 0){
            printf("V_sem was called on process with ID %d\n", (*currently_running)->ID);
            PCB* process = List_trim(semaphores[sem_id].blocked_on_this_semaphore);
            if((*currently_running)->ID == 0){
                process->state = RUNNING;
                *currently_running = process;
                printf("The process with ID %d was unblocked and put as the currently running process\n", process->ID);
            }
            else{
                process->state = READY;
                if(process->priority == 0){
                    List_append(high, process);
                    printf("The process with ID %d was unblocked on put on the high priority queue\n", process->ID);
                }
                else if(process->priority == 1){
                    List_append(norm, process);
                    printf("The process with ID %d was unblocked on put on the normal priority queue\n", process->ID);
                }
                else{
                    List_append(low, process);
                    printf("The process with ID %d was unblocked on put on the low priority queue\n", process->ID);
                }
            }
        }
        else{
            printf("No porcess was unblocked, because there was no process blocked on this semaphore\n");
        }
    }
    else{
        printf("V_sem - The semaphore with ID %d is not initialized\n", sem_id);
    }
}

// dump complete state information of process to screen (this includes process
// status and anything else you can think of)
int procInfo(int pID, PCB* initProcess, PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue, Semaphore* semaphores)
{
    PCB* processToBeFound;
    PCB* currentProcess = List_first(highPriorityQueue);
    bool doYouFoundID = false;

    if (pID == 0 )
    {
        printf("***priting initial process***\n");
        processToBeFound = initProcess;
        doYouFoundID = true;
    }

    // search in high queue
    if (!doYouFoundID)
    {
    while (currentProcess != NULL)
    {
        if (currentProcess->ID != pID)
        {
            currentProcess = List_next(highPriorityQueue);
        }
        else
        {
            printf("***priting from high queue***\n");
            processToBeFound = currentProcess;
            currentProcess = NULL;
            doYouFoundID = true;
        }
    }
    }

    // if not found, search in normal queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(normPriorityQueue);
         while (currentProcess != NULL)
        {
            if (currentProcess->ID != pID)
            {
                currentProcess = List_next(normPriorityQueue);
            }
            else
            {   
                printf("***priting from normal queue***\n");
                processToBeFound = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
            }
        }
    }

    // if not found, search in low queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(lowPriorityQueue);
        while (currentProcess != NULL)
        {
            if (currentProcess->ID != pID)
            {
                currentProcess = List_next(lowPriorityQueue);
            }
            else
            {   
                printf("***priting from low queue***\n");
                processToBeFound = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
            }
        }
    }

    // if not found, search in receive queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(receiveQueue);
        while (currentProcess != NULL)
        {
            if (currentProcess->ID != pID)
            {
                currentProcess = List_next(receiveQueue);
            }
            else
            {   
                printf("***priting from receive queue***\n");
                processToBeFound = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
            }
        }
    }


    // if not found, find in send block queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(sendQueue);
        while (currentProcess != NULL)
        {
            if (currentProcess->ID != pID)
            {
                currentProcess = List_next(sendQueue);
            }
            else
            {   
                printf("***priting from send queue***\n");
                processToBeFound = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
            }
        }
    }


    // check the running process
    if (!doYouFoundID){
        if((*runningProcess)->ID == pID){
            printf("***priting from running process***\n");
            processToBeFound = *runningProcess;
            doYouFoundID = true;
        }
    }


    // check the semaphore queues
    for(int i = 0; (i < 5) && (!doYouFoundID); i++){ // check all semaphore queues

        if(semaphores[i].initialized != 0){
            List_first(semaphores[i].blocked_on_this_semaphore);
            processToBeFound = List_search(semaphores[i].blocked_on_this_semaphore, compare, (void*)&pID);
            if(processToBeFound != NULL){
                printf("***priting from semaphore %d queue***\n", i);
                doYouFoundID = true;
            }
        }  
    }


    // if found, print all info
    if (doYouFoundID)
    {
        printf("The id of the found process is: %d, the priority is: %d.\n", processToBeFound->ID, processToBeFound->priority);
        printf("The state of the found process is: %d.\n", processToBeFound->state);
        printf("The message of the found process is: %s.\n", processToBeFound->msg);
        return 0;
    }

    return -1;
}




// display all process queues and their contents
void totalInfo(PCB* initProcess, PCB** runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue, Semaphore* semaphores)
{
    PCB* currentProcess;
    
    // print initial process
    printf("***priting initial process***\n");
    printf("The id of the initial process is: %d, the priority is: %d\n", (*initProcess).ID, (*initProcess).priority);
    printf("The state of the initial process is: %d\n", (*initProcess).state);
    printf("The message of the initial process is: %s\n", (*initProcess).msg);

    // print running process
    printf("***priting running process***\n");
    printf("The id of the found process is: %d, the priority is: %d\n", (*runningProcess)->ID, (*runningProcess)->priority);
    printf("The state of the found process is: %d\n", (*runningProcess)->state);
    printf("The message of the found process is: %s\n", (*runningProcess)->msg);

    if (List_count(highPriorityQueue)!=0)
    {            
        printf("***In the high priority queue***\n");
        currentProcess = List_first(highPriorityQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(highPriorityQueue);
        }

    }

    if (List_count(normPriorityQueue)!=0)
    {
        currentProcess = List_first(normPriorityQueue);
        printf("***In the normal priority queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(normPriorityQueue);
        }

    }

    if (List_count(lowPriorityQueue)!=0)
    {
        currentProcess = List_first(lowPriorityQueue);
        printf("***In the low priority queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(lowPriorityQueue);
        }

    }

    if (List_count(sendQueue)!=0)
    {
        currentProcess = List_first(sendQueue);
        printf("***In the send queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The sending_ID of the found process is: %d\n", currentProcess->sending_ID);
            printf("The message of the found process is: %s\n", currentProcess->msg);
            

            currentProcess = List_next(sendQueue);
        }
    }

    if (List_count(receiveQueue)!=0)
    {
        currentProcess = List_first(receiveQueue);
        printf("***In the receive queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(receiveQueue);
        }

    }

    for(int i = 0; (i < 5); i++){ // check all semaphore queues

        if(semaphores[i].initialized != 0){
            currentProcess = List_first(semaphores[i].blocked_on_this_semaphore);
            printf("***In semaphore %d queue***\n", i);
            while (currentProcess != NULL)
            {
                printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
                printf("The state of the found process is: %d\n", currentProcess->state);
                printf("The message of the found process is: %s\n", currentProcess->msg);

                currentProcess = List_next(semaphores[i].blocked_on_this_semaphore);
            }
        }  
    }
}