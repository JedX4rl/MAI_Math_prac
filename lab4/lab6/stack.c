//
// Created by Никита Третьяков on 14.12.2023.
//

#include "stack.h"

Status create_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return BAD_ALLOC;
    }
    stack->size = 0;
    stack->top = NULL;
    return OK;
}

void destruct_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return;
    }
    stack_node* curr = stack->top;
    while (curr != NULL)
    {
        stack_node* tmp = curr;
        curr = curr->next;
        destruct_node(tmp->node);
        free(tmp->node);
        free(tmp);
    }
    free(curr);
    stack->top = NULL;
    stack->size = 0;
}

void null_stack(Stack* stack)
{
    if (stack == NULL)
    {
        return;
    }
    stack->top = NULL;
    stack->size = 0;
}

Status stack_is_empty(Stack* stack)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (stack->size == 0)
    {
        return OK;
    }
    return NOT_OK;
}

Node_ptr stack_top(Stack stack)
{
    if (stack.top == NULL)
    {
        return NULL;
    }
    return stack.top->node;
}

Status push_stack(Stack* stack, Node_ptr node)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    stack_node* tmp = (stack_node*)malloc(sizeof(Stack));
    if (tmp == NULL)
    {
        return BAD_ALLOC;
    }
    tmp->node = node;
    tmp->next = stack->top;
    stack->top = tmp;
    stack->size++;
    return OK;
}

Status pop_stack(Stack* stack, Node_ptr* node)
{
    if (stack == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (!stack->size)
    {
        return INVALID_INPUT;
    }
    if (node != NULL)
    {
        *node = stack->top->node;
    }
    stack_node* tmp = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    free(tmp);
    return OK;
}

