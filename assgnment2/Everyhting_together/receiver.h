#ifndef _RECEIVER_H_
#define _RECEIVER_H_

void* receiveThread();
void receiveThread_create (List* pList);
void receiveThread_close();

#endif