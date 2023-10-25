#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum
{
    OK,
    INVALID_INPUT
} Status;
Status geometric_mean(double* res, int cnt, ...)
{
    if (cnt <= 0)
    {
        return INVALID_INPUT;
    }

    double result = 1.0;
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = cnt; i > 0; i--)
    {
        result *= va_arg(ptr, double);
    }
    va_end(ptr);
    if (!(cnt & 1) && result < 0)
    {
        *res = result;
        return OK;
    }
    *res = pow(result, 1.0 / cnt);
    return OK;
}

Status power(double* res, double number, int degree)
{
    if (degree == 0)
    {
        *res = 1;
        return OK;
    }
    if (degree % 2 == 0)
    {   
        double temp = 0;
        Status result = power(&temp, number * number, degree / 2);
        *res = temp;
        return OK;
    }
    else if (degree > 0)
    {
        double temp = 0;
        Status result = number * power(&temp, number * number, (degree - 1) / 2);
        *res = temp;
        *res *= number;
        return OK;
    }

    else 
    {
        double temp = 0;
        Status result = power(&temp, number, -degree);
        *res = 1 / temp;
        return OK;
    }

}

int main()
{
    double res = 0;
    // geometric_mean(&res, 2, 2.0, 8.0);
    // res = 0;
    power(&res, 2.0, 5);
    printf("%f", res);

    return 0;
}