#include <stdlib.h>

#include "Diff.h"

//--------------------------------------------------------------------------

const char* s = "(520-20)/50$";
int p = 0;

int GetG ()
{
    int val = GetE ();
    if (s[p] != '$')
        SyntaxError ();
    p++;
    return val;
}

int GetE ()
{
    int val = GetT ();
    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        int val2 = GetT ();
        if (op == '+') val += val2;
        else           val -= val2;
    }
    return val;
}

int GetT ()
{
    int val = GetP ();
    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        int val2 = GetP ();
        if (op == '*') val *= val2;
        else           val /= val2;
    }
    return val;
}

int GetP ()
{
    if (s[p] == '(')
    {
        p++;
        int val = GetE ();
        if (s[p] != ')')
            SyntaxError ();
        p++;
        return val;
    }
    else
        return GetN ();
}

int GetN ()
{
    int val = 0;
    int old_p = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
    }

    if (old_p == p)
        SyntaxError ();

    return val;
}

void SyntaxError ()
{
    abort();
}

//--------------------------------------------------------------------------