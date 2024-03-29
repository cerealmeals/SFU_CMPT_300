#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 

#define port 12345
#define MAX_LENGTH 1024

int main(int argc, char *argv[])
{
    // initialize
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    // printf("%d \n", addr.sin_addr.s_addr);
    memset(&addr.sin_zero, 0, sizeof(addr));

    // create a socket for UDP
    int s = socket(AF_INET, SOCK_DGRAM, 0);

    // bind the socket and the port
    bind(s, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    //char messageRx[MAX_LENGTH];

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
            ferror("error! \n");
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

        printf("Received message is %d bytes: %s", receivedBytes, messageRx);

        // send messages
        char messageSend[MAX_LENGTH];
        sendto(s, &messageSend, strlen(messageSend), 0, (struct sockaddr *)&addrRomote, sizeof(struct sockaddr_in));
        printf("sending message is: %s \n", messageSend);

    }while(1);
    close(s);
}
