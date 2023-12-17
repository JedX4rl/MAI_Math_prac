//
// Created by Никита Третьяков on 13.12.2023.
//

#include "my_lib.h"

Status create_node(Node_ptr* new, char* key, char* value)
{
    if (*new == NULL || key == NULL || value == NULL)
    {
        return INVALID_INPUT;
    }
    *new = (Node_ptr)malloc(sizeof(Node));
    if (*new == NULL)
    {
        return BAD_ALLOC;
    }
    (*new)->value = value;
    (*new)->key = key;
    (*new)->next = NULL;
    return OK;
}

Status add_node(Node_ptr* head, char* key, char* value)
{
    if (key == NULL || value == NULL)
    {
        return INVALID_INPUT;
    }
    Node_ptr new;
    Status st = create_node(&new, key, value);
    if (st)
    {
        return st;
    }
    if (*head == NULL)
    {
        *head = new;
        return OK;
    }
    Node_ptr tmp = *head;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = new;
    return OK;
}

Status add_similar_node(Node_ptr* head, char** key, char* value, ull* size)
{
    Node_ptr new;
    Status st = create_node(&new, *key, value);
    if (st)
    {
        return st;
    }
    if (*head == NULL)
    {
        *head = new;
        *size = 1;
        return st;
    }
    if (!strcmp((*head)->key, *key))
    {
        free((*head)->value);
        (*head)->value = value;
        (*head)->key = *key;
        free(new);
        return OK;
    }
    Node_ptr tmp = *head;

    while (tmp->next != NULL && strcmp(tmp->key, new->key) != 0)
    {
        tmp = tmp->next;
    }
    if (!strcmp(tmp->key, new->key))
    {
        free(tmp->value);
        tmp->value = new->value;
        tmp->key = new->key;
        free(new);
    }
    else
    {
        tmp->next = new;
        (*size)++;
    }
    return OK;
}

Node_ptr find_by_key(Node_ptr list, const char* key)
{
    if (list == NULL || key == NULL)
    {
        return NULL;
    }
    Node_ptr curr = list;
    while (curr != NULL)
    {
        if (!strcmp(curr->key, key))
        {
            return curr;
        }
        curr = curr->next;
    }
    return curr;
}

void destruct_list(Node_ptr* head)
{
    if (*head == NULL)
    {
        return;
    }
    Node_ptr curr = *head;
    while (curr != NULL)
    {
        Node_ptr tmp = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
        curr = tmp;
    }
}

void print_list(Node_ptr list)
{
    Node_ptr curr = list;
    while (curr != NULL)
    {
        printf("(%s\t%s)", curr->key, curr->value);
        curr = curr->next;
    }
    printf("\n");
}