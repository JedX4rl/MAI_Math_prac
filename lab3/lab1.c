#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC
} Status;

Status sum(long long a, long long b, long long* res)
{
    long long result = a ^ b;
    long long remain = (a & b) << 1;
    if (remain != 0)
    {
        return sum(result, remain, res);
    }
    *res = result;
    return OK;
}


Status minus(long long a, long long b, long long* res)
{
    long long negative_b;
    sum(~b, 1, &negative_b);
    sum(a, negative_b, res);
    return OK;
}

Status int_to_char(long long a, int base, long long* res)
{
    if (a <= 9)
    {
        sum(a, '0', res);
    }
    else
    {
        sum(a, 'A', res);
        minus(*res, 10, res);
    }
        return OK;
}

Status product(long long a, long long b, long long* res)
{
    long long result = 0;
    long long mask = 1;
    long long temp;

    while (b != 0)
    {
        if (b & mask)
        {
            sum(result, a, &temp);
            result = temp;
        }
        a <<= 1;
        b >>= 1;
    }
    *res = result;
    return OK;
}

Status decrement(long long a, long long* res)
{
    long long temp = 0;
    minus(a, 1, &temp);
    *res = temp;
    return OK;
}

Status power(long long a, long long degree, long long* res)
{
    long long temp;
    long long result = a;
    if (degree > 1)
    {
        while(degree > 1)
        {
            product(a, result, res);
            decrement(degree, &temp);
            degree = temp;
            result = *res;
        }
    }
    else if (degree == 1)
    {
        *res = a;
    }

    else
    {
        return  INVALID_INPUT;
    }

    return OK;
}

Status to_2r_system(int r, long long num, char** res)
{
    if (1 > r || r > 5)
    {
        return INVALID_INPUT;
    }

    char* result = (char*)malloc(sizeof(char) * 33);
    if (result == NULL)
    {
        return  BAD_ALLOC;
    }
    result[32] = '\0';

    long long temp = 0;
    long long current = 0;
    long long position = 0;
    long long idx_of_str = 0;
    long long cnt = 0;
    long long base;
    long long step = 0;

    power(2, r, &base); //-->base
    minus(32, r, &cnt); //-->counter
    power(2, r, &step); //-->temp_3
    minus(step, 1, &step); //-->temp_3 == 2^r - 1

    while (current <= cnt)
    {
        long long symb = 0;
        long long bit = (num >> current) & step;
        int_to_char(bit, base, &symb);
        result[idx_of_str] = (char)symb;
        sum(current, r, &current);
        sum(idx_of_str, 1, &idx_of_str);
    }
    long long length = (int) strlen(result);
    long long zero_finish = length;
    minus(length, 1, &length);
    long long i = 0;
    while (i < length)
    {
        char ch = result[i];
        result[i] = result[length];
        result[length] = ch;
        sum(i, 1, &i);
        minus(length, 1, &length);
    }
    while (result[position] == '0')
    {
        sum(position, 1, &position);
    }
    while (temp < strlen(result) - position)
    {
        long long idx = 0;
        sum(temp, position, &idx);
        result[temp] = result[idx];
        result[idx] = '0';
        sum(temp, 1, &temp);
    }
    if (position == zero_finish)
    {
        result[0] = '0';
        result[1] = '\0';
        *res = result;
        return OK;
    }
    long long last = (int) strlen(result);
    minus(last, position, &last);
    result[last] = '\0';
    *res = result;
    return OK;
}

int main()
{
    long long res = 0;
    long long res_2 = 0;
    minus(0, -1, &res_2);
    char* result;
    Status status = to_2r_system(2, 6, &result);
    if (status == OK)
    {
        printf("%s\n", result); //TODO -
        free(result);
    }
    else
    {
        printf("Invalid input\n");
    }
    return 0;
}
