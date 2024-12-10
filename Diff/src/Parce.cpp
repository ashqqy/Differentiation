#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "Parce.h"

//--------------------------------------------------------------------------

const char* s = "1+x$";
int p = 0;

tree_node_t* GetG ()
{
    tree_node_t* root_node = GetPlus ();
    if (s[p] != '$')
        SyntaxError (s[p]);
    p++;
    return root_node;
}

tree_node_t* GetPlus ()
{
    tree_node_t* first_node = GetMult ();
    while (s[p] == '+' || s[p] == '-')
    {   
        tree_data_t value;
        value.type = OP;
        if (s[p] == '+')
            value.content.operation = ADD;
        else
            value.content.operation = SUB;

        p++;
        tree_node_t* second_node = GetMult ();

        tree_node_t* parent_node = NodeCreate (value);
        NodeLink (first_node, &parent_node->left);
        NodeLink (second_node, &parent_node->right);

        first_node = parent_node;
    }

    return first_node;
}

tree_node_t* GetMult ()
{
    tree_node_t* first_node = GetBracket ();
    while (s[p] == '*' || s[p] == '/')
    {
        tree_data_t value;
        value.type = OP;
        if (s[p] == '*')
            value.content.operation = MUL;
        else
            value.content.operation = DIV;

        p++;
        tree_node_t* second_node = GetBracket ();

        tree_node_t* parent_node = NodeCreate (value);
        NodeLink (first_node, &parent_node->left);
        NodeLink (second_node, &parent_node->right);
        
        first_node = parent_node;
    }
    return first_node;
}

tree_node_t* GetBracket ()
{
    if (s[p] == '(')
    {
        p++;
        tree_node_t* node = GetPlus ();
        if (s[p] != ')')
            SyntaxError (s[p]);
        p++;
        return node;
    }
    else if ('a' <= s[p] && s[p] <= 'z')
        return GetVariable ();
    else
        return GetNumber ();
}

tree_node_t* GetNumber ()
{
    tree_data_t value;
    value.type           = NUM;
    value.content.number = 0;

    int old_p = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        value.content.number = value.content.number * 10 + s[p] - '0';
        p++;
    }

    if (old_p == p)
        SyntaxError (s[p]);

    return NodeCreate (value);;
}

tree_node_t* GetVariable ()
{
    tree_data_t value;
    value.type           = VAR;
    value.content.number = 0;

    int old_p = p;
    if ('a' <= s[p] && s[p] <= 'z')
    {
        value.content.variable = s[p];
        p++;
    }

    if (old_p == p)
        SyntaxError (s[p]);

    return NodeCreate (value);
}

void SyntaxError (char symb)
{
    printf ("Syntax error: %c\n", symb);
    assert(0);
}

//--------------------------------------------------------------------------