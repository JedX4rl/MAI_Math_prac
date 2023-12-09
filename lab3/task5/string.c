//
// Created by Никита Третьяков on 06.12.2023.
//

#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "my_lib.h"

Status create_string(char* s, String** result)
{
    *result = (String*)malloc(sizeof(String));
    if (*result == NULL)
    {
        return BAD_ALLOC;
    }
    if (s == NULL)
    {
        (*result)->size = 0;
        (*result)->str = NULL;
        return OK;
    }
    int length = (int)strlen(s);
    (*result)->str = (char*)malloc(sizeof(char) * (length + 1));
    if ((*result)->str == NULL)
    {
        free(*result);
        return BAD_ALLOC;
    }
    (*result)->str[length] = '\0';
    for (int i = 0; i < length; ++i)
    {
        (*result)->str[i] = s[i];
    }
    (*result)->size = length;
    return OK;
}

Status destroy_string(String* string)
{
    if (string->str == NULL)
    {
        return NULL_POINTER;
    }
    free(string->str);
    string->str = NULL;
    string->size = 0;
    return OK;
}

int my_str_cmp(String str1, String str2)
{
    if (str1.str == NULL || str2.str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = str1.size;
    int length_2 = str2.size;
    int min = length_1;
    if (length_1 > length_2)
    {
        min = length_2;
    }
    for (int i = 0; i < min; ++i)
    {
        if (str1.str[i] != str2.str[i])
        {
            return str1.str[i] < str2.str[i] ? -1 : 1;
        }
    }
    return 0;
}

int compare_strings(String str1, String str2)
{
    if (str1.str == NULL || str2.str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = str1.size;
    int length_2 = str2.size;
    if (length_2 != length_1)
    {
        return 0;
    }
    return my_str_cmp(str1, str2) == 0;
}

Status my_strcpy(String* dest, String* src)
{
    if (src->str == NULL || dest->str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = dest->size;
    int length_2 = src->size;

    for (int i = 0; i < length_2; ++i)
    {
        if (i == length_1)
        {
            break;
        }
        dest->str[i] = src->str[i];
    }
    dest->str[length_2] = '\0';
    return OK;
}

Status copy_str(String* dest, String* source)
{
    create_string(source->str, dest);
    return OK;
}

Status concatenation(String* str1, String* str2)
{
    int length = str1->size + str2->size;
    my_realloc(&(str1->str), length);
    for (int i = 0; i < length; ++i)
    {
        str1->str[str1->size + i] = str2->str[i];
    }
    str1->size = length;
    return OK;
}