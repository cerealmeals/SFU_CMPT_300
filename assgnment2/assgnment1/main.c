#include <stdio.h>
#include <assert.h> 
#include "list.h"

void List_print(List* pList) 
{
    printf("List: ");
    Node* current = pList->head;

    if (current == NULL)
    {
        printf("Empty!");
    }
    while (current != NULL) 
    {
        if (current->item != NULL)
            printf("%d ", *((int*)current->item));
        else
            printf("NULL");
        current = current->next;
    }

    printf("\n");
}

void List_freeItem(void *pItem) 
{
    pItem = NULL;
}

bool List_compare(void *pItem, void *pComparisonArg) 
{
    if (*((int*)pItem)== *((int*)pComparisonArg))
        return true;
    return false;
}

int main()
{
    // First test case, test List_create()
    // testCreate();
    printf("*** Testing List_create()... ***\n");
    List* myList = List_create();

    List_print(myList); 

    assert(myList != NULL);
    assert(myList->current == NULL);
    assert(myList->head == NULL);
    assert(myList->tail == NULL);
    assert(myList->countNumber == 0);

    printf("List_create passed.\n");
    // printf("The value of first item is: %d \n", *((int*)myList->head->item));

    // Second test case, test List_count() when myList is empty
    printf("*** Testing List_count()... ***\n");
    assert(List_count(myList) == 0);
    printf("List_count passed when myList is empty.\n");

    // Third test case, test List_append()
    printf("*** Testing List_append()... ***\n");
    // Append first item
    int item1 = 10;
    assert(List_append(myList, &item1) == 0);
    assert(myList->countNumber == 1);
    printf("The number of nodes is: %d \n", myList->countNumber);
    printf("The value of appended item is: %d \n", *((int*)myList->tail->item));
    List_print(myList); 
    // Append second item
    int item2 = 20;
    assert(List_append(myList, &item2) == 0);
    assert(myList->countNumber == 2);
    printf("The number of nodes is: %d \n", myList->countNumber);
    printf("The value of appended item is: %d \n", *((int*)myList->tail->item)); 
    List_print(myList); 
    // Append third item
    int item3 = 5;
    assert(List_append(myList, &item3) == 0);
    assert(myList->countNumber == 3);
    printf("The number of nodes is: %d \n", myList->countNumber);
    printf("The value of appended item is: %d \n", *((int*)myList->tail->item));
    List_print(myList); 

    // Fourth test case, test List_prepend()
    printf("*** Testing List_prepend()... ***\n");
    // Prepend first item
    int item4 = 25;
    assert(List_prepend(myList, &item4) == 0);
    assert(myList->countNumber == 4);
    printf("The number of nodes is: %d \n", myList->countNumber);
    printf("The value of prepended item is: %d \n", *((int*)myList->head->item));    
    List_print(myList); 
    // Prepend second item
    int item5 = 11;
    assert(List_prepend(myList, &item5) == 0);
    assert(myList->countNumber == 5);
    printf("The number of nodes is: %d \n", myList->countNumber);
    printf("The value of prepended item is: %d \n", *((int*)myList->head->item));
    List_print(myList); 

    // Fifth test case, test List_prepend()
    printf("*** Testing List_first()... ***\n");
    printf("The value of first item is: %d \n", *((int*)myList->head->item));
    // Sixth test case, test List_curr()
    printf("*** Testing List_curr()... ***\n");
    printf("The value of current item is: %d \n", *((int*)myList->current->item));
    // 9th test case, test List_next()
    printf("*** Testing List_next()... ***\n");
    printf("The value of next item is: %d \n", *((int*)List_next(myList)));
    List_print(myList); 
    // 7th test case, test List_last()
    printf("*** Testing List_last()... ***\n");
    printf("The value of last item is: %d \n", *((int*)List_last(myList)));
    List_print(myList); 
    // 8th test case, test List_curr()
    printf("*** Testing List_curr()... ***\n");
    printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    List_print(myList); 

    // Test List_prev()
    printf("*** Testing List_prev()... ***\n");
    printf("The value of previous item is: %d \n", *((int*)List_prev(myList)));
    List_print(myList); 
    
    // Test List_insert_after()
    printf("*** Testing List_insert_after()... ***\n");
    int item6 = 121;
    void* pItem6 = &item6; 
    if (List_insert_after(myList, pItem6) == 0)
    {
        printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    }
    else
    {
        printf("*** Error! Failed to insert after the current item! ***\n");
    }
    printf("The value of previous item is: %d \n", *((int*)List_prev(myList)));
    List_print(myList);

    // Test List_insert_before()
    printf("*** Testing List_insert_before()... ***\n");
    int item7 = 72;
    void* pItem7 = &item7; 
        printf("The value of current item is: %d \n", *((int*)List_curr(myList)));

    if (List_insert_before(myList, pItem7) == 0)
    {
        printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    }
    else
    {
        printf("*** Error! Failed to insert before the current item! ***\n");
    }
    // printf("The value of previous item is: %d \n", *((int*)List_prev(myList)));
    // printf("The value of next item is: %d \n", *((int*)List_next(myList)));
    List_print(myList);

    // Test List_remove()
    printf("*** Testing remove()... ***\n");
    printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    printf("The value of removed item is: %d \n", *((int*)List_remove(myList)));
    List_print(myList); 
    printf("The value of current item is: %d \n", *((int *)(myList->current->item)));
    
    // remove the tail
    printf("*** Testing remove the tail... ***\n");
    List_print(myList);
    printf("The value of last item is: %d \n", *((int*)List_last(myList)));
    // List_print(myList); 
    printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    printf("The value of removed item is: %d \n", *((int*)List_remove(myList)));
    List_print(myList);

    // Test List_trim()
    printf("*** Testing List_trim()... ***\n");
    printf("The value of trimmed item is: %d \n", *((int*)List_trim(myList)));
    List_print(myList);

    // printf("The value of trimmed item is: %d \n", *((int*)List_trim(myList)));
    // List_print(myList);

    printf("**********************************\n");

    // Test List_concat()
    printf("*** Testing List_concat()... ***\n");
    List* list1 = List_create();
    // current null test
    printf("*** Testing when current is NULL... ***\n");
    if (List_curr(list1)==NULL)
    {
        printf("The current item is NULL! \n");
    }
    else
    {
        printf("The value of current item is: %d \n", *((int*)List_curr(myList)));
    }
    List* list2 = List_create();

    // first item null test
    if (List_first(list1)==NULL)
    {
        printf("The first item is NULL! \n");
    }
    else
    {
        printf("The value of first item is: %d \n", *((int*)List_first(myList)));
    }

    // next item null test
    if (List_next(list1)==NULL)
    {
        printf("The next item is NULL! \n");
    }
    else
    {
        printf("The value of next item is: %d \n", *((int*)List_next(myList)));
    }
    // Test case 1: Concatenate an empty list2 to an empty list1
    List_concat(list1, list2);
    printf("List1 after concatenation:\n");
    List_print(list1);
    // Test case 2: Concatenate list2 to list1
    printf("Appending items to list1 and list2... \n");
    int item8 = 10;
    int item9 = 20;
    int item10 = 30;
    int item11 = 40;
    int item12 = 50;
    List_append(list1, &item8);
    // previous null test
    printf("*** Testing when previou is NULL... ***\n");
    if (List_prev(list1)==NULL)
    {
        printf("The previous item is NULL! \n");
    }
    else
    {
        printf("The value of previous item is: %d \n", *((int*)List_next(myList)));
    }
    List_append(list1, &item9);
    List_append(list2, &item10);
    List_append(list2, &item11);
    List_append(list2, &item12);

    printf("List1 before concatenation:\n");
    List_print(list1);

    printf("List2 before concatenation:\n");
    List_print(list2);

    // Test case 3: Concatenate list2 to list1
    List_concat(list1, list2);
    printf("List1 after concatenation:\n");
    List_print(list1);
    printf("The number of list1 items is: %d \n", List_count(list1));

    // Test list_search()
    // search not found
    printf("*** Testing List_concat()... ***\n");
    int searchItem1 = 80;
    void* pSearchItem1 = &searchItem1;
    if (List_search(list1, List_compare, pSearchItem1)==NULL)
    {
        printf("The searched item is not found! \n");
    }
    else
    {
        printf("The searched item is: %d \n", *((int*)List_search(list1, List_compare, pSearchItem1)));
    }

    // search found
    int searchItem2 = 20;
    void* pSearchItem2 = &searchItem2;
    if (List_search(list1, List_compare, pSearchItem2)==NULL)
    {
        printf("The searched item is not found! \n");
    }
    else
    {
        printf("The searched item is: %d \n", *((int*)List_search(list1, List_compare, pSearchItem2)));
    }

    // Test List_free()
    printf("*** Testing List_free()... ***\n");
    List_free(list1, *List_freeItem);
    printf("** Freeing list1... ** \n");
    List_print(list1);

    List_free(list2, *List_freeItem);
    printf("** Freeing list2... ** \n");
    List_print(list2);

    List_free(myList, *List_freeItem);
    printf("** Freeing myList... ** \n");
    List_print(myList);

}