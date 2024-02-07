#include "list.h"

void print_list(List* plist);

bool compare(void* arg1, void* arg2);

void free_int(void* pItem);
//
void Test_List_create_normal();
//
void Test_List_create_empty();
//
void Test_List_count_normal(List* plist, int answer);
//
void Test_List_count_empty(List* plist, int answer);
//
void Test_List_first_normal(List* plist, int answer);
//
void Test_List_first_empty(List* plist, int answer);
//
void Test_List_last_normal(List* plist, int answer);
//
void Test_List_last_empty(List* plist, int answer);
//
void Test_List_next_normal(List* plist, int answer);
//
void Test_List_next_empty(List* plist, int answer);
//
void Test_List_prev_normal(List* plist, int answer);
//
void Test_List_prev_empty(List* plist, int answer);
//
void Test_List_curr_normal(List* plist, int answer);
//
void Test_List_curr_empty(List* plist, int answer);
//
void Test_List_curr_OOB_end(List* plist, int answer);
//
void Test_List_curr_OOB_start(List* plist, int answer);
//
void Test_List_insert_after_normal(List* plist, int answer);
//
void Test_List_insert_after_empty(List* plist, int answer);
//
void Test_List_insert_before_normal(List* plist, int answer);
//
void Test_List_insert_before_empty(List* plist, int answer);
//
void Test_List_append_normal(List* plist, int answer);
//
void Test_List_append_empty(List* plist, int answer);
//
void Test_List_prepend_normal(List* plist, int answer);
//
void Test_List_prepend_empty(List* plist, int answer);
//
void Test_List_remove_normal(List* plist, int answer);
//
void Test_List_remove_empty(List* plist, int answer);
//
void Test_List_remove_last(List* plist, int answer);
//
void Test_List_trim_normal(List* plist, int answer);
//
void Test_List_trim_empty(List* plist, int answer);
//
void Test_List_trim_last(List* plist, int answer);
//
void Test_List_concat_normal(List* plist1, List* plist2);
//
void Test_List_concat_empty_list1(List* plist1, List* plist2);
//
void Test_List_concat_empty_list2(List* plist1, List* plist2);
//
void Test_List_free_normal(List* plist, FREE_FN pItemFreeFn);
//
void Test_List_free_empty(List* plist, FREE_FN pItemFreeFn);
//
void Test_List_search_normal(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);
//
void Test_List_search_OOB_start(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);
//
void Test_List_search_OOB_end(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);
//
void Test_List_search_empty(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);