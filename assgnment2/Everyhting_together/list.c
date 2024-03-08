#include "list.h"
#include <assert.h>
#include <stdio.h>
//#include <cstddef>

Node nodes[LIST_MAX_NUM_NODES];
List heads[LIST_MAX_NUM_HEADS];



List free_nodes;
Free_Heads free_heads;
int free_nodes_count(){
    return free_nodes.count;
}
int free_heads_count(){
    return free_heads.count;
}
Node* new_node(){
    //printf("New_node: number of free nodes: %d\n", free_nodes.count-1);
    //check if this is the last node if yes set last to 0
    if(free_nodes.first == free_nodes.last){
        free_nodes.last = 0;
    }
    //pick the first node in the list
    Node* to_return = free_nodes.first;
    //if the list was empty return a 0 pointer
    if(to_return == 0){
        return to_return;
    }
    
    //set the new first node and make sure it has no prev
    free_nodes.first = free_nodes.first->next;
    
    if(free_nodes.first != 0){
        free_nodes.first->prev = 0;
    }
    //deacrement the count of free nodes
    free_nodes.count--; 
    
    //reset the node
    to_return->next = 0;
    to_return->pointer = 0;
    return to_return;
}

void return_node(Node* node){
    if(free_nodes.count == 0){
        free_nodes.last = node;
        node->next = 0;
    }
    else{
        free_nodes.first->prev = node;
        node->next = free_nodes.first;
    }
    node->prev = 0;
    node->pointer = 0;
    free_nodes.first = node;
    free_nodes.count++;
}

void return_head(List* plist){
    List* temp = free_heads.current;
    free_heads.current = plist;
    plist->next = temp;
    plist->last = 0;
    plist->first = 0;
    plist->count = 0;
    plist->current = 0;

    free_heads.count++;
}

void first_node(List* pList, Node* new){
    pList->current = new;
    pList->first = new;
    pList->last = new;
    new->next = 0;
    new->prev = 0;
}

List* List_create(){
    static bool first = true;
    if(first){
        // initialize heads and list them in the free_heads header
        free_heads.count = LIST_MAX_NUM_HEADS;
        free_heads.current = &(heads[0]);
        int i = 0;
        for(; i < LIST_MAX_NUM_HEADS-1; i++){
            heads[i].count = 0;
            heads[i].current = 0;
            heads[i].first = 0;
            heads[i].last = 0;
            heads[i].next = &(heads[i+1]);
        }
        heads[i].count = 0;
        heads[i].current = 0;
        heads[i].first = 0;
        heads[i].last = 0;
        heads[i].next = 0;

        // initialize nodes and list them in the free_nodes header
        free_nodes.count = LIST_MAX_NUM_NODES;
        free_nodes.next = 0;
        i = 0;
        nodes[i].next = &(nodes[i+1]);
        nodes[i].prev = 0;
        free_nodes.first = &(nodes[i]);
        i++;
        for(; i < LIST_MAX_NUM_NODES-1; i++){
            nodes[i].pointer = 0;
            nodes[i].next = &(nodes[i+1]);
            nodes[i].prev = &(nodes[i-1]);

        }
        nodes[i].next = 0;
        nodes[i].prev = &(nodes[i-1]);
        free_nodes.last = &(nodes[i]);

        first = false;
    }

    if(free_heads.count != 0){
        List* to_return = free_heads.current;
        free_heads.current = free_heads.current->next;
        free_heads.count--;
        //printf("List_create: number of free heads: %d\n",free_heads.count);
        return to_return;
    }
    else{
        return 0;
    }
}

int List_count(List* pList){
    return pList->count;
}

void* List_first(List* pList){
    if(pList->count == 0){
        return 0;
    }
    pList->current = pList->first;
    return pList->first->pointer;
}

void* List_last(List* pList){
    if(pList->count == 0){
        return 0;
    }
    pList->current = pList->last;
    return pList->last->pointer;
}

void* List_next(List* pList){
    if(pList->count == 0){
        return 0;
    }
    if(pList->current == LIST_OOB_START){
        pList->current = pList->first;
    }
    else if(pList->current == LIST_OOB_END){
        pList->current = 0;
    }
    else{
        pList->current = pList->current->next;
    }
    if(pList->current == 0){
        pList->current = LIST_OOB_END;
        return 0;
    }
    return pList->current->pointer;
}

void* List_prev(List* pList){
    if(pList->count == 0){
        return 0;
    }
    if(pList->current == LIST_OOB_START){
        pList->current = 0;
    }
    else if(pList->current == LIST_OOB_END){
        pList->current = pList->last;
    }
    else{
        pList->current = pList->current->prev;
    }
    if(pList->current == 0){
        pList->current = LIST_OOB_START;
        return 0;
    }
    return pList->current->pointer;
}

void* List_curr(List* pList){
    if(pList->current == LIST_OOB_END||pList->current == LIST_OOB_START){
        return 0;
    }
    else if(pList->current == 0){
        return 0;
    }
    return pList->current->pointer;
}

int List_insert_after(List* pList, void* pItem){
    Node* new = new_node();
    if(new == 0){
        return LIST_FAIL;
    }
    new->pointer = pItem;

    if(pList->count == 0){
        first_node(pList, new);
    }
    else{
        new->next = pList->current->next;
        new->prev = pList->current;
        if(new->next != 0){
            pList->current->next->prev = new;
        }
        pList->current->next = new;
        pList->current = new;
        if(new->next == 0){
            pList->last = new;
        }
    }
    pList->count++;

    return LIST_SUCCESS;
}

int List_insert_before(List* pList, void* pItem){
    
    Node* new = new_node();
    if(new == 0){
        return LIST_FAIL;
    }
    new->pointer = pItem;
    if(pList->count == 0){
        first_node(pList, new);
    }
    else{
        new->next = pList->current;
        new->prev = pList->current->prev;
        if(new->prev != 0){
            pList->current->prev->next = new;
        }
        pList->current->prev = new;
        pList->current = new;
        if(new->prev == 0){
            pList->first = new;
        }
    }
    pList->count++;
    

    return LIST_SUCCESS;
}

int List_append(List* pList, void* pItem){
    Node* new = new_node();
    if(new == 0){
        return LIST_FAIL;
    }
    new->pointer = pItem;
    if(pList->count == 0){
        first_node(pList, new);
    }
    else{
        new->next = 0;
        new->prev = pList->last;
        pList->last->next = new;
        pList->last = new;
        pList->current = new;
    }
    
    pList->count++;
    
    return LIST_SUCCESS;
}

int List_prepend(List* pList, void* pItem){
    Node* new = new_node();
    if(new == 0){
        return LIST_FAIL;
    }
    new->pointer = pItem;
    if(pList->count == 0){
        first_node(pList, new);
    }
    else{
        new->prev = 0;
        new->next = pList->first;
        pList->first->prev = new;
        pList->first = new;
        pList->current = new;
    }
    pList->count++;

    return LIST_SUCCESS;
}

void* List_remove(List* pList){
    if(pList->current == LIST_OOB_START||pList->current == LIST_OOB_END){
        return 0;
    }
    if(pList->current == 0){
        return 0;
    }
    
    void* to_return = pList->current->pointer;
    Node* temp = pList->current;
    if(temp->prev != 0){
        temp->prev->next = temp->next;
    }
    else{
        pList->first = temp->next;
    }
    if(temp->next != 0){
        temp->next->prev = temp->prev;
    }
    else{
        pList->last = temp->prev;
    }

    pList->current = temp->next;
    pList->count--;
    if(pList->current == 0){
        pList->current = LIST_OOB_END;
    } 

    return_node(temp);
    return to_return;
}

void* List_trim(List* pList){
    if(pList->last == 0){
        return 0;
    }

    void* to_return = pList->last->pointer;
    Node* temp = pList->last;
    pList->last = temp->prev;
    if(pList->last != 0){
        pList->last->next = 0;
    }
    else{
        pList->first = 0;
    }
    pList->current = pList->last;
    pList->count--;

    return_node(temp);
    return to_return;
    
}

void List_concat(List* pList1, List* pList2){
    assert(pList1 != pList2);
    if(pList1->count!= 0){
        pList1->last->next = pList2->first;
    }
    else{
        pList1->first = pList2->first;
        pList1->last = pList2->last;
    }
    if(pList2->count != 0){
        pList2->first->prev = pList1->last;
        pList1->last = pList2->last;
    }
    

    pList1->count += pList2->count;
    
    return_head(pList2);
}

void List_free(List* pList, FREE_FN pItemFreeFn){
    while (pList->current != 0){
        Node* temp = pList->current;
        (*pItemFreeFn)(temp->pointer);
        pList->current = pList->current->next;
        return_node(temp);
    }
    return_head(pList);
}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    if(pList->current == LIST_OOB_START){
        pList->current = pList->first;
    }
    else if(pList->current == LIST_OOB_END){
        return 0;
    }

    while(pList->current != 0){
        if((*pComparator)(pList->current->pointer, pComparisonArg)){
            return pList->current->pointer;
        }
        pList->current = pList->current->next;
    }
    pList->current = LIST_OOB_END;
    return 0;
}