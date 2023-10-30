#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#define BUFF_SIZE 256

typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK,
    BAD_ALLOC,
    OVERFLOWED,
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
        case NOT_OK:
            printf("Not OK\n");
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case OVERFLOWED:
            printf("Overflow\n");
            return;
        case NULL_POINTER:
            printf("NULL pointer error\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status char_to_int(char ch, int* res, int base)
{
    int temp = 0;
    if ('0' <= ch && ch <= '9')
    {
        temp = ch  - '0';
        if (temp > base - 1)
        {
            return INVALID_INPUT;
        }
    }
    else if('A' <= ch && ch <= 'Z')
    {
        temp = ch - 'A' + 10;
        if (temp > base - 1)
        {
            return INVALID_INPUT;
        }
    }
    *res = temp;
    return OK;
}

Status is_lower(char* str, int length)
{
    for (int i = 0; i < length; ++i)
    {
        char ch = str[i];
        if ('A' <= ch && ch <= 'Z')
        {
            return NOT_OK;
        }
    }
    return OK;
}

Status is_upper(char* str, int length)
{
    for (int i = 0; i < length; ++i)
    {
        char ch = str[i];
        if ('a' <= ch && ch <= 'z')
        {
            return NOT_OK;
        }
    }
    return OK;
}

Status roman_to_int(const char* str, int* result) {
    int length = (int) strlen(str);

    if (!length)
    {
        return  INVALID_INPUT;
    }
    *result = 0;
    int position = length - 1;
    for (int i = length - 1; i >= 0; --i)
    {
        if (str[i] != 'I' && str[i] != 'V' && str[i] != 'X' && str[i] != 'L' && str[i] != 'C' && str[i] != 'D' && str[i] != 'M')
        {
            position = i;
        }
    }


    for (int i = position; i >= 0; --i)
    {
        char curr = str[i];
        switch (curr)
        {
            case 'I':
                if (str[i + 1] == 'X' || str[i + 1] == 'V')
                {
                    (*result)--;
                }
                else
                {
                    (*result)++;
                }
                break;
            case 'V':
                (*result) += 5;
                break;
            case 'X':
                if (str[i+1] == 'L' || str[i+1] == 'C')
                {
                    (*result) -= 10;
                }
                else
                {
                    (*result) += 10;
                }
                break;
            case 'L':
                (*result) += 50;
                break;
            case 'C':
                if (str[i + 1] == 'D' || str[i+1] == 'M')
                {
                    (*result) -= 100;
                }
                else
                {
                    (*result) += 100;
                }
                break;
            case 'M':
                (*result) += 1000;
            default:
                break;
        }
    }
    if ((*result) > 3999)
    {
        (*result = 0);
        return INVALID_INPUT;
    }
    return OK;
}

Status from_Zeckendorf(const char* str, unsigned int* result)
{
    int length = (int)strlen(str);
    if (!length)
    {
        return INVALID_INPUT;
    }
    unsigned number_1 = 0;
    unsigned number_2 = 1;
    unsigned current = 0;
    *result = 0;
    for (int i = 0; i < length - 1; i++)
    {
        char c = str[i];
        if (c == '1')
        {
            (*result) += current;
        }
        number_1 = number_2;
        number_2 = current;
        current = number_1 + number_2;
    }
    return OK;
}

Status to_int(const char* str, int base, int* result, int flag)
{
    if (2 > base || base > 36)
    {
        base = 10;
    }
    long long int dec_number = 0;
    int power = 0;
    int length = (int)strlen(str);
    for (int i = length - 1; i >= 0; --i)
    {
        char ch = str[i];
        int temp = 0;
        if (!isalnum(ch))
        {
            return INVALID_INPUT;
        }
        if (isalpha(ch))
        {
            if (flag == 0)
            {
                if (!('a' <= ch && ch <= 'z'))
                {
                    return INVALID_INPUT;
                }
                ch = toupper(ch);
            }
            else if (!('A' <= ch && ch <= 'Z'))
            {
                return INVALID_INPUT;
            }
        }
        if (char_to_int(ch, &temp, base) != OK)
        {
            return INVALID_INPUT;
        }

        dec_number += temp * pow(base, power);
        if (dec_number > INT_MAX)
        {
            return OVERFLOWED;
        }
        power++;
    }
    *result = dec_number;
    return OK;
}

int overfscanf(FILE* stream, const char* format, ...)
{
    if (!stream)
    {
        errors(NOT_OK);
        return NOT_OK;
    }
    int size_of_format = (int)strlen(format);
    if (!size_of_format)
    {
        errors(NOT_OK);
        return NOT_OK;
    }
    int argc = 0;
    for (int i = 0; i < size_of_format; ++i)
    {
        if (format[i] == '%')
        {
            if (format[i+1] != '%')
            {
                argc++;
            }
        }
    }
    va_list ptr;
    va_start(ptr, argc);
    int index = 0;
    char* flag = NULL;
    int result = 0;
    for (int i = 0; i < argc; ++i)
    {
        int size_of_flag = 1;
        int capacity_of_flag = 2;
        while (format[index] != '%' && index < size_of_format)
        {
            index++;
        }
        flag = (char*)malloc(sizeof(char) * (capacity_of_flag * 2));
        if (flag == NULL)
        {
            errors(BAD_ALLOC);
            return BAD_ALLOC;
        }
        flag[0] = '%';
        index++;
        while ((isalnum(format[index])) && index < size_of_format)
        {
            char ch = format[index];
            flag[size_of_flag] = ch;
            size_of_flag++;

            if (size_of_flag == capacity_of_flag - 1)
            {
                capacity_of_flag *= 2;
                char *new_flag = (char*)realloc(flag, capacity_of_flag);
                if (new_flag != NULL)
                {
                    flag = new_flag;
                }
                if (new_flag == NULL)
                {
                    free(flag);
                    errors(BAD_ALLOC);
                    return BAD_ALLOC;
                }

            }
            index++;
        }
        flag[size_of_flag] = '\0';
        if (!strcmp(flag, "%Ro\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            char temp[BUFF_SIZE];
            result += fscanf(stream, "%s", temp);
            Status st_roman = roman_to_int(temp, ptr_arg);
        }
        else if (!strcmp(flag, "%Zr\0"))
        {
            unsigned int* ptr_arg = va_arg(ptr, unsigned int*);
            char temp[BUFF_SIZE];
            result += fscanf(stream, "%s", temp);
            Status st_zeck = from_Zeckendorf(temp, ptr_arg);
        }
        else if (!strcmp(flag, "%Cv\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char num_str[BUFF_SIZE];
            result += fscanf(stream, "%s", num_str);
            int length = (int)strlen(num_str);
            if (is_upper(num_str, length) == OK)
            {
                printf("Letters are in uppercase\n");
                (*ptr_arg) = 0;
            }
            else
            {
                Status st_cv = to_int(num_str, base, ptr_arg, 0);
            }
        }
        else if (!strcmp(flag, "%CV\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char num_str[BUFF_SIZE];
            result += fscanf(stream, "%s", num_str);
            int length = (int)strlen(num_str);
            if (is_lower(num_str, length) == OK)
            {
                printf("Letters are in lowercase\n");
                (*ptr_arg) = 0;
            }
            else
            {
                Status st_cv = to_int(num_str, base, ptr_arg, 1);
            }
        }
        else
        {
            void* temp_arg = va_arg(ptr, void*);
            result += fscanf(stream, flag, temp_arg);
        }
        free(flag);
    }
    va_end(ptr);
    return OK;
}

int oversscanf(char* buff, const char* format, ...)
{
    if (!buff)
    {
        return NULL_POINTER;
    }
    int size_of_format = (int)strlen(format);
    if (!size_of_format)
    {
        return NOT_OK;
    }
    int argc = 0;
    for (int i = 0; i < size_of_format; ++i)
    {
        if (format[i] == '%')
        {
            if (format[i+1] != '%')
            {
                argc++;
            }
        }
    }
    va_list ptr;
    va_start(ptr, argc);
    int index = 0;
    int index_of_buff = 0;
    char* flag = NULL;
    int result = 0;
    for (int i = 0; i < argc; ++i)
    {
        int size_of_flag = 1;
        int capacity_of_flag = 2;
        while (format[index] != '%' && index < size_of_format)
        {
            index++;
        }
        flag = (char*)malloc(sizeof(char) * (capacity_of_flag * 2));
        if (flag == NULL)
        {
            errors(BAD_ALLOC);
            return BAD_ALLOC;
        }
        flag[0] = '%';
        index++;
        while ((isalnum(format[index])) && index < size_of_format)
        {
            char ch = format[index];
            flag[size_of_flag] = ch;
            size_of_flag++;

            if (size_of_flag == capacity_of_flag - 1)
            {
                capacity_of_flag *= 2;
                char *new_flag = (char*)realloc(flag, capacity_of_flag);
                if (new_flag != NULL)
                {
                    flag = new_flag;
                }
                if (new_flag == NULL)
                {
                    free(flag);
                    errors(BAD_ALLOC);
                    return BAD_ALLOC;
                }

            }
            index++;
        }
        flag[size_of_flag] = '\0';
        if (!strcmp(flag, "%Ro\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            char temp[BUFF_SIZE];
            result += sscanf(buff + index_of_buff, "%s", temp);
            index_of_buff += (int)strlen(temp) + 1;
            Status st_roman = roman_to_int(temp, ptr_arg);
            if (st_roman != OK)
            {
                return -1;
            }
        }
        else if (!strcmp(flag, "%Zr\0"))
        {
            unsigned int* ptr_arg = va_arg(ptr, unsigned int*);
            char temp[BUFF_SIZE];
            result += sscanf(buff + index_of_buff, "%s", temp);
            index_of_buff += (int)strlen(temp) + 1;
            Status st_zeck = from_Zeckendorf(temp, ptr_arg);
            if (st_zeck != OK)
            {
                return -1;
            }
        }
        else if (!strcmp(flag, "%Cv\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char num_str[BUFF_SIZE];
            result += sscanf(buff + index_of_buff, "%s", num_str);
            int length = (int)strlen(num_str);
            index_of_buff += (int) strlen(num_str) + 1;
            if (is_upper(num_str, length) == OK)
            {
                printf("Letters are in uppercase\n");
                (*ptr_arg) = 0;
            }
            else
            {
                Status st_cv = to_int(num_str, base, ptr_arg, 0);
                if (st_cv != OK)
                {
                    return -1;
                }
            }
        }
        else if (!strcmp(flag, "%CV\0"))
        {
            int* ptr_arg = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char num_str[BUFF_SIZE];
            result += sscanf(buff + index_of_buff, "%s", num_str);
            index_of_buff += (int) strlen(num_str) + 1;
            int length = (int)strlen(num_str);
            if (is_lower(num_str, length) == OK)
            {
                printf("Letters are in lowercase\n");
                (*ptr_arg) = 0;
            }
            else
            {
                Status st_cv = to_int(num_str, base, ptr_arg, 1);
                if (st_cv != OK)
                {
                    return -1;
                }
            }
        }
        else
        {
            void* temp_arg = va_arg(ptr, void*);
            result += sscanf(buff + index_of_buff, flag, temp_arg);
            index_of_buff += (int) strlen((const char*) temp_arg) + 1;
        }
        free(flag);
    }
    va_end(ptr);
    return OK;
}

int main(int argc, char* argv[])
{
    char s[BUFF_SIZE];
    int num1, num2, num3;
    unsigned int num4;
    oversscanf("XX 7 001001011", "%Ro %d %Zr", &num1, &num2, &num4);
    FILE* file = fopen("/Users/nikitatretakov/Uni/Labs/MathPracticum/lab6/file1.txt", "r");

    overfscanf(file, "%Ro %d %Zr", &num1, &num2, &num4);
    printf("%d %d %u\n", num1, num2, num4);
    char c = ')';
    int n = 0;
    char_to_int(c, &n, 10);
    printf("%d\n", n);
}
