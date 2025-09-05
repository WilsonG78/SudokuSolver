#include "ListOfLists.h"


ListOfLists* init_ListOfLists(){
    ListOfLists *new_list = (ListOfLists*)malloc(sizeof(ListOfLists));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list; 
}

void free_List(List* list) {
    if (!list) return;
    ListNode* current = list->head;
    while (current) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void free_ListOfLists(ListOfLists* lists) {
    if (!lists) return;
    ListOfListsNode* current = lists->head;
    while (current) {
        ListOfListsNode* next = current->next;
        free_List(current->data);
        free(current);
        current = next;
    }
    free(lists);
}


void push_node(List *list , int val){
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->data = val;
    node->next = NULL;
    if(!list->head){
        list->head = node;
        list->tail =node;
    }
    else{
        list->tail->next = node;
        list->tail = node;
    }
}


void push_list(ListOfLists *list , List *new_list){
    ListOfListsNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->data = new_list;
    node->next = NULL;
    if(!list->head){
        list->head = node;
        list->tail = node;
    }
    else{
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}
