//
// Created by Никита Третьяков on 18.12.2023.
//

#ifndef LAB5_MY_LIB_H
#define LAB5_MY_LIB_H
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

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
    END_OF_FILE,
    INVALID_BRACES_ORDER,
    DIVISION_BY_ZERO,
    ZERO_POWERED_ZERO
}Status;

void free_all(int cnt, ...);
Status my_realloc(void** ptr, ull size);
void errors(Status status);
int max(int a, int b);
Status get_str(char** str);
long long power(long long num, long long deg);
Status read_until(const char* string, const char* stop, const char** end_ptr, char** new_string);
Status binary_power(ll num, ll power, ll *res);
Status read_line(FILE* file, char** line);
#endif //LAB5_MY_LIB_H
