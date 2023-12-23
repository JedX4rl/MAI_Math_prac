//
// Created by Никита Третьяков on 14.12.2023.
//

#ifndef LAB6_STACK_H
#define LAB6_STACK_H
#include "my_lib.h"
#include "op_tree.h"

typedef struct stack_node
{
    Node_ptr node;
    struct stack_node* next;
} stack_node;

typedef struct
{
    stack_node* top;
    ull size;
} Stack;

Status create_stack(Stack* stack);
void destruct_stack(Stack* stack);
Status stack_is_empty(Stack* stack);
Node_ptr stack_top(Stack stack);
Status push_stack(Stack* stack, Node_ptr node);
Status pop_stack(Stack* stack, Node_ptr* node);
void null_stack(Stack* stack);
#endif //LAB6_STACK_H
