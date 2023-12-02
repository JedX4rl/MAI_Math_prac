#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef long long int ll;
typedef unsigned long long ull;

typedef enum
{
    OK,
    INVALID_INPUT,
    OPENING_ERROR,
    DIVISION_BY_ZERO,
    INVALID_ARGUMENT,
    BAD_ALLOC,
    UNINITIALIZED_VARIABLE
} Status;

typedef enum
{
    ADD,
    ASSIGN,
    SUB,
    MULT,
    DIV,
    MOD,
    PRINT,
    NONE
} Operation_status;

void errors(Status st)
{
    switch (st)
    {
        case OK:
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        case DIVISION_BY_ZERO:
            printf("Division by zero\n");
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case INVALID_ARGUMENT:
            printf("Invalid argument\n");
            return;
        case UNINITIALIZED_VARIABLE:
            printf("ERROR! Using of uninitialised variable\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

typedef struct
{
    char* name;
    ll value;
} Memory_cell;

typedef struct
{
    ull size;
    ull cnt;
    Memory_cell* variables;
} Memory;

Status my_realloc(void** ptr, ull size)
{
    if (ptr == NULL || size == 0)
    {
        return INVALID_ARGUMENT;
    }
    void *new_ptr = realloc(*ptr, size);
    if (new_ptr != NULL)
    {
        *ptr = new_ptr;
        return OK;
    }
    return BAD_ALLOC;
}

void free_all(int cnt, ...)
{
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = 0; i < cnt; ++i)
    {
        void* tmp = va_arg(ptr, void*);
        free(tmp);
    }
    va_end(ptr);
}

int is_word(const char* str)
{
    if (str == NULL || *str == '\0')
    {
        return 0;
    }
    for (ull i = 0; str[i]; ++i)
    {
        if (!isalpha(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

int is_number(const char* str)
{
    if (str == NULL || *str == '\0')
    {
        return 0;
    }
    if (!isdigit(str[0]) && str[0] != '-' && str[0] != '+')
    {
        return 0;
    }
    for (ull i = 1; str[i]; ++i)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

Status null_memory(Memory* ptr)
{
    if (ptr == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ptr->size = 0;
    ptr->cnt = 0;
    ptr->variables = NULL;
    return OK;
}

Status destruct_memory(Memory* ptr)
{
    if (ptr == NULL)
    {
        return INVALID_ARGUMENT;
    }
    for (ull i = 0; i < ptr->cnt; ++i)
    {
        free(ptr->variables[i].name);
    }
    free(ptr->variables);
    ptr->size = 0;
    ptr->cnt = 0;
    ptr->variables = NULL;
    return OK;
}

int compare_memory(const void* a, const void* b)
{
    Memory_cell* a_cell = (Memory_cell*)a;
    Memory_cell* b_cell = (Memory_cell*)b;
    return strcmp(a_cell->name, b_cell->name);
}

Status insert_memory_cell(Memory* memory, const char* name, ll value)
{
    if (memory == NULL || name == NULL || *name == '\0')
    {
        return INVALID_ARGUMENT;
    }
    if (memory->variables == NULL)
    {
        memory->variables = (Memory_cell*)malloc(sizeof(Memory_cell) * 2);
        if (memory->variables == NULL)
        {
            return BAD_ALLOC;
        }
        memory->cnt = 0;
        memory->size = 2;
    }
    if (memory->cnt == memory->size)
    {
        Status st = my_realloc(&(memory->variables), sizeof(Memory) * 2);
        if (st != OK)
        {
            return BAD_ALLOC;
        }
        memory->size *= 2;
    }
    Memory_cell tmp;
    tmp.name = (char*)malloc(sizeof(char) * strlen(name) + 1);
    if (tmp.name == NULL)
    {
        return BAD_ALLOC;
    }
    strcpy(tmp.name, name);
    tmp.value = value;
    memory->variables[memory->cnt++] = tmp;
    qsort(memory->variables, memory->cnt, sizeof(Memory_cell), compare_memory);
    return OK;
}

Status search_memory(Memory memory, const char* name, ll* idx)
{
    if (name == NULL || idx == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (memory.cnt == 0)
    {
        *idx = -1;
        return OK;
    }
    ull left_idx = 0;
    ull right_idx = memory.cnt;
    while (left_idx < right_idx)
    {
        ull mid_idx = (left_idx + right_idx) / 2;
        int compare = strcmp(name, memory.variables[mid_idx].name);
        if (compare > 0)
        {
            left_idx = mid_idx + 1;
        }
        else if (compare < 0)
        {
            right_idx = mid_idx;
        }
        else
        {
            *idx = mid_idx;
            return OK;
        }
    }
    *idx = -1;
    return OK;
}

Status get_variable_value(Memory memory, const char* name, ll* value)
{
    if (name == NULL || value == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ll idx = 0;
    Status st = search_memory(memory, name, &idx);
    if (st)
    {
        return st;
    }
    if (idx == -1)
    {
        return UNINITIALIZED_VARIABLE;
    }
    *value = memory.variables[idx].value;
    return OK;
}

Status change_variable_value(Memory* memory, const char* name, ll value)
{
    if (memory == NULL || name == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ll idx = 0;
    Status st = search_memory(*memory, name, &idx);
    if (st)
    {
        return st;
    }
    if (idx == -1)
    {
        st = insert_memory_cell(memory, name, value);
        return st;
    }
    memory->variables[idx].value = value;
    return OK;
}

Status get_argument_value(Memory memory, const char* argument, ll* value)
{
    if (argument == NULL || value == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (is_word(argument))
    {
        return get_variable_value(memory, argument, value);
    }
    if (is_number(argument))
    {
        ll tmp = strtoll(argument, NULL, 10);
        *value = tmp;
        return OK;
    }
    return INVALID_INPUT;
}

Status print_variable(Memory memory, const char* name)
{
    if (name == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (!is_word(name))
    {
        return INVALID_INPUT;
    }
    ll idx;
    Status st = search_memory(memory, name, &idx);
    if (st)
    {
        return st;
    }
    if (idx == -1)
    {
        return UNINITIALIZED_VARIABLE;
    }
    printf("%s = %lld\n", memory.variables[idx].name, memory.variables[idx].value);
    return OK;
}

Status print_all_variables(Memory memory)
{
    if (!memory.cnt)
    {
        printf("No initialized variables\n");
        return OK;
    }
    for (ull i = 0; i < memory.cnt; ++i)
    {
        printf("%s = %lld\n", memory.variables[i].name, memory.variables[i].value);
    }
    return OK;
}

Status read_expression(FILE* file, char** string)
{
    if (file == NULL || string == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull counter = 0;
    ull size = 2;
    *string = (char*)malloc(sizeof(char) * size);
    if (*string == NULL)
    {
        return BAD_ALLOC;
    }
    char ch = getc(file);
    int running = 1;
    while (running)
    {
        while (ch == ' ' || ch == '\n' || ch == '\t')
        {
            ch = getc(file);
        }
        running = 0;
    }
    while (ch != ';' && !feof(file))
    {
        if (counter == size - 2)
        {
            size *= 2;
            Status st = my_realloc(string, sizeof(char) * size);
            if (st)
            {
                free(*string);
                return BAD_ALLOC;
            }
        }
        (*string)[counter++] = ch;
        ch = getc(file);
    }
    if (ch == ';')
    {
        (*string)[counter++] = ch;
    }
    (*string)[counter] = '\0';
    return OK;
}

Status read_until(const char* string, const char* stop, const char** end_ptr, char** new_string)
{
    if (string == NULL  || new_string == NULL || stop == NULL)
    {
        return INVALID_ARGUMENT;
    }
    ull counter = 0;
    ull size = 2;
    *new_string = (char*)malloc(sizeof(char) * size);
    if (*new_string == NULL)
    {
        return BAD_ALLOC;
    }
    char symbols[256];
    for (int i = 0; i < 256; ++i)
    {
        symbols[i] = 1;
    }
    for (ull i = 0; stop[i]; ++i)
    {
        symbols[(int)stop[i]] = 0;
    }
    const char* ptr = string;

    ull curr = (int)*ptr;
    while (symbols[(int)*ptr] && *ptr != '\0')
    {
        if (counter == size - 1)
        {
            size *= 2;
            Status st = my_realloc(new_string, sizeof(char) * size);
            if (st)
            {
                free(*new_string);
                return st;
            }
        }
        (*new_string)[counter++] = *ptr;
        ptr++;
    }
    (*new_string)[counter] = '\0';
    *end_ptr = ptr;
    return OK;
}

int is_arith_operator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%';
}

Operation_status char_to_operation(char ch)
{
    switch (ch)
    {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MULT;
        case '/':
            return DIV;
        case '%':
            return MOD;
        case '=':
            return ASSIGN;
        default:
            return NONE;
    }
}

Status parse(const char* source, char** str1, char** str2, char** str3, Operation_status* operation, ull* amount_of_arguments)
{
    if (source == NULL || str1 == NULL || str2 == NULL || str3 == NULL || operation == NULL || amount_of_arguments == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (*source == '\0')
    {
        *str1 = NULL;
        *str2 = NULL;
        str3 = NULL;
        *operation = NONE;
        *amount_of_arguments = 0;
        return OK;
    }
    Status st = OK;
    Operation_status tmp;
    const char* ptr = source;
    ull cnt = 0;
    char* tmp1 = NULL;
    char* tmp2 = NULL;
    char* tmp3 = NULL;

    st = read_until(ptr, " =;", &ptr, &tmp1);
    cnt++;

    if (!st && !strcmp(tmp1, "print"))
    {
        tmp = PRINT;
        if (*ptr == ' ')
        {
            st = read_until(++ptr, ";", &ptr, &tmp2);
            cnt++;
        }
    }

    else
    {
        st = *ptr == '=' ? OK : INVALID_INPUT;
        tmp = ASSIGN;
        ptr++;
        if(!st && (*ptr == '-' || *ptr == '+'))
        {
            char sign = *ptr;
            char* temp = NULL;
            st = read_until(++ptr, "+-/*%;", &ptr, &temp);
            tmp2 = st ? NULL : ((char*)malloc(sizeof(char) * strlen(temp + 2)));
            st = st ? st : (tmp2 != NULL ? OK : BAD_ALLOC);
            if (!st)
            {
                strcpy(tmp2 + 1, temp);
                tmp2[0] = sign;
            }
            free(temp);
        }
        else
        {
            st = st ? st : read_until(ptr, "+-/*%;", &ptr, &tmp2);
        }
        cnt++;
        if (!st && is_arith_operator(*ptr))
        {
            tmp = char_to_operation(*ptr);
            st = read_until(++ptr, ";", &ptr, &tmp3);
            cnt++;
        }
    }
    if (!st && tmp1 != NULL)
    {
        st = is_word(tmp1) ? OK : INVALID_INPUT;
    }
    if (!st && tmp2 != NULL)
    {
        st = (is_word(tmp2) || is_number(tmp2)) ? OK : INVALID_INPUT;
    }
    if (!st && tmp3 != NULL)
    {
        st = (is_word(tmp3) || is_number(tmp3)) ? OK : INVALID_INPUT;
    }
    if (st)
    {
        free_all(3, tmp1, tmp2, tmp3);
        return st;
    }
    *operation = tmp;
    *str1 = tmp1;
    *str2 = tmp2;
    *str3 = tmp3;
    *amount_of_arguments = cnt;
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
    Memory memory;
    null_memory(&memory);
    Status st = OK;
    while (!st && !feof(input))
    {
        char* current_expression = NULL;
        Operation_status current_operation;
        char* tmp_1 = NULL;
        char* tmp_2 = NULL;
        char* tmp_3 = NULL;
        ull amount_of_arguments = 0;
        st = read_expression(input, &current_expression);
        st = st ? st : parse(current_expression, &tmp_1, &tmp_2, &tmp_3, &current_operation, &amount_of_arguments);
        free(current_expression);
        ll value_1 = 0;
        ll value_2 = 0;

        switch (current_operation)
        {
            case ASSIGN:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1);
                break;
            }
            case ADD:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : get_argument_value(memory, tmp_3, &value_2);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1 + value_2);
                break;
            }
            case SUB:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : get_argument_value(memory, tmp_3, &value_2);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1 - value_2);
                break;
            }
            case MULT:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : get_argument_value(memory, tmp_3, &value_2);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1 * value_2);
                break;
            }
            case DIV:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : get_argument_value(memory, tmp_3, &value_2);
                st = st ? st : (value_2 == 0 ? DIVISION_BY_ZERO : OK);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1 / value_2);
                break;
            }
            case MOD:
            {
                st = st ? st : get_argument_value(memory, tmp_2, &value_1);
                st = st ? st : get_argument_value(memory, tmp_3, &value_2);
                st = st ? st : (value_2 == 0 ? DIVISION_BY_ZERO : OK);
                st = st ? st : change_variable_value(&memory, tmp_1, value_1 % value_2);
                break;
            }
            case PRINT:
            {
                if (amount_of_arguments == 1)
                {
                    st = st ? st : print_all_variables(memory);
                }
                else
                {
                    st = st ? st : print_variable(memory, tmp_2);
                }
                break;
            }
            default:
                break;
        }
        free_all(3, tmp_1, tmp_2, tmp_3);
    }
    fclose(input);
    destruct_memory(&memory);
    if (st)
    {
        errors(st);
        return st;
    }
    return st;
}
