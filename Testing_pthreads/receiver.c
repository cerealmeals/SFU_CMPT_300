#include "receiver.h"
#include "list.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_t thread3;
static char* dynamicArray;
static unsigned int MAX_LENGTH = 100;
pthread_mutex_t mutex;
List* receive_list;

// delete when all threads combine together
void free_char2(void* pItem) {
  // cast pItem to a char pointer
  char* i = (char*) pItem;
  // free the memory allocated for the string
  free(i);
}

void* receiveThread()
{
    /*** socket thing to get the message in, don't do for now ***/
    receive_list = List_create(); // change when list shared
    if (receive_list == NULL) {
        printf("Error: receive_list is not initialized\n");
        return;
    }

    printf("18\n");
    dynamicArray = (char*)malloc(MAX_LENGTH * sizeof(char));

    printf("20\n");
    pthread_mutex_lock(&mutex);
    // TODO: condition variables....
    strcpy(dynamicArray, "hello world"); // copy message from the soket later
    List_prepend(receive_list, (void*)dynamicArray);
    pthread_mutex_unlock(&mutex);

}

void receiveThread_create()
{
    pthread_create(&thread3, NULL, receiveThread, NULL);
}



void receiveThread_close()
{
    // cancel the receive thread
    pthread_cancel(thread3);

    // wait for all to be done
    pthread_join(thread3, NULL);

    // free memory
    pthread_mutex_lock(&mutex);
    { 
        FREE_FN free_er = free_char2;
        List_free(receive_list, free_er);
    }
    pthread_mutex_unlock(&mutex);
   
}

void screenOutput_thread()
{
    
}