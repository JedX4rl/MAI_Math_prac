#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

typedef long long ll;
typedef unsigned long long ull;

typedef enum
{
    OK,
    BAD_ALLOC,
    NULL_POINTER,
    INVALID_INPUT,
    NOT_OK,
    OPENING_ERROR,
    OVERFLOWED
} Status;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NULL_POINTER:
            printf("NULL pointer error\n");
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case NOT_OK:
            printf("Not OK\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        case OVERFLOWED:
            printf("Overflow\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status my_realloc(void** ptr, int size)
{
    void* new_ptr = realloc(*ptr, size);
    if (new_ptr != NULL)
    {
        *ptr = new_ptr;
        return OK;
    }
    *ptr = NULL;
    return BAD_ALLOC;
}

Status int_to_Roman(char** result, int number)
{
    *result = (char*)malloc(sizeof(char) * 16);
    if (*result == NULL)
    {
        return BAD_ALLOC;
    }
    int size = 16;
    if (1 > number || number > 3999)
    {
        return INVALID_INPUT;
    }
    int tmp = 0;
    int vars[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4,1};
    char* Roman_symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int position = 0;
    while (number > 0)
    {
        if (number >= vars[position])
        {
            if (tmp - 2 == size)
            {
                size *= 2;
                Status st = my_realloc(result, sizeof(char) * size);
                if (st)
                {
                    free(*result);
                    return st;
                }
            }
            (*result)[tmp++] = Roman_symbols[position][0];
            if (position & 1)
            {
                (*result)[tmp++] = Roman_symbols[position][1];
            }
            number -= vars[position];
        }
        else
        {
            position++;
        }
    }
    (*result)[tmp] = '\0';
    return OK;
}

Status to_Zeckendorf(unsigned number, char** result)
{
    *result = (char*)malloc(sizeof(char) * 16);
    if (*result == NULL)
    {
        return BAD_ALLOC;
    }
    if (number == 0)
    {
        strcpy(*result, "0");
        return OK;
    }
    else if (number == 1)
    {
        strcpy(*result, "11");
        return OK;
    }
    unsigned size = 8;
    unsigned int* Fibonacci = (unsigned int*)malloc(sizeof(unsigned int) * size);
    if (Fibonacci == NULL)
    {
        free(*result);
        return BAD_ALLOC;
    }
    Fibonacci[0] = 0;
    Fibonacci[1] = 1;
    int tmp = 1;
    while (Fibonacci[tmp++] < number)
    {
        if (tmp >= size)
        {
            size *= 2;
            Status st = my_realloc(&Fibonacci, sizeof(unsigned int) * size);
            if (st)
            {
                free(*result);
                free(Fibonacci);
                return BAD_ALLOC;
            }
        }
        Fibonacci[tmp] = Fibonacci[tmp - 1] + Fibonacci[tmp - 2];
    }
    if (Fibonacci[tmp] != number)
    {
        tmp--;
    }
    int i = 0;
    size = 8;
    for (; tmp > 0; tmp--)
    {
        if (i >= size - 1)
        {
            size *= 2;
            Status st = my_realloc(result, sizeof(char) * size);
            if (st)
            {
                free(*result);
                free(Fibonacci);
                return BAD_ALLOC;
            }
        }
        if (number >= Fibonacci[tmp])
        {
            number -= Fibonacci[tmp];
            (*result)[i] = '1';
        }
        else
        {
            (*result)[i] = '0';
        }
        i++;
    }
    (*result)[i] = '\0';
    free(Fibonacci);
    return OK;
}

Status check_symbol(char symbol, int base, char type)
{
    if (!isalnum(symbol))
    {
        return INVALID_INPUT;
    }
    if (base < 10)
    {
        if (!isdigit(symbol))
        {
            return INVALID_INPUT;
        }
        if (symbol - '0' >= base)
        {
            return INVALID_INPUT;
        }
    }
    int tmp = (type == 'a') ? symbol - 'a' + 10 : symbol - 'A' + 10;
    if (tmp >= base)
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status to(const char* number, int base, char** result)
{
    *result = (char*)malloc(sizeof(char) * 64);
    if (*result == NULL)
    {
        return BAD_ALLOC;
    }
    if (base < 2 || base > 36)
    {
        base = 10;
    }
    long long res = 0;
    int is_negative = 0;
    if (number[0] == '-')
    {
        is_negative = 1;
    }
    int i = is_negative;
    Status st = OK;
    while (number[i] != '\0')
    {
        char ch = number[i];
        if (isalpha(ch) && !('a' <= ch && ch <= 'z'))
        {
            free(*result);
            return INVALID_INPUT;
        }
        st = check_symbol(ch, base, 'a');
        if (st)
        {
            free(*result);
            return st;
        }
        if (ch >= 'a')
        {
            ch -= 39;
        }
        ch -= '0';

        if ((LLONG_MAX - ch) / base < res)
        {
            free(*result);
            return OVERFLOWED;
        }
        res = res * base + ch;
        i++;
    }
    if (is_negative)
    {
        res *= -1;
    }
    snprintf(*result, sizeof(*result), "%lld", res);
    return OK;
}

Status TO(const char* number, int base, char** result)
{
    *result = (char*)malloc(sizeof(char) * 64);
    if (*result == NULL)
    {
        return BAD_ALLOC;
    }
    if (base < 2 || base > 36)
    {
        base = 10;
    }
    long long res = 0;
    int is_negative = 0;
    if (number[0] == '-')
    {
        is_negative = 1;
    }
    int i = is_negative;
    Status st = OK;
    while (number[i] != '\0')
    {
        char ch = number[i];
        if (isalpha(ch) && !('A' <= ch && ch <= 'Z'))
        {
            free(*result);
            return INVALID_INPUT;
        }
        st = check_symbol(ch, base, 'A');
        if (st)
        {
            free(*result);
            return st;
        }
        if (ch >= 'A')
        {
            ch -= 7;
        }
        ch -= '0';

        if ((LLONG_MAX - ch) / base < res)
        {
            free(*result);
            return OVERFLOWED;
        }
        res = res * base + ch;
        i++;
    }
    if (is_negative)
    {
        res *= -1;
    }
    snprintf(*result, sizeof(*result), "%lld", res);


    return OK;
}

Status mem_dump(char** string, void* ptr, ull size)
{
    *string = (char*)malloc(sizeof(char) * 16);
    if (*string == NULL)
    {
        return BAD_ALLOC;
    }
    int capacity = 16;
    Status st = OK;
    unsigned char *p = (unsigned char*)ptr;
    int tmp = 0;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 7; j >= 0; --j)
        {
            if (tmp + 2 == capacity)
            {
                capacity *= 2;
                st = my_realloc(string, sizeof(char) * capacity);
                if (st)
                {
                    free(*string);
                    return st;
                }
            }
            (*string)[tmp] = ((p[i] >> j) & 1) + '0';
            tmp++;
        }
        (*string)[tmp] = ' ';
        tmp++;
    }
    (*string)[tmp] = '\0';
    return OK;
}

int overfprintf(FILE* file, const char* flag, ...)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    char* string = NULL;
    int char_cnt = 0;
    int result = 0;
    int i = 0;

    va_list arguments;
    va_start(arguments, flag);

    while (flag[i] != '\0')
    {
        if (flag[i] == '%' && flag[i+1] != '\0')
        {
            if (flag[i + 1] == 'R' && flag[i + 2] == 'o')
            {
                int curr = va_arg(arguments, int);
                if (int_to_Roman(&string, curr))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'Z' && flag[i + 2] == 'r')
            {
                int curr = va_arg(arguments, int);
                if (to_Zeckendorf(curr, &string))
                {
                    return -1;
                }
                int tmp = (int)strlen(string);
                for (int j = tmp - 1; j >= 0; j--)
                {
                    fputc(string[j], file);
                }
                fputc('1', file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 't' && flag[i + 2] == 'o')
            {
                char* number = va_arg(arguments, char*);
                int base = va_arg(arguments, int);
                if (to(number, base, &string))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'T' && flag[i + 2] == 'O')
            {
                char* number = va_arg(arguments, char*);
                int base = va_arg(arguments, int);
                if (TO(number, base, &string))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'i')
            {
                int number = va_arg(arguments, int);
                if (mem_dump(&string, &number, sizeof(signed int)))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'u')
            {
                int number = va_arg(arguments, unsigned int);
                if (mem_dump(&string, &number, sizeof(unsigned int)))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'd')
            {
                int number = va_arg(arguments, double );
                if (mem_dump(&string, &number, sizeof(double)))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'f')
            {
                int number = va_arg(arguments, double);
                if (mem_dump(&string, &number, sizeof(float)))
                {
                    return -1;
                }
                fputs(string, file);
                char_cnt++;
                i += 2;
                free(string);
            }
            else
            {
                char* another_flag = (char*)malloc(sizeof(char) * (strlen(flag) + 1));
                if (another_flag == NULL)
                {
                    free(string);
                    return -1;
                }
                int j = 0;
                int tmp = i;
                do
                {
                    another_flag[j++] = flag[tmp];
                    tmp++;
                } while (flag[tmp] != '%' && flag[tmp] != '\0');
                another_flag[j] = '\0';
                void* buff = va_arg(arguments, void*);
                result = fprintf(file, another_flag, buff);
                char_cnt += result;
                i = tmp - 1;
                free(another_flag);
            }
        }
        else if (flag[i] == '%')
        {
            return -1;
        }
        else
        {
            fputc(flag[i], file);
            char_cnt++;
        }
        i++;
    }
    va_end(arguments);
    return char_cnt;
}

int oversprintf(char* res, const char* flag, ...)
{
    char* string = NULL;
    int char_cnt = 0;
    int result = 0;
    int i = 0;

    va_list arguments;
    va_start(arguments, flag);

    while (flag[i] != '\0')
    {
        if (flag[i] == '%' && flag[i+1] != '\0')
        {
            if (flag[i + 1] == 'R' && flag[i + 2] == 'o')
            {
                int curr = va_arg(arguments, int);
                if (int_to_Roman(&string, curr))
                {
                    return -1;
                }
                int length = (int) strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'Z' && flag[i + 2] == 'r')
            {
                int curr = va_arg(arguments, int);
                if (to_Zeckendorf(curr, &string))
                {
                    return -1;
                }
                int tmp = (int)strlen(string);
                for (int j = tmp - 1; j >= 0; j--)
                {
                    res[char_cnt++] = string[j];
                }
                res[char_cnt] = '1';
                char_cnt++;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 't' && flag[i + 2] == 'o')
            {
                char* number = va_arg(arguments, char*);
                int base = va_arg(arguments, int);
                if (to(number, base, &string))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'T' && flag[i + 2] == 'O')
            {
                char* number = va_arg(arguments, char*);
                int base = va_arg(arguments, int);
                if (TO(number, base, &string))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'i')
            {
                int number = va_arg(arguments, int);
                if (mem_dump(&string, &number, sizeof(signed int)))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'u')
            {
                int number = va_arg(arguments, unsigned int);
                if (mem_dump(&string, &number, sizeof(unsigned int)))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'd')
            {
                int number = va_arg(arguments, double );
                if (mem_dump(&string, &number, sizeof(double)))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else if (flag[i + 1] == 'm' && flag[i + 2] == 'f')
            {
                int number = va_arg(arguments, double);
                if (mem_dump(&string, &number, sizeof(float)))
                {
                    return -1;
                }
                int length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += length;
                i += 2;
                free(string);
            }
            else
            {
                char* another_flag = (char*)malloc(sizeof(char) * (strlen(flag) + 1));
                if (another_flag == NULL)
                {
                    free(string);
                    return -1;
                }
                int j = 0;
                int tmp = i;
                do
                {
                    another_flag[j++] = flag[tmp];
                    tmp++;
                } while (flag[tmp] != '%' && flag[tmp] != '\0');
                another_flag[j] = '\0';
                void* buff = va_arg(arguments, void*);
                string = (char*)malloc(sizeof(char) * 64);
                if (string == NULL)
                {
                    free(string);
                    return -1;
                }
                int length = (int)strlen(string);
                char* new_buff[256];
                result = sprintf(string, another_flag, buff);
                length = (int)strlen(string);
                memcpy(res + char_cnt, string, sizeof(char) * length);
                char_cnt += result;
                i = tmp - 1;
                free(another_flag);
                free(string);
            }
        }
        else if (flag[i] == '%')
        {
            return -1;
        }
        else
        {
            res[char_cnt++] = flag[i];
        }
        i++;
    }
    res[char_cnt++] = '\0';
    va_end(arguments);
    return char_cnt;
}




int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    FILE* file = fopen(argv[1], "w");
    if (file == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    char ch = 'K';
    if (overfprintf(file, "int %d\n%c\n%s\n\n\n", 14844, ch, "aba__ali") == -1)
    {
        errors(NOT_OK);
    }
    if (overfprintf(file, "%Ro\nhfjahfa72613\n%Zr\n", 142, 89) == -1)
    {
        errors(NOT_OK);
    }
    if (overfprintf(file, "%to\n%TO\n", "a", 16, "A", 16) == -1)
    {
        errors(NOT_OK);
    }
    char* test = (char*)malloc(sizeof(char) * 256);
    if (test == NULL)
    {
        return BAD_ALLOC;
    }
    int a = 78417;

    if (oversprintf(test, "ABcdhukfha41 %d\n%mi\n%Ro\n%Zr\n%TO", -31, a, 30, 13, "F", 16) == -1)
    {
        errors(NOT_OK);
    }
    printf("%s\n", test);
    free(test);
    return 0;
}
