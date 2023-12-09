#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "string.h"
#include "my_lib.h"
#include "binary_tree.h"

Status is_separated(const char separators[], int size, char ch)
{
    for (int i = 0; i < size; ++i)
    {
        if (separators[i] == ch)
        {
            return OK;
        }
    }
    return INVALID_INPUT;
}

Status build_tree(FILE* file, char separators[], Node_ptr* res, int amount_of_separators)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }

    Node_ptr root = NULL;
    Node_ptr node = NULL;
    Status st = OK;
    int size = 4;
    int amount_of_words = 0;
    int running = 1;
    int length = 0;
    char ch;

    char* temp_word = (char*)malloc(sizeof(char) * size);
    if (temp_word == NULL)
    {
        *res = NULL;
        return BAD_ALLOC;
    }

    while (running)
    {
        ch = fgetc(file);
        if (ch == EOF)
        {
            ch = fgetc(file);
        }
        if (ch == EOF)
        {
            running = 0;
        }
        if (length == 0)
        {
            temp_word = (char*)malloc(sizeof(char) * size);
            if (temp_word == NULL)
            {
                destruct_tree(root);
                *res = NULL;
                return BAD_ALLOC;
            }
        }
        if (!is_separated(separators, amount_of_separators, ch) || ch == '\n' || ch == '\t' || ch == EOF)
        {
            if (length != 0 && amount_of_words == 0)
            {

                st = create_tree(1, temp_word, &root);
                root->length = length;
                length = 0;
                if (st)
                {
                    free(temp_word);
                    temp_word = NULL;
                    destruct_tree(root);
                    *res = NULL;
                    return st;

                }
                amount_of_words++;
                free(temp_word);
                temp_word = NULL;
            }
            else if(amount_of_words != 0 && length != 0)
            {
                node = search_by_name(root, temp_word);
                if (node)
                {
                    node->cnt++;
                    node->length = length;
                }
                else
                {
                    st = add_node(1, temp_word, root);
                    if (st)
                    {
                        free(temp_word);
                        temp_word = NULL;
                        destruct_tree(root);
                        *res = NULL;
                        return st;
                    }
                    node = search_by_name(root, temp_word);
                    node->length = length;
                }
                length = 0;
                amount_of_words++;
                free(temp_word);
                temp_word = NULL;
            }
        }
        else if (is_separated(separators, amount_of_separators, ch) && ch != '\n' && ch != '\t' && ch != EOF)
        {
            if (length - 1 == size)
            {
                size *= 2;
                st = my_realloc(&temp_word, sizeof(char) * size);
                if (st)
                {
                    free(temp_word);
                    temp_word = NULL;
                    destruct_tree(root);
                    *res = NULL;
                    return st;
                }
            }
            temp_word[length++] = ch;
        }

    }
    free(temp_word);
    temp_word = NULL;
    *res = root;
    return OK;
}

Node_ptr get_max_cnt_node(Node_ptr root)
{
    Node_ptr node_left = root;
    Node_ptr node_right = root;
    if (root == NULL)
    {
        return NULL;
    }
    if (root->cnt >= node_left->cnt)
    {
        node_left = root;
    }
    if (root->cnt >= node_right->cnt)
    {
        node_right = root;
    }
    if (root->left)
    {
        node_left = get_max_cnt_node(root->left);
    }
    if (root->right)
    {
        node_right = get_max_cnt_node(root->right);
    }

    return node_left->cnt > node_right->cnt ? node_left : node_right;
}

Node_ptr the_longest_word(Node_ptr root)
{
    if (root == NULL)
    {
        return NULL;
    }
    Node_ptr tmp = root;
    while (tmp->right)
    {
        tmp = tmp->right;
    }
    return tmp;
}

Node_ptr the_shortest_word(Node_ptr root)
{
    if (root == NULL)
    {
        return NULL;
    }
    Node_ptr tmp = root;
    while (tmp->left)
    {
        tmp = tmp->left;
    }
    return tmp;
}

void most_popular(Node_ptr root, Node_ptr arr[], int amount)
{
    if (root == NULL)
    {
        return;
    }
    Node_ptr current = root;
    Node_ptr temp = NULL;
    for (int i = 0; i < amount; ++i)
    {
        if (arr[i] == NULL)
        {
            arr[i] = current;
            break;
        }
        else if (arr[i]->cnt < current->cnt)
        {
            temp = arr[i];
            arr[i] = current;
            current = temp;
        }
    }
    most_popular(root->left, arr, amount);
    most_popular(root->right, arr, amount);
}

void print_to_file(Node_ptr root, FILE* file)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < root->cnt; ++i)
    {
        fprintf(file, "%s_", root->word->str);
        fflush(file);
    }
    print_to_file(root->left, file);
    print_to_file(root->right, file);
}


int main(int argc, char* argv[])
{

    if (argc < 3)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    int amount_of_separators = argc - 2;
    char* separators = (char*)malloc(sizeof(char) * (amount_of_separators));
    if (separators == NULL)
    {
        fclose(file);
        errors(BAD_ALLOC);
        return BAD_ALLOC;
    }
    for (int i = 2, j = 0; i < argc; ++i, j++)
    {
        if (strlen(argv[i]) != 1 && argv[i] == EOF)
        {
            fclose(file);
            errors(INVALID_INPUT);
            return INVALID_INPUT;
        }
        separators[j] = *argv[i];
    }
    Node_ptr tree;
    int running = 1;
    Status st = build_tree(file, separators, &tree, amount_of_separators);
    if (st)
    {
        destruct_tree(tree);
        fclose(file);
        errors(st);
        return st;
    }

    while (!st && running)
    {
        printf("Available commands: exit, print_tree, print_statistic, amount_of_word, most_popular, longest_word,\n"
               "shortest_word, depth, save_to_file\n");
        char* command = NULL;
        char* name = NULL;
        printf("Enter command: \n");
        st = get_str(&command);
        if (!st) {
            if (!strcmp(command, "exit"))
            {
                printf("Bye!\n");
                running = 0;
            }
            else if (!strcmp(command, "print_tree"))
            {
                print_tree(tree, 3);
            }
            else if (!strcmp(command, "print_statistic"))
            {
                print_symmetrically(tree);
            }
            else if (!strcmp(command, "amount_of_word"))
            {
                printf("Enter the word\n");
                st = get_str(&name);
                if (!st) {
                    Node_ptr node = search_by_name(tree, name);
                    if (node)
                    {
                        printf("%d\n", node->cnt);
                    }
                    else
                    {
                        printf("0\n");
                    }
                }
                free(name);
            }
            else if (!st && !strcmp(command, "most_popular"))
            {
                printf("Enter amount\n");
                st = get_str(&name);
                int n = (int) strtol(name, NULL, 10);
                if (!st)
                {
                    Node_ptr *arr = (Node_ptr *) calloc(n, sizeof(Node_ptr));
                    if (arr == NULL)
                    {
                        st = BAD_ALLOC;
                    }
                    if (!st)
                    {
                        most_popular(tree, arr, n);
                        for (int i = 0; i < 3; ++i)
                        {
                            printf("%s\n", (arr[i])->word->str);
                        }
                        free(*arr);
                    }
                }
                free(name);
            }
            else if (!st && !strcmp(command, "longest_word"))
            {
                Node_ptr node = the_longest_word(tree);
                printf("%s has %d symbols\n", node->word->str, node->length);
            }
            else if (!st && !strcmp(command, "shortest_word"))
            {
                Node_ptr node = the_shortest_word(tree);
                printf("%s has %d symbols\n", node->word->str, node->length);
            }
            else if (!st && !strcmp(command, "depth"))
            {
                int d = depth(tree);
                print_tree(tree, 2);
                printf("The depth of tree is %d\n", d);
            }
            else if (!st && !strcmp(command, "save_to_file"))
            {
                printf("Enter the name of the file\n");
                st = get_str(&name);
                if (!st)
                {
                    FILE* out = fopen(name, "w");
                    if (out == NULL)
                    {
                        st = OPENING_ERROR;
                    }
                    if (!st)
                    {
                        printf("Current tree:\n");
                        print_tree(tree, 3);
                        print_to_file(tree, out);
                        fclose(out);
                        FILE* new_file = fopen(name, "r");
                        Node_ptr new_root;
                        if (new_file == NULL)
                        {
                            st = OPENING_ERROR;
                        }
                        if (!st)
                        {
                            destruct_tree(tree);
                            st =  build_tree(out, separators, &new_root, amount_of_separators);
                        }
                        fclose(out);
                        if (!st)
                        {
                            printf("_________NEW_TREE__________\n");
                            print_tree(new_root, 3);
                        }
                    }
                }
                free(name);
            }
            else
            {
                printf("Unknown command 0_0\n");
            }
        }
        if (st)
        {
            running = 0;
        }
        free(command);
    }
    if (st)
    {
        errors(st);
    }
    destruct_tree(tree);
    fclose(file);
    return st;
}
