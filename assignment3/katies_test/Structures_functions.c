#include "Structures_functions.h"
#include <stdio.h>

// Copy the currently running process and put it on the ready
// Q corresponding to the original process' priority.
// Attempting to Fork the "init" process (see below) should fail.
// Return pID on success, -1 on failure 
int forkProcess()
{
    // if "init" process, return -1
    if (initProcess->state == 0)
        return -1;

    // copy current process
    PCB* copy_process;
    copy_process = (PCB*)alloc(sizeof(runningProcess));
    copy_process->ID = runningProcess->ID;
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
    else  (copy_process->priority == 2)
    {
        List_append(lowPriorityQueue, (void*)copy_process);
    }
}

// send a message to another process - block until reply
// report scheduling information,  and reply source and text (once
// reply arrives)
// Return 0 on success, -1 on failure 
int sendProcess(int pID, char* msg)
{
    printf("The id of running process is: %d, the priority is: %d.\n", runningProcess->pID, runningProcess->priority);

    // send a message
    PCB* processToBeSent;
    PCB* currentProcess = List_first(highPriorityQueue);
    int doYouFoundID = 0;

    // search in high queue
    while (currentProcess != NULL)
    {
        if (currentProcess->pID != pID)
        {
            currentProcess = List_next(highPriorityQueue);
        }
        else
        {
            processToBeSent = currentProcess;
            currentProcess = NULL;
            doYouFoundID = 1;
        }
    }

    // if not found, search in normal queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(normPriorityQueue);
         while (currentProcess != NULL)
        {
            if (currentProcess->pID != pID)
            {
                currentProcess = List_next(normPriorityQueue);
            }
            else
            {
                processToBeSent = currentProcess;
                currentProcess = NULL;
                doYouFoundID = 1;
            }
        }
    }

    // if not found, search in low queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(lowPriorityQueue);
         while (currentProcess != NULL)
        {
            if (currentProcess->pID != pID)
            {
                currentProcess = List_next(lowPriorityQueue);
            }
            else
            {
                processToBeSent = currentProcess;
                currentProcess = NULL;
                doYouFoundID = 1;
            }
        }
    }

    // if not found, search in receive queue
    if (!doYouFoundID)
    {
        PCB* currentProcess = List_first(receiveQueue);
         while (currentProcess != NULL)
        {
            if (currentProcess->pID != pID)
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
                else  (processToBeSent->priority == 2)
                {
                    List_append(lowPriorityQueue, (void*)processToBeSent);
                }

                currentProcess = NULL;
                doYouFoundID = 1;
            }
        }
    }

    // send the message
    if (doYouFoundID)
    {
        char copyMsg[40];
        strcpy(copyMsg, msg);
        processToBeSent->msg = copyMsg;
        printf("The message to be sent is: %s \n", copyMsg);

        // block myself
        processToBeSent->state = BLOCKED;

        // put it into the send blocked queue       
        List_append(sendQueue, (void*)runningProcess);

        // decide next running process
        runningProcess = schedule_next_runningProcess();

        // print scheduling infomation
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->pID, runningProcess->priority);
    
        return 0;
    }

    return -1;
}

// schedule next running process
void* schedule_next_runningProcess()
{
    int number_of_high_pro = List_count(highPriorityQueue);
    int number_of_norm_pro = List_count(normPriorityQueue);
    int number_of_low_pro = List_count(lowPriorityQueue);

    int currentPriority = runningProcess->priority;

    // if no more processes in ready queue, use initial process
    if (number_of_high_pro+number_of_norm_pro+number_of_low_pro==0)
    {
        runningProcess = init_process;
        runningProcess->state = RUNNING;
        return;
    }

    if (currentPriority == 0)
    {
        // prevent lower priority from staving
        if (number_of_norm_pro - number_of_high_pro > 2)
        {
            runningProcess = List_first(normPriorityQueue);
            List_remove(normPriorityQueue);
        }
        else
        {
            runningProcess = List_first(highPriorityQueue);
            List_remove(highPriorityQueue);
        }
    }
    else if (currentPriority == 1)
    {
        if (number_of_low_pro - number_of_norm_pro > 2)
        {
            runningProcess = List_first(lowPriorityQueue);
            List_remove(lowPriorityQueue);
        }
        else
        {
            runningProcess = List_first(normPriorityQueue);
            List_remove(normPriorityQueue);
        }
    }
    else if (currentPriority == 2)
    {
        runningProcess = List_first(lowPriorityQueue);
        List_remove(lowPriorityQueue);
    }

    runningProcess->state = RUNNING;
    return;
}


// receive a message - block until one arrives
// report: scheduling information and (once msg is received) the
// message text and source of message
void receiveProcess()
{
    // if no message received, block
    if (runningProcess->msg == NULL)
    {
        printf("Blocked, no message received\n");
        if (initProcess->state != 0) // can't block initial process
        {
            runningProcess->state = 2;
            List_append(receiveQueue, runningProcess);
        }
        schedule_next_runningProcess();
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->pID, runningProcess->priority);

    }
    else
    {
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->pID, runningProcess->priority);
        printf("The message received is: %s.\n", runningProcess->msg);
    }

}

// unblocks sender and delivers reply 
// Return 0 on success, -1 on failure 
int replyProcess(int pID, char* msg)
{
    // find sender
    PCB* sender_process;
    PCB* current = List_first(sendQueue);

    bool ifFound = false;

    while (current != NULL)
    {
        if (current->pID != pID)
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
            List_append(highPriorityQueue);
        }
        else if (sender_process->priority == 1)
        {
            List_append(normPriorityQueue);
        }
        else if (sender_process->priority == 2)
        {
            List_append(lowPriorityQueue);
        }

        strcpy(sender_process->msg, msg);
        printf("The message replied is: %s\n", sender_process->msg);
        return 0;
    }

    return -1;
}

// dump complete state information of process to screen (this includes process
// status and anything else you can think of)
void procInfo(int pID)
{

}

// display all process queues and their contents
void totalInfo()
{
    
}