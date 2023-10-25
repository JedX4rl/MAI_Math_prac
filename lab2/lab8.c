#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum
{
    OK,
    NOT_OK,
    INVALID_INPUT,
    BAD_ALLOC,
} Status;

Status add_zeros(char* str, char* res, unsigned long length, unsigned long res_length)
{
    unsigned long temp = res_length - length;
    for (int i = 0; i < res_length; ++i)
    {
        if (i >= temp)
        {
            res[i] = str[i-temp];
        }
        else
        {
            res[i] = '0';
        }
    }
    return  OK;
}

Status is_valid(int base, char* num)
{
    if (num == NULL)
    {
        return  NOT_OK;
    }
    if (num[0] == '-')
    {
        return  INVALID_INPUT;
    }
    unsigned long length = strlen(num);
    if (base <= 10)
    {
        for (unsigned long i = 0; i < length; ++i)
        {
            if (num[i] - '0' >= base)
            {
                return INVALID_INPUT;
            }
        }
    }
    else
    {
        for (unsigned long i = 0; i < length; ++i)
        {
            char c = num[i];
            if (!isalnum(c))
            {
                return INVALID_INPUT;
            }
            else if (toupper(c) - 'A' + 10 >= base)
            {
                return  INVALID_INPUT;
            }
        }
    }
    return  OK;
}

Status ascii_code(char* digit)
{
    char temp = *digit;
    if (temp >= 'a')
    {
        temp -= 39;
    }
    else if (temp >= 'A')
    {
        temp -= 7;
    }
    temp -= '0';
    *digit = temp;
    return  OK;
}

Status num_convertor(int base, int* next_digit, char num1, char num2, char* res)
{
    ascii_code(&num1);
    ascii_code(&num2);
    *res = num1 + num2 + *next_digit;
    *next_digit = 0;
    if (*res >= base)
    {
        *next_digit = 1;
        *res -= base;
    }

    *res += '0';
    if (*res > '9') {
        *res += 7;
    }
    return  OK;
}

Status zero_skipper(char** res)
{
    int length = (int) strlen(*res);
    int flag = 0;
    int position = 0;
    char* temp;

    while (flag != 1)
    {
        if ((*res)[position] != '0')
        {
            flag = 1;
        }
        else
        {
            position++;
        }
    }
    if (length == 2 && (*res)[0] == '0' && (*res)[1] == '0')
    {
        position = 1;
    }
    int length_of_ans = length - position;
    temp = (char*)malloc(sizeof(char) * (length_of_ans + 1));
    if (temp == NULL)
    {
        return  BAD_ALLOC;
    }
    temp[length_of_ans] = '\0';
    for (int i = 0; i < length_of_ans; ++i)
    {
        temp[i] = (*res)[i + position];
    }
    *res = temp;
    return  OK;
}

Status sum(int base, char* num1, char* num2, char** answer)
{
    if (num1 == NULL || num2 == NULL)
    {
        return  INVALID_INPUT;
    }
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    char* second_num = NULL;
    char* greater_num = NULL;
    unsigned long size_of_res = len2 + 1;
    unsigned long size_of_second = len1;
    if (len1 > len2)
    {
        second_num = num2;
        greater_num = num1;
        size_of_res = len1 + 1;
        size_of_second = len2;
    }
    else
    {
        second_num = num1;
        greater_num = num2;
    }
    char* result = (char*)malloc(sizeof(char) * (size_of_res + 2));
    if (result == NULL)
    {
        return  BAD_ALLOC;
    }
    char* sec_num = (char*)malloc(sizeof(char) * (size_of_res)); //free sec_num;
    if (sec_num == NULL)
    {
        free(result);
        return  BAD_ALLOC;
    }
    sec_num[size_of_res] = '\0';
    result[size_of_res+1] = '\0';
    int next_digit = 0;
    add_zeros(second_num, sec_num, size_of_second, size_of_res - 1);

    for (unsigned long i = size_of_res; i > 1; i--)
    {
        char temp;
        num_convertor(base, &next_digit, greater_num[i-2], sec_num[i-2], &temp);
        result[i-1] = temp;
    }
    if (next_digit == 1)
    {
        result[0] = '1';
    }
    else
    {
        result[0] = '0';
    }
    if (zero_skipper(&result) != OK)
    {
        free(sec_num);
        free(result);
        return  BAD_ALLOC;
    }
    free(sec_num);
    *answer = result;
    return  OK;

}

Status function(char** res, int base, int cnt, ...)
{
    if (base < 2 || base > 36)
    {
        return INVALID_INPUT;
    }
    if (cnt < 1)
    {
        return  INVALID_INPUT;
    }
    va_list numbers;
    va_start(numbers, cnt);
    *res = va_arg(numbers, char*);
    char* num1;
    for (int i = 1; i < cnt; ++i)
    {
        num1 = *res;
        char* num2 = va_arg(numbers, char*);
        if (is_valid(base, num1) != OK || is_valid(base, num2) != OK)
        {
            return INVALID_INPUT;
        }
        if (sum(base, num1, num2, res) == BAD_ALLOC)
        {
            free(res);
            return  BAD_ALLOC;
        }

    }

    return OK;
}

int main() {
    char *answer;
    if (function(&answer, 36, 3, "pz", "1", "f") == OK)
    {
        printf("%s\n", answer);
        free(answer);
        return  OK;
    }
    else
    {
        printf("something went wrong\n");
        return  NOT_OK;
    }
    return 0;
}