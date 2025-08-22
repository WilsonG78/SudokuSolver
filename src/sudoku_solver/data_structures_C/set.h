

typedef struct Node
{
    int val;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int height;
}Node;

typedef struct set
{
    Node *root;

}set;

void init_set(set *tree);
void insert_set(set *tree, int val);
int contains_set(set *tree, int val);
void delete_set(set *tree, int val);
void free_set(set *tree);


