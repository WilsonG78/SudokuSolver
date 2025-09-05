#include <stdlib.h>


typedef struct ListNode{
    ListNode *next;
    int data
} ListNode;


typedef struct List{
    ListNode *head;
    ListNode *tail;
}List;

typedef struct ListOfListsNode{
    ListOfListsNode* next;
    List *data;
}ListOfListsNode;


typedef struct ListOfLists
{
    ListOfListsNode *head;
    ListOfListsNode *tail;
    int size;
}ListOfLists;


List *init_List(){
    List* new_list = (List*)malloc(sizeof(List));
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}