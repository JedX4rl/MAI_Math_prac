//
// Created by Никита Третьяков on 13.12.2023.
//

#include "my_lib.h"

int get_number(char ch)
{
    if (isdigit(ch)) return ch - '0';
    if (isupper(ch)) return ch - 'A' + 10;
    if (islower(ch)) return ch - 'a' + 20;
    return -1;
}

ull pre_hash(const char* key)
{
    ull res = get_number(key[0]);
    int idx = 1;
    while (key[idx] != '\0')
    {
        res *= 62;
        res %= MOD;
        res += get_number(key[idx++]);
        res %= MOD;
    }
    return res;
}

ull hash(const char* key, const ull hash_size)
{
    return pre_hash(key) % hash_size;
}

Status needs_rehash(const Hash_table* h_table)
{
    ull min = h_table->lists_sizes[0];
    ull max = h_table->lists_sizes[0];

    for (int i = 0; i < h_table->hash_size; ++i)
    {
        if (h_table->lists_sizes[i] > max)
        {
            max = h_table->lists_sizes[i];
        }
        if (h_table->lists_sizes[i] < min)
        {
            min = h_table->lists_sizes[i];
        }
    }

    if (max / min >= 2)
    {
        return OK;
    }
    return NOT_OK;
}

Status create_h_table(Hash_table** h_table, ull hash_size, ull (*hash) (const char*, const ull))
{
    *h_table = (Hash_table*)malloc(sizeof(Hash_table));
    if (*h_table == NULL)
    {
        return BAD_ALLOC;
    }
    (*h_table)->hash_size = hash_size;
    (*h_table)->lists_sizes = NULL;
    (*h_table)->table = (Node_ptr*)calloc(hash_size, sizeof(Node_ptr));
    if ((*h_table)->table == NULL)
    {
        free(*h_table);
        return BAD_ALLOC;
    }

    (*h_table)->lists_sizes = (ull*)calloc(hash_size, sizeof(ull));
    if ((*h_table)->lists_sizes == NULL)
    {
        free((*h_table)->table);
        free(*h_table);
        return BAD_ALLOC;
    }
    (*h_table)->hash = hash;
    return OK;
}

void destruct_hash_table(Hash_table** h_table)
{
    if (*h_table == NULL)
    {
        return;
    }
    for (int i = 0; i < (*h_table)->hash_size; ++i)
    {
        destruct_list(&(*h_table)->table[i]);
    }
    free((*h_table)->table);
    free((*h_table)->lists_sizes);
    free(*h_table);
    *h_table = NULL;
}

int rehash(Cache_ptr cache, Hash_table** old)
{
    if (cache == NULL || *old == NULL)
    {
        return INVALID_INPUT;
    }
    ull new_size = (*old)->hash_size * 2;
    Hash_table* new;
    Status st = create_h_table(&new, new_size, (*old)->hash);
    if (st)
    {
        return BAD_ALLOC;
    }
    new->hash_size = new_size;
    Cache_ptr curr = cache;
    while (curr != NULL)
    {
        ull c_hash = curr->value;
        char* new_key = curr->key;
        char* new_value = find_by_key((*old)->table[c_hash % (*old)->hash_size], new_key)->value;
        st = add_similar_node(&(new->table[c_hash % new_size]), &new_key, new_value, &(new->lists_sizes[c_hash%new_size]));
        if (st)
        {
            destruct_hash_table(&new);
            return st;
        }
        curr = curr->next;
    }
    destruct_hash_table(old);
    *old = new;
    return OK;
}

Status insert(Hash_table** h_table, Cache_ptr* cache, char* key, char* value)
{
    ull hash = (*h_table)->hash(key, MOD);
    Status st = add_cache_node(cache, &key, hash);
    if (st)
    {
        return st;
    }
    hash %= (*h_table)->hash_size;
    st = add_similar_node((&(*h_table)->table[hash]), &key, value, &((*h_table)->lists_sizes[hash]));
    if (st)
    {
        return st;
    }
    while (!needs_rehash(*h_table))
    {
        st = rehash(*cache, h_table);
        if (st)
        {
            return st;
        }
    }
    return OK;
}

char* find_value(const Hash_table* h_table, const char* key)
{
    ull hash = h_table->hash(key, h_table->hash_size);
    Node_ptr res = find_by_key(h_table->table[hash], key);
    return res == NULL ? NULL : res->value;
}

void print_h_table(const Hash_table* h_table)
{
    printf("Hashtable for hash size %llu: \n", h_table->hash_size);
    for (ull i = 0; i < h_table->hash_size; ++i)
    {
       Node_ptr curr = h_table->table[i];
       if (curr != NULL)
       {
           printf("Hash value: %llu; size: %llu \t", i, h_table->lists_sizes[i]);
       }
        while (curr != NULL)
        {
            printf("(%s %s)", curr->key, curr->value);
            curr = curr->next;
        }
        if (h_table->table[i] != NULL)
        {
            printf("\n");
        }
    }
}