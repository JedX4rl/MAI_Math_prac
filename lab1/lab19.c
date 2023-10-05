#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    OVERFLOWED,
    BAD_ALLOC
} Status;

Status is_int_number(char s[])
{
    int cnt = 0;
    int length = strlen(s);

    if (length > 10)
    {
        return INVALID_INPUT;
    }

    if (s[0] != '-')
    {
        for (int i = 0; i < length; i++)
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
            if (cnt >= 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }

    else
    {
        for (int i = 1; i < length; i++)
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
            if (cnt >= 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }
    return OK;
}


Status char_to_int(char s[], int* n)
{
    long k = atol(s);
    if (k > INT_MAX)
    {
        return INVALID_INPUT;
    }
    
    *n = (int)k;
    return OK;
}

Status function_1(int arr[], int size, int* minimum, int* maximum)
{
    int min_idx = 0;
    int max_idx = 0;
    int max = arr[0];
    int min = arr[0];
    int i;
    int temp;
    for (i = 0; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
            max_idx = i;
        }
        if (arr[i] < min)
        {
            min = arr[i];
            min_idx = i;
        }
    }
    *minimum = min;
    *maximum = max;
    temp = min;
    arr[min_idx] = max;
    arr[max_idx] = temp;

    return OK;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        printf("Check the amount of arguments\n");
        return INVALID_INPUT;
    }

    if (is_int_number(argv[1]) == INVALID_INPUT || is_int_number(argv[2]) == INVALID_INPUT || is_int_number(argv[3]) == INVALID_INPUT)
    {
        printf("Invalid input\n");
        return  INVALID_INPUT;
    }

    int a, b, c, min, max;

    if (((char_to_int(argv[1], &c)) == INVALID_INPUT) || ((char_to_int(argv[2], &a)) == INVALID_INPUT) || ((char_to_int(argv[3], &b)) == INVALID_INPUT))
    {
        printf("Overflow\n");
        return OVERFLOWED;
    }
    const int size = c;

    if (a >= b)
    {
        printf("b must be greater than a\n");
        return INVALID_INPUT;
    }

    srand(time(NULL));
    int array[size];
    printf("-------part 1-------\n");
    printf("The array is:\n");
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % (b - a + 1) + a;
        printf("%d ", array[i]);
        if (i == size - 1)
        {
            printf("\n");
        }
    }

    if (function_1(array, size, &min, &max) == OK)
    {
        printf("The modified array where the minimum is %d and the maximum is %d:\n", min, max);
        for (int i = 0; i < size; i++)
        {
            printf("%d ", array[i]);
        }
    }
    /*----------------------part 2----------------------*/
    printf("\n-------part 2-------\n");
    
    int a_size = rand() % (10000 - 10 + 1) + 10;
    int b_size = rand() % (10000 - 10 + 1) + 10;

    int* a_array;
    int* b_array;
    a_array = (int*)malloc(sizeof(int) * a_size);
    if (a_array == NULL)
    {
        return BAD_ALLOC;
    }
    b_array = (int*)malloc(sizeof(int) * b_size);
    if (b_array == NULL)
    {
        return BAD_ALLOC;
    }

    for (int i = 0; i < a_size; i++)
    {
        a_array[i] = rand() % (-1000 - 1000 + 1) + 1000;
        printf("%d ", a_array[i]);
    }
    printf("\n");
    for (int i = 0; i < b_size; i++)
    {
        b_array[i] = rand() % (-1000 - 1000 + 1) + 1000;
        printf("%d ", b_array[i]);
    }
    printf("\n");
    int* c_array;
    c_array = (int*)malloc(sizeof(int) * a_size);
    if (c_array == NULL)
    {
        return BAD_ALLOC;
    } 

    int part_a;
    int part_b = 0;
    int part_c;
    int min_diff = INT_MAX;

    for (int i = 0; i < a_size; i++)
    {
        part_a = a_array[i];


        for (int j = 0; j < b_size; j++)
        {
           if (abs(abs(a_array[i]) - abs(b_array[j])) < min_diff)
           {
                min_diff = abs(abs(a_array[i]) - abs(b_array[j]));
                part_b = b_array[i];
           }
        }
        c_array[i] = part_a + part_b;
        printf("%d ", c_array[i]);
    }

    free(a_array);
    free(b_array);
    free(c_array);
    return 0;
}