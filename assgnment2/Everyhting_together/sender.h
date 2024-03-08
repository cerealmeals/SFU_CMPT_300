#ifndef _SENDER_H_
#define _SENDER_H_

void* senderThread();
void senderThread_create (List* pList);
void senderThread_close();

#endif