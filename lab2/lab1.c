#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK,
    OVERFLOW,
    BAD_ALLOC
} Status;

void Usage()
{
    printf("Correct flags: -l -r -u -n -c (-c contains many argc)\n");
    printf("Enter flag, string, and only for -c unsigned int seed with arbitary amount of strings\n");
}

Status strlength(char* str, int *result)
{
    int size = 0;
    while (str[size] != '\0')
    {
        if (size > INT_MAX - 1)
        {
            return OVERFLOW;
        }
        size++;
    }
    *result = size;
    return OK;
}

// int strlength(char* str)
// {
//     int size = 0;
//     while (str[size] != '\0')
//     {
//         if (size > INT_MAX - 1)
//         {
//             return -1;
//         }
//         size++;
//     }
//     return size;
// }


// Status my_strcat
char* my_strcat(char* result, char* source) //to do
{
    int size = 0;
    strlength(result, &size);
    char* ptr = result + size;
    while (*source != '\0') {
        *ptr++ = *source++;
    }
    *ptr = '\0';
    return result;
}

Status is_unsigned_int(char* str)
{
    int length = 0;
    strlength(str, &length);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
    
}

Status input_checker(int argc, char* argv[])
{
    int size = 0;
    strlength(argv[1], &size);
    if (argc < 3)
    {   
        return INVALID_INPUT;
    }
    if (size != 2)
    {
        return INVALID_INPUT;
    }
    if (argv [1][0] != '-')
    {
        return INVALID_INPUT;
    }
    char ch = argv[1][1];
    if (ch != 'l' && ch != 'r' && ch != 'u' && ch != 'n'&&  ch != 'c')
    {
        return INVALID_INPUT;
    }
    if (ch == 'c')
    {
        int len;
        if (argc < 4)
        {
            return INVALID_INPUT;
        }
        else if (is_unsigned_int(argv[3]) != OK)
        {
            return INVALID_INPUT;
        }
        else
        {
            return OK;
        }
    }
    if (argc != 3)
    {
        return INVALID_INPUT;
    }
    
    return OK;
}

Status flag_i(char* str, int* res)
{
    int temp;
    strlength(str, &temp);
        if (temp < 0)
        {
            return OVERFLOW;
        }
    *res = temp;
    return OK;
}

Status flag_r(char* str)
{
    int size = 0;
    strlength(str, &size);
    char* start = str;
    char* end = str + size - 1;
    while (start < end)
    {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
    return OK;
}

Status flag_u(char* str, char** str2)
{
    int length = 0;
    strlength(str, &length);
    *str2 = (char*)malloc(sizeof(char) * length + 1);
    if (str2 == NULL)
    {
        return BAD_ALLOC;
    }
    (*str2)[length] = '\0';
    my_strcat(*str2, str);
    for (int i = 1; i < length; i+=2)
    {
        (*str2)[i] = toupper(str[i]);
    }

    return OK;
    
}

Status flag_n(char* str, char* result)
{
    int position = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
        {
            result[position++] = str[i];
        }
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
        {
            result[position++] = str[i];
        }
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]))
        {
            result[position++] = str[i];
        }
    }
    result[position] = '\0';
    return OK;
    
}

Status flag_c(int argc, char* argv[], char** string)
{
    if (string == NULL)
    {
        return NOT_OK;
    }
    int amount_of_strings = argc - 3;
    long long temp = 0;
    strlength(argv[2], &temp);
    int* flags = (int*)calloc(amount_of_strings, sizeof(int));
    if (flags == NULL)
    {
        return BAD_ALLOC;
    }
    char** arr_of_strings = (char**)malloc(sizeof(char*) * (amount_of_strings));
    if (arr_of_strings == NULL)
    {
        return BAD_ALLOC;
    }
    arr_of_strings[0] = argv[2];

    for (int i = 1; i < amount_of_strings; i++)
    {
        arr_of_strings[i] = argv[i+3];
        int len = 0;
        strlength(argv[i+3], &len);
        temp += len;
    }
    *string = (char*)malloc(sizeof(char) * (temp + 1));
    if (*string == NULL)
    {
        return BAD_ALLOC;
    }
    for (int i = 0; i != amount_of_strings; i++)
    {
        int j = rand() % amount_of_strings;
        while (flags[j % amount_of_strings] != 0)
        {
            j++;
        }
        j %= amount_of_strings;
        flags[j] = 1;
        my_strcat(*string, arr_of_strings[j]);
    }
    (*string)[temp] = '\0';
    free(arr_of_strings);
    return OK;
}

// int compare(void *a, void *b)
// {
//     return *(int*)a - *(int*)b;
// }

int main(int argc, char* argv[])
{
    if (input_checker(argc, argv) != OK)
    {
        printf("Invalid input, check entered data\n");
        Usage();
        return INVALID_INPUT;
    }
    int temp = 0;
    char* str = argv[2];
    int length = 0;
    strlength(str, &length);
    char* result = (char*)malloc(sizeof(char) * (length + 1));
    if (result == NULL)
    {
        printf("Allocation error occured\n");
        return BAD_ALLOC;
    }
    char* string;
    unsigned int seed;
    switch (argv[1][1])
    {
    case 'l':
        if (flag_i(argv[2], &temp) == OVERFLOW)
        {
            printf("Overflow\n");
        }
        printf("%d\n", temp);
        return OK;
    case 'r':
        flag_r(str);
        printf("%s\n", str);
        return OK;
    case 'u':
        flag_u(str, &string);
        printf("%s\n", string);
        free(string);
        return OK;
    case 'n':
        flag_n(str, result);
        printf("%s\n", result);
        free(result);
        return OK;
    case 'c':
        if ((seed = atoi(argv[3])) == 0)
        {
            printf("Invalid input\n");
            Usage();
            return INVALID_INPUT;
        }
        srand(seed);
        if (flag_c(argc, argv, &string) != OK)
        {
            return NOT_OK;
        }
        printf("%s\n", string);
        free(string);
    default:
        break;
    }
    return 0;
}
