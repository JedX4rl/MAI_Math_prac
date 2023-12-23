#include <stdio.h>
#include "my_lib.h"
#include "stack.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    Status st = OK;
    FILE* in = fopen(argv[1], "r");
    if (in == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    srand(time(NULL));
    char* out_name = NULL;
    char* input_path = argv[1];
    char* out_path = NULL;
    while (out_path == NULL || !strcmp(input_path, out_path))
    {
        free(out_name);
        free(out_path);
        st = generate_random_string(&out_name);
        st = st ? st : file_out_name(input_path, out_name, &out_path);
    }
    free(out_name);
    if (st)
    {
        fclose(in);
        errors(st);
        return st;
    }
    FILE* out = fopen(out_path, "w");
    if (out == NULL)
    {
        fclose(in);
        free(out_path);
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    printf("New file: %s\n", out_path);
    free(out_path);

    char* expression = NULL;
    st = read_line(in, &expression);
    Exp_tree tree;
    null_tree(&tree);
    st = st ? st : create_exp_tree(&tree, expression);
    st = st ? st : print_expression_table(out, tree);
    destruct_exp_tree(&tree);
    free(expression);
    fclose(in);
    fclose(out);
    if (st)
    {
        errors(st);
    }
    return st;
}
