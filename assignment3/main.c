#include <stdio.h>
#include "list.h"
#include "Structures_functions.h"

int main(){

PCB init;
init.ID = 0;
init.priority = 2;
init.state = RUNNING;
// initialization stuff...
// like the 5 list that make the queues...
//
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
    int sem_ID;
    switch(c[0]){
        case 'C' : // Sam
            printf("did it get into Create?\n");
            scanf("%d", &ID);
            scanf("%d", &Priority);
            if(Priority == 0){
                Create_Proccess(ID, Priority, &running_PCB, highPriorityQueue);
            }
            else if(Priority == 1){
                Create_Proccess(ID, Priority, &running_PCB, normPriorityQueue);
            }
            else{
                Create_Proccess(ID, Priority, &running_PCB, lowPriorityQueue);
            }
            
            break;

        case 'F' :  // Kaite
            printf("did it get into Fork?\n");
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
            break;

        case 'R' :  // Katie
            break;
            
        case 'Y' :  // Katie
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
            break;

        case 'T' :  // Katie
            break;

        default:
            printf("The letter %c is not a command\nYour choices are C, F, K, E, Q, S, R, Y, N, P, V, I, T.\nThey are case sensitive\n", c[0]);
    }
    
}
    //free all those lists even though you shouldn't need to
    printf("you killed the init process the program is ending\n");
    return 0;
}
