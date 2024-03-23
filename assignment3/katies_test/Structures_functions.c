#include "Structures_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*sam's functions*/
void Create_Proccess(int id, int prio,  PCB* currently_running, List* queue){
    PCB* proccess = (PCB*)malloc(sizeof(PCB));

    proccess->ID = id;
    proccess->priority = prio;
    proccess->state = READY;

    if(currently_running->ID == 0){
        proccess->state = RUNNING;
        *currently_running = *proccess;
        printf("Proccess with ID: %d replaced the init proccess and is now running\n", id);
        printf("within create, running ID: %d\n", currently_running->ID);
        printf("within create, running priority: %d\n", currently_running->priority);
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


int Kill_Proccess(int id, PCB* init, PCB* currently_running, List*high, List*norm, List*low){
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

int Exit_Running_Proccess(PCB* init, PCB* currently_running, List*high, List*norm, List*low){
    if(currently_running->ID == 0){
        // end the program because the only process is the init process
        printf("Within Exit, you killed the init process\n");
        return 0;
    }
    else{
        printf("You killed the currently running process it had ID %d\n", currently_running->ID);
        // free(currently_running);
        // fill_in_running_with_next_process(init, currently_running, high, norm, low);
        return 1;
    }
}

void fill_in_running_with_next_process(PCB* init, PCB* currently_running, List*high, List*norm, List*low){
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
        printf("The new Running process had ID %d\nIt was taken for the high priority queue\n", currently_running->ID);
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
        printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", currently_running->ID);
        currently_running->state = RUNNING;
        track_norm++;
        high_turn = 1;
    }
    else if( low_turn && (0 != List_count(low))) {
        currently_running = List_first(low);
        List_remove(low);
        currently_running->state = RUNNING;
        printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", currently_running->ID);
        high_turn = 1;
    }
    else{
        //weird stuff can happen if stuff is killed so check all the list again
        if(0 != List_count(high)){
            currently_running = List_first(high);
            List_remove(high);
            currently_running->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the high priority queue\n", currently_running->ID);
        }
        else if(0 != List_count(norm)){
            currently_running = List_first(norm);
            List_remove(norm);
            currently_running->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the normal priority queue\n", currently_running->ID);
        }
        else if(0 != List_count(low)){
            currently_running = List_first(low);
            List_remove(low);
            currently_running->state = RUNNING;
            printf("The new Running process had ID %d\nIt was taken for the low priority queue\n", currently_running->ID);
        }

        // then if all those fail there is nothing to do so set the init process to running
        currently_running = &init;
        currently_running->state = RUNNING;
        printf("The new Running proces is the init process because there are no other process");
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
/*end of sam's functions*/


// Copy the currently running process and put it on the ready
// Q corresponding to the original process' priority.
// Attempting to Fork the "init" process (see below) should fail.
// Return pID on success, -1 on failure 
int forkProcess(PCB* runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue) 
{
    // if "init" process, return -1
    if (runningProcess->ID == 0)
    {
        printf("**The running process is the initial process.**\n");
        return -1;
    }

    // copy current process
    PCB* copy_process = NULL;
    copy_process = (PCB*)malloc(sizeof(runningProcess));
    copy_process->ID = runningProcess->ID + 1;
    copy_process->priority = runningProcess->priority;
    copy_process->state = runningProcess->state;
    strcpy(copy_process->msg, runningProcess->msg);

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


    printf("**within folk, the folked process priority is %d.**\n", copy_process->priority);
    return copy_process->ID;
}

// schedule next running process
// lower priority processes will be stavaing but testing for now
PCB* nextRunningProcess(PCB* initProcess, PCB* runningProcess,List* highPriorityQueue, List* normPriorityQueue,List* lowPriorityQueue)
{
    int number_of_high_pro = List_count(highPriorityQueue);
    int number_of_norm_pro = List_count(normPriorityQueue);
    int number_of_low_pro = List_count(lowPriorityQueue);
    

    // if no more processes in ready queue, use initial process
    if (number_of_high_pro+number_of_norm_pro+number_of_low_pro==0)
    {
        runningProcess = initProcess;
        runningProcess->state = RUNNING;
        return;
    }

    if (number_of_high_pro>0)
    {
        runningProcess = (PCB*)List_first(highPriorityQueue);
        List_remove(highPriorityQueue);
        runningProcess->state = RUNNING;
        return runningProcess;
    }
    if (number_of_norm_pro!=0)
    {
        runningProcess = (PCB*)List_first(normPriorityQueue);
        List_remove(normPriorityQueue);
        runningProcess->state = RUNNING;
        return runningProcess;
    }
    if (number_of_low_pro!=0)
    {
        runningProcess = (PCB*)List_first(lowPriorityQueue);
        List_remove(lowPriorityQueue);
        runningProcess->state = RUNNING;
        return runningProcess;
    }

    runningProcess->state = RUNNING;
    return runningProcess;
}

// send a message to another process - block until reply
// report scheduling information,  and reply source and text (once
// reply arrives)
// Return 0 on success, -1 on failure 
int sendProcess(int pID, char* msg, PCB* initProcess, PCB* runningProcess,List* sendQueue, List* receiveQueue, List* highPriorityQueue, List* normPriorityQueue,List* lowPriorityQueue)
{
    // printf("The id of running process is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);

    // send a message
    PCB* processToBeSent;
    PCB* currentProcess = List_first(highPriorityQueue);
    bool doYouFoundID = false;

    // search in high queue
    while (currentProcess != NULL)
    {
        if (currentProcess->ID != pID)
        {
            currentProcess = List_next(highPriorityQueue);
        }
        else
        {
            processToBeSent = currentProcess;
            currentProcess = NULL;
            doYouFoundID = true;
            printf("find in high queue\n");
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
                processToBeSent = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
                printf("find in norm queue\n");
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
                processToBeSent = currentProcess;
                currentProcess = NULL;
                doYouFoundID = true;
                printf("find in low queue\n");
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
                processToBeSent = currentProcess;
                processToBeSent->state = READY;

                // Remove it from the receive block queue
                List_remove(receiveQueue);

                // put it into the ready queue
                if (processToBeSent->priority == 0)
                {
                    List_append(highPriorityQueue, (void*)processToBeSent);
                }
                else if (processToBeSent->priority == 1)
                {
                    List_append(normPriorityQueue, (void*)processToBeSent);
                }
                else if(processToBeSent->priority == 2)
                {
                    List_append(lowPriorityQueue, (void*)processToBeSent);
                }

                currentProcess = NULL;
                doYouFoundID = true;
                printf("find in high queue\n");
            }
        }
    }


    // send the message
    if (doYouFoundID)
    {
        strcpy(processToBeSent->msg, msg);
        printf("The message to be sent is: %s \n", processToBeSent->msg);
        printf("The id of receiver process is: %d, the priority is: %d.\n", processToBeSent->ID, processToBeSent->priority);

        // block myself
        PCB* copy_process = NULL;
        copy_process = (PCB*)malloc(sizeof(runningProcess));
        copy_process->ID = runningProcess->ID;
        copy_process->priority = runningProcess->priority;
        copy_process->state = BLOCKED;
        strcpy(copy_process->msg, runningProcess->msg);

        // put it into the send blocked queue       
        List_append(sendQueue, (void*)copy_process);

        // // decide next running process
        // runningProcess = nextRunningProcess(initProcess, runningProcess, highPriorityQueue, normPriorityQueue,lowPriorityQueue);
        // // fill_in_running_with_next_process(initProcess, runningProcess, highPriorityQueue, normPriorityQueue,lowPriorityQueue);

        // // print scheduling infomation
        // printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);
    
        return 0;
    }

    return -1;
}



// receive a message - block until one arrives
// report: scheduling information and (once msg is received) the
// message text and source of message
int receiveProcess(PCB* initProcess, PCB* runningProcess,List*receiveQueue)
{
    // if no message received, block
    if (strlen(runningProcess->msg) == 0)
    {
        printf("Blocked, no message received\n");
        if (initProcess->state != 0) // can't block initial process
        {
            runningProcess->state = BLOCKED;
            List_append(receiveQueue, runningProcess);
        }
        return -1;
    }
    else
    {
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);
        printf("The message received is: %s\n", runningProcess->msg);
        return 0;
    }

}

// unblocks sender and delivers reply 
// Return 0 on success, -1 on failure 
int replyProcess(int pID, char* msg, List* sendQueue,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue)
{
    // find sender
    PCB* sender_process;
    PCB* current = List_first(sendQueue);
    //printf("372\n");
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
            //printf("85\n");
            List_remove(sendQueue);
            //printf("87\n");
            current = NULL;
            ifFound = true;
            //printf("88\n");
        }
    }

    if (ifFound)
    {
        // put it in the ready queue
        //printf("97\n");
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

// dump complete state information of process to screen (this includes process
// status and anything else you can think of)
int procInfo(int pID, PCB* initProcess, PCB* runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue)
{
    PCB* processToBeFound;
    PCB* currentProcess = List_first(highPriorityQueue);
    bool doYouFoundID = false;

    if (pID == 0 )
    {
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
                processToBeFound = currentProcess;
                currentProcess = NULL;
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
void totalInfo(PCB* initProcess, PCB* runningProcess,List*highPriorityQueue, List*normPriorityQueue,List*lowPriorityQueue, List* receiveQueue, List* sendQueue)
{
    PCB* currentProcess;

    // print running process
    printf("***priting running process***\n");
    printf("The id of the found process is: %d, the priority is: %d\n", runningProcess->ID, runningProcess->priority);
    printf("The state of the found process is: %d\n", runningProcess->state);
    printf("The message of the found process is: %s\n", runningProcess->msg);

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
        printf("***In the send priority queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(sendQueue);
        }
    }

    if (List_count(receiveQueue)!=0)
    {
        currentProcess = List_first(receiveQueue);
        printf("***In the receive priority queue***\n");
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d\n", currentProcess->state);
            printf("The message of the found process is: %s\n", currentProcess->msg);

            currentProcess = List_next(receiveQueue);
        }

    }
}