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
#include <errno.h>

#define MAX_BUFFER 256
List* keyboard_list;
List* sending_list;
pthread_mutex_t keyboard_mutex;
pthread_cond_t keyboard_condition_empty;
pthread_cond_t keyboard_condition_full;
pthread_mutex_t sending_mutex;
pthread_cond_t sending_condition_empty;
pthread_cond_t sending_condition_full;
pthread_mutex_t socket_mutex;


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
        printf("start of print loop\n");
        pthread_mutex_lock(&keyboard_mutex);
        if(List_count(keyboard_list)==0){
            printf("waiting for message to print\n");
            pthread_cond_wait(&keyboard_condition_empty, &keyboard_mutex);
        }
        char * message = List_trim(keyboard_list);
        pthread_cond_signal(&keyboard_condition_full);
        pthread_mutex_unlock(&keyboard_mutex);
        
        i = strlen(message);
        ch = message[0];

        // printf("Your message: ");
        
        if((ch != '!')||(i!=2)){
            printf("The message that should be printed: %s", message);
        //     for(int j = 0; j < strlen(message); j++){
        //         putchar(message[j]);
        //         fflush(stdout);
        //         sleep_sec2(100);
        //     }
        }
        free(message);
        printf("end of print loop\n");
        
    }while((ch != '!')||(i!=2));
    printf("after of print loop\n");
}

void *get_user_input(){
    char * input;
    do{
        printf("start of input loop\n");
        input = (char*) malloc(MAX_BUFFER);
        fgets(input, MAX_BUFFER, stdin);
        // input[MAX_BUFFER - 1] = '\0';
        // printf("%ld", sizeof(input));


        // make sure there is space on the Keyboard list 
        // and more sure no one is using it right now
        // then prepend the message to the list
        pthread_mutex_lock(&keyboard_mutex);
        if(List_count(keyboard_list)== LIST_MAX_NUM_NODES){
            pthread_cond_wait(&keyboard_condition_full, &keyboard_mutex);
        }
        List_prepend(keyboard_list, (void*)input);
        pthread_cond_signal(&keyboard_condition_empty);
        pthread_mutex_unlock(&keyboard_mutex);

        
        // make sure there is space on the sending list 
        // and more sure no one is using it right now
        // then prepend the message to the list
        pthread_mutex_lock(&sending_mutex);
        if(List_count(sending_list)== LIST_MAX_NUM_NODES){
            pthread_cond_wait(&sending_condition_full, &sending_mutex);
        }
        List_prepend(sending_list, (void*)input);
        pthread_cond_signal(&sending_condition_empty);
        pthread_mutex_unlock(&sending_mutex);
        printf("end of input loop\n");

    }while((input[0] != '!')||(strlen(input)!=2));
    printf("after of input loop\n");
}

void *send_message(void *arg){
    
    
    char **argv = (char**)arg;

    struct sockaddr sending_addr;
    struct addrinfo hints, *results;

    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    
    if ((status = getaddrinfo(argv[2], argv[3], &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    
    sending_addr = *(results->ai_addr);

    freeaddrinfo(results);

    char ch;
    int i;

    do{
        printf("start of sending loop\n");
        // take something off the list
        pthread_mutex_lock(&sending_mutex);
        if(List_count(sending_list)==0){
            printf("waiting for message to send\n");
            pthread_cond_wait(&sending_condition_empty, &sending_mutex);
        }
        char * message = List_trim(sending_list);
        pthread_cond_signal(&sending_condition_full);
        pthread_mutex_unlock(&sending_mutex);
        
        i = strlen(message);
        ch = message[0];

        // send the message over the socket to the other address
        //pthread_mutex_lock(&socket_mutex);
        if(sendto( socket_desc, &message, i, 0, &(sending_addr), sizeof(sending_addr)) < 0){
        printf("Unable to send message\n");
        }
        //pthread_mutex_unlock(&socket_mutex);
        

        free(message);
        printf("end of sending loop\n");

    }while((ch != '!')||(i!=2));
    printf("after of sending loop\n");
    close(socket_desc);
}

void *recieve_message(void * arg){
    
    
    char **argv = (char**)arg;
    // initialzie socket stuff
    char hostname[100];
    size_t size = 100;
    gethostname(hostname, size);
    //printf("hostname: %s\n", hostname);

    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    // initialize
    struct sockaddr receiving_addr;
    struct addrinfo hints, *results;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    
    if ((status = getaddrinfo(hostname, argv[1], &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    receiving_addr = *(results->ai_addr);

    freeaddrinfo(results);
    
    if(bind(socket_desc, &receiving_addr, sizeof(receiving_addr)) < 0){
        printf("Couldn't bind to the port\n");
    }
    
    char * receive;
    receive = (char*) malloc(MAX_BUFFER);
    do{
        printf("start of receive loop\n");
        // receiving a message from the socket
        
        //pthread_mutex_lock(&socket_mutex);
        recvfrom(socket_desc, receive, sizeof(receive), 0, NULL, NULL);
        //fprintf(stderr, "frecvfrom: %s(%d)\n", strerror(errno), errno);
        //pthread_mutex_unlock(&socket_mutex);

        // put the message on the list
        pthread_mutex_lock(&keyboard_mutex);
        if(List_count(keyboard_list)== LIST_MAX_NUM_NODES){
            pthread_cond_wait(&keyboard_condition_full, &keyboard_mutex);
        }
        List_prepend(keyboard_list, (void*)receive);
        pthread_cond_signal(&keyboard_condition_empty);
        pthread_mutex_unlock(&keyboard_mutex);

        printf("end of receive loop\n");
    }while((receive[0] != '!')||(strlen(receive)!=2));
    printf("after of receive loop\n");
    close(socket_desc);

}

int main(int argc, char *argv[]) {

    if(argc != 4){
        printf("not the right number of arguments");
        return -1;
    }
    //initialize the lists being used
    keyboard_list = List_create();
    sending_list = List_create();

    // initialize the mutex's and conditions being used
    pthread_mutex_init(&keyboard_mutex, NULL);
    pthread_cond_init(&keyboard_condition_empty, NULL);
    pthread_cond_init(&keyboard_condition_full, NULL);
    pthread_mutex_init(&sending_mutex, NULL);
    pthread_cond_init(&sending_condition_empty, NULL);
    pthread_cond_init(&sending_condition_full, NULL);
    pthread_mutex_init(&socket_mutex, NULL);

    //threads being used
    pthread_t threadkeyboard, threadScreen, threadReceive, threadSend;
    
    // to caputre outpud of threads, will probably get rid of this later
    int iret1, iret2, iret3, iret4;

    /* Create independent threads each of which will execute function */
    // gets user input and but message in keyboard_list
    iret1 = pthread_create( &threadkeyboard, NULL, get_user_input, NULL);

    // get message from list and put on screen
    iret2 = pthread_create( &threadScreen, NULL, print_to_screen, NULL);

    // Receive message from socket
    iret3 = pthread_create( &threadReceive, NULL, send_message, (void*)argv);

    // Send message using socket
    iret4 = pthread_create( &threadSend, NULL, recieve_message, (void*)argv);



    /* Wait till threads are complete before main continues. Unless we */
    /* wait we run the risk of executing an exit which will terminate */
    /* the process and all threads before the threads have completed. */
    
    pthread_join( threadkeyboard, NULL);
    printf("1000\n");
    pthread_join( threadScreen, NULL);
    printf("1001\n");
    pthread_join( threadReceive, NULL);
    printf("1002\n");
    pthread_join( threadSend, NULL);
    printf("Thread Keybaord returns: %d\n",iret1);
    printf("Thread Screen returns: %d\n",iret2);
    printf("Thread Receive returns: %d\n",iret3);
    printf("Thread Send returns: %d\n",iret4);
    
    
    // free the lists
    FREE_FN free_er = free_char;
    List_free(keyboard_list, free_er);
    List_free(sending_list, free_er);

    //free the mutex's
    
    pthread_mutex_destroy(&keyboard_mutex);
    pthread_cond_destroy(&keyboard_condition_empty);
    pthread_cond_destroy(&keyboard_condition_full);
    pthread_mutex_destroy(&sending_mutex);
    pthread_cond_destroy(&sending_condition_empty);
    pthread_cond_destroy(&sending_condition_full);
    pthread_mutex_destroy(&socket_mutex);
    return 0;
    // katieTesting_function();
}