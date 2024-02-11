/* Create a thread for sending messages from the sender, and take it off the output list */
/* TODO: Send meassages using socket, fix the endless loop issue */
#include "list.h"
#include "receiver.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_t thread4;
static char* messageToBeSent;
static pthread_mutex_t sender_mutex = PTHREAD_MUTEX_INITIALIZER;;
List* sender_list;
pthread_cond_t condition_empty1 = PTHREAD_COND_INITIALIZER;

void* senderThread()
{
    while(1)
    {
        pthread_mutex_lock(&sender_mutex);
        if (List_count(sender_list) > 0) 
        {
            messageToBeSent = List_first(sender_list);

            // if (strcmp(messageToBeSent, "!") == 0)
            // {
            //     pthread_cancel(thread4);
            //     break;
            // } 

            printf("Sending: %s", messageToBeSent);
            List_remove(sender_list);
            // pthread_cond_wait(&condition_empty1, &sender_mutex);
        }
        
        pthread_mutex_unlock(&sender_mutex);
    }
}

void senderThread_create(List* pList)
{
    sender_list = pList;
    pthread_create(&thread4, NULL, senderThread, NULL);
}

void senderThread_close()
{
    // cancel the receive thread
    pthread_cancel(thread4);

    // wait for all to be done
    pthread_join(thread4, NULL);
}