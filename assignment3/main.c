#include <stdio.h>
#include "list.h"
#include "Structures_functions.h"

int main(){

struct PCB init;
init.ID = 0;
init.priority = 0;
init.state = RUNNING;
// initialization stuff...
// like the 5 list that make the queues...
// 
List* highPriorityQueue = List_create();
List* normPriorityQueue = List_create();
List* lowPriorityQueue = List_create();
List* sendQueue = List_create();
List* receiveQueue = List_create();

struct PCB running_PCB;
running_PCB.ID = -1;
running_PCB.priority = -1;
running_PCB.state = RUNNING;




    while(1) // not infinite but for now...
    {
        printf("waiting for next command\n");
        // get user input
        char c[2]; // first letter of the scanf

        scanf("%s", c);
        //break it into space separeted stuff...

        
        
        switch(c[0]){
            case 'C' : // Sam
                printf("did it get into Create?\n");
                break;

            case 'F' :  // Kaite
                printf("did it get into Fork?\n");
                break;

            case 'K' :  // Sam
                break;

            case 'E' :  // Sam
                break;

            case 'Q' :  // Sam
                break;

            case 'S' :  // Katie
                break;

            case 'R' :  // Katie
                break;
                
            case 'Y' :  // Katie
                break;

            case 'N' :  // Sam
                break;

            case 'P' :  // Sam
                break;

            case 'V' :  // Sam
                break;

            case 'I' :  // Katie
                break;

            case 'T' :  // Katie
                break;

            default:    
                // looping
        }
    }
    return 0;
}
