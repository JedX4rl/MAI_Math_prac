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
    while (st != END_OF_FILE)
    {
        Stack postfix;
        ll result = 0;
        null_stack(&postfix);
        st = convert_to_postfix(infix, &postfix);
        st = st ? st : calculate_postfix(postfix, &result);
        if (st)
        {
            destruct_stack(&postfix);
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
                destruct_stack(&postfix);
            }
            else
            {
                tmp = n_data;
            }
        }
        if (st != BAD_ALLOC)
        {
            tmp[counter] = curr;
            counter++;
        }
        st = read_line(file, &infix);
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
    *cnt = counter;
    *data = tmp;
    return st;
}

Status new_out_path(const char* in, char** out)
{
    if (in == NULL || out == NULL)
    {
        return INVALID_ARGUMENT;
    }
    char* out_tmp = (char*)malloc(sizeof(char) * strlen(in) + 5);
    if (out_tmp == NULL)
    {
        return BAD_ALLOC;
    }
    ll dot_position = -1;
    for (ll i = 0; in[i]; ++i)
    {
        if (in[i] == '.')
        {
            dot_position = i;
        }
        if (in[i] == '/' || in[i] == '\\')
        {
            dot_position = -1;
        }
    }
    if (dot_position != -1)
    {
        sprintf(out_tmp, "%s", in);
        sprintf(out_tmp + dot_position, "_error");
    }
    else
    {
        sprintf(out_tmp, "%s_error", in);
    }
    *out = out_tmp;
    return OK;
}

void fprintf_error(FILE* out, Status st)
{
    switch (st)
    {
        case OK:
            return;
        case INVALID_ARGUMENT:
            fprintf(out, "Invalid argument detected\n");
            return;
        case INVALID_INPUT:
            fprintf(out, "Invalid input\n");
            return;
        case INVALID_BRACKETS_ORDER:
            fprintf(out, "Invalid brackets order\n");
            return;
        case DIVISION_BY_ZERO:
            fprintf(out, "Division by zero error\n");
            return;
        case ZERO_POWERED_ZERO:
            fprintf(out, "Zero powered zero\n");
            return;
        case OPENING_ERROR:
            fprintf(out, "File cannot be opened\n");
            return;
        case END_OF_FILE:
            fprintf(out, "End of file reached unexpectedly\n");
            return;
        case OVERFLOWED:
            fprintf(out, "Overflow\n");
            return;
        case BAD_ALLOC:
            fprintf(out, "Allocation error\n");
            return;
        default:
            fprintf(out, "Something went wrong\n");
            return;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        errors(OPENING_ERROR);
        return INVALID_INPUT;
    }
    Status st = OK;
    Status file_st = OK;
    int running = 1;
    ull cnt = 1;
    while (!st && cnt < argc)
    {
        if (!file_st)
        {
            FILE* current = fopen(argv[cnt], "r");
            if (current == NULL)
            {
                file_st = OPENING_ERROR;
            }
            ull counter = 0;
            expression_data* data = NULL;
            file_st = file_st ? file_st : file_processing(current, &counter, &data);
            printf("<---%s--->\n", argv[cnt]);
            if (counter == 0)
            {
                printf("Empty file\n");
            }
            Status file_err_st = OK;
            char* err_path = NULL;
            FILE* err_file = NULL;
            for (ull i = 0; i < counter && !file_err_st; ++i)
            {
                if (data[i].is_valid)
                {
                    if (err_file == NULL && !file_err_st)
                    {
                        file_err_st = new_out_path(argv[cnt], &err_path);
                        if (!file_err_st)
                        {
                            err_file = fopen(err_path, "w");
                        }
                        if (!file_err_st && err_file == NULL)
                        {
                            file_err_st = OPENING_ERROR;
                        }
                    }
                    if (!file_err_st)
                    {
                        fprintf(err_file, "%llu: %s - ", i, data[i].infix);
                        fprintf_error(err_file, data[i].is_valid);
                    }
                }
                else
                {
                    printf("%llu: | Infix: %s | ", i + 1, data[i].infix);
                    printf("Postfix: ");
                    print_stack(data[i].postfix);
                    if (data[i].postfix.size == 0)
                    {
                        file_st = INVALID_ARGUMENT;
                    }
                    printf("| Result: %lld |\n", data[i].result);
                }
            }
            if (!file_err_st && err_file != NULL)
            {
                printf("Some errors occurred in current file. They were printed into the error file: %s\n", err_path);
            }
            if (file_err_st)
            {
                printf("Error caused by error file: ");
                errors(file_err_st);
            }
            for (ull i = 0; i < counter; ++i)
            {
                free(data[i].infix);
                destruct_stack(&data[i].postfix);
            }
            if (current != NULL)
            {
                fclose(current);
            }
            if (err_file != NULL)
            {
                fclose(err_file);
            }
            free(data);
            free(err_path);
        }
        if (file_st)
        {
            errors(file_st);
        }
        cnt++;
        printf("\n");
    }
    if (st == END_OF_FILE)
    {
        st = OK;
    }
    if (st)
    {
        errors(st);
    }
    return st;
}
