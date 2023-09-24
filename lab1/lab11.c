#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

void info()
{
    printf("\n");
    printf("Tretiakov Nikita Mikhailovich M8O-213B-22\n");
    printf("Program for making different operations that depends on flag\n");
    printf("\n");
}

void error()
{
    printf("Wrong format\n");
    printf("Enter:\"filename\" \"long long int number\" \"flag starts with '-' or '/'\"\n");
    printf("\n");
}

void usage()
{
    printf("-h to print natural numbers are multiples of this and less than or equal to 100\nor to say if there are no suitable\n\n");
    printf("-p to find out if the number is simple or not simple\n\n");
    printf("-s turn a number into separate digits and print them in a string\n\n");
    printf("-e to print a table of powers (a number must be less than or equal to 10)\n\n");
    printf("-a to get a sum of numbers from 1 to entered one\n\n");
    printf("-f to get the factorial of the number\n\n");

}

bool converter(char s[], long long int *res)
{
    int length = strlen(s);
    int flag = 0;
    if (s[0] != '-')
    {   for(int i = 1; i < length; i++)
        {
            if(!(isdigit(s[i])))
            {
                flag = 1;
                return true;
            }
        }
    }

    if (s[0] == '-' && flag == 0)
    {
        if (length >= 20)
            {
                return true;
            }
        for (int i = 1; i < length; i++)
        {
            *res = *res * 10 + (s[i] - '0');
        }
        *res *= -1;
        return false;  
    }

    if (length >= 19)
    {
        return true;
    }

    for (int i = 0; i < length; i++)
    {
        *res = *res * 10 + (s[i] - '0');
    }
    return false;
    
}


int is_simple(long long int n)
{
    if (n <= 1)
        return -1;

    if(n != 2 && n%2 == 0)
    {
        return -2;
    }


    for (long long i = 3; i < sqrt(n); i+=2)
    {
        if (!(n % i))
            {
                return -2;
            }
    }
    return 0;
}

char* number_to_string(long long int n)
{
    int length = 1;
    long long int temp = n;
    char* string;

    while (temp /= 10)
        ++length;

    temp = llabs(n);


    if (n > 0)
    {   int size = length * 2;
        string = (char*)malloc(sizeof(char) * size);

        if(string == NULL)
        {
            free(string);
            return false;
        }

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
    {   int size = length * 2 + 1;
        string = (char*)malloc(sizeof(char) * size);
        
        if(string == NULL)
        {
            free(string);
            return false;
        }

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
    return string;

}

int table_of_powers(long long int n , long long table[][10])
{
    
    for(int i = 1; i <= 10; i++)
    {
    long long int result = i;
    table[0][i-1] = result;

        for (int j = 2; j <= n; j++)
        {
            result *= i;
            table[j-1][i-1] = result;

        }
        
    }
    return 0;
    
    

}

int sum(long long int n)
{
    if (n < 1)
        return 0;
    long long int ammount = n;
    return ((n-1)/2*n);
}

int factorial(long long int n)
{
    if (n == 0)
        return 1;
    return n*factorial(n-1);
}



int main(int argc, char* argv[])
{
    info();

    if(argc != 3 || (*argv[2] != '-' && *argv[2] != '/') || strlen(argv[2]) != 2)
    {
        error();
        usage();
        return 1;
    }

    long long int temp = 0;

    if (converter(argv[1], &temp))
    {
        printf("Your number must be in \"long long int\" type\n");
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
        if(is_simple(temp) == -1)
            printf("The number must be greater than 1");

        else if (is_simple(temp) == -2)
            printf("The number %d is NOT simple!", temp);
        else
            printf("The number %d is simple!", temp);
    }

    if (argv[2][1] == 's')
    {
        char* s = number_to_string(temp);
        printf("%s", s);
        free(s);
    }

    if(argv[2][1] == 'e')
    {
        if(1 > temp || temp > 10)

        {
            printf("The number must be greater than zero and less then eleven");
            return -5;
        
        }

        long long table[temp][10];

        table_of_powers(temp, table);

        for (int i = 0; i < temp; i++)

            {
                printf("|%5d|", i+1);
                
                for (int j = 0; j < 10; j++)

                    printf("%-12lld", table[i][j]);

                printf("\n");
            }
    }

    if(argv[2][1] == 'a')
    {
        printf("%d", sum(temp));
    }

    if(argv[2][1] == 'f')
    {
        if(temp >= 20)
        {
            printf("Overflow");
            return 3;
        }
        
        if(temp < 0)
        {
            printf("Number must be greater than or equal to 0");
            return 7;
        }

        printf("%d", factorial(temp));
    }

    if(argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' && argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f')
    {
        usage();
        return 4;
    }

    return 0;

}
