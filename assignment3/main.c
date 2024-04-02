#include <stdio.h>
#include "list.h"
#include "Structures_functions.h"

int main(){

// the init process
PCB init;
init.ID = 0;
init.priority = 2;
init.state = RUNNING;
int for_different_process_id = 1;

// initialization stuff...
// like the 5 list that make the queues...
//
List* highPriorityQueue = List_create();
List* normPriorityQueue = List_create();
List* lowPriorityQueue = List_create();
List* sendQueue = List_create();
List* receiveQueue = List_create();

//stuff to keep track of semaphores
Semaphore semaphores[5];
for(int i = 0; i < 5; i++){
    semaphores[i].initialized = 0;
}

// the pointer to keep track of the running process
PCB* running_PCB;
running_PCB = &init;

int stop = 1;
while(stop) // not infinite but for now...
    {
    printf("\nMain, running ID:%d\n", (running_PCB->ID));
    printf("waiting for next command\n");
    
    // get user input
    char c[2]; // first letter of the scanf

    scanf("%s", c);
    //break it into space separeted stuff...

    
    int ID;
    int Priority;
    int sem_ID;
    char msg[40];
    int mutex_value;
    int check = 0;

    switch(c[0]){
        case 'C' : // Sam
            
            scanf("%d", &Priority);
            if(Priority == 0){
                Create_Process(Priority, &running_PCB, highPriorityQueue, &for_different_process_id);
            }
            else if(Priority == 1){
                Create_Process(Priority, &running_PCB, normPriorityQueue, &for_different_process_id);
            }
            else if(Priority == 2){
                Create_Process(Priority, &running_PCB, lowPriorityQueue, &for_different_process_id);
            }
            else{
                printf("The Priority can only be 0,1,2\n");
            }
            break;

        case 'F' :  // Kaite
            
            check = forkProcess(&running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, &for_different_process_id);
            if (check == -1)
            {
                printf("**Fail to fork the process.**\n");
            }
            else
            {
                printf("**Folk Succeed, the new process ID is %d.**\n", check);
            }
            break;

        case 'K' :  // Sam          The kill command can kill process on the send and receive queue's and can't kill exit if anything is on any queue
            
            scanf("%d", &ID);
            stop = Kill_Process(ID, &init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, sendQueue, receiveQueue, semaphores);
            break;

        case 'E' :  // Sam          The Exit command can't exit if there are things blocked on the send, receive, or semaphores
            
            stop = Exit_Running_Process(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, sendQueue, receiveQueue, semaphores);
            break;

        case 'Q' :  // Sam
            
            // put the currently running Process on its appropriate queue
            if(running_PCB->priority == 0){
                List_append(highPriorityQueue, running_PCB);
            }
            else if(running_PCB->priority == 1){
                List_append(normPriorityQueue, running_PCB);
            }
            else{
                List_append(lowPriorityQueue, running_PCB);
            }
            fill_in_running_with_next_process(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'S' :  // Katie
            
            scanf("%d", &ID);
            fgets(msg, 40, stdin);
            check = sendProcess(ID, msg, &init, &running_PCB, sendQueue, receiveQueue, highPriorityQueue, normPriorityQueue, lowPriorityQueue, semaphores);
            
            if (check==-1)
            {
                printf("Failed to send messages, can't find the process\n");
            }
            else if(check == 1){
                printf("The init process cannot send since I cannot be blocked\n");
            }
            else
            {
                // decide next running process
                fill_in_running_with_next_process(&init, &running_PCB, highPriorityQueue, normPriorityQueue,lowPriorityQueue);
            }
            break;

        case 'R' :  // Katie
            
            check = receiveProcess(&running_PCB, receiveQueue, sendQueue);
            
            if (check==-1)
            {
                fill_in_running_with_next_process(&init, &running_PCB, highPriorityQueue, normPriorityQueue,lowPriorityQueue);
                
            }
            break;
            
        case 'Y' :  // Katie
            
            scanf("%d", &ID);
            // printf("Input message:\n");
            fgets(msg, 40, stdin);
            check = replyProcess(ID, msg, sendQueue, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            if (check == -1)
            {
                printf("Failed to reply, cound't find the receiver\n");
            }
            break;

        case 'N' :  // Sam
            scanf("%d", &sem_ID);
            scanf("%d", &mutex_value);
            if(sem_ID < 0 || sem_ID > 4){
                printf("Semaphore's id must be between 0 and 4\n");
            }
            else{
                Create_New_sem(semaphores, sem_ID, mutex_value);
            }
            break;

        case 'P' :  // Sam
            scanf("%d", &sem_ID);
            if(sem_ID < 0 || sem_ID > 4){
                printf("Semaphore's id must be between 0 and 4\n");
            }
            else{
                P_sem(semaphores, sem_ID, &init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            }
            break;

        case 'V' :  // Sam
            scanf("%d", &sem_ID);
            if(sem_ID < 0 || sem_ID > 4){
                printf("Semaphore's id must be between 0 and 4\n");
            }
            else{
                V_sem(semaphores, sem_ID, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            }
            break;

        case 'I' :  // Katie
            
            scanf("%d", &ID);
            check = procInfo(ID, &init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, receiveQueue, sendQueue, semaphores);
            if (check == -1)
            {
                printf("**There is no process with ID: %d**\n", ID);
            }
            break;

        case 'T' :  // Katie
            
            totalInfo(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, receiveQueue, sendQueue, semaphores);
            break;

        default:
            printf("The letter %c is not a command\nYour choices are C, F, K, E, Q, S, R, Y, N, P, V, I, T.\nThey are case sensitive\n", c[0]);
    }
    
}
    printf("you killed the init process the simulation is ending\n");
    return 0;
}
