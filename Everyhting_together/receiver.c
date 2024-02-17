/* Create a thread for receiving messages from the sender, and put it onto the input list */

#include "list.h"
#include "receiver.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define port 1234
#define MAX_LENGTH 1024

static pthread_t thread3;
static char* dynamicArray;
static unsigned int MAX_LENGTH = 100;
static pthread_mutex_t receive_mutex = PTHREAD_MUTEX_INITIALIZER;;
List* receive_list;
static int socket;

// free message
void free_item(void* pItem) 
{
  if (pItem)
    free(pItem);
}

void* receiveThread()
{
    // initialize
    struct sockaddr_in addr;    
    memset(&addr.sin_zero, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    // printf("22\n");
    dynamicArray = (char*)malloc(MAX_LENGTH * sizeof(char));   
     // create a socket for UDP
    socket = socket(AF_INET, SOCK_DGRAM, 0);

    do
    {
        struct sockaddr_in addrRomote;
        char messageRx[MAX_LENGTH];

        // receive messages
        // Return the number of bytes
        unsigned int i =  sizeof(struct sockaddr_in);
        int receivedBytes = recvfrom(s, &messageRx, MAX_LENGTH, 0, (struct sockaddr *)&addrRomote, &i);
        if (receivedBytes==-1)
        {
            perror("error! \n");
        }

        // make the string terminated
        int terminated;
        if (receivedBytes<MAX_LENGTH)
        {
            terminated = receivedBytes;
        }
        else
        {
            terminated = MAX_LENGTH-1;
        }
        messageRx[terminated] = 0;

        printf("Received message is %d bytes: %s", receivedBytes, messageRx);
    }while(1);

    strcpy(dynamicArray, messageRx); // shallow copy message from the socket 

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
