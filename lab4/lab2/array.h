//
// Created by Никита Третьяков on 21.12.2023.
//

#ifndef LAB2_ARRAY_H
#define LAB2_ARRAY_H
#include "my_lib.h"

typedef struct Array
{
    char name;
    int* data;
    int size;
    int capacity;
} Array;

Status create_array(Array** arr, char ch);
Array* find_array(Array** storage, int size, char name);
Status load(Array** arr, char name, const char* filename);
Status add_to_storage(Array** storage, Array* a, int* capacity);
Status save(Array* arr, const char* filename);
Status rand_array(Array** array, char name, char* args);
Status concatenation(Array* a, Array* b);
Status Free(Array* array);
Status delete_items(Array* arr, int idx, int cnt);
Status copy(Array* a, int start, int end, Array** b);
void sort(Array* a, char mode);
void Shuffle(Array* a);
void stats(Array* a);
void print(Array* array, char name, char* mode);
void free_storage(Array** storage, int size);



#endif //LAB2_ARRAY_H
