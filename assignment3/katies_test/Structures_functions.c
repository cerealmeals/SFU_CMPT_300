#include "Structures_functions.h"
#include <stdio.h>

// Copy the currently running process and put it on the ready
// Q corresponding to the original process' priority.
// Attempting to Fork the "init" process (see below) should fail.
// Return pID on success, -1 on failure 
int forkProcess()
{
    // if "init" process, return -1
    if (init_process.state == 0)
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

    // block by calling P(S)

    // print info
    

    return -1;
}

// receive a message - block until one arrives
// report: scheduling information and (once msg is received) the
// message text and source of message
void receiveProcess()
{
    // receive a message

    // block by calling P(S)

    // print info
}

// unblocks sender and delivers reply 
// Return 0 on success, -1 on failure 
int replyProcess(int pID, char* msg)
{
    // unblock sender, call V(S)

    // deliver reply??
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