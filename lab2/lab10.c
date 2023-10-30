#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>


typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC,
    NULL_POINTER
} Status;

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

Status calculate_Horner(double x, int n, double* coefficients, double* result)
{
    if (coefficients == NULL || result == NULL)
    {
        return NULL_POINTER;
    }
    *result = 0;
    for (int i = n; i >= 0; i--)
    {
        *result *= x;
        *result += coefficients[i];
    }
    return  OK;
}

Status decomposition(double eps, double a, double** result, int n, ...)
{
    if (result == NULL)
    {
        return NULL_POINTER;
    }

    double* coefficients = (double*)malloc(sizeof(double) * (n + 1));
    if (coefficients == NULL)
    {
        return  BAD_ALLOC;
    }
    *result = (double*)malloc(sizeof(double) * (n + 1));
    if (result == NULL)
    {
        free(coefficients);
        return BAD_ALLOC;
    }

    va_list arguments;
    va_start(arguments, n);
    for (int i = 0; i <= n; ++i)
    {
        coefficients[i] = va_arg(arguments, double);
    }
    va_end(arguments);

    double factorial = 1;
    int curr_position = 0;

    for (int i = 0; i <= n; ++i)
    {

        curr_position = n - i;
        Status calculate_status = calculate_Horner(a, curr_position, coefficients, *result + i);
        if (calculate_status != OK)
        {
            free(coefficients);
            free(*result);
            return calculate_status;
        }
        (*result)[i] /= factorial;
        factorial *= i + 1;
        for (int j = 0; j < curr_position; ++j)
        {
            int temp = j + 1;
            coefficients[j] = temp * coefficients[temp];
        }
        coefficients[curr_position] = 0;
    }
    free(coefficients);
    return OK;
}

int main()
{
    double* result;
    int n = 3;
    Status status = decomposition(1, 2.6, &result, n, 0.0, 3.6, 0.0, 12.0);
    if (status != OK)
    {
        errors(status);
        return status;
    }
    for (int i = 0; i <= n; ++i) {
        printf("g%d = %f\n", i, result[i]);
    }
    return 0;
}
