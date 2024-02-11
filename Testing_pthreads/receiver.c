/* Create a thread for receiving messages from the sender, and put it onto the input list */

#include "list.h"
#include "receiver.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_t thread3;
static char* dynamicArray;
static unsigned int MAX_LENGTH = 100;
static pthread_mutex_t receive_mutex = PTHREAD_MUTEX_INITIALIZER;;
List* receive_list;

// free message
void free_item(void* pItem) 
{
  if (pItem)
    free(pItem);
}

void* receiveThread()
{
    // printf("22\n");
    dynamicArray = (char*)malloc(MAX_LENGTH * sizeof(char));   
     
    /*** TODO: socket thing to get the message in ***/
    char message[12] = "Hello world";
    strcpy(dynamicArray, message); // copy message from the soket later

    pthread_mutex_lock(&receive_mutex);
    // TODO: condition variables....
    List_prepend(receive_list, (void*)dynamicArray);
    // printf("%s\n",dynamicArray);
    pthread_mutex_unlock(&receive_mutex);
    
    return NULL;
}

// create receive thread, pass the input list
void receiveThread_create(List* pList)
{
    receive_list = pList;
    pthread_create(&thread3, NULL, receiveThread, NULL);
}

void receiveThread_close()
{
    // cancel the receive thread
    pthread_cancel(thread3);

    // wait for all to be done
    pthread_join(thread3, NULL);

    // free memory
    pthread_mutex_lock(&receive_mutex);
    { 
        List_free(receive_list, free_item);
    }
    pthread_mutex_unlock(&receive_mutex);
   
}
