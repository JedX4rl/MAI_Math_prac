//
// Created by Никита Третьяков on 06.12.2023.
//
#include "my_lib.h"
#ifndef LAB9_STRING_H
#define LAB9_STRING_H

typedef struct String
{
    char* str;
    int size;
} String;

Status create_string(char* s, String** result);
Status destroy_string(String* string);
int my_str_cmp(String str1, String str2);
int compare_strings(String str1, String str2);
Status my_strcpy(String* dest, String* src);
Status copy_str(String* dest, String* source);
Status concatenation(String* str1, String* str2);

#endif
