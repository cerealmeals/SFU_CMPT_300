/* Create a thread for sending messages from the sender, and take it off the output list */
/* TODO: Send meassages using socket, fix the endless loop issue */
#include "list.h"
#include "receiver.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> 
static pthread_t thread4;
static char* messageToBeSent;
static pthread_mutex_t sender_mutex = PTHREAD_MUTEX_INITIALIZER;;
List* sender_list;
pthread_cond_t condition_empty1 = PTHREAD_COND_INITIALIZER;
static bool flag = true;
static int s;

void* senderThread()
{
    while(flag)
    {
        pthread_mutex_lock(&sender_mutex);
    
        if (List_count(sender_list) > 0) 
        {
            messageToBeSent = (char*)List_first(sender_list);
            if (*messageToBeSent == '!')
            {
                pthread_mutex_unlock(&sender_mutex);
                printf("Sender thread received termination signal.\n");
                flag = false;
                break;
            } 

            printf("Sending: %s", messageToBeSent);
            
            // socket stuff to send messages
            // struct sockaddr_in addrRomote;
            // sendto(s, &messageToBeSent, strlen(messageToBeSent), 0, (struct sockaddr *)&addrRomote, sizeof(struct sockaddr_in));
            
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