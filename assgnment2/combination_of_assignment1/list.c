#include <stdio.h>
#include <assert.h>
#include "list.h"

// define nodes and list array, should be static
static Node nodeArray[LIST_MAX_NUM_NODES];
static List listArray[LIST_MAX_NUM_HEADS];

// define the numbers of lists and nodes that have been used
static unsigned int countNotEmptyNodes = 0;
static unsigned int listsUsedCount = 0;

static int initialized = 0;

// Makes a new, empty list, and returns its reference on success.
// Returns a NULL pointer on failure.
List *List_create()
{
    if (!initialized)
    {
        for (unsigned int i = 0; i < LIST_MAX_NUM_HEADS; ++i)
        {
            listArray[i].current = NULL;
            listArray[i].head = NULL;
            listArray[i].tail = NULL;
            listArray[i].countNumber = 0;
            listArray[i].boundCheck = LIST_OOB_START;
        }

        for (unsigned int i = 0; i < LIST_MAX_NUM_NODES; ++i)
        {
            nodeArray[i].item = NULL;
            nodeArray[i].previous = NULL;
            nodeArray[i].next = NULL;
        }
        initialized = 1;
    }
    // If there is still space 
    if (listsUsedCount < LIST_MAX_NUM_HEADS)
    {
        // printf("countNotEmptyLists: %d \n", listsUsedCount);

        List *createList = &listArray[listsUsedCount];
        createList->current = NULL;
        createList->head = NULL;
        createList->tail = NULL;
        createList->countNumber = 0;
        createList->boundCheck = LIST_OOB_START;
        listsUsedCount++;
        return createList;
    }
    return NULL;
}

// Returns the number of items in pList.
int List_count(List *pList)
{
    assert(pList != NULL);
    return pList->countNumber;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_first(List *pList)
{
    assert(pList != NULL);

    if (pList->head == NULL)
    {
        pList->current = NULL;
        return NULL;
    }

    pList->current = pList->head;
    return pList->head->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_last(List *pList)
{
    assert(pList != NULL);

    if (pList->head == NULL)
    {
        pList->current = NULL;
        return NULL;
    }

    pList->current = pList->tail;
    return pList->tail->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond end of pList.
void *List_next(List *pList)
{
    if (pList->current == NULL)
    {
        pList->boundCheck = LIST_OOB_END;
        return NULL;
    }
    if (pList->current->next == NULL)
    {
        pList->boundCheck = LIST_OOB_END;
        return NULL;
    }
    // pList->boundCheck = LIST_OOB_START;
    pList->current = pList->current->next;
    return pList->current->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void *List_prev(List *pList)
{
    if (pList->current == pList->head || pList->head == NULL)
    {
        pList->current = NULL;
        pList->boundCheck = LIST_OOB_START;
        return NULL;
    }
    pList->current = pList->current->previous;
    return pList->current->item;
}

// Returns a pointer to the current item in pList.
void *List_curr(List *pList)
{
    if (pList->current != NULL)
        return pList->current->item;
    return NULL;
}

// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_after(List *pList, void *pItem)
{
    assert(pList != NULL);

    // First case: the list is empty, add it as the head
    if (pList == NULL)
    {
        Node *newNode = &nodeArray[pList->countNumber];

        if (!newNode)
            return LIST_FAIL;

        newNode->item = pItem;
        newNode->next = NULL;
        newNode->previous = NULL;

        pList->current = newNode;
        pList->head = newNode;
        pList->tail = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }

    // Second case, the list is not empty, current is NULL or current is the tail
    if (pList->current->next != NULL || pList->current == NULL)
    {
        Node *newNode = &nodeArray[pList->countNumber];

        if (!newNode)
            return LIST_FAIL;

        newNode->item = pItem;
        newNode->next = NULL;
        newNode->previous = pList->tail;

        pList->current = newNode;
        pList->tail->next = newNode;
        pList->tail = newNode;
        pList->countNumber++;
        countNotEmptyNodes++; 

        return LIST_SUCCESS;
    }

    // Third: add after the current item
    Node *newNode = &nodeArray[pList->countNumber];

    if (!newNode)
        return LIST_FAIL;

    newNode->item = pItem;
    newNode->next = pList->current->next;
    newNode->previous = pList->current;

    pList->current->next = newNode; 
    pList->current = newNode;

    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_before(List *pList, void *pItem)
{
    // printf("208 \n");
    assert(pList != NULL);

    // Check available spaces
    if (countNotEmptyNodes < LIST_MAX_NUM_NODES)
    {
        // First case: the list is empty, add it as the head
        if (pList->head == NULL)
        {
            Node *newNode = &nodeArray[pList->countNumber];

            if (!newNode)
                return LIST_FAIL;

            newNode->item = pItem;
            newNode->next = NULL;
            newNode->previous = NULL;

            pList->current = newNode;
            pList->head = newNode;
            pList->tail = newNode;
            pList->countNumber++;
            countNotEmptyNodes++;
            // printf("209\n");
            return LIST_SUCCESS;
        }
        // printf("235\n");
        // Second case: If the current pointer is before the start of the pList, the item is added at the start.
        if (pList->current->previous == NULL)
        {
            // printf("214\n");
            Node *newNode = &nodeArray[pList->countNumber];

            newNode->item = pItem;
            newNode->next = pList->head;
            newNode->previous = NULL;

            pList->current = newNode;
            pList->head = newNode;
            pList->countNumber++;
            countNotEmptyNodes++;

            return LIST_SUCCESS;
        }

        // Third case: If the current pointer is beyond the end of the pList, the item is added at the end.
        if (pList->current->previous->next == NULL)
        {
            Node *newNode = &nodeArray[pList->countNumber];
            // printf("235\n");

            if (!newNode)
                return LIST_FAIL;

            newNode->item = pItem;
            newNode->next = NULL;
            newNode->previous = pList->tail;

            pList->current = newNode;
            pList->tail = newNode;
            pList->countNumber++;
            countNotEmptyNodes++;

            return LIST_SUCCESS;
        }
        // printf("274\n");

        if (pList->current == pList->tail)
        {
            Node *newNode = &nodeArray[pList->countNumber];

            if (!newNode)
                return LIST_FAIL;

            newNode->item = pItem;
            newNode->next = NULL;
            newNode->previous = pList->tail;

            pList->current = newNode;
            pList->tail = newNode;
            pList->countNumber++;
            countNotEmptyNodes++;

            return LIST_SUCCESS;
        }
        // Fourth case: Adds item to pList directly before the current item
        Node *newNode = &nodeArray[pList->countNumber];
        newNode->item = pItem;
        newNode->previous = pList->current->previous;
        newNode->next = pList->current;

        pList->current->previous = newNode;

        if (newNode->previous != NULL)
            newNode->previous->next = newNode;

        pList->current = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }

    return LIST_FAIL;
}

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List *pList, void *pItem)
{
    assert(pList != NULL);

    if (pList->head == NULL)
    {
        Node *newNode = &nodeArray[countNotEmptyNodes];

        newNode->item = pItem;
        newNode->next = NULL;
        newNode->previous = NULL;

        pList->current = newNode;
        pList->tail = newNode;
        pList->head = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }
    else
    {
        Node *newNode = &nodeArray[countNotEmptyNodes];

        newNode->item = pItem;
        newNode->next = NULL;
        pList->tail->next = newNode;
        newNode->previous = pList->tail;

        pList->current = newNode;
        pList->tail = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }

    return LIST_FAIL;
}

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List *pList, void *pItem)
{
    assert(pList != NULL);

    if (pList->head == NULL)
    {
        Node *newNode = &nodeArray[pList->countNumber];

        if (!newNode)
            return LIST_FAIL;

        newNode->item = pItem;
        newNode->next = NULL;
        newNode->previous = NULL;

        pList->current = newNode;
        pList->tail = newNode;
        pList->head = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }
    else
    {
        Node *newNode = &nodeArray[pList->countNumber];

        if (!newNode)
            return LIST_FAIL;

        newNode->item = pItem;
        newNode->next = pList->head;
        newNode->previous = NULL;

        pList->current = newNode;
        pList->head = newNode;
        pList->countNumber++;
        countNotEmptyNodes++;

        return LIST_SUCCESS;
    }

    return LIST_FAIL;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void *List_remove(List *pList)
{
    assert(pList != NULL);
    if (pList->head == NULL || pList->current == NULL || pList->current == pList->tail->next)
    {
        return NULL;
    }
    Node *removeNode = pList->current;
    void *removeItem = removeNode->item;
    // return removeItem;
    if (pList->countNumber == 1)
    {
        pList->head = NULL;
        pList->tail = NULL;
        pList->current->item = NULL;
        pList->current = NULL;
        pList->countNumber--;
        countNotEmptyNodes--;
    }
    if (removeNode == pList->head)
    {
        // printf("666 \n");
        pList->head = removeNode->next;
        if (pList->head != NULL)
            pList->head->previous = NULL;
        pList->current->item = NULL;
        pList->current = NULL;
        pList->countNumber--;
        countNotEmptyNodes--;
    }
    if (removeNode == pList->tail)
    {
        // printf("555 \n");
        pList->tail = removeNode->previous;
        if (pList->tail != NULL)
            pList->tail->next = NULL;
        pList->current->item = NULL;
        pList->current = NULL;
        pList->countNumber--;
        countNotEmptyNodes--;
    }
    else
    {
        // printf("222 \n");
        // if (removeNode->previous != NULL)
        //{
        // printf("111 \n");
        pList->current->previous->next = removeNode->next;
        //}
        if (removeNode->next != NULL)
        {
            pList->current->next->previous = removeNode->previous;
        }
        pList->current = removeNode->next;

        removeNode->item = NULL;
        removeNode->next = NULL;
        removeNode->previous = NULL;
        pList->countNumber--;
        countNotEmptyNodes--;
        return removeItem;
    }

    return removeItem;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void *List_trim(List *pList)
{
    if (pList == NULL || pList->head == NULL)
        return NULL;

    Node *lastNode = pList->tail;
    void *lastItem = lastNode->item;

    if (lastNode->previous == NULL)
    {
        pList->head = NULL;
        pList->tail = NULL;
    }
    else
    {
        lastNode->previous->next = NULL;
        pList->tail = lastNode->previous;
    }

    pList->current = pList->tail;
    pList->countNumber--;
    return lastItem;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List *pList1, List *pList2)
{
    assert(pList1 != NULL);
    assert(pList2 != NULL);

    if (pList1->head == NULL)
    {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->countNumber = pList2->countNumber;
    }
    else
    {
        pList1->tail->next = pList2->head;
        if (pList2->head != NULL)
        {
            pList2->head->previous = pList1->tail;
        }
        pList1->tail = pList2->tail;
        pList1->countNumber += pList2->countNumber;
    }

    // delete list2 
    pList2->head = NULL;
    pList2->tail = NULL;
    pList2->current = NULL;
    pList2->countNumber = 0;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
typedef void (*FREE_FN)(void *pItem);
void List_free(List *pList, FREE_FN pItemFreeFn)
{
    assert(pList != NULL);

    Node *trackNode = pList->head;

    while (trackNode != NULL)
    {
        Node *nextNode = trackNode->next;

        if (pItemFreeFn != NULL)
        {
            (*pItemFreeFn)(trackNode->item);
        }

        trackNode->item = NULL;
        trackNode->previous = NULL;
        trackNode->next = NULL;

        trackNode = nextNode;
    }

    pList->head = NULL;
    pList->tail = NULL;
    pList->current = NULL;
    pList->countNumber = 0;
}

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
//
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void *pItem, void *pComparisonArg);
void *List_search(List *pList, COMPARATOR_FN pComparator, void *pComparisonArg)
{
    assert(pList != NULL);
    assert(pComparator != NULL);

    Node *startNode = NULL;

    if (pList->current == NULL || pList->boundCheck == LIST_OOB_START)
    {
        startNode = pList->head;
    }
    else
    {
        startNode = pList->current;
    }

    Node *currentNode = startNode;

    while (currentNode != NULL)
    {
        if (pComparator(currentNode->item, pComparisonArg))
        {
            pList->current = currentNode;
            return currentNode->item;
        }

        currentNode = currentNode->next;
    }
    pList->current = NULL;
    return NULL;
}