#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void Usage()
{
    printf("Enter data up to the rules: \"./filename\" \"EPS (0 < EPS < 1)\" \"flag\"\n");
}

bool not_simple(int n)
{
    if (n > 3)
    {
        if (n % 2 == 0)
            return true;
        for (int i = 3; i <= sqrt(n); i+=2)
        {
            if (n % i == 0)
                return true;
        }
    }
    return false;
}

int char_to_double(char s[], double *n, int *length)
{
    *length = strlen(s) - 2;
    
    if(*length > 17)
    {
        return -1;
    }


    *n = atof(s);

    if(*n == 0)
    {
        return -2;  
    }

    return 0;

}

char your_choice(char s[])
{
    if(s[0] == 'e')
        return 'e';
    
    if(s[0] == 'p')
        return 'p';
    
    if(s[0] == 'l')
        return 'l';
    
    if(s[0] == 'r')
        return 'r';

    if(s[0] == 'g')
        return 'g';
    
    else
        return 'F';
}

double sum_n(int n)
{
    double sum = 1;
    for (int i = 2; i <= n; i++)
    {
        sum += 1.0/i;
    }

    return sum;
    
}


double limit_e(double Eps)
{
    double prev, current  = 0;
    double n = 1;

    do
    {
        prev = current;
        n *= 2;
        current = pow((1 + 1/n), n);

    } while (fabs(prev - current) > Eps);
    return current;

}

double row_e(double Eps)
{
    double current_sum = 0;
    double prev_sum = -1;
    double factorial_n = 1;
    for (int n = 0; n <= 20; n++)
    {
        factorial_n *= n;
        if (n == 0)
            factorial_n  = 1;
        current_sum += 1.0/factorial_n;
        if (fabs(current_sum - prev_sum) < Eps)
            break;
        prev_sum = current_sum;

    }
    return current_sum;
    
}

double equation_e(double Eps)
{
    double x = 0.0;
    double left = 2.0;
    double right = 3.0;
    x = (left + right) / 2;

    while (fabs(log(x) - 1) > Eps)
    {
        x = (left + right) / 2;
        if (log(x) < 1)
            left = x;
        else
            right = x;
    }

    return x;
}

double limit_pi(double Eps)
{
    double current_limit = 4.0;
    double prev_limit = 0.0;
    int n = 2;
    do
    {
        prev_limit = current_limit;
        current_limit = current_limit * (4.0 * n * ( n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    } while (fabs(prev_limit - current_limit) >= Eps);
    return current_limit;
}

double row_pi(double Eps)
{
    double current_sum = 1.0;
    double prev_sum = 0;
    int n = 1;
    
    do
    {
        prev_sum = current_sum;
        n++;
        current_sum -= 1.0/(2*n - 1);
        n++;
        current_sum += 1.0/(2*n - 1);

    } while (fabs(prev_sum - current_sum) > Eps);

    return current_sum * 4;
    
    
}

double equation_pi(double Eps)
{
    double x = 0;
    double left = 2;
    double right = 4;
    x = (right + left) / 2;
    
    do
    {
        x = (right + left) / 2;
        if(sin(x) > 0)
            left = x;
        else
            right = x;

    } while (fabs(left - right) > Eps);

    return x;
}

double limit_ln(double Eps)
{
    double prev = 0.0;
    double current = 1.0;
    double n = 1;
    do
    {
        prev = current;
        n++;
        current = n * (pow(2.0, 1.0/n) - 1.0);
        
    } while (fabs(prev - current) > 0.000001);
    return current;
    
}

double row_ln(double Eps)
{
    double prev = 0;
    double current = 1;
    int n = 1;
    do
    {
        prev = current;
        n++;
        current -= 1.0/n;
        n++;
        current += 1.0/n;

    } while (fabs(prev - current) > Eps);

    return current;
    
}

double equation_ln(double Eps)
{
    double left = 0.5;
    double right = 0.7;
    double x;
    do
    {
        x = (left + right)/2.0;
        if (exp(x) > 2)
            right = x;
        else
            left = x;


    } while (fabs(exp(x) - 2) > Eps);

    return x;
}

double limit_root_of_2(double Eps)
{
    double prev = 0;
    double current = -0.5;
    int n = 0;
    do
    {
        prev = current;
        n++;
        current = (2.0 * prev - pow(prev, 2.0) + 2)/2;

    } while (fabs(prev - current) > Eps);
    return current;
    
}

double product_root_of_2(double Eps)
{
    double prev = 0;
    double current = 0;
    int k = 2;
    current = pow(2.0, 1.0/4.0);
    do
    {
        prev = current;
        k++;
        double power = 1.0/pow(2,k);
        current *= pow(2,power);
        
    } while (fabs(prev - current) > Eps);
    return current;

}

double equation_root_of_2(double Eps)
{
    double left = 1.2;
    double right = 1.5;
    double x = (left + right)/2;
    do
    {
        x = (left + right) / 2;
        if(pow(x,2) > 2)
            right = x;
        else
            left = x;
    } while (fabs(pow(x,2) - 2) > Eps);
    return x;
    
}

double limit_gamma(double Eps)
{
    double prev = 0;
    double current = 1;
    int n = 1;
    do
    {
        prev = current;
        n *= 2;
        current = sum_n(n) - log(n);

    } while (fabs(prev - current) > Eps);

    return current;
  
}

double row_gamma(double Eps)
{
    double prev = 0;
    double current = 0.5;
    int k = 2;
    double l = 0;

    do
    {
        prev = current;
        k++;
        l = sqrt(k);
        if (fmod(l,1.0) == 0)
            {
                k++;
                l = (int)pow(k, 1.0/2.0);
            }
        current += 1.0/pow((int)l,2.0) - 1.0/k;


    } while (fabs(prev - current) > Eps);

    return current - pow(M_PI,2)/6;
    
}

double equation_gamma(double Eps)
{
    int p = 2;
    double current = log(2) * 0.5;
    double prev = 0;
    double product = 0.5;

    do
    {
        prev = current;
        do
        {
            p++;
        } while (not_simple(p));
        product *= (p - 1.0)/p;
        current = log(p) * product;
        

    } while (fabs(prev - current) > Eps);
    return (-log(current));
 
}


int main(int argc, char* argv[])
{
    const int param = 1;

    double EPS = 0;
    int length = 0;

    if(*argv[1] == '-')
    {
        printf("Eps must be greater than 0\n");
        return 1;
    }

    switch ((char_to_double(argv[1], &EPS, &length)))
    {
    case 0:
        printf("EPS = %.*f\n", length, EPS);
        break;

    case -1:
        printf("Overflow\n");
        break;

    case -2:
        {
            printf("Eps cannot be zero\n");
            return 4;
        }
        break;
    
    default:
        break;
    }

    switch (your_choice(argv[2]))
    {
    case 'e':
        {   
            printf("The values of e\n");
            printf("From limit: %.*f\n", length, limit_e(EPS));
            printf("From row: %.*f\n", length, row_e(EPS));
            printf("From equation: %.*f\n", length, equation_e(EPS));
        }
        break;
    
    case 'p':
        {
            printf("The values of Pi\n");
            printf("From limit: %.*f\n", length, limit_pi(EPS));
            printf("From row: %.*f\n", length, row_pi(EPS));
            printf("From equation: %.*f\n", length, equation_pi(EPS));
        }
        break;
    
    case 'l':
        {
            printf("The values of ln2\n");
            printf("From limit: %.*f\n", length, limit_ln(EPS));
            printf("From row: %.*f\n", length, row_ln(EPS));
            printf("From equation: %.*f\n", length, equation_ln(EPS));
        }
        break;
    
    case 'r':
        {
            printf("The values of root_of_2\n");
            printf("From limit: %.*f\n", length, limit_root_of_2(EPS));
            printf("From row: %.*f\n", length, product_root_of_2(EPS));
            printf("From equation: %.*f\n", length, equation_root_of_2(EPS));
        }
        break;
    
    case 'g':
        {
            printf("The values of Gamma\n");
            printf("From limit: %.*f\n", length, limit_gamma(EPS));
            printf("From row: %.*f\n", length, row_gamma(EPS));
           printf("From equation: %.*f", length, equation_gamma(EPS));
        }
        break;
    
    default:
        Usage();
        break;
    }

    return 0;
    
}