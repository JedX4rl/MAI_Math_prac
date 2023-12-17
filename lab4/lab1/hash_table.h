//
// Created by Никита Третьяков on 13.12.2023.
//

#ifndef LAB1_2_HASH_TABLE_H
#define LAB1_2_HASH_TABLE_H
#define HASH_SIZE 128
#define MOD 288230376151711744 // 2^58

typedef unsigned long long ull;

typedef struct Node
{
    char* key;
    char* value;
    struct Node* next;
} Node, *Node_ptr;

typedef struct Hash_table
{
    Node** table;
    ull* lists_sizes;
    ull hash_size;
    ull (*hash) (const char*, const ull);
} Hash_table;

typedef struct Cache
{
    char* key;
    ull value;
    struct Cache* next;
} Cache, *Cache_ptr;


#endif //LAB1_2_HASH_TABLE_H
