//
// Created by Никита Третьяков on 14.12.2023.
//

#ifndef LAB6_MY_LIB_H
#define LAB6_MY_LIB_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

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
    EXISTS,
    UNKNOWN_LOGIN,
    INCORRECT_PASSWORD,
    NEW_USER,
    LOGGED,
    BANNED
}Status;

void free_all(int cnt, ...);
Status my_realloc(void** ptr, ull size);
void errors(Status status);
int max(int a, int b);
Status get_str(char** str);
long long power(long long num, long long deg);
#endif //LAB6_MY_LIB_H
