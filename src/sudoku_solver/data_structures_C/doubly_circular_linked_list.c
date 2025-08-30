#include <stdlib.h>


//Data Structure to implement X Knuth's Algorithm
typedef struct Node
{
    Node *left;
    Node *right;
    Node *up;
    Node *down;

}Node;


typedef struct ColNode{
    ColNode *left;
    ColNode *right;
    Node *up;
    Node *down;
    int node_in_col;
}ColNode;
//TODO
