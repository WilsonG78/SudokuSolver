#ifndef LISTOFLISTS_H
#define LISTOFLISTS_H
#include <stdlib.h>
#include <stdio.h>
#include "node.h"

struct  ListNode;


typedef struct ListNode{
    struct ListNode *next;
    Node *data;
} ListNode;


typedef struct List{
    ListNode *head;
    ListNode *tail;
}List;

typedef struct ListOfListsNode{
    struct ListOfListsNode* next;
    List *data;
}ListOfListsNode;


typedef struct ListOfLists
{
    ListOfListsNode *head;
    ListOfListsNode *tail;
    int size;
}ListOfLists;

ListOfLists* init_ListOfLists();
List *init_List();
void free_List(List* list);
void free_ListOfLists(ListOfLists* lists);
void push_node(List *list , Node *pointer);
void push_list(ListOfLists *list , List *new_list);


#endif