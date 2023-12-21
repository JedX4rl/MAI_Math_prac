//
// Created by Никита Третьяков on 18.12.2023.
//

#include "my_lib.h"

int max(int a, int b)
{
    return a > b ? a : b;
}

void free_all(int cnt, ...)
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
            printf("Overflow!\n");
            return;
        case END_OF_FILE:
            printf("End of file\n");
            return;
        case INVALID_BRACES_ORDER:
            printf("Invalid braces order\n");
            return;
        case DIVISION_BY_ZERO:
            printf("Division by zero\n");
            return;
        case ZERO_POWERED_ZERO:
            printf("Zero powered zero\n");
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

long long power(long long num, long long deg)
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

Status binary_power(ll num, ll power, ll *res)
{
    if (res == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (power < 0)
    {
        return INVALID_ARGUMENT;
    }
    if (num == 0 && power == 0)
    {
        return INVALID_INPUT;
    }
    ll tmp = 1;
    ll n = num;
    while (power > 0)
    {
        if (power & 1)
        {
            ll checker = n < 0 ? -n : n;
            if ((tmp > LLONG_MAX / checker) || (tmp < LLONG_MIN / checker)) {
                return OVERFLOWED;
            }
            tmp *= n;
        }
        n *= n;
        power >>= 1;
    }
    *res = tmp;
    return OK;
}

int is_prime(const int number)
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

Status read_until(const char* string, const char* stop, const char** end_ptr, char** new_string)
{
    if (string == NULL  || new_string == NULL || stop == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull counter = 0;
    ull size = 2;
    *new_string = (char*)malloc(sizeof(char) * size);
    if (*new_string == NULL)
    {
        return BAD_ALLOC;
    }
    char symbols[256];
    for (int i = 0; i < 256; ++i)
    {
        symbols[i] = 1;
    }
    for (ull i = 0; stop[i]; ++i)
    {
        symbols[(int)stop[i]] = 0;
    }
    const char* ptr = string;

    ull curr = (int)*ptr;
    while (symbols[(int)*ptr] && *ptr != '\0')
    {
        if (counter == size - 1)
        {
            size *= 2;
            Status st = my_realloc(new_string, sizeof(char) * size);
            if (st)
            {
                free(*new_string);
                return st;
            }
        }
        (*new_string)[counter++] = *ptr;
        ptr++;
    }
    (*new_string)[counter] = '\0';
    *end_ptr = ptr;
    return OK;
}

Status read_line(FILE* file, char** line)
{
    if (file == NULL || line == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull cnt = 0;
    ull size = 2;
    *line = (char*)malloc(sizeof(char) * size);
    if (*line == NULL)
    {
        return BAD_ALLOC;
    }
    char ch = getc(file);
    if (ch == EOF)
    {
        free(*line);
        *line = NULL;
        return END_OF_FILE;
    }
    while (!feof(file) && ch != '\n')
    {
        if (cnt + 2 > size)
        {
            Status st = my_realloc(line, sizeof(char) * (size * 2));
            if (st)
            {
                free(*line);
                return st;
            }
            size *= 2;
        }
        (*line)[cnt++] = ch;
        ch = getc(file);
    }
    (*line)[cnt] = '\0';
    return OK;
}
