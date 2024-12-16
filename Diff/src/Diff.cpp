#include "CustomAssert.h"
#include "Tree.h"
#include "Diff.h"

//--------------------------------------------------------------------------

#define DIFF_(node) Diff (node)
#define DIFF_L_ Diff (node->left)
#define DIFF_R_ Diff (node->right)
#define COPY_L_ SubtreeCopy (node->left)
#define COPY_R_ SubtreeCopy (node->right)

#define NUM_(num) \
    NodeCreate (tree_data_t {.content = {.number = num}, .type = NUM})

#define CONST_(name) \
    NodeCreate (tree_data_t {.content = {.constant = name}, .type = CONST})

// TODO #define OPER_(op_, left_node, right)

#define ADD_(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = ADD}, .type = OP}, left_node, right_node)

#define SUB_(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = SUB}, .type = OP}, left_node, right_node)

#define MUL_(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = MUL}, .type = OP}, left_node, right_node)

#define DIV_(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = DIV}, .type = OP}, left_node, right_node)

#define DEG_(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = DEG}, .type = OP}, left_node, right_node)

#define LN_(node) \
    NodeCreate (tree_data_t {.content = {.function = LN}, .type = FUNC}, node)

#define SIN_(node) \
    NodeCreate (tree_data_t {.content = {.function = SIN}, .type = FUNC}, node)

#define COS_(node) \
    NodeCreate (tree_data_t {.content = {.function = COS}, .type = FUNC}, node)

//--------------------------------------------------------------------------

tree_node_t* Diff (tree_node_t* node)
{
    CustomAssert (node != NULL);

    switch (node->data.type)
    {
        case NUM: 
        case CONST:
            return NUM_ (0);

        case VAR:
            return NUM_ (1);

        case OP:
        {
            switch (node->data.content.operation)
            {
                case ADD: 
                    return ADD_ (DIFF_L_, DIFF_R_);

                case SUB:
                    return SUB_ (DIFF_L_, DIFF_R_);

                case MUL:
                    return ADD_ (MUL_(DIFF_L_, COPY_R_), 
                                 MUL_(DIFF_R_, COPY_L_));
 
                case DIV:
                    return DIV_ (SUB_ (MUL_ (DIFF_L_, COPY_R_), 
                           MUL_ (DIFF_R_, COPY_L_)), MUL_ (COPY_R_, COPY_R_));

                case DEG:
                {
                    if (node->left->data.type == NUM || node->left->data.type == CONST)
                        return MUL_ (MUL_ (DEG_ (COPY_L_,  COPY_R_), LN_ (COPY_L_)), DIFF_R_);

                    else if (node->right->data.type == NUM)
                        return MUL_ (MUL_ (COPY_R_, DEG_ (COPY_L_, NUM_ (node->right->data.content.number - 1))), DIFF_L_);
                    
                    else if (node->right->data.type == CONST)
                        return MUL_ (MUL_ (COPY_R_, DEG_ (COPY_L_, SUB_ (COPY_R_, NUM_ (1)))), DIFF_L_);

                    else 
                    {
                        tree_node_t* ln_node = LN_ (node->left);
                        node->left = CONST_ (EXP);
                        node->right = MUL_ (ln_node, node->right);
                        return DIFF_ (node);
                    }
                }

                default:
                    break;
            }
            break;
        }

        case FUNC:
        {
            switch (node->data.content.function)
            {
                case SIN:
                    return MUL_ (COS_ (COPY_L_), DIFF_L_);
                case COS:
                    return MUL_ (MUL_ (NUM_ (-1), SIN_ (COPY_L_)), DIFF_L_);
                case TG:
                    return MUL_ (DIV_ (NUM_ (1), DEG_ (COS_ (COPY_L_), NUM_ (2))), DIFF_L_);
                case CTG:
                    return MUL_ (MUL_ (NUM_ (-1), DIV_ (NUM_ (1), DEG_ (SIN_ (COPY_L_), NUM_ (2)))), DIFF_L_);
                case LN:
                    return MUL_ (DIV_ (NUM_ (1), COPY_L_), DIFF_L_);

                default:
                    break;
            }
            break;
        }

        case SP_SYMB:
        default:
            break;
    }

    return NULL;
}

//--------------------------------------------------------------------------
