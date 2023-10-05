#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>


typedef enum
{
    OK,
    INVALID_INPUT
} Status;

void Usage()
{
    printf("Program to calculate the sum with a given accuracy Eps\n");
    printf("Enter data: file, Eps, x\n");
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

    if (argc != 3)
    {
        return INVALID_INPUT;
    }
    
    if (is_number(argv[1]) == INVALID_INPUT || is_number(argv[2]) == INVALID_INPUT)
    {
        return INVALID_INPUT;
    }

    if (*argv[1] == '-')
    {
        return INVALID_INPUT;
    }

    return OK;
}

double a_sum(double eps, double x)
{
    int n = 0;
    double sum = 0;
    double element = 1;
    do
    {
        sum += element;
        ++n;
        element *= x/n;
        
    } while (fabs(element) >= eps);
    return sum;
    
}

double b_sum(double eps, double x)
{
    int n = 0;
    double sum = 0;
    double element = 1;

    do
    {
       sum += element;
       ++n;
       element *= (-1.0 * x * x) / (2 * n * (2 * n - 1.0));

    } while (fabs(element) >= eps);

    return sum;
    
}

double sum_c(double eps, double x)
{
    int n = 0;
    double sum = 0;
    double element = 1;
    do
    {
        sum += element;
        ++n;
        element *= (9.0 * n * n * x * x) / (9.0 * n * n - 9.0 * n + 2.0);
    } while (fabs(element) >= eps);
    return sum;
}

double sum_d(double eps, double x)
{
    int n = 1;
    double sum = 0;
    double element = -1.0 * x * x / 2.0;
    do
    {
        sum += element;
        ++n;
        element *= (-1.0 * x * x * (2.0 * n - 1)) / (2.0 * n);
        
    } while (fabs(element) >= eps);

    return sum;
    
}





int main(int argc, char* argv[])
{
    if (input_checker(argc, argv) == INVALID_INPUT)
    {
        printf("Invalid input, check the entered data\n");
        Usage();
        return INVALID_INPUT;
    }
    int length = strlen(argv[1]) - 2;
    double eps = char_to_double(argv[1]);
    if (fabs(eps) <= 0)
    {
        printf("Eps must be greater than 0\n");
        return INVALID_INPUT;
    }
    double x = char_to_double(argv[2]);
    printf("eps = %.*f\n", length, eps);
    printf("%.*f\n", length, a_sum(eps, x));
    printf("%.*f\n", length, b_sum(eps, x));
    if (fabs(x) < 1)
    {
        printf("%.*f\n", length, sum_c(eps, x));
        printf("%.*f\n", length, sum_d(eps, x));
    }
    else 
    {
        printf("Here x must be in (1;1) range\n");
        printf("Here too :)\n");
    }

    return 0;
}