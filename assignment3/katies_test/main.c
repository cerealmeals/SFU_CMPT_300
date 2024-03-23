#include <stdio.h>
#include "list.h"
#include "Structures_functions.h"

int main()
{
PCB init;
init.ID = 0;
init.priority = 2;
init.state = RUNNING;

// initialization stuff...
List* highPriorityQueue = List_create();
List* normPriorityQueue = List_create();
List* lowPriorityQueue = List_create();
List* sendQueue = List_create();
List* receiveQueue = List_create();

//stuff to keep track of semaphores
Semaphore semaphores[4];
int how_many_semaphores_are_currently_in_use = 0;

PCB running_PCB;
running_PCB = init;

int stop = 1;

while(stop) // not infinite but for now...
    {
    printf("waiting for next command\n");
    printf("Main, running ID:%d\n", running_PCB.ID);
    
    // get user input
    char c[2]; // first letter of the scanf

    scanf("%s", c);
    //break it into space separeted stuff...

    int ID;
    int Priority;
    char msg[40];
    int sem_ID;
    int check = 0;

    switch(c[0]){
        case 'C' : // Sam
            printf("did it get into Create?\n");
            scanf("%d", &ID);
            scanf("%d", &Priority);
            printf("The priority is %d.\n", Priority);
            if(Priority == 0){
                Create_Proccess(ID, Priority, &running_PCB, highPriorityQueue);
                printf("The number of the queue is %d.\n", List_count(highPriorityQueue));
            }
            else if(Priority == 1){
                Create_Proccess(ID, Priority, &running_PCB, normPriorityQueue);
            }
            else{
                Create_Proccess(ID, Priority, &running_PCB, lowPriorityQueue);
            }
            
            break;

        case 'F' :  // Kaite
            printf("***did it get into Fork?***\n");
            check = forkProcess(&running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            if (check == -1)
            {
                printf("**Fail to fork the process.**\n");
            }
            else
            {
                printf("**Folk Succeed, the new process ID is %d.**\n", check);
            }
            break;

        case 'K' :  // Sam
            printf("did it get into Kill?\n");
            scanf("%d", &ID);
            stop = Kill_Proccess(ID, &init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'E' :  // Sam
            printf("did it get into Exit?\n");
            stop = Exit_Running_Proccess(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'Q' :  // Sam
            printf("did it get into Quantum?\n");
            if(running_PCB.priority == 0){
                List_append(highPriorityQueue, &running_PCB);
            }
            else if(running_PCB.priority == 1){
                List_append(normPriorityQueue, &running_PCB);
            }
            else{
                List_append(lowPriorityQueue, &running_PCB);
            }
            fill_in_running_with_next_process(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'S' :  // Katie
            printf("Get into send!\n");
            scanf("%d", &ID);
            fgets(msg, 40, stdin);
            check = sendProcess(ID, msg, &init, &running_PCB, sendQueue, receiveQueue, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            
            if (check==-1)
            {
                printf("Failed to send messages, can't find the process\n");
            }
            else
            {
                // decide next running process
                // fill_in_running_with_next_process(&init, &running_PCB, highPriorityQueue, normPriorityQueue,lowPriorityQueue);
                running_PCB = *nextRunningProcess(&init, &running_PCB, highPriorityQueue, normPriorityQueue,lowPriorityQueue);

                // print scheduling infomation
                printf("***The id of running process now is: %d, the priority is: %d.\n", running_PCB.ID, running_PCB.priority);
            }
            break;

        case 'R' :  // Katie
            printf("Get into receive\n");
            check = receiveProcess(&init, &running_PCB, receiveQueue);
            
            if (check==-1)
            {
                printf("Failed to receive messages, no message coming\n");  
                // decide next running process
                running_PCB = *nextRunningProcess(&init, &running_PCB, highPriorityQueue, normPriorityQueue,lowPriorityQueue);

                // print scheduling infomation
                printf("***The id of running process now is: %d, the priority is: %d.\n", running_PCB.ID, running_PCB.priority);         
            }
            break;
            
        case 'Y' :  // Katie
            printf("Get into reply\n");
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
            Create_New_sem(semaphores, sem_ID, how_many_semaphores_are_currently_in_use);
            break;

        case 'P' :  // Sam
            scanf("%d", &sem_ID);
            break;

        case 'V' :  // Sam
            scanf("%d", &sem_ID);

            break;

        case 'I' :  // Katie
            printf("Get into process info!\n");
            scanf("%d", &ID);
            check = procInfo(ID, &init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, receiveQueue, sendQueue);
            if (check == -1)
            {
                printf("**There is no process with ID: %d**\n", ID);
            }
            break;

        case 'T' :  // Katie
            // working
            printf("Get into total info!\n");
            totalInfo(&init, &running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue, receiveQueue, sendQueue);
            break;

        default:
            printf("The letter %c is not a command\nYour choices are C, F, K, E, Q, S, R, Y, N, P, V, I, T.\nThey are case sensitive\n", c[0]);
    }
    
}
    //free all those lists even though you shouldn't need to
    printf("you killed the init process the program is ending\n");
    return 0;
}
