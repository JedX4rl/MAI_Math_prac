//
// Created by Никита Третьяков on 13.12.2023.
//

#ifndef LAB1_2_MY_LIB_H
#define LAB1_2_MY_LIB_H
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

typedef long long int ll;
typedef unsigned long long ull;

typedef enum
{
    OK,
    BAD_ALLOC,
    NOT_OK,
    INVALID_INPUT,
    NULL_POINTER,
    OPENING_ERROR

} Status;

//-------------Hashtable-------------//

int get_number(char ch);
ull pre_hash(const char* key);
ull hash(const char* key, ull hash_size);
Status needs_rehash(const Hash_table* h_table);
Status create_h_table(Hash_table** h_table, ull hash_size, ull (*hash) (const char*, const ull));
void destruct_hash_table(Hash_table** h_table);
int rehash(Cache_ptr cache, Hash_table** old);
Status insert(Hash_table** h_table, Cache_ptr* cache, char* key, char* value);
char* find_value(const Hash_table* h_table, const char* key);
void print_h_table(const Hash_table* h_table);

//-------------list---------------//

Status create_node(Node_ptr* new, char* key, char* value);
Status add_node(Node_ptr* head, char* key, char* value);
Status add_similar_node(Node_ptr* head, char** key, char* value, ull* size);
Node_ptr find_by_key(Node_ptr list, const char* key);
void destruct_list(Node_ptr* head);
void print_list(Node_ptr list);

//-----------cache-------------//

Status create_cache_node(Cache_ptr* cache, char* key, ull value);
Status add_cache_node(Cache_ptr* head, char** key, ull value);
void destruct_cache(Cache_ptr list);
void print_cache(Cache_ptr list);

//----------my_lib------------//

void free_all(int cnt, ...);
Status my_realloc(void** ptr, ull size);
void errors(Status status);
int max(int a, int b);
Status get_str(char** str);
long long power(long long num, long long deg);
Status read_until(const char* string, const char* stop, const char** end_ptr, char** new_string);


#endif //LAB1_2_MY_LIB_H
