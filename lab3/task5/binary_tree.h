//
// Created by Никита Третьяков on 06.12.2023.
//

#ifndef LAB9_BINARY_TREE_H
#define LAB9_BINARY_TREE_H

#include "string.h"

typedef struct tree
{
    int cnt;
    int length;
    String* word;
    struct tree* left;
    struct tree* right;
    struct tree* parent;
} Node, *Node_ptr;

void print_tree(Node_ptr root, int space);
void print_symmetrically(Node_ptr root);
int depth(Node_ptr node);
void free_node(Node_ptr node);
void destruct_tree(Node_ptr root);
Status create_tree(int counter, char* word, Node_ptr* node);
Status add_node(int counter, char* word, Node_ptr root);
Node_ptr min_node(Node_ptr root);
Node_ptr max_node(Node_ptr root);
Node_ptr search_min(Node_ptr root);
Node_ptr search_by_name(Node_ptr root, char* word);
void delete_node(Node_ptr root, char* word);


#endif //LAB9_BINARY_TREE_H
