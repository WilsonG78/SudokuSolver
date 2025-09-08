

typedef struct SetNode
{
    int val;
    struct SetNode *left;
    struct SetNode *right;
    struct SetNode *parent;
    int height;
}SetNode;

typedef struct set
{
    SetNode *root;

}set;

void init_set(set *tree);
void insert_set(set *tree, int val);
int contains_set(set *tree, int val);
void delete_set(set *tree, int val);
void free_set(set *tree);


