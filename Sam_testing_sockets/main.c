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
    char *hostname;
    size_t size = 264;

    gethostname(hostname, size);

    printf("%s\n", hostname);


    struct addrinfo hints, *res;
    int sockfd;

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(NULL, hton(port), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // make a socket:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // bind the socket and the port
    bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    char messageRx[MAX_LENGTH];

    do
    {
        struct sockaddr_in addrRomote;
        

        // receive messages
        // Return the number of bytes
        unsigned int i =  sizeof(struct sockaddr_in);
        int receivedBytes = recvfrom(sockfd, &messageRx, MAX_LENGTH, 0, (struct sockaddr *)&addrRomote, &i);

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
        char messageSend[MAX_LENGTH];
        messageSend[0] = 'R';
        messageSend[1] = '\0';
        sendto(sockfd, &messageSend, strlen(messageSend), 0, (struct sockaddr *)&addrRomote, sizeof(struct sockaddr_in));
        printf("sending message is: %s \n", messageSend);

       
    }while(messageRx[0] != '!');
  close(s);
}
