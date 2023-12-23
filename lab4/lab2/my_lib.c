//
// Created by Никита Третьяков on 21.12.2023.
//

#include "my_lib.h"

int max(int a, int b)
{
    return a > b ? a : b;
}
int min(int a, int b)
{
    return a < b ? a : b;
}

__attribute__((unused)) void free_all(int cnt, ...)
{
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = 0; i < cnt; ++i)
    {
        char* tmp = va_arg(ptr, char*);
        free(tmp);
    }
    va_end(ptr);
}

Status my_realloc(void** ptr, ull size)
{
    if (ptr == NULL || size == 0)
    {
        return INVALID_INPUT;
    }
    void* new_ptr = realloc(*ptr, size);
    if (new_ptr != NULL)
    {
        *ptr = new_ptr;
        return OK;
    }
    *ptr = NULL;
    return BAD_ALLOC;
}

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NULL_POINTER:
            printf("NULL pointer error\n");
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case NOT_OK:
            printf("Not OK\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        case INVALID_ARGUMENT:
            printf("Invalid argument\n");
            return;
        case OVERFLOWED:
            printf("Overflow\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status get_str(char** str)
{
    int max_size = 20;
    int temp_size = 0;
    *str = (char*)malloc(sizeof(char) * max_size);
    if (*str == NULL)
    {
        return BAD_ALLOC;
    }
    char c;
    while ((c = getchar()) != '\n')
    {
        if (temp_size + 1 == max_size)
        {
            max_size *= 2;
            if (my_realloc(str, max_size + 1) != OK)
            {
                free(*str);
                return BAD_ALLOC;
            }
        }
        (*str)[temp_size] = c;
        temp_size++;
    }
    (*str)[temp_size] = '\0';
    return OK;
}

__attribute__((unused)) long long power(long long num, long long deg)
{
    long result = 1;
    while(deg)
    {
        if (deg % 2 == 0)
        {
            deg /= 2;
            num *= num;
        }
        else
        {
            deg--;
            result *= num;
        }
    }
    return result;
}

__attribute__((unused)) int is_prime(const int number)
{
    if (number == 0 || number == 1)
    {
        return 0;
    }
    if (number == 2)
    {
        return 1;
    }
    if (!(number & 1))
    {
        return 0;
    }
    for (int i = 3; i * i <= number; i += 2)
    {
        if (!(number % i))
        {
            return 0;
        }
    }
    return 1;
}