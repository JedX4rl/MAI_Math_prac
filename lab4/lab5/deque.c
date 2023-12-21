//
// Created by Никита Третьяков on 18.12.2023.
//

#include "deque.h"
Status null_deque(Deque* deque)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    deque->size = 0;
    deque->beginning = deque->end = NULL;
    return OK;
}

void next_iterator(iterator* curr)
{
    curr->node = curr->node->next;
}

void prev_iterator(iterator* curr)
{
    curr->node = curr->node->prev;
}

iterator beginning(Deque deque)
{
    return (iterator) {deque.beginning};
}

iterator end(Deque deque)
{
    return (iterator) {deque.end};
}

int equal(iterator a, iterator b)
{
    return a.node == b.node;
}

Status create_deque(Deque* deque)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Node_ptr terminator = (Node_ptr)malloc(sizeof(deque_node));
    if (terminator == NULL)
    {
        return BAD_ALLOC;
    }
    terminator->content = NONE;
    terminator->prev = terminator->next = NULL;
    terminator->data = NULL;
    deque->beginning = deque->end = terminator;
    deque->size = 0;
    return OK;
}

void destruct_deque(Deque* deque)
{
    if (deque == NULL)
    {
        return;
    }
    Node_ptr node = deque->beginning;
    while (node != deque->end)
    {
        Node_ptr tmp = node;
        node = node->next;
        free(tmp->data);
        free(tmp);
    }
    free(node);
    deque->beginning = deque->end = NULL;
    deque->size = 0;

}

Status empty(Deque* deque)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    return deque->size ? 0 : 1;
}

Status first_item(Deque deque, n_content* content, void** data)
{
    if (data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (content)
    {
        *content = deque.beginning->content;
    }
    if (data)
    {
        *data =deque.beginning->data;
    }
    return OK;
}

Status last_item(Deque deque, n_content* content, void** data)
{
    if (data == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (content)
    {
        *content = deque.end->prev->content;
    }
    if (data != NULL && deque.end->prev)
    {
        *data = deque.end->prev->data;
    }
    return OK;
}

Status push_front(Deque* deque, n_content content, void* data)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Node_ptr new_node = (Node_ptr)malloc(sizeof(deque_node));
    if (new_node == NULL)
    {
        return BAD_ALLOC;
    }
    new_node->content = content;
    new_node->data = data;
    new_node->next = deque->beginning;
    new_node->prev = deque->end;
    deque->beginning = new_node;
    deque->size++;
    return OK;
}

Status push_back(Deque* deque, n_content content, void* data)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Node_ptr new_node = (Node_ptr)malloc(sizeof(deque_node));
    if (new_node == NULL)
    {
        return BAD_ALLOC;
    }
    new_node->content = content;
    new_node->data = data;
    new_node->next = deque->end;
    new_node->prev = deque->end->prev;
    if (deque->size == 0)
    {
        deque->beginning = new_node;
    }
    else
    {
        deque->end->prev->next = new_node;
    }
    deque->end->prev = new_node;
    deque->size++;
    return OK;
}

Status pop_front(Deque* deque, n_content* content, void** data)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (deque->beginning == deque->end)
    {
        return INVALID_ARGUMENT;
    }
    if (content)
    {
        *content = deque->beginning->content;
    }
    if (data)
    {
        *data = deque->beginning->data;
    }
    Node_ptr tmp = deque->beginning;
    deque->beginning = deque->beginning->next;
    deque->size--;
    free(tmp);
    return OK;
}

Status pop_back(Deque* deque, n_content* content, void** data)
{
    if (deque == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (deque->beginning == deque->end)
    {
        return INVALID_ARGUMENT;
    }
    if (content)
    {
        *content = deque->end->prev->content;
    }
    if (data)
    {
        *data = deque->end->prev->data;
    }
    Node_ptr tmp = deque->end->prev;
    if (tmp == deque->beginning)
    {
        deque->beginning = deque->end;
        deque->end->prev = deque->end->prev->next;
    }
    else
    {
        deque->end->prev->prev->next = deque->end;
        deque->end->prev = deque->end->prev->prev;
    }
    deque->size--;
    free(tmp);
    return OK;
}

void print_deque(Deque deque)
{
    if (deque.beginning == NULL)
    {
        return;
    }
    iterator current = beginning(deque);
    iterator last = end(deque);
    while (!equal(current, last))
    {
        if (current.node->content == CONST)
        {
            printf("%lld ", *((ll*)current.node->data));
        }
        else if (current.node->content == OPERATION)
        {
            printf("%c ", *((char*)current.node->data));
        }
        else
        {
            return;
        }
        next_iterator(&current);
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

Status convert_to_postfix(const char* infix, Deque* postfix)
{
    if (infix == NULL || postfix == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Deque expression, operations;
    null_deque(&expression);
    null_deque(&operations);
    Status st = create_deque(&expression);
    st = st ? st : create_deque(&operations);
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
            st = another != NONE ? OK : INVALID_ARGUMENT;
            while (!st && another != L_BRACKET)
            {
                st = push_back(&expression, OPERATION, another_data);
                st = st ? st : pop_back(&operations, NULL, NULL);
                last_item(operations, &another, &another_data);
                st = st ? st :(another != NONE ? OK : INVALID_ARGUMENT);
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
        st = st ? st : (curr != L_BRACKET ? OK : INVALID_INPUT);
        st = st ? st : push_back(&expression, OPERATION, data);
    }
    destruct_deque(&operations);
    if (st)
    {
        destruct_deque(&expression);
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
            if ((a > LLONG_MAX + b) || (a < LLONG_MIN + b))
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
                return INVALID_INPUT;
            }
            *res = a / b;
            return OK;
        }
        case '%':
        {
            if (b == 0)
            {
                return INVALID_INPUT;
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

Status calculate_postfix(Deque postfix, ll* result)
{
    if (result == NULL)
    {
        return INVALID_ARGUMENT;
    }
    Deque args;
    null_deque(&args);
    Status st = create_deque(&args);

    iterator curr = beginning(postfix);
    iterator last = end(postfix);

    while (!equal(curr, last) && !st)
    {
        if (curr.node->content == CONST)
        {
            ll* data = (ll*)malloc(sizeof(ll));
            st = data ? OK : BAD_ALLOC;
            if (!st)
            {
                *data = *((ll*)curr.node->data);
                st = push_back(&args, CONST, data);
            }
        }
        else if (curr.node->content == OPERATION)
        {
            char* operation = (char*)curr.node->data;
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
        next_iterator(&curr);
    }
    void *tmp = NULL;
    if (!st)
    {
        st = args.size == 1 ? OK : INVALID_INPUT;
        st = st ? st : pop_back(&args, NULL, &tmp);
    }
    destruct_deque(&args);
    if (st)
    {
        return st;
    }
    *result = (*(ll*)tmp);
    free(tmp);
    return OK;
}

