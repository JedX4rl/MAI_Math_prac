//
// Created by Никита Третьяков on 21.02.2024.
//

#ifndef LAB9_BIN_HEAP_H
#define LAB9_BIN_HEAP_H

#include <stdio.h>
#include <stdlib.h>

typedef enum Status
{
    OK,
    BAD_ALLOC,
    NULL_ARG,
    NOT_OK
} Status;

typedef struct request
{
    unsigned id;
    unsigned prior;
    char time[21];
    char* txt;
} request;

typedef struct bin_heap
{
    unsigned int size;
    unsigned int capacity;
    request** elements;
    int (*compare)(const request *, const request *);
} bin_heap;

int compare_pq_key(const request* a, const request* b);

void bh_destruct_node(request* node);
void bh_destruct_heap(bin_heap* heap);

void bh_set_null(bin_heap* heap);
Status bh_construct_heap(bin_heap* heap, int (*compare)(const request *, const request *));
Status bh_top(const bin_heap* heap, request** result);
Status bh_size(const bin_heap* heap, unsigned int* res);

Status bh_sift_down(bin_heap* heap);
Status bh_sift_up(bin_heap* heap);
Status bh_insert(bin_heap* heap, request* new_el);

Status bh_request_copy(const request* src, request** res);
Status bh_heap_copy(bin_heap* dest, const bin_heap* src);

Status bh_pop(bin_heap* heap, request** min);
Status bh_destructive_merge(bin_heap* a, bin_heap* b, bin_heap* result);
Status bh_merge(const bin_heap* a, const bin_heap* b, bin_heap* result);

#endif //LAB9_BIN_HEAP_H
