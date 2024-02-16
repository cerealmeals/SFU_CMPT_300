#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include "list.h"
#include "receiver.h"
#include "sender.h"
#include "sleep.h"

#define MAX_BUFFER 256
List* keyboard_list;
List* receiving_list;
pthread_mutex_t mutex;
pthread_cond_t condition_empty;
pthread_cond_t condition_full;


void free_char(void* pItem) {
  // cast pItem to a char pointer
  char* i = (char*) pItem;
  // free the memory allocated for the string
  free(i);
}

void *print_to_screen(){
    char ch;
    int i;
    do{
        
        pthread_mutex_lock(&mutex);
        if(List_count(keyboard_list)==0){
            pthread_cond_wait(&condition_empty, &mutex);
        }
        char * message = List_trim(keyboard_list);
        pthread_cond_signal(&condition_full);
        pthread_mutex_unlock(&mutex);
        
        i = strlen(message);
        ch = message[0];

        if((ch != '!')||(i!=2)){

            for(int j = 0; j < strlen(message); j++){
                putchar(message[j]);
                fflush(stdout);
                sleep_sec2(100);
            }
        }

        
        free(message);
        
    }while((ch != '!')||(i!=2));
}

void *get_user_input(){
    char * input;
    do{
        input = (char*) malloc(MAX_BUFFER);
        fgets(input, MAX_BUFFER, stdin);
        // input[MAX_BUFFER - 1] = '\0';
        // printf("%ld", sizeof(input));

        pthread_mutex_lock(&mutex);
        if(List_count(keyboard_list)== LIST_MAX_NUM_NODES){
            pthread_cond_wait(&condition_full, &mutex);
        }
        List_prepend(keyboard_list, (void*)input);
        pthread_cond_signal(&condition_empty);
        pthread_mutex_unlock(&mutex);

                //printf("%s", input);
        // *input = getchar();
        // printf("%c", *input);
        //printf("\n");
    }while((input[0] != '!')||(strlen(input)!=2));
}
int main() {
    keyboard_list = List_create();
    receiving_list =List_create();
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition_empty, NULL);
    pthread_cond_init(&condition_full, NULL);
    pthread_t threadkeyboard, threadScreen, threadReceive, threadSend;
    //char *message1 = "Thread 1";
    //char *message2 = "Thread 2";
    int iret1, iret2, iret3, iret4;
    /* Create independent threads each of which will execute function */

    // gets user input and but message in keyboard_list
    iret1 = pthread_create( &threadkeyboard, NULL, get_user_input, NULL);

    // get message from list and put on screen
    iret2 = pthread_create( &threadScreen, NULL, print_to_screen, NULL);

    // Receive message from socket
    iret3 = pthread_create( &threadReceive, NULL, NULL, NULL);

    // Send message using socket
    iret4 = pthread_create( &threadSend, NULL, NULL, NULL);



    /* Wait till threads are complete before main continues. Unless we */
    /* wait we run the risk of executing an exit which will terminate */
    /* the process and all threads before the threads have completed. */
    pthread_join( threadkeyboard, NULL);
    pthread_join( threadScreen, NULL);
    pthread_join( threadReceive, NULL);
    pthread_join( threadSend, NULL);
    printf("Thread Keybaord returns: %d\n",iret1);
    printf("Thread Screen returns: %d\n",iret2);
    printf("Thread Receive returns: %d\n",iret3);
    printf("Thread Send returns: %d\n",iret4);

    FREE_FN free_er = free_char;
    List_free(keyboard_list, free_er);
    List_free(receiving_list, free_er);
    exit(0);
    return 0;
    // katieTesting_function();
}