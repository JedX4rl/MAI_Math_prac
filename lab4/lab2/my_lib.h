//
// Created by Никита Третьяков on 21.12.2023.
//

#ifndef LAB2_MY_LIB_H
#define LAB2_MY_LIB_H

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef long long int ll;
typedef unsigned long long ull;

typedef enum
{
    OK,
    BAD_ALLOC,
    NOT_OK,
    INVALID_INPUT,
    INVALID_ARGUMENT,
    NULL_POINTER,
    OPENING_ERROR,
    OVERFLOWED,
    EXISTS __attribute__((unused)),
    UNKNOWN_LOGIN __attribute__((unused)),
    INCORRECT_PASSWORD __attribute__((unused)),
    EXIT
}Status;

__attribute__((unused)) void free_all(int cnt, ...);
Status my_realloc(void** ptr, ull size);
void errors(Status status);
int max(int a, int b);
int min(int a, int b);
Status get_str(char** str);

__attribute__((unused)) long long power(long long num, long long deg);

#endif //LAB2_MY_LIB_H
