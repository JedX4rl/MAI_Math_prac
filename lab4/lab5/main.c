#include <stdio.h>
#include "my_lib.h"
#include "deque.h"

Status file_processing(FILE* file, ull* cnt, expression_data** data)
{
    if (file == NULL || cnt == NULL || data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull counter = 0;
    ull size = 2;
    expression_data* tmp = (expression_data*)malloc(sizeof(expression_data) * size);
    if (tmp == NULL)
    {
        return BAD_ALLOC;
    }
    char* infix = NULL;
    Status st = OK;
    st = read_line(file, &infix);
    if (st == END_OF_FILE)
    {
        free(tmp);
        *cnt = 0;
        *data = NULL;
        return OK;
    }
    while (!st)
    {
        Deque postfix;
        ll result = 0;
        null_deque(&postfix);
        st = convert_to_postfix(infix, &postfix);
        st = st ? st : calculate_postfix(postfix, &result);
        if (st)
        {
            destruct_deque(&postfix);
        }
        expression_data curr;
        curr.is_valid = st;
        curr.infix = infix;
        curr.postfix = postfix;
        curr.result = result;
        if (counter == size)
        {
            size *= 2;
            expression_data* n_data = (expression_data*)realloc(tmp, sizeof(expression_data) * size);
            if (n_data == NULL)
            {
                st = BAD_ALLOC;
                free(infix);
                destruct_deque(&postfix);
            }
            else
            {
                tmp = n_data;
            }
        }
        if (!st)
        {
            tmp[counter] = curr;
            counter++;
        }
        st = st ? st : read_line(file, &infix);
    }
    if (st == END_OF_FILE)
    {
        st = OK;
    }
    if (!st)
    {
        expression_data* temp = (expression_data*)realloc(tmp, sizeof(expression_data) * size);
        if (temp == NULL)
        {
            st = BAD_ALLOC;
        }
        else
        {
            tmp = temp;
        }
    }
    if (st)
    {
        for (ull i = 0; i < counter; ++i)
        {
            free(tmp[0].infix);
            destruct_deque(&tmp[0].postfix);
        }
        free(tmp);
        return st;
    }
    *cnt = counter;
    *data = tmp;
    return OK;
}
//
//Status new_out_path(const char* in, char** out)
//{
//    if (in == NULL || out == NULL)
//    {
//        return INVALID_ARGUMENT;
//    }
//    char* out_tmp = (char*)malloc(sizeof(char) * strlen(in) + 5);
//    if ()
//}

int main()
{
    Deque test;
    create_deque(&test);
    ll t;
    FILE* in = fopen("/Users/nikitatretakov/Uni/Labs/MathPracticum/Laboratory_work4/lab5/in.txt", "r");
    ull te;
    expression_data* data;
    file_processing(in, &te, &data);
//    char* g;
//    get_str(&g);
    Status st = convert_to_postfix("16+24", &test);
    print_deque(test);
    calculate_postfix(test, &t);
    printf("\n%lld\n", t);
    return 0;
}
