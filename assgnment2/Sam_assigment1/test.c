#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void free_int(void* pItem) {
  // cast pItem to a char pointer
  int* i = (int*) pItem;
  // free the memory allocated for the string
  free(i);
}

bool compare(void* arg1, void* arg2){
    int i = *(int*)arg1;
    int j = *(int*)arg2;

    if(i==j){
        return true;
    }
    else{
        return false;
    }
}

void print_list(List* plist){
    printf("List contains: ");
    if(plist->count != 0){
        Node* starting_current = plist->current;
        int* print = List_first(plist);
        printf("%d,", (*print));
        bool flag = true;
        while(flag){
            print = List_next(plist);
            if(print != 0){
                printf(" %d,", (*print));
            }
            else{
                flag = false;
            }
        }
        printf("\b ");
        plist->current = starting_current;
    }
    printf("\n");
}

void Test_List_create_normal(){
    List* list = List_create();
    assert(list != 0);
    FREE_FN delete = free_int;
    List_free(list, delete);
}

void Test_List_create_empty(){
    List* list = List_create();
    assert(list == 0);
}

void Test_List_count_normal(List* plist, int answer){
    int guess = List_count(plist);
    assert(guess == answer);
}

void Test_List_count_empty(List* plist, int answer){
    int guess = List_count(plist);
    assert(guess == answer); 
}

void Test_List_first_normal(List* plist, int answer){
    int* guess = List_first(plist);
    assert(*guess == answer);
}

void Test_List_first_empty(List* plist, int answer){
    int* guess = List_first(plist);
    assert(guess == 0);
}

void Test_List_last_normal(List* plist, int answer){
    int* guess = List_last(plist);
    assert(*guess == answer);
}

void Test_List_last_empty(List* plist, int answer){
    int* guess = List_last(plist);
    assert(guess == 0);
}

void Test_List_next_normal(List* plist, int answer){
    int* guess = List_next(plist);
    assert(*guess == answer);
}

void Test_List_next_empty(List* plist, int answer){
    int* guess = List_next(plist);
    assert(guess == 0);
}

void Test_List_prev_normal(List* plist, int answer){
    int* guess = List_prev(plist);
    assert(*guess == answer);
}

void Test_List_prev_empty(List* plist, int answer){
    int* guess = List_prev(plist);
    assert(guess == 0);
}

void Test_List_curr_normal(List* plist, int answer){
    int* guess = List_curr(plist);
    assert(*guess == answer);
}

void Test_List_curr_empty(List* plist, int answer){
    int* guess = List_curr(plist);
    assert(guess == 0);
}

void Test_List_curr_OOB_end(List* plist, int answer){
    int* guess = List_curr(plist);
    assert(guess == 0);
}

void Test_List_curr_OOB_start(List* plist, int answer){
    int* guess = List_curr(plist);
    assert(guess == 0);
}

void Test_List_insert_after_normal(List* plist, int answer){
    int check = List_insert_after(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_insert_after_empty(List* plist, int answer){
    int check = List_insert_after(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_insert_after_none(List* plist, int answer){
    int check = List_insert_after(plist, &answer);
    assert(check == -1);
}

void Test_List_insert_before_normal(List* plist, int answer){
    int check = List_insert_before(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_insert_before_empty(List* plist, int answer){
    int check = List_insert_before(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_insert_before_none(List* plist, int answer){
    int check = List_insert_before(plist, &answer);
    assert(check == -1);
}

void Test_List_append_normal(List* plist, int answer){
    int check = List_append(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_append_empty(List* plist, int answer){
    int check = List_append(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_append_none(List* plist, int answer){
    int check = List_append(plist, &answer);
    assert(check == -1);
    print_list(plist);
}

void Test_List_prepend_normal(List* plist, int answer){
    int check = List_prepend(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_prepend_empty(List* plist, int answer){
    int check = List_prepend(plist, &answer);
    assert(check == 0);
    print_list(plist);
}

void Test_List_prepend_none(List* plist, int answer){
    int check = List_prepend(plist, &answer);
    assert(check == -1);
    print_list(plist);
}

void Test_List_remove_normal(List* plist, int answer){
    int* guess = List_remove(plist);
    assert(*guess == answer);
}

void Test_List_remove_empty(List* plist, int answer){
    int* guess = List_remove(plist);
    assert(guess == 0);
}

void Test_List_remove_last(List* plist, int answer){
    int* guess = List_remove(plist);
    assert(*guess == answer);
}

void Test_List_trim_normal(List* plist, int answer){
    int* guess = List_trim(plist);
    assert(*guess == answer);
}

void Test_List_trim_empty(List* plist, int answer){
    int* guess = List_trim(plist);
    assert(guess == 0);
}

void Test_List_trim_last(List* plist, int answer){
    int* guess = List_trim(plist);
    assert(*guess == answer);
}

void Test_List_concat_normal(List* plist1, List* plist2){
    printf("list1: ");
    print_list(plist1);
    printf("list2: ");
    print_list(plist2);
    int size1 = List_count(plist1);
    int size2 = List_count(plist2);
    List_concat(plist1, plist2);
    int size3 = List_count(plist1);
    assert(size3 == (size1+size2));
    print_list(plist1);
}

void Test_List_concat_empty_list1(List* plist1, List* plist2){
    printf("list1: ");
    print_list(plist1);
    printf("list2: ");
    print_list(plist2);
    int size1 = List_count(plist1);
    int size2 = List_count(plist2);
    List_concat(plist1, plist2);
    int size3 = List_count(plist1);
    assert(size3 == (size1+size2));
    print_list(plist1);
}

void Test_List_concat_empty_list2(List* plist1, List* plist2){
    printf("list1: ");
    print_list(plist1);
    printf("list2: ");
    print_list(plist2);
    int size1 = List_count(plist1);
    int size2 = List_count(plist2);
    List_concat(plist1, plist2);
    int size3 = List_count(plist1);
    assert(size3 == (size1+size2));
    print_list(plist1);
}

void Test_List_free_normal(List* plist, FREE_FN pItemFreeFn){
    int cur_heads = free_heads_count();
    int cur_nodes = free_nodes_count();
    List_free(plist, pItemFreeFn);
    assert(cur_heads+1 == free_heads_count());
    assert(cur_nodes+1 == free_nodes_count());
}

void Test_List_free_empty(List* plist, FREE_FN pItemFreeFn){
    int cur_heads = free_heads_count();
    int cur_nodes = free_nodes_count();
    List_free(plist, pItemFreeFn);
    assert(cur_heads+1 == free_heads_count());
    assert(cur_nodes == free_nodes_count());
}

void Test_List_search_normal(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    int* guess = List_search(pList, pComparator, pComparisonArg);
    assert(*guess != 0);
}

void Test_List_search_OOB_start(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    int* guess = List_search(pList, pComparator, pComparisonArg);
    assert(*guess != 0);
}

void Test_List_search_OOB_end(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    int* guess = List_search(pList, pComparator, pComparisonArg);
    assert(guess == 0);
}

void Test_List_search_empty(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    int* guess = List_search(pList, pComparator, pComparisonArg);
    assert(guess == 0);
}