#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum
{
    OK,
    YES,
    NO,
    INVALID_INPUT
} Status;

typedef struct
{
    double x;
    double y;
} Point;

Status product(double* res, Point a, Point b, Point c)
{
    double x1 = b.x - a.x;
    double y1 = b.y - a.y;
    double x2 = c.x - b.x;
    double y2 = c.y - b.y;
    *res = x1 * y2 - x2 * y1;
    return YES;
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
 
Status is_a_polygon(int cnt, ...)
{
    if (cnt < 6 || cnt % 2 != 0)
    {
        return INVALID_INPUT;
    }

    int direction = 0;

    va_list ptr;
    va_start(ptr, cnt);
    Point a, b, c;
    int amount = cnt / 2;

    a.x = va_arg(ptr, double);
    a.y = va_arg(ptr, double);
    b.x = va_arg(ptr, double);
    b.y = va_arg(ptr, double);
    c.x = va_arg(ptr, double);
    c.y = va_arg(ptr, double);

    double a_1, a_2, b_1, b_2;

    a_1 = a.x;
    a_2 = a.y;

    for (int i = amount - 1; i > 0; i--)
    {
        if (i != amount - 1)
        {
            a.x = b.x;
            a.y = b.y;
            b.x = c.x;
            b.y = c.y;
            c.x = va_arg(ptr, double);
            c.y = va_arg(ptr, double);
        }
        if (i == 1)
        {
            c.x = a_1;
            c.y = a_2;
        }
        double cross = 0;
        product(&cross, a, b, c);
        if (cross)
        {
            if (direction == 0)
            {
                direction = (cross > 0) ? 1 : -1;
            } else if (cross > 0 && direction == -1)
            {
                return NO;
            }
            else if (cross < 0 && direction == 1)
            {
                return NO;
            }
        }
    }
    va_end(ptr);
    return YES;
}

double polynimial(int cnt, ...)
{
    double current_sum = 0;
    va_list ptr;
    va_start(ptr, cnt);
    int point = va_arg(ptr, double);
    int degree = va_arg(ptr, int);
    for (int i = cnt - 2; i > 0; i--)
    {
        double part = va_arg(ptr, double);
        double temp = 0;
        power(&temp, point, degree);
        current_sum += part * temp; //горнер
        degree--;
    }
    va_end(ptr);
    return current_sum;
}
int main()
{


    switch (is_a_polygon(10, 2.0, 0.0, 1.0, 1.0, -1.0, 1.0, -2.0, -1.0, 0.0, .0))
    {
    case YES:
        printf("The polygon is convex\n");
        break;
    case NO:
        printf("The polygon is not convex\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input, check entered data\n");
        break;
    }
    printf("%f\n",polynimial(6, 2.0, 3, 2.0, 1.0, 1.0, 4.0));

    return 0;
}