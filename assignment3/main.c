#include <stdio.h>
#include "list.h"
#include "Structures_functions.h"

int main(){

struct PCB init;
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
List* semaphores;
int how_many_semaphores_are_currently_in_use

struct PCB* running_PCB;
running_PCB->ID = 0;
running_PCB->priority = 2;
running_PCB->state = RUNNING;

int stop = 1;
while(stop) // not infinite but for now...
    {
    printf("waiting for next command\n");
    // get user input
    char c[2]; // first letter of the scanf

    scanf("%s", c);
    //break it into space separeted stuff...

    
    
    switch(c[0]){
        case 'C' : // Sam
            printf("did it get into Create?\n");
            int ID;
            int Priority;
            scanf("%d", ID);
            scanf("%d", Priority);
            if(Priority == 0){
                Create_Proccess(ID, Priority, running_PCB, highPriorityQueue);
            }
            else if(Priority == 1){
                Create_Proccess(ID, Priority, running_PCB, normPriorityQueue);
            }
            else{
                Create_Proccess(ID, Priority, running_PCB, lowPriorityQueue);
            }
            
            break;

        case 'F' :  // Kaite
            printf("did it get into Fork?\n");
            break;

        case 'K' :  // Sam
            printf("did it get into Kill?\n");
            int ID;
            scanf("%d", ID);
            stop = Kill_Proccess(ID, running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'E' :  // Sam
            printf("did it get into Exit?\n");
            stop = Exit_Running_Proccess(running_PCB, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;

        case 'Q' :  // Sam
            printf("did it get into Quantum?\n");
            if(currently_running->priority = 0){
                List_append(high, currently_running);
            }
            else if(currently_running = 1){
                List_append(norm, currently_running);
            }
            else{
                List_append(low, currently_running);
            }
            fill_in_running_with_next_process(init, currently_running, highPriorityQueue, normPriorityQueue, lowPriorityQueue);
            break;
+
        case 'S' :  // Katie
            break;

        case 'R' :  // Katie
            break;
            
        case 'Y' :  // Katie
            break;

        case 'N' :  // Sam
            int sem_ID
            scanf("%d", sem_ID);
            Create_New_sem(semaphores, sem_ID, how_many_semaphores_are_currently_in_use)
            break;

        case 'P' :  // Sam
            int sem_ID
            scanf("%d", sem_ID);

            break;

        case 'V' :  // Sam
            int sem_ID
            scanf("%d", sem_ID);

            break;

        case 'I' :  // Katie
            break;

        case 'T' :  // Katie
            break;

        default:
            printf("The letter %c is not a command\n
                Your choices are C, F, K, E, Q, S, R, Y, N, P, V, I, T.\n
                They are case sensitive\n", c);
    }
    printf("you killed the init process the program is ending\n");
}
    //free all those lists even though you shouldn't need to

    return 0;
}
