//
// Created by Никита Третьяков on 06.12.2023.
//

#include <string.h>
#include <printf.h>
#include "binary_tree.h"
#include "my_lib.h"

Status create_tree(int counter, char* word, Node_ptr* node)
{
    *node = (Node_ptr)malloc(sizeof(Node));
    if (*node == NULL)
    {
        *node = NULL;
        return BAD_ALLOC;
    }
    String* string;
    if (create_string(word, &string))
    {
        free(*node);
        *node = NULL;
        return BAD_ALLOC;
    }
    (*node)->cnt = counter;
    (*node)->length = 0;
    (*node)->word = string;
    (*node)->left = (*node)->right = NULL;
    (*node)->parent = NULL;
    return OK;
}

void free_node(Node_ptr node)
{
    if (node == NULL)
    {
        return;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->cnt = 0;
    node->length = 0;
    destroy_string(node->word);
    free(node);
    node = NULL;
}

void destruct_tree(Node_ptr root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left)
    {
        destruct_tree(root->left);
    }
    if (root->right)
    {
        destruct_tree(root->right);
    }
    free_node(root);
    root = NULL;
}

Status add_node(int counter, char* word, Node_ptr root)
{
    Node_ptr tmp_root = root;
    Node_ptr parent = NULL;
    Node_ptr temp = (Node_ptr)malloc(sizeof(Node));
    if (temp == NULL)
    {
        root = NULL;
        return BAD_ALLOC;
    }
    String* str;
    if (create_string(word, &str))
    {
        free(temp);
        return BAD_ALLOC;
    }
    temp->cnt = counter;
    temp->word = str;
    int length = (int)strlen(word);
    while (tmp_root)
    {
        parent = tmp_root;
        if (length > parent->length || (length == parent->length && strcmp(word, parent->word->str) > 0))
        {
            tmp_root = tmp_root->right;
        }
        else
        {
            tmp_root = tmp_root->left;
        }
    }
    temp->parent = parent;
    temp->left = temp->right = NULL;
    if (length > parent->length || (length == parent->length && strcmp(word, parent->word->str) > 0))
    {
        parent->right = temp;
    }
    else
    {
        parent->left = temp;
    }
    return OK;
}

void print_tree(Node_ptr root, int space)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left)
    {
        print_tree(root->left, space + strlen(root->word->str));
    }
    for (int i = 0; i < space; ++i)
    {
        printf("   ");
    }
    printf("%s\n", root->word->str);
    if (root->right)
    {
        print_tree(root->right, space + strlen((root->word->str)));
    }
}

void print_symmetrically(Node_ptr root)
{
    if (root == NULL)
    {
        return;
    }
    print_symmetrically(root->left);
    printf("word: \"%s\" count %d\n", root->word->str, root->cnt);
    print_symmetrically(root->right);
}


int depth(Node_ptr node)
{
    if (node == NULL)
    {
        return 0;
    }
    int max_left = depth(node->left);
    int max_right = depth(node->right);
    return max(max_left, max_right) + 1;
}

Node_ptr min_node(Node_ptr root)
{
    Node_ptr node = root;
    while (node->left)
    {
        node = node->left;
    }
    return node;
}

Node_ptr max_node(Node_ptr root)
{
    Node_ptr node = root;
    while (node->right)
    {
        node = node->right;
    }
    return node;
}

Node_ptr search_by_name(Node_ptr root, char* word)
{
    if (root == NULL || !strcmp(word, root->word->str))
    {
        return root;
    }
    if (strlen(word) > root->length || (strlen(word) == root->length && strcmp(word, root->word->str) > 0))
    {
        return search_by_name(root->right, word);
    }
    else
    {
        return search_by_name(root->left, word);
    }
}

Node_ptr search_min(Node_ptr root)
{
    Node_ptr tmp_root = root;
    Node_ptr node = NULL;
    if (root->right)
    {
        return min_node(root->right);
    }
    node = tmp_root->parent;
    while (node && (tmp_root == node->right))
    {
        tmp_root = node;
        node = node->parent;
    }
    return node;
}

void delete_node(Node_ptr root, char* word)
{
    Node_ptr node = NULL;
    Node_ptr parent = NULL;

    node = search_by_name(root, word);
    if (node->left == NULL && node->right == NULL) {
        parent = node->parent;
        if (node == parent->right) {
            parent->right = NULL;
        } else {
            parent->left = NULL;
        }
        free_node(node);
        node = NULL;
    }
    if (node->left == NULL && node->right)
    {
        parent = node->parent;
        if (parent == NULL)
        {
            root = node->right;
        }
        else
        {
            if (node == parent->right)
            {
                parent->right = node->right;
            }
            else
            {
                parent->left = node->right;
            }
        }
        free_node(node);
        node = NULL;
    }
    if (node->left && node->right == NULL)
    {
        parent = node->parent;
        if (parent == NULL)
        {
            root = node->left;
        }
        else
        {
            if (node == parent->left)
            {
                parent->left = node->left;
            }
            else
            {
                parent->right = node->left;
            }
        }
        free_node(node);
        node = NULL;
    }
    if (node->left && node->right)
    {
        parent = search_min(node);
        strcpy(node->word->str, parent->word->str);
        node->cnt = parent->cnt;
        parent->parent->left = parent->right;
        free_node(parent);
        parent = NULL;
    }
}

