//
// Created by Никита Третьяков on 13.12.2023.
//

#include "my_lib.h"

Status create_cache_node(Cache_ptr* cache, char* key, ull value)
{
    *cache = (Cache_ptr)malloc(sizeof(Cache));
    if (*cache == NULL)
    {
        return BAD_ALLOC;
    }
    (*cache)->key = key;
    (*cache)->value = value;
    (*cache)->next = NULL;
    return OK;
}

Status add_cache_node(Cache_ptr* head, char** key, ull value)
{
    Cache_ptr new;
    Status st = create_cache_node(&new, *key, value);
    if (st)
    {
        return st;
    }
    if (*head == NULL)
    {
        *head = new;
        return st;
    }
    if (!strcmp((*head)->key, *key))
    {
        (*head)->value = value;
        free(new->key);
        new->key = (*head)->key;
        free(new);
        *key = (*head)->key;
        return OK;
    }
    Cache_ptr tmp = *head;

    while (tmp->next != NULL && strcmp(tmp->key, new->key) != 0)
    {
        tmp = tmp->next;
    }
    if (!strcmp(tmp->key, new->key))
    {
        tmp->value = new->value;
        free(new->key);
        new->key = tmp->key;
        free(new);
        *key = tmp->key;
    }
    else
    {
        tmp->next = new;
    }
    return OK;
}

void destruct_cache(Cache_ptr list)
{
    if (list == NULL)
    {
        return;
    }
    Cache_ptr curr = list;
    while (curr != NULL)
    {
        Cache_ptr tmp = curr->next;
        free(curr);
        curr = tmp;
    }
}
void print_cache(Cache_ptr list)
{
    if (list == NULL)
    {
        return;
    }
    Cache_ptr curr = list;
    while (curr != NULL)
    {
        printf("(%s\t%llu)", curr->key, curr->value);
        curr = curr->next;
    }
    printf("\n");
}