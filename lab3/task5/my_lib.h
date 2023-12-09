//
// Created by Никита Третьяков on 06.12.2023.
//

#ifndef LAB9_MY_LIB_H
#define LAB9_MY_LIB_H

#include <stdarg.h>
#include <stdlib.h>

typedef long long int ll;
typedef unsigned long long ull;

typedef enum
{
    OK,
    BAD_ALLOC,
    NOT_OK,
    INVALID_INPUT,
    NULL_POINTER,
    OPENING_ERROR
}Status;

void free_all(int cnt, ...);
Status my_realloc(void** ptr, ull size);
void errors(Status status);
int max(int a, int b);
Status get_str(char** str);


#endif //LAB9_MY_LIB_H
