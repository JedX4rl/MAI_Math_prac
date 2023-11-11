#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC,
    NULL_POINTER,
    OPENING_ERROR
} Status;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NULL_POINTER:
            printf("NULL pointer error\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

typedef struct Node
{
    char key;
    struct Node* child;
    struct Node* brother;
    struct Node* parent;
} Node, *Node_ptr;

typedef struct tree
{
    Node* root;
} tree;

Status destroy_tree(Node* node)
{
    if (node == NULL)
    {
        return OK;
    }
    destroy_tree(node->child);
    destroy_tree(node->brother);
    free(node);
    return OK;
}

Status insert_child(Node* node, char data)
{
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL)
    {
        return BAD_ALLOC;
    }
    new->key = data;
    new->parent = node;
    new->brother = NULL;
    new->child = NULL;
    node->child = new;
    return OK;
}

Status insert_brother(Node* node, char data)
{
    Node* new = (Node*)malloc(sizeof(Node));
    if (new == NULL)
    {
        return BAD_ALLOC;
    }
    new->key = data;
    new->parent = node->parent;
    new->child = NULL;
    node->brother = new;
    new->brother = NULL;
    return OK;
}

Status create_tree(tree* tree, char* exp)
{
    size_t length = strlen(exp);
    char symb;
    Node* root = (Node*)malloc(sizeof(Node));
    if (root == NULL)
    {
        return BAD_ALLOC;
    }
    int i = 0;
    root->key = exp[i];
    root->child = NULL;
    root->brother = NULL;
    root->parent = NULL;
    tree->root = root;
    i++;
    Status insert;
    Node* curr = tree->root;
    for (i; i < length; ++i)
    {
        symb = exp[i];
        switch (symb)
        {
            case '(':
                i++;
                insert = insert_child(curr, exp[i]);
                if (insert == BAD_ALLOC)
                {
                    return BAD_ALLOC;
                }
                curr = curr->child;
                break;
            case ')':
                curr = curr->parent;
                break;
            case ',':
                i++;
                insert = insert_brother(curr, exp[i]);
                if (insert == BAD_ALLOC)
                {
                    return BAD_ALLOC;
                }
                curr = curr->brother;
                break;
            default:
                break;
        }    
    }
    return OK;
}

Status print_node(FILE* file, Node* node, int depth)
{
    if (node == NULL)
    {
        return OK;
    }
    if (node->brother)
    {
        print_node(file, node->brother, depth);
    }
    if (node->child)
    {
        print_node(file, node->child,depth + 1);
    }
    for (int i = 0; i < depth; ++i)
    {
        fprintf(file, "    ");
    }
    fprintf(file, "%c    \n", node->key);
    return OK;
}

Status print_tree(FILE* file, tree* tree, char* exp)
{
    int depth = 0;
    fprintf(file, "Exp: %s\n", exp);
    print_node(file, tree->root, depth);
    fprintf(file, "\n");
    return OK;
}

Status processing(FILE* input, FILE* output)
{
    if (input == NULL || output == NULL)
    {
        return NULL_POINTER;
    }
    tree root;
    root.root = NULL;
    Status create;
    char* exp = NULL;
    int read;
    while ((read = getline(&exp, &(size_t){0}, input)) != -1)
    {
        if (read == -1)
        {
            free(exp);
            return BAD_ALLOC;
        }
        create = create_tree(&root, exp);
        if (create == BAD_ALLOC)
        {
            free(exp);
            return BAD_ALLOC;
        }
        print_tree(output, &root, exp);
        free(exp);
        destroy_tree(root.root);
        exp = NULL;
        root.root = NULL;
    }
    free(exp);
    return OK;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    const char* filename_in = argv[1];
    const char* filename_out = argv[2];
    FILE* input;
    FILE* output;
    if ((input = fopen(filename_in, "r")) == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    if ((output = fopen(filename_out, "w")) == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    Status status = processing(input, output);
    if (status != OK)
    {
        errors(status);
        return status;
    }
    fclose(input);
    fclose(output);
    return 0;
}
