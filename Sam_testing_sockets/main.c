#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 

#define port 12345
#define MAX_LENGTH 1024

int main(int argc, char *argv[]){
    char hostname[100];
    size_t size = 100;
    gethostname(hostname, size);
    
    printf("%s\n", hostname);
    struct addrinfo hints, *results;
    int sockme, sockyou, status;

    // first, load up addresultss structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;
    // hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(hostname, argv[1], &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    
    // make a socket:

    sockme = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    // bind the socket and the port
    bind(sockme, results->ai_addr, results->ai_addrlen);
    
    freeaddrinfo(results); // free the linked list
    
    if ((status = getaddrinfo(argv[2], argv[3], &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    


    sockyou = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    
    struct sockaddr* addrRomote = (results->ai_addr);

    freeaddrinfo(results); // free the linked list
    
    char messageRx[] = "testing testing";
    for(int k = 0; k < 5; k++)
    {
        
        // receive messages
        // Return the number of bytes
        unsigned int i =  sizeof(struct sockaddr_in);
        int receivedBytes = 0;
        if(k != 0){
            receivedBytes = recvfrom(sockyou, &messageRx, strlen(messageRx), 0, addrRomote, &i);
        }
        
        // make it terminated
        // messageRx[receivedBytes] = '\0';
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
        printf("Received message is %d bytes: %s \n", receivedBytes, messageRx);

        // send messages
        char messageSend[16];
        printf("Type your message, less then 15 characters\n");
        scanf("%15s", &messageSend);
        sendto(sockme, &messageSend, strlen(messageSend), 0, addrRomote, sizeof(struct sockaddr_in));
        printf("sending message is: %s \n", messageSend);

       
    }
  close(sockme);
  close(sockyou);
}
