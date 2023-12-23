//
// Created by Никита Третьяков on 14.12.2023.
//

#ifndef LAB6_OP_TREE_H
#define LAB6_OP_TREE_H
#include "my_lib.h"

typedef enum
{
    NONE,
    VAR,
    CONST,
    L_BRACKET,
    R_BRACKET,
    CONJUNCTION,
    DISJUNCTION,
    INVERSION,
    IMPLICATION,
    COIMPLICATION,
    SUM_BY_2,
    EQUIVALENCE,
    SHEFFER,
    WEBB
} information;

typedef struct tree_node
{
    information info;
    char data;
    struct tree_node* left;
    struct tree_node* right;
} tree_node, *Node_ptr;

typedef struct
{
    Node_ptr root;
    char var_cnt;
    bool flag[26];
} Exp_tree;

Status create_node(Node_ptr* node, information info, char data);
void destruct_node(Node_ptr node);
Status create_exp_tree(Exp_tree* tree, const char* str);
Status calculate_exp_tree(Exp_tree tree, const bool* values, bool* result);
Status print_expression_table(FILE* out, Exp_tree tree);
Status generate_random_string(char** string);
Status file_out_name(const char* input_name, const char* output_name, char** out);
Status read_line(FILE* file, char** line);
void null_tree(Exp_tree* tree);
void destruct_exp_tree(Exp_tree* tree);
#endif //LAB6_OP_TREE_H
