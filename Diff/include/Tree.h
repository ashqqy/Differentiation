#ifndef TREE
#define TREE

#include <stdio.h>

//--------------------------------------------------------------------------

#define VERIFICATION

#ifdef VERIFICATION
#define TREE_VERIFY(tree) TreeVerify (tree);
#else
#define TREE_VERIFY(...) ;
#endif // VERIFICATION


// убрать ассерт и добавить очистку дерева
#define TreeErrCheck$(tree, tree_err) \
        if (tree_err != TREE_OK) \
                { \
            printf ("%sERROR:%s %s %s \n", RED_COLOR, MAGENTA_COLOR, TreeErrorDescr (tree_err), DEFAULT_COLOR); \
            TreeDump (tree); \
            assert ("TreeNotOk" && !TREE_OK); \
        } \
    
//--------------------------------------------------------------------------

typedef int node_data_t;

enum type_t
{
    NUM = 1,
    VAR = 2,
    OP  = 3
};

enum operations_t
{
    ADD    = 1,
    SUB    = 2,
    MUL    = 3,
    DIV    = 4
};

struct node_t
{
    node_t*     parent;
    type_t      type;
    node_data_t data;
    node_t*     left;
    node_t*     right;
};

struct tree_t
{
    node_t* root;
    int     n_nodes;
};

enum tree_error_t
{
    TREE_OK             = 1,

    INIT_ERR            = 2,

    UNINITIALIZED_TREE  = 3,
    NODE_NULL_PTR       = 4,
    NODE_WITHOUT_PARENT = 5, 
    NODE_BAD_PARENT     = 6,
    NODE_BAD_RCHILD     = 7,
    NODE_BAD_LCHILD     = 8

};

//--------------------------------------------------------------------------

node_t* TreeInit (tree_t* tree, type_t type = NUM, node_data_t data = 0);

node_t* NodeCreate (tree_t* tree, type_t type, node_data_t data, node_t* parent, node_t* left, node_t* right);
node_t* NodeDataEdit (tree_t* tree, node_t* node, type_t new_type, node_data_t new_data);

void TreeVerify (tree_t* tree);
tree_error_t NodeVerify (node_t* node, tree_error_t* tree_err);
const char* TreeErrorDescr (tree_error_t tree_err);

void TreeDump (tree_t* tree);
void TreeNodeDescrDump (FILE* dump_file, node_t* node);
void TreeNodeLinkDump (FILE* dump_file, node_t* node);

//--------------------------------------------------------------------------

#endif //TREE