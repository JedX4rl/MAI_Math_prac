#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

void Usage()
{
    printf("Program for finding the max number by modula and turn it into another base\n");
}

typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC
} Status;

Status end(char* s)
{
    if (s == NULL)
    {
        return INVALID_INPUT;
    }
    if (s[0] == 'S' && s[1] == 't' && s[2] == 'o' && s[3] == 'p' && !s[4])
    {
        return OK;
    }
    return INVALID_INPUT;
}

char to_char(int number)
{
    if (number < 10)
    {
        return number + '0';
    }
    if (number < 36)
    {
        return 'A' + number - 10;
    }
    return -1;
}

Status str_valid_check(char* str, long long* number, long long base)
{
    if (str[0] == 'S' && str[1] == 't' && str[2] == 'o' && str[3] == 'p')
    {
        return OK;
    }
    char *ending;
    *number = strtoll(str, &ending, base);
    return (ending != str + strlen(str));
}

Status get_string(char** string)
{
    char* input = (char*)malloc(sizeof(char) * 1);
    if (input == NULL)
    {
        return BAD_ALLOC;
    }
    input[0] = '\0';
    char ch;
    int amount = 1;

    while ((ch = getchar()) != '\n')
    {
        if (ch == EOF)
        {
            break;
        }
        if (strlen(input) == amount)
        {
            char* temp = (char*)realloc(input, sizeof(char) * (amount * 2 + 1));
            if (temp == NULL)
            {
                return BAD_ALLOC;
            }
            input = temp;
            free(temp);
        }
        input[amount] = '\0';
        input[amount-1] = ch;
        amount++;
    }
    int length = strlen(input);
    if (end(input) != OK)
    {
        for (int i = 0; i < length; i++)
        {
            if (isalpha(input[i]))
            {
                if (!('A' <= input[i] && input[i] <= 'Z'))
                {
                    free(input);
                    return INVALID_INPUT;
                }
            }
        }
        
    }
    *string = input;
    return OK;
}

Status get_max_number(long long base, long long* result, char** answer)
{
    int size = 0;
    char* str = NULL;
    char* ending;
    char* max_string;
    long long current_number = -1;
    long long max_number = 0;

    while (end(str) != OK)
    { 
        if (get_string(&str) != OK)
        {
            free(str);
            return INVALID_INPUT;
        }
        if (str_valid_check(str, &current_number, base) != 0)
        {
            free(str);
            return INVALID_INPUT;
        }
        if (labs(current_number) > labs(max_number))
        {
            max_number = current_number;
            max_string = str;
        }
        
    }
    *result = max_number;
    *answer = max_string;
    free(str);
    return OK;
}

Status without_zeros(char** s, int length)
{
    int is_host_zero = 1;
    int position = 0;
    int is_negative = 0;
    if ((*s)[0] == '-')
    {
        is_negative = 1;
    }
    if ((*s)[0] == 0)
    {
        is_host_zero = 1;
    }
    else 
    {
        is_host_zero = 0;
    }
    if (is_host_zero)
    {
        for (int i = 0; i < length; i++)
        {
            if ((*s)[i] == '0')
            {
                position = i;
                position++;
            }
        }
    }
    int size = length - position + 1;
    if (is_negative)
    {
        position++;
        size++;
    }
    char* string = (char*)malloc(sizeof(char) * size);
    if (string == NULL)
    {
        return BAD_ALLOC;
    }
    string[size - 1] = '\0';
    if (is_negative)
    {   
        string[0] = '-';
        for (int i = 1; i < size-1; i++)
        {
            string[i] = (*s)[position + (i - 1)];
        }
    }
    else
    {
        for (int i = 0; i < size-1; i++)
        {
            string[i] = (*s)[position + i];
        }
    }
    free(*s);
    *s = string;
    return OK;
}

Status convert(long long number, int base, char res[])
{
    int current_position = 0;
    long long div = 1;

    if (number < 0)
    {
        res[current_position++] = '-';
        while (div <= LLONG_MAX / base && -div * base > number)
        {
            div *= base;
        }
    }
    else
        while (div <= LLONG_MAX / base && div * base > number)
        {
            div *= base;
        }
    while (div > 0)
    {
        res[current_position++] = to_char(llabs(number / div % base));
        div /= base;
    }
    res[current_position] = '\0';
    return OK;
}

int main()
{
    Usage();
    printf("Enter the base of a numeral system\n");
    long long base;
    scanf("%lld", &base);
    if (2 > base || base > 36)
    {
        printf("The base must be [2..36]\n");
        return INVALID_INPUT;
    }
    long long result;
    char* max_str;
    printf ("Enter the numbers, then enter: Stop\n");
    if (get_max_number(base, &result, &max_str) != OK)
    {
        printf("Invalid input, check entered data, letter must be in uppercase\n");
        return INVALID_INPUT;
    }
    if (without_zeros(&max_str, strlen(max_str)) != OK)
    {
        printf("Bad alloc\n");
        free(max_str);
        return BAD_ALLOC;
    }
    printf("The max number is: ");
    printf("%s\n", max_str);
    char str_9[60];
    char str_18[60];
    char str_27[60];
    char str_36[60];

    convert(result, 9, str_9);
    convert(result, 18, str_18);
    convert(result, 27, str_27);
    convert(result, 36, str_36);

    printf("The number in base 9: %s\n", str_9);
    printf("The number in base 18: %s\n", str_18);
    printf("The number in base 27: %s\n", str_27);
    printf("The number in base 36: %s\n", str_36);

    free(max_str);
}