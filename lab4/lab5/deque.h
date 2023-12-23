//
// Created by Никита Третьяков on 18.12.2023.
//

#ifndef LAB5_DEQUE_H
#define LAB5_DEQUE_H
#include "my_lib.h"
typedef enum n_content
{
    NONE,
    CONST,
    OPERATION,
    L_BRACKET,
    R_BRACKET
} n_content;

typedef struct stack_node
{
    n_content content;
    void* data;
    struct stack_node* next;
} stack_node, *Node_ptr;

typedef struct iterator
{
    Node_ptr node;
} iterator;

typedef struct Stack
{
    Node_ptr beginning;
    Node_ptr end;
    ull size;
} Stack;


typedef struct expression_data
{
    Status is_valid;
    char* infix;
    Stack postfix;
    ll result;
} expression_data;


Status null_stack(Stack* stack);
iterator end(Stack stack);
Status create_stack(Stack* stack);
void destruct_stack(Stack* stack);
Status last_item(Stack stack, n_content* content, void** data);
Status push_back(Stack* stack, n_content content, void* data);
Status pop_back(Stack* stack, n_content* content, void** data);
void print_stack(Stack stack);
Status convert_to_postfix(const char* infix, Stack* postfix);
Status calculate_postfix(Stack postfix, ll* result);

#endif //LAB5_DEQUE_H
