#ifndef NODE_H
#define NODE_H
struct Node;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    struct Node *up;
    struct Node *down;
    struct ColNode *column;
    int rowID;
    int colID;

}Node;

#endif