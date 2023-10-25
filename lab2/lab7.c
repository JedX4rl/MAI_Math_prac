#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum
{
    OK,
    INVALID_INPUT
} Status;
Status bisection_method(double *res, double a, double b, double eps, double (*equation)(double))
{
    double c = 0;
    if (equation(a) * equation(b) >= 0)
    {
        return 0;
    }
    while (fabs(a - b) > eps)
    {
        c = (a + b) / 2;
        if (equation(c) == 0.0)
        {
            return OK;
        }
        else if (equation(a) * equation(c) < 0)
        {
            b = c;
        }
        else 
        {
            a = c;
        }
    }
    *res = c;
    return OK;
}

double equation(double x)
{
    return x * x - 4 * x + 2;
}

int foo(int x, int y)
{
    return 1;
}

int boo(int x, int y)
{
    return 2;
}

int main()
{
    double a, b, eps;
    
    int (*operations)(int, int) = foo;
    operations(5, 6);

    printf("Enter left and right borders of the interval\n");
    printf("Enter a:\n");
    scanf("%lf", &a);
    printf("Enter b:\n");
    scanf("%lf", &b);
    printf("Enter eps:\n");
    scanf("%lf", &eps);
    double res = 0;
    bisection_method(&res, a, b , eps, equation);
    printf("%f\n", res);

    return 0;
}