#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#define EPS 1e-10
typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC,
    NULL_POINTER,
    OVERFLOWED
} Status;

typedef enum
{
    PRIME,
    COMPOSITE,
    ANY
} Num_type;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NULL_POINTER:
            printf("Null pointer error\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status gcd (unsigned long long a, unsigned long long b, unsigned long long* res)
{
    while (b)
    {
        a %= b;
        unsigned long long temp = a;
        a = b;
        b = temp;
    }
    *res = a;
    return  OK;
}

Status get_fraction(double fraction, unsigned long long* numerator, unsigned long long* denominator)
{
    if (numerator == NULL || denominator == NULL)
    {
        return  NULL_POINTER;
    }

    double temp = fraction;
    *denominator = 1;
    while (fabs(temp - floor(temp)) >= EPS)
    {
        if (temp > ULLONG_MAX / 10)
        {
            return OVERFLOWED;
        }
        temp *= 10;
        *denominator *= 10;
    }
    *numerator = (unsigned long long)temp;
    unsigned long long divisor = 0;
    gcd(*numerator,*denominator, &divisor);
    *numerator /= divisor;
    *denominator /= divisor;
    return OK;
}

Num_type is_prime(unsigned long long a)
{
    if (a <= 1)
    {
        return ANY;
    }
    if (a == 2)
    {
        return  PRIME;
    }
    if (!(a & 1))
    {
        return COMPOSITE;
    }
    for (int i = 3; i * i < a; i += 2)
    {
        if (a % i == 0)
        {
            return COMPOSITE;
        }
    }
    return  PRIME;
}

Status is_finite(int base, double fraction, char* answer)
{
    if (answer == NULL)
    {
        return NULL_POINTER;
    }

    if (2 > base || base > 36)
    {
        return INVALID_INPUT;
    }

    unsigned long long numerator = 0, denominator = 0;
    Status status = get_fraction(fraction, &numerator, &denominator);
    if (status != OK)
    {
        return status;
    }
    for (unsigned long long i = 1; i * i <= denominator ; i ++)
    {

        if (denominator % i == 0)
        {
            if ((is_prime(denominator / i) == PRIME && base % (denominator / i) != 0) || (is_prime(i) == PRIME && base % i != 0))
            {
                *answer = 'N';
                return OK;
            }
        }
    }
    *answer = 'Y';
    return OK;
}

Status result(char** answer, int base, int cnt, ...)
{
    if (answer == NULL)
    {
        return  NULL_POINTER;
    }
    va_list fractions;
    va_start(fractions, cnt);

    *answer = (char*)malloc(sizeof(char) * cnt);
    for (unsigned long long i = 0; i < cnt; ++i)
    {
        double fraction = va_arg(fractions, double);
        Status status = is_finite(base, fraction, *answer + i);
        if (status != OK)
        {
            free(*answer);
            return status;
        }
    }
    return  OK;
}

int main()
{
    char* is_finite;
    int base = 10;
    int cnt = 4;
    Status answer = result(&is_finite, base, cnt, 0.25, 0.15, 0.75, 1.0);
    if (answer != OK)
    {
        errors(answer);
        return answer;
    }
    for (int i = 0; i < cnt; ++i)
    {
        if (is_finite[i] == 'Y')
        {
            printf("Fraction %d is finite in a base %d\n", i + 1, base);
        }
        else
        {
            printf("Fraction %d is not finite in a base %d\n", i + 1, base);
        }
    }
    return 0;
}
