#include "stdlib.h"
#include "array_2d.h"
#include "ListOfLists.h"
#include "grid.h"
//Data Structure to implement X Knuth's Algorithm
typedef struct Node
{
    Node *left;
    Node *right;
    Node *up;
    Node *down;
    struct ColNode *column;
    int rowID;
    int colID;

}Node;

typedef struct ColNode{
    Node *node;
    int size;
}ColNode;


typedef struct DLX{
    ColNode *columns;
    ColNode *root;
    Node **solution;
    int solutionSize;
    int rows;
    int cols;
}DLX;


typedef struct Vertex{
    Node *data;
    Vertex *next;
    Vertex *prev;
}Vertex;

typedef struct Stack
{
    Vertex *head;
    Vertex *tail;
    int size;
}Stack;

DLX* init_DLX(Array2D* problem_matrix);
void free_DLX(DLX *dlx);
Array2D* transform_grid_to_DLX_naive(grid *g);
ListOfLists * solve_dlx(DLX *dlx);

