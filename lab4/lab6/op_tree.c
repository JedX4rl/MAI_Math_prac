//
// Created by Никита Третьяков on 14.12.2023.
//

#include "op_tree.h"
#include "stack.h"

Status create_node(Node_ptr* node, information info, char data)
{
    if (node == NULL)
    {
        return INVALID_ARGUMENT;
    }
    *node = (Node_ptr)malloc(sizeof(tree_node));
    if (*node == NULL)
    {
        return BAD_ALLOC;
    }
    (*node)->info = info;
    (*node)->data = data;
    (*node)->right = (*node)->left = NULL;
    return OK;
}

void destruct_node(Node_ptr node)
{
    if (node->left)
    {
        destruct_node(node->left);
        free(node->left);
    }
    if (node->right)
    {
        destruct_node(node->right);
        free(node->right);
    }
}

void null_tree(Exp_tree* tree)
{
    tree->root = NULL;
}

int get_priority(information info)
{
    switch (info)
    {
        case INVERSION:
            return 3;
        case WEBB:
        case SHEFFER:
        case COIMPLICATION:
        case CONJUNCTION:
            return 2;
        case DISJUNCTION:
        case IMPLICATION:
        case SUM_BY_2:
        case EQUIVALENCE:
            return 1;
        default:
            return 0;
    }
}

int is_operation(information info)
{
    switch (info)
    {
        case INVERSION:
        case WEBB:
        case SHEFFER:
        case COIMPLICATION:
        case CONJUNCTION:
        case DISJUNCTION:
        case IMPLICATION:
        case SUM_BY_2:
        case EQUIVALENCE:
            return 1;
        default:
            return 0;
    }
}

Status get_exp_token(const char* str, const char** end, information* info, char* data)
{
    if (str == NULL || end == NULL || info == NULL || data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Status st = OK;
    if (isalpha(*str))
    {
        *info = VAR;
        *data = toupper(*str);
    }
    else if (*str == '0' || *str == '1')
    {
        *info = CONST;
        *data = *str;
    }
    else if (*str == '&')
    {
        *info = CONJUNCTION;
    }
    else if (*str == '|')
    {
        *info = DISJUNCTION;
    }
    else if (*str == '~')
    {
        *info = INVERSION;
    }
    else if (*str == '=')
    {
        *info = EQUIVALENCE;
    }
    else if (*str == '!')
    {
        *info = SHEFFER;
    }
    else if (*str == '?')
    {
        *info = WEBB;
    }
    else if (*str == '(')
    {
        *info = L_BRACKET;
    }
    else if (*str == ')')
    {
        *info = R_BRACKET;
    }
    else if (*str == '-')
    {
        str++;
        if (*str == '>')
        {
            *info = IMPLICATION;
        }
        else
        {
            st = INVALID_INPUT;
        }
    }
    else if (*str == '+')
    {
        str++;
        if (*str == '>')
        {
            *info = COIMPLICATION;
        }
        else
        {
            st = INVALID_INPUT;
        }
    }
    else if (*str == '<')
    {
        str++;
        if (*str == '>')
        {
            *info = SUM_BY_2;
        }
        else
        {
            st = INVALID_INPUT;
        }
    }
    else
    {
        st = INVALID_INPUT;
    }
    if (st)
    {
        *end = str;
        return st;
    }
    str++;
    *end = str;
    return OK;
}

Status tokens_valid(information previous, information current)
{
    if (is_operation(previous) && is_operation(current) && current != INVERSION)
    {
        return INVALID_INPUT;
    }
    if ((previous == CONST || previous == VAR) && (current == CONST || current == VAR))
    {
        return INVALID_INPUT;
    }
    if ((previous == L_BRACKET && current == R_BRACKET) || (previous == R_BRACKET && current == L_BRACKET))
    {
        return INVALID_INPUT;
    }
    if ((is_operation(previous) && current == R_BRACKET) || (previous == L_BRACKET && (is_operation(current) && current != INVERSION)))
    {
        return INVALID_INPUT;
    }
    if (((previous == R_BRACKET) & (current == CONST || current == VAR)) || ((previous == CONST || previous == VAR) && current == L_BRACKET))
    {
        return INVALID_INPUT;
    }
    return OK;
}

void destruct_exp_tree(Exp_tree* tree)
{
    if (tree->root == NULL)
    {
        return;
    }
    destruct_node(tree->root);
    free(tree->root);
}

Status create_exp_tree(Exp_tree* tree, const char* str)
{
    if (tree == NULL || str == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Status st = OK;
    Exp_tree tmp;
    tmp.var_cnt = 0;
    for (int i = 0; i < 26; ++i)
    {
        tmp.flag[i] = false;
    }
    Stack arguments;
    Stack operations;
    null_tree(&tmp);
    null_stack(&arguments);
    null_stack(&operations);
    st = create_stack(&operations);
    st = st ? st : create_stack(&operations);
    information previous = NONE;
    const char* ptr = str;
    while (*ptr && !st)
    {
        information current = NONE;
        char data;
        st = get_exp_token(ptr, &ptr, &current, &data);
        st = st ? st : tokens_valid(previous, current);

        if (!st && (current == VAR || current == CONST))
        {
            if (current == VAR)
            {
                if (!tmp.flag[data - 'A'])
                {
                    tmp.var_cnt++;
                }
                tmp.flag[data - 'A'] = true;
            }
            Node_ptr node = NULL;
            st = create_node(&node, current, data);
            st = st ? st : push_stack(&arguments, node);
        }

        else if (!st && is_operation(current))
        {
            Node_ptr node = stack_top(operations);
            information another = node != NULL ? node->info : NONE;
            int curr_priority = get_priority(current);
            int another_priority = get_priority(another);
            while (!st && another_priority >= curr_priority && current != INVERSION)
            {
                int counter = another == INVERSION ? 1 : 2;
                st = arguments.size >= counter ? OK : INVALID_INPUT;
                if (counter == 2)
                {
                    st = st ? st : pop_stack(&arguments, &node->right);
                }
                st = st ? st : pop_stack(&arguments, &node->left);
                st = st ? st : push_stack(&arguments, node);
                st = st ? st : pop_stack(&operations, NULL);
                node = stack_top(operations);
                another = node != NULL ? node->info : NONE;
                another_priority = get_priority(another);
            }
            st = st ? st : create_node(&node, current, data);
            st = st ? st : push_stack(&operations, node);
        }

        else if (!st && current == L_BRACKET)
        {
            Node_ptr node = NULL;
            st = create_node(&node, current, data);
            st = st ? st : push_stack(&operations, node);
        }

        else if (!st && current == R_BRACKET)
        {
            Node_ptr node = NULL;
            node = stack_top(operations);
            st = node != NULL ? OK : INVALID_INPUT;
            while (!st && node->info != L_BRACKET)
            {
                int counter = node->info == INVERSION ? 1 : 2;
                st = arguments.size >= counter ? OK : INVALID_INPUT;
                if (counter == 2)
                {
                    st = st ? st: pop_stack(&arguments, &node->right);
                }
                st = st ? st : pop_stack(&arguments, &node->left);
                st = st ? st : push_stack(&arguments, node);
                st = st ? st : pop_stack(&operations, NULL);
                node = stack_top(operations);
                st = st ? st : (node != NULL ? OK : INVALID_INPUT);
            }
            node = NULL;
            st = st ? st : pop_stack(&operations, &node);
            free(node);
        }
        previous = current;
    }
    while (!st && operations.size > 0)
    {
        int counter = 0;
        Node_ptr node = NULL;
        st = pop_stack(&operations, &node);
        if (!st)
        {
            counter = node->info == INVERSION ? 1 : 2;
        }
        st = st ? st : (node->info != L_BRACKET ? OK : INVALID_INPUT);
        st = st ? st : (arguments.size >= counter ? OK : INVALID_INPUT);

        if (counter == 2)
        {
            st = st ? st : pop_stack(&arguments, &node->right);
        }
        st = st ? st : pop_stack(&arguments, &node->left);
        st = st ? st : push_stack(&arguments, node);

        if (st)
        {
            destruct_node(node);
            free(node);
        }
    }
    if (!st)
    {
        st = arguments.size == 1 ? OK : INVALID_INPUT;
        Node_ptr node = NULL;
        st = st ? st : pop_stack(&arguments, &node);
        tmp.root = node;
    }
    destruct_stack(&arguments);
    destruct_stack(&operations);
    if (st)
    {
        destruct_exp_tree(&tmp);
        return st;
    }
    *tree = tmp;
    memcpy(tree->flag, tmp.flag, sizeof(bool) * 26);
    return OK;
}

Status calculate_exp_tree_node(const tree_node* node, const bool table[26], bool* result)
{
    if (result == NULL)
    {
        return INVALID_ARGUMENT;
    }
    switch (node->info)
    {
        case NONE:
        case L_BRACKET:
        case R_BRACKET:
            return INVALID_INPUT;
        case VAR:
            if (node->data - 'A' < 0 || node->data - 'A' > 25)
            {
                return INVALID_INPUT;
            }
            *result = table[node->data - 'A'];
            return OK;
        case CONST:
            if (node->data != '0' && node->data != '1')
            {
                return INVALID_INPUT;
            }
            *result = node->data == '1' ? true : false;
            return OK;
        default:
            break;
    }

    if (node->info == INVERSION)
    {
        if (node->left == NULL)
        {
            return INVALID_INPUT;
        }
        Status st = calculate_exp_tree_node(node->left, table, result);
        *result = st ? *result : !(*result);
        return st;
    }
    if (node->left == NULL || node->right == NULL)
    {
        return INVALID_INPUT;
    }
    bool left_res;
    bool right_res;
    Status st = calculate_exp_tree_node(node->left, table, &left_res);
    st = st ? st : calculate_exp_tree_node(node->right, table, &right_res);
    if (st)
    {
        return st;
    }
    switch (node->info)
    {
        case CONJUNCTION:
            *result = left_res && right_res;
            return OK;
        case DISJUNCTION:
            *result = left_res || right_res;
            return OK;
        case IMPLICATION:
            *result = !left_res || right_res;
            return OK;
        case COIMPLICATION:
            *result = left_res && !right_res;
            return OK;
        case SUM_BY_2:
            *result = left_res ^ right_res;
            return OK;
        case EQUIVALENCE:
            *result = left_res == right_res;
            return OK;
        case SHEFFER:
            *result = !(left_res && right_res);
            return OK;
        case WEBB:
            *result = !(left_res || right_res);
            return OK;
        default:
            return INVALID_INPUT;
    }
}

Status calculate_exp_tree(Exp_tree tree, const bool* values, bool* result)
{
    if (tree.root == NULL || values == NULL || result == NULL)
    {
        return INVALID_ARGUMENT;
    }
    bool table[26];
    for (ull i = 0, j = 0; i < 26; ++i)
    {
        if (tree.flag[i])
        {
            table[i] = values[j++];
        }
    }
    bool tmp_res;
    Status st = calculate_exp_tree_node(tree.root, table, &tmp_res);
    if (st)
    {
        return st;
    }
    *result = tmp_res;
    return OK;
}

Status print_expression_table(FILE* out, Exp_tree tree)
{
    if (out == NULL || tree.root == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Status st = OK;
    bool* values = (bool*)malloc(sizeof(bool) * tree.var_cnt);
    if (values == NULL)
    {
        return BAD_ALLOC;
    }
    if (!tree.var_cnt)
    {
        bool result;
        st = calculate_exp_tree(tree, values, &result);
        free(values);
        if (st)
        {
            return st;
        }
        fprintf(out, "Value is: %d\n", result);
        return OK;
    }
    for (ull i = 0; i < 26; ++i)
    {
        if (tree.flag[i])
        {
            fprintf(out, "| %c ", 'A' + (int) i);
        }
    }
    fprintf(out, "| value |\n");
    ull end = 2 << (tree.var_cnt - 1);
    for (ull vars = 0; vars < end; ++vars)
    {
        ull tmp_vars = vars;
        for (ull i = tree.var_cnt; i > 0; i--)
        {
            values[i - 1] = tmp_vars & 1;
            tmp_vars >>= 1;
        }
        bool res;
        st = calculate_exp_tree(tree, values, &res);
        if (st)
        {
            free(values);
            return st;
        }
        for (ull i = 0; i < tree.var_cnt; ++i)
        {
            fprintf(out, "| %d ", values[i]);
        }
        fprintf(out, "|  %d  |\n", res);
    }
    free(values);
    return OK;
}

Status read_line(FILE* file, char** line)
{
    if (file == NULL || line == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull cnt = 0;
    ull size = 2;
    *line = (char*)malloc(sizeof(char) * size);
    if (*line == NULL)
    {
        return BAD_ALLOC;
    }
    char ch = getc(file);
    while (!feof(file) && ch != '\n')
    {
        if (cnt + 2 > size)
        {
            Status st = my_realloc(line, sizeof(char) * (size * 2));
            if (st)
            {
                free(*line);
                return st;
            }
            size *= 2;
        }
        (*line)[cnt++] = ch;
        ch = getc(file);
    }
    (*line)[cnt] = '\0';
    return OK;
}

Status generate_random_string(char** string)
{
    if (string == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull size = 2;
    ull cnt = 0;
    *string = (char*)malloc(sizeof(char) * size);
    if (*string == NULL)
    {
        return BAD_ALLOC;
    }
    char symbols[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char ch = symbols[rand() % 63];
    while ((ch != '\0' || cnt == 0) && cnt < 63)
    {
        while (cnt == 0 && ch == '\0')
        {
            ch = symbols[rand() % 63];
        }
        if (cnt + 2 > size)
        {
            Status st = my_realloc(string, sizeof(char) * (size * 2));
            if (st)
            {
                free(string);
                return BAD_ALLOC;
            }
            size *= 2;
        }
        (*string)[cnt++] = ch;
        ch = symbols[rand() % 63];
    }
    (*string)[cnt] = '\0';
    return OK;
}

Status file_out_name(const char* input_name, const char* output_name, char** out)
{
    if (input_name == NULL || output_name == NULL || out == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull tmp = 0;
    for (ull i = 0; input_name[i]; ++i)
    {
        if (input_name[i] == '/' || input_name[i] == '\\')
        {
            tmp = i;
        }
    }
    *out = (char*)malloc(sizeof(char) * (tmp + strlen(output_name) + 2));
    if (*out == NULL)
    {
        return BAD_ALLOC;
    }
    memcpy(*out, input_name, sizeof(char) * (tmp + 1));
    strcpy(*out + tmp + 1, output_name);
    return OK;
}