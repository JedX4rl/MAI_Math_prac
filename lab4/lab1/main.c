#include <stdio.h>
#include "my_lib.h"
#include "hash_table.h"

Status is_key(const char* key)
{
    if (key == NULL)
    {
        return INVALID_INPUT;
    }
    for (int i = 0; key[i]; ++i)
    {
        if (!isalnum(key[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status file_processing(FILE* in)
{
    if (in == NULL)
    {
        return INVALID_INPUT;
    }
    FILE* out = fopen("/Users/nikitatretakov/Uni/Labs/MathPracticum/Laboratory_work4/lab1_2/out.txt", "w");
    if (out == NULL)
    {
        return OPENING_ERROR;
    }
    char* curr_line = NULL;
    int running = 1;
    size_t size = 0;
    Hash_table* h_table;
    Cache_ptr cache = NULL;
    Status st = create_h_table(&h_table, 128, hash);
    if (st)
    {
        destruct_hash_table(&h_table);
        destruct_cache(cache);
        fclose(out);
        return OPENING_ERROR;
    }
    while ((running = getline(&curr_line, &size, in)) != -1)
    {
        if (running == -1)
        {
            fclose(in);
            fclose(out);
            destruct_hash_table(&h_table);
            destruct_cache(cache);
            free(curr_line);
            return BAD_ALLOC;
        }
        int length = (int)strlen(curr_line);
        char* key = (char*)malloc(sizeof(char*) * length);
        if (key == NULL)
        {
            fclose(in);
            fclose(out);
            free(curr_line);
            destruct_hash_table(&h_table);
            destruct_cache(cache);
            return BAD_ALLOC;
        }
        char* value = (char*)malloc(sizeof(char*) * length);
        if (value == NULL)
        {
            fclose(in);
            fclose(out);
            free(curr_line);
            free(key);
            destruct_hash_table(&h_table);
            destruct_cache(cache);
            return BAD_ALLOC;
        }
        if (sscanf(curr_line, "#define %s %[^\n]", key, value) != 2)
        {
            if (!strcmp(curr_line, "\n") || !strcmp(curr_line, "\0"))
            {
                fprintf(out, "%s", curr_line);
            }
            free(curr_line);
            curr_line = NULL;
            if (key)
            {
                free(key);
            }
            if (value)
            {
                free(value);
            }
            break;
        }
        if (is_key(key))
        {
            free(curr_line);
            curr_line = NULL;
            fclose(in);
            fclose(out);
            destruct_hash_table(&h_table);
            destruct_cache(cache);
            return st;
        }
        st = insert(&h_table, &cache, key, value);
        if (st)
        {
            free(curr_line);
            curr_line = NULL;
            fclose(in);
            fclose(out);
            destruct_hash_table(&h_table);
            destruct_cache(cache);
            if (key)
            {
                free(key);
            }
            if (value)
            {
                free(value);
            }
            return st;
        }
        fprintf(out, "%s", curr_line);
        free(curr_line);
        curr_line = NULL;
        key = NULL;
        value = NULL;
    }
    free(curr_line);
    char ch = fgetc(in);
    int capacity = 32;
    int idx = 0;
    char* curr_word = (char*)malloc(sizeof(char) * capacity);
    if (curr_word == NULL)
    {
        fclose(in);
        fclose(out);
        destruct_hash_table(&h_table);
        destruct_cache(cache);
        return BAD_ALLOC;
    }
    int success = 0;
    while (ch != EOF)
    {
        if (idx + 1 == capacity)
        {
            st = my_realloc(&curr_line, sizeof(char) * (capacity * 2));
            if (st)
            {
                free(curr_word);
                destruct_hash_table(&h_table);
                destruct_cache(cache);
                fclose(in);
                fclose(out);
                return st;
            }
            capacity *= 2;
        }
        if (!isspace(ch) && ch != EOF)
        {
            curr_word[idx++] = ch;
            success = 1;
        }
        else
        {
            if (success)
            {
                curr_word[idx] = '\0';
                idx = 0;
                char* word_to_replace = find_value(h_table, curr_word);
                if (word_to_replace == NULL)
                {
                    fprintf(out, "%s", curr_word);
                    fprintf(out, "%c", ch);
                }
                else
                {
                    fprintf(out, "%s", word_to_replace);
                    fprintf(out, "%c", ch);
                }
                free(curr_word);
                curr_word = NULL;
                capacity = 32;
                curr_word = (char*)malloc(sizeof(char) * capacity);
                if (curr_word == NULL)
                {
                    fclose(in);
                    fclose(out);
                    destruct_hash_table(&h_table);
                    destruct_cache(cache);
                    return BAD_ALLOC;
                }
                success = 0;
            }
            else
            {
                fprintf(out, "%c", ch);
            }
        }
        ch = fgetc(in);
    }
    if (success)
    {
        curr_word[idx] = '\0';
        idx = 0;
        char* word_to_replace = find_value(h_table, curr_word);
        if (word_to_replace == NULL)
        {
            fprintf(out, "%s", curr_word);

        }
        else
        {
            fprintf(out, "%s", word_to_replace);
        }
        success = 0;
    }
    if (curr_word)
    {
        free(curr_word);
    }
    fclose(in);
    fclose(out);
    destruct_hash_table(&h_table);
    destruct_cache(cache);
    return OK;
}



int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    FILE* input = fopen(argv[1], "r");
    if (input == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    Status st = file_processing(input);
    return 0;
}