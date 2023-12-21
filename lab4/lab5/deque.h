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

typedef struct deque_node
{
    n_content content;
    void* data;
    struct deque_node* prev;
    struct deque_node* next;
} deque_node, *Node_ptr;

typedef struct iterator
{
    Node_ptr node;
} iterator;

typedef struct Deque
{
    Node_ptr beginning;
    Node_ptr end;
    ull size;
} Deque;

typedef struct expression_data
{
    Status is_valid;
    char* infix;
    Deque postfix;
    ll result;
} expression_data;


Status null_deque(Deque* deque);
void next_iterator(iterator* curr);
void prev_iterator(iterator* curr);
iterator beginning(Deque deque);
iterator end(Deque deque);
int equal(iterator a, iterator b);
Status create_deque(Deque* deque);
void destruct_deque(Deque* deque);
Status empty(Deque* deque);
Status first_item(Deque deque, n_content* content, void** data);
Status last_item(Deque deque, n_content* content, void** data);
Status push_back(Deque* deque, n_content content, void* data);
Status pop_back(Deque* deque, n_content* content, void** data);
void print_deque(Deque deque);
Status convert_to_postfix(const char* infix, Deque* postfix);
Status calculate_postfix(Deque postfix, ll* result);

#endif //LAB5_DEQUE_H
