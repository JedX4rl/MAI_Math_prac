//
// Created by Никита Третьяков on 18.12.2023.
//

#include "deque.h"
Status null_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    stack->size = 0;
    stack->beginning = stack->end = NULL;
    return OK;
}

iterator end(Stack stack)
{
    return (iterator) {stack.end};
}

Status create_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Node_ptr node = (Node_ptr)malloc(sizeof(stack_node));
    if (node == NULL)
    {
        return BAD_ALLOC;
    }
    node->data = NULL;
    node->content = NONE;
    stack->beginning = node;
    stack->end = node;
    stack->size = 0;
    return OK;
}

void destruct_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return;
    }
    Node_ptr node = stack->beginning;
    while (node != NULL)
    {
        Node_ptr tmp = node;
        node = node->next;
        free(tmp->data);
        free(tmp);
    }
    free(node);
    stack->beginning = stack->end = NULL;
    stack->size = 0;

}


Status last_item(Stack stack, n_content* content, void** data)
{
    if (data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (content && stack.end)
    {
        *content = stack.end->content;
    }
    if (data != NULL && stack.end)
    {
        *data = stack.end->data;
    }
    if (!stack.end)
    {
        *data = NULL;
    }
    return OK;
}

Status push_back(Stack* stack, n_content content, void* data)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Node_ptr new_node = (Node_ptr)malloc(sizeof(stack_node));
    if (new_node == NULL)
    {
        return BAD_ALLOC;
    }
    new_node->next = NULL;
    new_node->content = content;
    new_node->data = data;
    if (stack->size == 0)
    {
        stack->beginning = new_node;
    }
    else
    {
        stack->end->next = new_node;
    }
    stack->end = new_node;
    stack->size++;
    return OK;
}


Status pop_back(Stack* stack, n_content* content, void** data)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (stack->size == 0)
    {
        return INVALID_ARGUMENT;
    }
    if (content)
    {
        *content = stack->end->content;
    }
    if (data)
    {
        *data = stack->end->data;
    }
    Node_ptr tmp = stack->beginning;
    if (tmp == stack->end)
    {
        stack->beginning = stack->end = NULL;
    }
    else
    {
        while (tmp->next->next != NULL)
        {
            tmp = tmp->next;
        }
        Node_ptr last = tmp->next;
        tmp->next = NULL;
        stack->end = tmp;
        free(last);

    }
    stack->size--;
    return OK;
}

void print_stack(Stack stack)
{
    if (stack.beginning == NULL)
    {
        return;
    }
    Node_ptr current = stack.beginning;
    while (current != NULL)
    {
        if (current->content == CONST)
        {
            printf("%lld ", *((ll*)current->data));
        }
        else if (current->content == OPERATION)
        {
            printf("%c ", *((char*)current->data));
        }
        else
        {
            return;
        }
        current = current->next;
    }
}

int is_operation(char ch)
{
    switch (ch)
    {
        case '^':
        case '*':
        case '/':
        case '%':
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

int get_operation(char ch)
{
    switch (ch)
    {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

Status to_decimal(const char* string, ll* result)
{
    if (string == NULL || result == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (((*string == '+' || *string == '-') && string[1] == '\0') || *string == '\0')
    {
        return INVALID_ARGUMENT;
    }
    errno = 0;
    char* ptr;
    ll res = strtoll(string ,&ptr, 10);
    if (*ptr != '\0')
    {
        return INVALID_ARGUMENT;
    }
    if (errno == ERANGE)
    {
        return OVERFLOWED;
    }
    *result = res;
    return OK;
}

Status get_expression_operation(const char* string, const char** end, n_content* content, void** data)
{
    if (string == NULL || end == NULL || content == NULL || data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (*string == '(')
    {
        *content = L_BRACKET;
        *data = NULL;
    }
    else if (*string == ')')
    {
        *content = R_BRACKET;
        *data = NULL;
    }
    else if (is_operation(*string))
    {
        char* tmp = (char*)malloc(sizeof(char));
        if (tmp == NULL)
        {
            return BAD_ALLOC;
        }
        tmp[0] = *string;
        *content = OPERATION;
        *data = tmp;
    }
    else
    {
        return INVALID_INPUT;
    }
    string++;
    *end = string;
    return OK;
}

Status get_expression_const(const char* string, const char** end, void** data)
{
    if (string == NULL || end == NULL || data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Status st = OK;
    char* tmp = NULL;
    ll* temp_data = (ll*)malloc(sizeof(ll));
    if (temp_data == NULL)
    {
        st = BAD_ALLOC;
    }
    int sign = 1;
    st = temp_data ? OK : BAD_ALLOC;
    if (*string == '+' || *string == '-')
    {
        sign = *string == '+' ? 1 : -1;
    }
    st = st ? st : read_until(string, "()+-*/^%", end, &tmp);
    st = st ? st : to_decimal(tmp, temp_data);
    free(tmp);
    if (st)
    {
        free(temp_data);
        return st;
    }
    *temp_data *= sign;
    *data = temp_data;
    return OK;
}

Status token_is_valid(n_content prev, n_content curr)
{
    if (prev == OPERATION && curr == OPERATION)
    {
        return INVALID_INPUT;
    }
    if (prev == CONST && curr == CONST)
    {
        return INVALID_INPUT;
    }
    if ((prev == L_BRACKET && curr == R_BRACKET) || (prev == R_BRACKET && curr == L_BRACKET))
    {
        return INVALID_INPUT;
    }
    if ((prev == OPERATION && curr == R_BRACKET) || (prev == L_BRACKET && curr == OPERATION))
    {
        return INVALID_INPUT;
    }
    if ((prev == R_BRACKET && curr == CONST) || (prev == CONST && curr == L_BRACKET))
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status convert_to_postfix(const char* infix, Stack * postfix)
{
    if (infix == NULL || postfix == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Stack expression, operations;
    null_stack(&expression);
    null_stack(&operations);
    Status st = create_stack(&expression);
    st = st ? st : create_stack(&operations);
    n_content prev = NONE;
    const char* ptr = infix;
    while (!st && *ptr)
    {
        n_content curr = NONE;
        void* data;
        if (prev != CONST && prev != R_BRACKET && *ptr != '(')
        {
            st = get_expression_const(ptr, &ptr, &data);
            curr = CONST;
        }
        else
        {
            st = get_expression_operation(ptr, &ptr, &curr, &data);
        }
        st = st ? st : token_is_valid(prev, curr);
        if (!st && curr == CONST)
        {
            st = push_back(&expression, CONST, data);
        }
        else if (!st && curr == OPERATION)
        {
            void* another = NULL;
            last_item(operations, NULL, &another);
            int curr_priority = get_operation(*((char*)data));
            int another_priority = another ? get_operation(*((char*)another)) : -1;
            while (!st && another_priority >= curr_priority)
            {
                st = push_back(&expression, OPERATION, another);
                st = st ? st : pop_back(&operations, NULL, NULL);
                last_item(operations, NULL, &another);
                another_priority = another != NULL ? get_operation(*((char*)another)) : -1;
            }
            st = st ? st : push_back(&operations, OPERATION, data);
        }
        else if (!st && curr == L_BRACKET)
        {
            st = push_back(&operations, L_BRACKET, NULL);
        }
        else if (!st && curr == R_BRACKET)
        {
            n_content another = NONE;
            void* another_data = NULL;
            last_item(operations, &another, &another_data);
            st = another != NONE ? OK : INVALID_BRACKETS_ORDER;
            while (!st && another != L_BRACKET)
            {
                st = push_back(&expression, OPERATION, another_data);
                st = st ? st : pop_back(&operations, NULL, NULL);
                last_item(operations, &another, &another_data);
                st = st ? st :(another != NONE ? OK : INVALID_BRACKETS_ORDER);
            }
            another_data = NULL;
            st = st ? st : pop_back(&operations, NULL, &another_data);
            free(another_data);
        }
        prev = curr;
    }
    while (!st && operations.size > 0)
    {
        n_content curr = NONE;
        void* data = NULL;
        st = pop_back(&operations, &curr, &data);
        st = st ? st : (curr != L_BRACKET ? OK : INVALID_BRACKETS_ORDER);
        st = st ? st : push_back(&expression, OPERATION, data);
    }
    destruct_stack(&operations);
    if (st)
    {
        destruct_stack(&expression);
        return st;
    }
    *postfix = expression;
    return OK;
}

Status calculate_operation(char operation, ll a, ll b, ll* res)
{
    if (res == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (b < 0 && (operation == '+' || operation == '-'))
    {
        operation = operation == '+' ? '-' : '+';
        b *= -1;
    }
    switch (operation)
    {
        case '+':
        {
            if ((a > LLONG_MAX - b) || (a + b < LLONG_MIN))
            {
                return OVERFLOWED;
            }
            *res = a + b;
            return OK;
        }
        case '-':
        {
            if ((a - b > LLONG_MAX) || (a < LLONG_MIN + b))
            {
                return OVERFLOWED;
            }
            *res = a - b;
            return OK;
        }
        case '*':
        {
            ll checker = b < 0 ? -b : b;
            if ((a > LLONG_MAX / b) || (a < LLONG_MIN / b))
            {
                return OVERFLOWED;
            }
            *res = a * b;
            return OK;
        }
        case '/':
        {
            if (b == 0)
            {
                return DIVISION_BY_ZERO;
            }
            *res = a / b;
            return OK;
        }
        case '%':
        {
            if (b == 0)
            {
                return DIVISION_BY_ZERO;
            }
            *res = a % b;
            return OK;
        }
        case '^':
        {
            return binary_power(a, b, res);
        }
        default:
            return INVALID_INPUT;
    }
}

Status calculate_postfix(Stack postfix, ll* result)
{
    if (result == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Stack args;
    null_stack(&args);
    Status st = create_stack(&args);

    Node_ptr curr = postfix.beginning;

    while (curr != NULL && !st)
    {
        if (curr->content == CONST)
        {
            ll* data = (ll*)malloc(sizeof(ll));
            st = data ? OK : BAD_ALLOC;
            if (!st)
            {
                *data = *((ll*)curr->data);
                st = push_back(&args, CONST, data);
            }
        }
        else if (curr->content == OPERATION)
        {
            char* operation = (char*)curr->data;
            void* a = NULL;
            void* b = NULL;
            ll* tmp = (ll*)malloc(sizeof(ll));
            st = tmp ? OK : BAD_ALLOC;
            st = st ? st : pop_back(&args, NULL, &b);
            st = st ? st : pop_back(&args, NULL, &a);
            st = st ? st : calculate_operation(*operation, *(ll*)a, *(ll*)b, tmp);
            st = st ? st : push_back(&args, CONST, tmp);
            if (st)
            {
                free(tmp);
            }
            free(a);
            free(b);
        }
        else
        {
            st = INVALID_INPUT;
        }
        curr = curr->next;
    }
    void *tmp = NULL;
    if (!st)
    {
        st = args.size == 1 ? OK : INVALID_INPUT;
        st = st ? st : pop_back(&args, NULL, &tmp);
    }
    destruct_stack(&args);
    if (st)
    {
        return st;
    }
    *result = (*(ll*)tmp);
    free(tmp);
    return OK;
}

