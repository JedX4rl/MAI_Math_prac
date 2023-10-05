#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    OVERFLOWED
} Status;

void usage()
{
    printf("Program to calculate the value of integral with the Eps accurence\n");
    printf("Enter: the second argument (double)eps\n");
}

Status is_number(char s[])
{
    int cnt = 0;
    if (s[0] != '-')
    {
        for (int i = 0; i < strlen(s); i++)
        {
            if (s[i] == '.')
            {
                cnt++;
            }
            if (s[i] != '.')
            {
                if (!isdigit(s[i]))
                {
                    return INVALID_INPUT;
                }
            }
            if (cnt > 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }

    else
    {
        for (int i = 1; i < strlen(s); i++)
        {
            if (s[i] == '.')
            {
                cnt++;
            }
            if (s[i] != '.')
            {
                if (!isdigit(s[i]))
                {
                    return INVALID_INPUT;
                }
            }
            if (cnt > 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }
    return OK;
}

double char_to_double(char s[])
{
    double n = atof(s);
    return n;
}

Status input_checker(int argc, char* argv[])
{
    if (argc != 2)
    {
        return INVALID_INPUT;
    }

    if (is_number(argv[1]) == INVALID_INPUT)
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status a_integral(double eps, double *result)
{   
    unsigned long long step = 1;
    double part = 1;
    double current = log(2);
    double prev = log(2);
    
    do
    {
       if (step > ULLONG_MAX / 2)
       {
        return OVERFLOWED;
       }

       prev = current;
       step *= 2;
       part /= 2;
       current = 0;
       double x;

       for (int i = step; i > 0; i--) // right
       {
            x = 1.0 * i / step;
            current += log(1.0 + x)/ x * part;
       }

    } while (fabs(current - prev) >= eps);
    *result = current;
    return OK;
}

Status b_integral(double eps, double *result)
{
    unsigned long long step = 1;
    double part = 1;
    double prev, current = exp(-0.5);

    do
    {
        if (step > ULLONG_MAX / 2)
        {
            return OVERFLOWED;
        }

        prev = current;
        step *= 2;
        part /= 2;
        current = 0;
        double x;

        for (int i = step; i > 0; i--) //right
        {
            x = 1.0 * i / step;
            current += exp(x * x * -0.5) * part;
        }
        

    } while (fabs(prev - current) >= eps);
    *result = current;
    return OK;
}

Status c_integral(double eps, double* result)
{
    unsigned long long step = 1;
    double part = 1;
    double prev, current = 0;

    do
    {
        if (step > ULLONG_MAX / 2)
        {
            return OVERFLOWED;
        }
        
        prev = current;
        step *= 2;
        part /= 2;
        current = 0;
        double x;

        for (int i = 0; i < step; i++)// left
        {
            x = 1.0 * i / step;
            current += log(1.0 / (1.0 - x)) * part;
        }

    } while (fabs(prev - current) >= eps);
    *result = current;
    return OK;
    
}

Status d_integral(double eps, double* result)
{
    unsigned long long step = 1;
    double part = 1;
    double prev, current = 1;

    do
    {
        if (step > ULLONG_MAX / 2)
        {
            return OVERFLOWED;
        }
        
        prev = current;
        step *= 2;
        part /= 2;
        current = 0;
        double x;

        for (int i = step; i > 0; i--) //right
        {
            x = 1.0 * i / step;
            current += pow(x, x) * part;
        }
        
    } while (fabs(prev - current) >= eps);
    *result = current;
    return OK;
}

int main(int argc, char* argv[])
{   
    usage();
    if (input_checker(argc, argv) == INVALID_INPUT)
    {
        printf("Invalid input, check the entered data\n");
        return INVALID_INPUT;
    }

    double eps;
    if ((eps = char_to_double(argv[1])) <= 0)
    {
        printf("Epsilon must be greater than 0\n");
        return INVALID_INPUT;
    }
    int length = strlen(argv[1]);
    double result;

    if (a_integral(eps, &result) == OK)
    {
        printf("%.*f\n", length, result);
    }
    else
        printf("There's an overflow\n");
    
    if (b_integral(eps, &result) == OK)
    {
        printf("%.*f\n", length, result);
    }
    else
        printf("There's an overflow\n");
    
    if (c_integral(eps, &result) == OK)
    {
        printf("%.*f\n", length, result);
    }
    else
        printf("There's an overflow\n");
    
    if (d_integral(eps, &result) == OK)
    {
        printf("%.*f\n", length, result);
    }
    else
        printf("There's an overflow\n");

    return 0;
}