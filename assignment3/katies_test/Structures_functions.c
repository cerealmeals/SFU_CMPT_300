#include "Structures_functions.h"

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
    
    // put it into the ready queue

}

// send a message to another process - block until reply
// report scheduling information,  and reply source and text (once
// reply arrives)
// Return 0 on success, -1 on failure 
int sendProcess(int pID, char* msg)
{
    // send a message

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