#ifndef _STDSHARED_H_
#define _STDSHARED_H_

enum FUNCTION_OPTION
{
    REGISTER = 1,
    LOGIN,
    EXIT,
};

struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
};

#endif