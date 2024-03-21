#include "Structures_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Processes
PCB* initProcess;
PCB* runningProcess;

// 5 Queues, 3 ready queues with priority, 1 send and 1 receive
List* highPriorityQueue;
List* normPriorityQueue;
List* lowPriorityQueue;
List* sendQueue;
List* receiveQueue;

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
    PCB* copy_process = NULL;
    copy_process = (PCB*)malloc(sizeof(runningProcess));
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
    else if(copy_process->priority == 2)
    {
        List_append(lowPriorityQueue, (void*)copy_process);
    }
}

// schedule next running process
void* nextRunningProcess()
{
    int number_of_high_pro = List_count(highPriorityQueue);
    int number_of_norm_pro = List_count(normPriorityQueue);
    int number_of_low_pro = List_count(lowPriorityQueue);

    int currentPriority = runningProcess->priority;

    // if no more processes in ready queue, use initial process
    if (number_of_high_pro+number_of_norm_pro+number_of_low_pro==0)
    {
        runningProcess = initProcess;
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

// send a message to another process - block until reply
// report scheduling information,  and reply source and text (once
// reply arrives)
// Return 0 on success, -1 on failure 
int sendProcess(int pID, char* msg)
{
    printf("The id of running process is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);

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
            }
        }
    }


    // send the message
    if (doYouFoundID)
    {
        char copyMsg[40];
        strcpy(copyMsg, msg);
        strcpy(processToBeSent->msg, copyMsg);
        printf("The message to be sent is: %s \n", copyMsg);

        // block myself
        processToBeSent->state = BLOCKED;

        // put it into the send blocked queue       
        List_append(sendQueue, (void*)runningProcess);

        // decide next running process
        runningProcess = nextRunningProcess();

        // print scheduling infomation
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);
    
        return 0;
    }

    return -1;
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
        nextRunningProcess();
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);

    }
    else
    {
        printf("The id of running process now is: %d, the priority is: %d.\n", runningProcess->ID, runningProcess->priority);
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
        printf("The message replied is: %s\n", sender_process->msg);
        return 0;
    }

    return -1;
}

// dump complete state information of process to screen (this includes process
// status and anything else you can think of)
void procInfo(int pID)
{
    PCB* processToBeFound;
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
            processToBeFound = currentProcess;
            currentProcess = NULL;
            doYouFoundID = true;
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
    }

    return -1;
}

// display all process queues and their contents
void totalInfo()
{
    PCB* currentProcess;

    if (List_count(highPriorityQueue)!=0)
    {
        currentProcess = List_first(highPriorityQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d.\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d.\n", currentProcess->state);
            printf("The message of the found process is: %s.\n", currentProcess->msg);

            currentProcess = List_next(highPriorityQueue);
        }

    }

    if (List_count(normPriorityQueue)!=0)
    {
        currentProcess = List_first(normPriorityQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d.\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d.\n", currentProcess->state);
            printf("The message of the found process is: %s.\n", currentProcess->msg);

            currentProcess = List_next(normPriorityQueue);
        }

    }

    if (List_count(lowPriorityQueue)!=0)
    {
        currentProcess = List_first(lowPriorityQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d.\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d.\n", currentProcess->state);
            printf("The message of the found process is: %s.\n", currentProcess->msg);

            currentProcess = List_next(lowPriorityQueue);
        }

    }

    if (List_count(sendQueue)!=0)
    {
        currentProcess = List_first(sendQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d.\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d.\n", currentProcess->state);
            printf("The message of the found process is: %s.\n", currentProcess->msg);

            currentProcess = List_next(sendQueue);
        }
    }

    if (List_count(receiveQueue)!=0)
    {
        currentProcess = List_first(receiveQueue);
        while (currentProcess != NULL)
        {
            printf("The id of the found process is: %d, the priority is: %d.\n", currentProcess->ID, currentProcess->priority);
            printf("The state of the found process is: %d.\n", currentProcess->state);
            printf("The message of the found process is: %s.\n", currentProcess->msg);

            currentProcess = List_next(receiveQueue);
        }

    }
}