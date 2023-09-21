#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

void Info()
{
    printf("\n");
    printf("Tretiakov Nikita Mikhailovich M8O-213B-22\n");
    printf("Program for making different operations that depends on flag\n");
    printf("\n");
}

void Error()
{
    printf("Wrong format\n");
    printf("Enter:\"filename\" \"long long int number\" \"flag starts with '-' or '/'\"\n");
    printf("\n");
}

void Usage()
{
    printf("-h to print natural numbers are multiples of this and less than or equal to 100\nor to say if there are no suitable\n\n");
    printf("-p to find out if the number is simple or not simple\n\n");
    printf("-s turn a number into separate digits and print them in a string\n\n");
    printf("-e to print a table of powers (a number must be less than or equal to 10)\n\n");
    printf("-a to get a sum of numbers from 1 to entered one\n\n");
    printf("-f to get the factorial of the number\n\n");

}

bool Converter(char s[], long long int *res)
{
    int lenght = strlen(s);
    for(int i = 0; i < lenght; i++)
    {
        if(s[i] < 48 || s[i] > 57)
        {
            printf("Your number must be in \"int\" type\n");
            return true;
        }
    }

    if (s[0] == '-')
    {
        if (lenght > 20)
            {
                printf("You are out of range");
                return true;
            }
        for (int i = 1; i < lenght; i++)
        {
            *res = *res * 10 + (s[i] - '0');
        }
        *res *= -1;
        return false;  
    }

    if (lenght > 19)
    {
        printf("You are out of range");
        return true;
    }

    for (int i = 0; i < lenght; i++)
    {
        *res = *res * 10 + (s[i] - '0');
    }
    return false;
    
}

bool Is_simple(long long int n)
{
    for (long long i = 2; i < n/2; i++)
    {
        if (!(n % i))
            {
                return true;
            }
    }
    return false;
}

void Number_to_string(long long int n)
{
    int lenght = 1;
    long long int temp = n;
    char* string;

    while (temp /= 10)
        ++lenght;

    temp = llabs(n);


    if (n > 0)
    {   int size = lenght * 2;
        string = (char*)malloc(sizeof(char) * size);
        string[size-1] = '\0';
        for (int i = size - 2; i >= 0; i-=2)
        {
            string[i] = (temp % 10) + '0';
            if(i != 1)
                string[i-1] = ' ';
            temp/= 10;
        }
    }

    if (n < 0)
    {   int size = lenght * 2 + 1;
        string = (char*)malloc(sizeof(char) * size);
        string[size-1] = '\0';
        for (int i = size - 2; i >= 0; i-=2)
        {
            string[i] = (temp % 10) + '0';
            if(i != 1)
                string[i-1] = ' ';
            temp/= 10;
        }
        string[0] = '-';
    }
    printf("%s", string);
    free(string);
    //return string;

}

int Summary(long long int n)
{
    if (n < 1)
        return 0;
    long long int ammount = n;
    return ((n-1)/2*n);
}

int Factorial(long long int n)
{
    if (n == 0)
        return 1;
    return n*Factorial(n-1);
}




int main(int argc, char* argv[])
{
    Info();

    if(argc != 3 || (*argv[2] != '-' && *argv[2] != '/') || strlen(argv[2]) != 2)
    {
        Error();
        Usage();
        return 1;
    }

    long long int temp = 0;

    if (Converter(argv[1], &temp))
    {
        return 2;
    }

    if (argv[2][1] == 'h')
    {
        if (temp <= 0 || temp > 100)
            printf("No divisible numbers by %lld", temp);
        else
        {
            int flag = 0;
            for (long long int i = temp; i <= 100; i++)
            {
                if (i % temp == 0)
                {
                    flag = 1;
                    printf("%lld ", i);
                }

            }

            if(!flag)
                printf("No divisible numbers by %lld", temp);
            
        }
    }

    if (argv[2][1] == 'p')
    {
        if(temp <= 0)
            printf("The number must be greater than 0");

        else if (Is_simple(temp))
            printf("The number %d is NOT simple!", temp);
        else
            printf("The number %d is simple!", temp);
    }

    if (argv[2][1] == 's')
    {
        //printf("%s", Number_to_string(temp));
        Number_to_string(temp);
    }

    if(argv[2][1] == 'e')
    {
        if (1 > temp || temp > 10)
            printf("The number must be greater than zero and less then eleven");
        else
        {
            for (int i = 1; i <= 10; i++)
            {
                for (long long j = 1; j <= temp; j++)
                {
                    printf("%.0f\t", pow(i,j));
                }
                printf("\n");
            }
        }
    }

    if(argv[2][1] == 'a')
    {
        printf("%d", Summary(temp));
    }

    if(argv[2][1] == 'f')
    {
        if(temp > 20)
            {
                printf("Overflow");
                return 3;
            }
        printf("%d", Factorial(temp));
    }

    if(argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' && argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f')
    {
        Usage();
        return 4;
    }

    return 0;

}