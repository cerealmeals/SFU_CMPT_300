#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include "list.h"
#include "sleep.h"
#include <errno.h>

#define MAX_BUFFER 256
List* receiving_list;
List* sending_list;
pthread_mutex_t receiving_mutex;
pthread_cond_t receiving_condition_empty;
pthread_cond_t receiving_condition_full;
pthread_mutex_t sending_mutex;
pthread_cond_t sending_condition_empty;
pthread_cond_t sending_condition_full;
int stop;

struct Thread_args{
    struct sockaddr addr;
    int *socket_desc;
};

void free_char(void* pItem) {
  // cast pItem to a char pointer
  char* i = (char*) pItem;
  // free the memory allocated for the string
  free(i);
}

void *print_to_screen(){
    do{
        pthread_mutex_lock(&receiving_mutex);
        if(List_count(receiving_list)==0)
        {
            pthread_cond_wait(&receiving_condition_empty, &receiving_mutex);
        }
        char * message = List_trim(receiving_list);
        pthread_cond_signal(&receiving_condition_full);
        pthread_mutex_unlock(&receiving_mutex);
        
        if (message!=NULL && message[0] != '!')
        {
            printf("Message from the other side: ");  
            printf("%s", message);
            // for(int j = 0; j < strlen(message); j++)
            // {
            //     putchar(message[j]);
            //     fflush(stdout);
            //     sleep_sec2(100);
            // }
        }
        
        free(message);
        
    }while(stop);
}

void *get_user_input(){
    char * input;
    do{
        input = (char*)malloc(MAX_BUFFER);
        fgets(input, MAX_BUFFER, stdin);

        // make sure there is space on the sending list 
        // and more sure no one is using it right now
        // then prepend the message to the list
        pthread_mutex_lock(&sending_mutex);
        if(List_count(sending_list)== LIST_MAX_NUM_NODES)       
        {
            pthread_cond_wait(&sending_condition_full, &sending_mutex);
        }
        List_prepend(sending_list, (void*)input);
        pthread_cond_signal(&sending_condition_empty);
        pthread_mutex_unlock(&sending_mutex);

        if((input[0] == '!')&&(strlen(input)==2)){
            stop = 0;
            pthread_cond_signal(&sending_condition_empty);
            pthread_cond_signal(&sending_condition_full);
            pthread_cond_signal(&receiving_condition_empty);
            pthread_cond_signal(&receiving_condition_full);
        }

    }while(stop);
}

void *send_message(void *arg){
    struct Thread_args *send = (struct Thread_args *) arg;
    int socket_desc = *(send->socket_desc);
    struct sockaddr sending_addr = send->addr;
    do{
        // take something off the list
        pthread_mutex_lock(&sending_mutex);
        if(List_count(sending_list)==0){
            pthread_cond_wait(&sending_condition_empty, &sending_mutex);
        }
        char * message = List_trim(sending_list);
        pthread_cond_signal(&sending_condition_full);
        pthread_mutex_unlock(&sending_mutex);

        // send the message over the socket to the other address
        if(sendto( socket_desc, message, MAX_BUFFER, 0, &(sending_addr), sizeof(sending_addr)) < 0){
        printf("S-talk is closed!\n");
        }

        free(message);

    }while(stop);
    
}

void *receive_message(void * arg){
    
    char * receive;
    int socket_desc = *((int*)(arg));
    do{

        // receiving a message from the socket
        receive = (char*) malloc(MAX_BUFFER);
        
        recvfrom(socket_desc, receive, MAX_BUFFER, 0, NULL, NULL);
        //fprintf(stderr, "recvfrom: %s(%d)\n", strerror(errno), errno);

        // put the message on the list
        pthread_mutex_lock(&receiving_mutex);
        if(List_count(receiving_list)== LIST_MAX_NUM_NODES){
            pthread_cond_wait(&receiving_condition_full, &receiving_mutex);
        }
        List_prepend(receiving_list, (void*)receive);
        pthread_cond_signal(&receiving_condition_empty);
        pthread_mutex_unlock(&receiving_mutex);

        if((receive[0] == '!')&&(strlen(receive)==2)){
            stop = 0;
            pthread_cond_signal(&sending_condition_empty);
            pthread_cond_signal(&sending_condition_full);
            pthread_cond_signal(&receiving_condition_empty);
            pthread_cond_signal(&receiving_condition_full);
        }

    }while(stop);
}

int main(int argc, char *argv[]) {

    if(argc != 4){
        printf("not the right number of arguments");
        return -1;
    }
    //initialize the lists being used
    receiving_list = List_create();
    sending_list = List_create();

    // initialize the mutex's and conditions being used
    pthread_mutex_init(&receiving_mutex, NULL);
    pthread_cond_init(&receiving_condition_empty, NULL);
    pthread_cond_init(&receiving_condition_full, NULL);
    pthread_mutex_init(&sending_mutex, NULL);
    pthread_cond_init(&sending_condition_empty, NULL);
    pthread_cond_init(&sending_condition_full, NULL);

    printf("********** Let's talk! **********\n");

    //socket stuff

    // initialzie socket stuff
    char hostname[50];
    size_t size = 50;
    gethostname(hostname, size);
    //printf("hostname: %s\n", hostname);

    int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    // initialize
    struct sockaddr receiving_addr, sending_addr;
    struct addrinfo hints, *results;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    
    if ((status = getaddrinfo(hostname, argv[1], &hints, &results)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    receiving_addr = *(results->ai_addr);

    freeaddrinfo(results);
    
    if(bind(socket_desc, (struct sockaddr*)&receiving_addr, sizeof(receiving_addr)) < 0){
        printf("Couldn't bind to the port\n");
    }

    if ((status = getaddrinfo(argv[2], argv[3], &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    
    sending_addr = *(results->ai_addr);

    freeaddrinfo(results);

    //threads being used
    pthread_t threadUserImput, threadScreen, threadReceive, threadSend;
    
    stop = 1;

    /* Create independent threads each of which will execute function */
    // gets user input and but message in receiving_list
    pthread_create( &threadUserImput, NULL, get_user_input, NULL);

    // Receive message from socket
    int *arguments = (int*)malloc(sizeof(socket_desc));
    *arguments = socket_desc;
    
    struct Thread_args send;
    send.addr = sending_addr;
    send.socket_desc = arguments;
    pthread_create( &threadSend, NULL, send_message, (void*)&send);
    
    // get message from list and put on screen // change 1
    pthread_create( &threadReceive, NULL, receive_message, (void*)arguments);

    // Send message using socket // change 2
    pthread_create( &threadScreen, NULL, print_to_screen, NULL);

    /* Wait till threads are complete before main continues. Unless we */
    /* wait we run the risk of executing an exit which will terminate */
    /* the process and all threads before the threads have completed. */
    
    
    pthread_join( threadScreen, NULL); 
    if(stop == 0)
    {
        pthread_cancel(threadReceive);
        pthread_cancel(threadUserImput);
    }
    pthread_join( threadUserImput, NULL);
    pthread_join( threadReceive, NULL);  
    pthread_join( threadSend, NULL);
    free(arguments);
    close(socket_desc);
    
    
    // free the lists
    FREE_FN free_er = free_char;
    List_free(receiving_list, free_er);
    List_free(sending_list, free_er);

    //free the mutex's 
    pthread_mutex_destroy(&receiving_mutex);
    pthread_cond_destroy(&receiving_condition_empty);
    pthread_cond_destroy(&receiving_condition_full);
    pthread_mutex_destroy(&sending_mutex);
    pthread_cond_destroy(&sending_condition_empty);
    pthread_cond_destroy(&sending_condition_full);
    
    return 0;
}