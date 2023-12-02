#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef long long int ll;
typedef unsigned long long int ull;

typedef enum
{
    OK,
    BAD_ALLOC,
    NULL_POINTER,
    INVALID_INPUT,
    NOT_OK,
    OPENING_ERROR,
    DIVISION_BY_ZERO,
    END_OF_FILE
} Status;

typedef enum
{
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    EVAL,
    DIFF,
    CMPS,
    ERROR
} Command_status;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case NULL_POINTER:
            printf("NULL pointer error\n");
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case NOT_OK:
            printf("Not OK\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        case DIVISION_BY_ZERO:
            printf("Divizion by zero error\n");
            return;
        case END_OF_FILE:
            printf("That's it! Have a nice day (.)(.)\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

typedef struct list_element
{
    ull power;
    ll coefficient;
    struct list_element* next;
} list_element;

typedef struct
{
    list_element* head;
    list_element* tail;
    ull size;
} List;

typedef struct
{
    list_element* current;
} iterator;

Status my_realloc(void** ptr, ull size)
{
    if (ptr == NULL || size == 0)
    {
        return INVALID_INPUT;
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

void next_iterator(iterator* tmp)
{
    if (tmp == NULL)
    {
        return;
    }
    tmp->current = tmp->current->next;
}


int is_equal(const iterator a, const iterator b)
{
    return a.current == b.current;
}

Status null_list(List* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

Status create_list(List* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list_element* terminator = (list_element*)malloc(sizeof(list_element));
    if (terminator == NULL)
    {
        return BAD_ALLOC;
    }
    terminator->power = 0;
    terminator->coefficient = 0;
    terminator->next = terminator;
    list->head = terminator;
    list->tail = terminator;
    list->size = 0;
    return OK;
}

Status destruct_list(List* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    if (list->head == NULL || list->tail == NULL)
    {
        return list->head == list->tail ? OK : BAD_ALLOC;
    }
    list_element* item = list->head;
    list_element* next_item = item->next;
    while (item != next_item)
    {
        free(item);
        item = next_item;
        next_item = item->next;
    }
    free(item);
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}

iterator head(List list)
{
    return (iterator) {list.head};
}

iterator tail(List list)
{
    return (iterator){list.tail};
}

Status insert_item(List* list, iterator i, ull power, ll coefficient)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list_element* new = (list_element*)malloc(sizeof(list_element));
    if (new == NULL)
    {
        return BAD_ALLOC;
    }
    new->power = power;
    new->coefficient = coefficient;
    if (is_equal(i, head(*list)))
    {
        new->next = list->head;
        list->head = new;
        list->size++;
        return OK;
    }
    list_element* prev = list->head;
    list_element* current = i.current;
    while (prev->next != current)
    {
        prev = prev->next;
        if (prev == list->tail)
        {
            return INVALID_INPUT;
        }
    }
    prev->next = new;
    new->next = current;
    list->size++;
    return OK;
}

Status delete_item(List* list, iterator i)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list_element* tmp = i.current;
    if (list->head == tmp)
    {
        list->head = tmp->next;
    }
    else
    {
        list_element* prev = list->head;
        while (prev->next != tmp)
        {
            prev = prev->next;
            if (prev->next == list->tail)
            {
                return INVALID_INPUT;
            }
        }
        prev->next = tmp->next;
    }
    free(tmp);
    list->size--;
    return OK;
}

void print_list(List list)
{
    list_element* item = list.head;
    if (list.size == 0)
    {
        printf("0\n");
        return;
    }
    if (list.size == 1 && list.head->coefficient == 0)
    {
        printf("0\n");
    }
    while (item != list.tail)
    {
        if (item->coefficient < 0)
        {
            printf("-");
        }
        else if (item != list.head)
        {
            printf("+");
        }
        if ((item->coefficient != 1 && item->coefficient != -1 && item->coefficient != 0) || item->power == 0)
        {
            printf("%lld", item->coefficient < 0 ? -item->coefficient : item->coefficient);
        }
        if (item->power != 0 && item->coefficient != 0)
        {
            printf("x^%llu", item->power);
        }
        item = item->next;
    }
    printf("\n");
}

Status copy_list(const List a, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    Status st = create_list(res);
    if (st)
    {
        return st;
    }
    list_element* current = a.head;
    while (current != current->next && !st)
    {
        st = insert_item(res, tail(*res), current->power, current->coefficient);
        current = current->next;
    }
    if (st)
    {
        destruct_list(res);
        return st;
    }
    return OK;

}

Status get_expression(FILE* file, char** str1, char** str2, char** status)
{
    if (file == NULL)
    {
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return INVALID_INPUT;
    }
    ull cnt = 0;
    ull counter_1 = 0;
    ull counter_2 = 0;
    ull size_1 = 8;
    ull size_2 = 8;
    *str1 = (char*)malloc(sizeof(char) * size_1);
    if (*str1 == NULL)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return BAD_ALLOC;
    }
    *str2 = (char*)malloc(sizeof(char) * size_2);
    if (*str2 == NULL)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return BAD_ALLOC;
    }
    *status = (char*)malloc(sizeof(char) * 5);
    if (*status == NULL)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return BAD_ALLOC;
    }
    char ch = getc(file);
    int running = 1;
    while (running)
    {
        while (ch == ' ' || ch == '\t' || ch == '\n' || ch == ';')
        {
            ch = getc(file);
        }
        running = 0;
        if (ch == '%')
        {
            while (ch != '\n' && ch != EOF)
            {
                ch = getc(file);
            }
            ch = getc(file);
            running = 1;
            if (ch == EOF)
            {
                running = 0;
            }
        }
        else if (ch == '[')
        {
            while (ch != ']' && ch != EOF)
            {
                ch = getc(file);
            }
            if (ch != ']')
            {
                free_all(3, *str1, *str2, *status);
                *str1 = NULL;
                *str2 = NULL;
                *status = NULL;
                return INVALID_INPUT;
            }
            ch = getc(file);
            running = 1;
        }
    }
    while (ch != '(' && ch != EOF && ch != '\n')
    {
        (*status)[cnt++] = ch;
        ch = getc(file);
    }
    if (ch != '(' && ch != EOF)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return INVALID_INPUT;
    }
    ch = getc(file);
    while (ch != ',' && ch != ';' && ch != EOF && ch != ')' && ch != '\0')
    {
        if (counter_1 == size_1 - 2)
        {
            size_1 *= 2;
            Status st = my_realloc(str1, sizeof(char) * size_1);
            if (st != OK)
            {
                free_all(3, *str1, *str2, *status);
                *str1 = NULL;
                *str2 = NULL;
                *status = NULL;
                return BAD_ALLOC;
            }
        }
        (*str1)[counter_1++] = ch;
        ch = getc(file);
    }
    if (ch != ')' && ch != EOF)
    {
        ch = getc(file);
    }
    while (ch != ';' && ch != EOF && ch != ')' && ch != '\0')
    {
        if (counter_2 == size_2 - 2)
        {
            size_2 *= 2;
            Status st = my_realloc(str2, sizeof(char) * size_2);
            if (st != OK)
            {
                free_all(3, *str1, *str2, *status);
                *str1 = NULL;
                *str2 = NULL;
                *status = NULL;
                return BAD_ALLOC;
            }
        }
        (*str2)[counter_2++] = ch;
        ch = getc(file);
    }
    if (ch != ')' && ch != EOF)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return INVALID_INPUT;
    }
    if (counter_1 == 0 && ch != EOF)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return INVALID_INPUT;
    }
    if (ch == EOF)
    {
        free_all(3, *str1, *str2, *status);
        *str1 = NULL;
        *str2 = NULL;
        *status = NULL;
        return END_OF_FILE;
    }
    (*str1)[counter_1] = '\0';
    (*status)[cnt] = '\0';
    if (counter_2 == 0)
    {
        free(*str2);
        *str2 = NULL;
        return OK;
    }
    (*str2)[counter_2] = '\0';
    return OK;
}

Status get_command(const char* command, Command_status* status, int* cnt, const char* poly_1, const char* poly_2, int* is_full)
{
    if (command == NULL)
    {
        return INVALID_INPUT;
    }

    int args_cnt = 0;

    if ((poly_1 == NULL && poly_2 != NULL) || (poly_1 != NULL && poly_2 == NULL))
    {
        args_cnt = 1;
    }
    if (poly_1 == NULL && poly_2 == NULL)
    {
        args_cnt = 0;
    }
    else if (poly_1 != NULL && poly_2 != NULL)
    {
        args_cnt = 2;
    }
    Status st = OK;
    if (!strcmp(command, "Add") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = ADD;
    }
    else if (!strcmp(command, "Sub") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = SUB;
    }
    else if (!strcmp(command, "Mult") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = MULT;
    }
    else if (!strcmp(command, "Div") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = DIV;
    }
    else if (!strcmp(command, "Mod") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = MOD;
    }
    else if (!strcmp(command, "Eval") && args_cnt == 1)
    {
        *is_full = 1;
        *status = EVAL;
    }
    else if (!strcmp(command, "Diff") && (args_cnt == 1 || args_cnt == 0))
    {
        *is_full = args_cnt == 1 ? 1 : 0;
        *status = DIFF;
    }
    else if (!strcmp(command, "Cmps") && (args_cnt == 1 || args_cnt == 2))
    {
        *is_full = args_cnt == 2 ? 1 : 0;
        *status = CMPS;
    }
    else
    {
        st = INVALID_INPUT;
        *status = ERROR;
    }
    free(command);
    command = NULL;
    *cnt = args_cnt;
    return st;
}

Status get_polynomial(List* polynomial, char* poly)
{
    if (polynomial == NULL || poly == NULL)
    {
        return INVALID_INPUT;
    }
    null_list(polynomial);
    Status st = create_list(polynomial);
    const char* ptr = poly;
    ull size = strlen(poly);
    int flag = 0;
    ull power = 0;
    ll coefficient = 0;
    while (!st && (*ptr != '\0' || flag == 3))
    {
        char* string = (char*)malloc(sizeof(char) * size);
        if (string == NULL)
        {
            st = BAD_ALLOC;
        }
        else if (flag == 0)
        {
            int sign = *ptr == '-' ? -1 : 1;
            char* checker = NULL;
            if (*ptr == '-' || *ptr == '+')
            {
                ++ptr;
            }
            int idx = 0;
            while (*ptr != '+' && *ptr != '-' && *ptr != '\0' && *ptr != 'x')
            {
                if (!isdigit(*ptr))
                {
                    st = INVALID_INPUT;
                }
                string[idx] = *ptr;
                idx++;
                ++ptr;
            }
            if (string[0] == '\0' && !st)
            {
                coefficient = sign;
            }
            else if (!st)
            {
                coefficient = strtoll(string, &checker, 10);
                coefficient *= sign;
            }
            if (*ptr == 'x' && !st)
            {
                flag = 1;
            }
            else if (!st)
            {
                power = 0;
                flag = 3;
            }

        }
        else if (flag == 1)
        {
            ptr++;
            if (*ptr == '+' || *ptr == '-' || *ptr == '\0')
            {
                power = 1;
                flag = 3;
            }
            else if (*ptr == '^')
            {
                flag = 2;
            }
            else
            {
                st = INVALID_INPUT;
            }
        }
        else if (flag == 2)
        {
            ptr++;
            char* checker = NULL;
            int idx = 0;
            while (*ptr != '+' && *ptr != '-' && *ptr != ')' && !st && isdigit(*ptr))
            {
                string[idx] = *ptr;
                idx++;
                ++ptr;
            }
            if (!isdigit(*ptr) && *ptr == '+' && *ptr == '-' && *ptr == ')')
            {
                st = INVALID_INPUT;
            }
            power = strtoull(string, &checker, 10);
            flag = 3;
        }
        else if (flag == 3)
        {
            if (coefficient != 0)
            {
                iterator current = head(*polynomial);
                iterator last = tail(*polynomial);
                while (current.current->power > power)
                {
                    next_iterator(&current);
                }
                if (current.current->power == power && !is_equal(current, last))
                {
                    current.current->coefficient += coefficient;
                }
                else
                {
                    st = insert_item(polynomial, current, power, coefficient);
                }
            }
            flag = 0;
        }
        free(string);
    }
    if (st)
    {
        destruct_list(polynomial);
    }
    return st;
}


Status add(List a, List b, List* res)
{
    Status st = OK;
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    List tmp;
    null_list(&tmp);
    st =  create_list(&tmp);
    iterator a_curr = head(a);
    iterator a_last = tail(a);
    iterator b_curr = head(b);
    iterator b_last = tail(b);
    int end_of_a = is_equal(a_curr, a_last);
    int end_of_b = is_equal(b_curr, b_last);
    while ((!end_of_a || !end_of_b) && !st)
    {
        if (!end_of_a && !end_of_b && a_curr.current->power == b_curr.current->power)
        {
            ull power = a_curr.current->power;
            ll coefficient = a_curr.current->coefficient + b_curr.current->coefficient;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
            next_iterator(&a_curr);
            next_iterator(&b_curr);
        }
        else if (end_of_b || (!end_of_a && a_curr.current->power > b_curr.current->power))
        {
            ull power = a_curr.current->power;
            ll coefficient = a_curr.current->coefficient;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
            next_iterator(&a_curr);
        }
        else
        {
            ull power = b_curr.current->power;
            ll coefficient = b_curr.current->coefficient;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
            next_iterator(&b_curr);
        }
        end_of_a = is_equal(a_curr, a_last);
        end_of_b = is_equal(b_curr, b_last);
    }
    if (st)
    {
        destruct_list(&tmp);
        return st;
    }
    destruct_list(res);
    *res = tmp;
    return OK;
}

Status sub(List a, List b, List* res)
{
    Status st = OK;
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    List tmp;
    null_list(&tmp);
    st =  create_list(&tmp);
    iterator a_curr = head(a);
    iterator a_last = tail(a);
    iterator b_curr = head(b);
    iterator b_last = tail(b);
    int end_of_a = is_equal(a_curr, a_last);
    int end_of_b = is_equal(b_curr, b_last);
    while ((!end_of_a || !end_of_b) && !st)
    {
        if (!end_of_a && !end_of_b && a_curr.current->power == b_curr.current->power)
        {
            ull power = a_curr.current->power;
            ll coefficient = a_curr.current->coefficient - b_curr.current->coefficient;
            if (coefficient != 0)
            {
                st = insert_item(&tmp, tail(tmp), power, coefficient);
            }
            next_iterator(&a_curr);
            next_iterator(&b_curr);
        }
        else if (end_of_b || (!end_of_a && a_curr.current->power > b_curr.current->power))
        {
            ull power = a_curr.current->power;
            ll coefficient = a_curr.current->coefficient;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
            next_iterator(&a_curr);
        }
        else
        {
            ull power = b_curr.current->power;
            ll coefficient = -b_curr.current->coefficient;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
            next_iterator(&b_curr);
        }
        end_of_a = is_equal(a_curr, a_last);
        end_of_b = is_equal(b_curr, b_last);
    }
    if (st)
    {
        destruct_list(&tmp);
        return st;
    }
    destruct_list(res);
    *res = tmp;
    return OK;
}

Status mult(List a, List b, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    List tmp;
    null_list(&tmp);
    Status st = create_list(&tmp);
    iterator curr = head(a);
    iterator last = tail(a);
    while (!st && !is_equal(curr, last))
    {
        List helper;
        null_list(&helper);
        st = copy_list(b, &helper);
        iterator curr_2 = head(helper);
        iterator last_2 = tail(helper);
        while (!st && !is_equal(curr_2, last_2))
        {
            curr_2.current->power += curr.current->power;
            curr_2.current->coefficient *= curr.current->coefficient;
            next_iterator(&curr_2);
        }
        st = st ? st : add(tmp, helper, &tmp);
        destruct_list(&helper);
        next_iterator(&curr);
    }
    if (st)
    {
        destruct_list(&tmp);
    }
    destruct_list(res);
    *res = tmp;
    return OK;
}

Status Div(List a, List b, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    if (b.size == 0)
    {
        return DIVISION_BY_ZERO;
    }
    List result, temp;
    null_list(&result);
    null_list(&temp);
    Status st = create_list(&result);
    if (!st)
    {
        st = copy_list(a, &temp);
    }
    ull current_power = head(temp).current->power;
    ll current_coefficient = head(temp).current->coefficient;
    ull division_power = head(b).current->power;
    ll division_coefficient = head(b).current->coefficient;
    int running = 1;
    while (!st && running)
    {
        List temp_2;
        null_list(&temp_2);
        st = copy_list(b, &temp_2);
        ull result_power = current_power - division_power;
        ll result_coefficient = current_coefficient / division_coefficient;
        st = st ? st : insert_item(&result, tail(result),result_power, result_coefficient);
        iterator current = head(temp_2);
        iterator last = tail(temp_2);
        while (!st && !is_equal(current, last))
        {
            current.current->power += result_power;
            current.current->coefficient *= result_coefficient;
            next_iterator(&current);
        }
        List temp_3;
        null_list(&temp_3);
        if (!st)
        {
            st = create_list(&temp_3);
        }
        if (!st)
        {
            st = sub(temp, temp_2, &temp_3);
        }
        destruct_list(&temp);
        destruct_list(&temp_2);
        temp = temp_3;
        if (!st)
        {
            current = head(temp);
            last = tail(temp);
            while (current.current->power >= division_power && current.current->coefficient < division_coefficient && !is_equal(current, last))
            {
                next_iterator(&current);
            }
            if (current.current->power < division_power || is_equal(current, last))
            {
                running = 0;
            }
            else
            {
                current_power = current.current->power;
                current_coefficient = current.current->coefficient;
            }
        }
    }
    destruct_list(&temp);
    if (st)
    {
        destruct_list(&result);
    }
    destruct_list(res);
    *res = result;
    return OK;
}
Status mod(List a, List b, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    if (b.size == 0)
    {
        return DIVISION_BY_ZERO;
    }
    List tmp;
    null_list(&tmp);
    Status st = copy_list(a, &tmp);

    ull current_power = head(tmp).current->power;
    ll current_coefficient = head(tmp).current->coefficient;
    ull division_power = head(b).current->power;
    ll division_coefficient = head(b).current->coefficient;
    int running = 1;

    while (!st && running)
    {
        List temp_2;
        null_list(&temp_2);
        st = copy_list(b, &temp_2);
        ull result_power = current_power - division_power;
        ll result_coefficient = current_coefficient / division_coefficient;

        iterator current = head(temp_2);
        iterator last = tail(temp_2);
        while (!st && !is_equal(current, last))
        {
            current.current->power += result_power;
            current.current->coefficient *= result_coefficient;
            next_iterator(&current);
        }
        List temp_3;
        null_list(&temp_3);
        if (!st)
        {
            st = create_list(&temp_3);
        }
        if (!st)
        {
            st = sub(tmp, temp_2, &temp_3);
        }
        destruct_list(&tmp);
        destruct_list(&temp_2);
        tmp = temp_3;
        if (!st)
        {
            current = head(tmp);
            last = tail(tmp);
            while (current.current->power >= division_power && current.current->coefficient < division_coefficient && !is_equal(current, last))
            {
                next_iterator(&current);
            }
            if (current.current->power < division_power || is_equal(current, last))
            {
                running = 0;
            }
            else
            {
                current_power = current.current->power;
                current_coefficient = current.current->coefficient;
            }
        }
    }
    if (st)
    {
        destruct_list(&tmp);
    }
    destruct_list(res);
    *res = tmp;
    return OK;
}

Status eval(List a, ll point, ll* result)
{
    ull prev_power = 0;
    iterator current = head(a);
    iterator last = tail(a);
    while (!is_equal(current, last))
    {
        for (ull i = current.current->power; i < prev_power; ++i)
        {
            *result *= point;
        }
        *result += current.current->coefficient;
        prev_power = current.current->power;
        next_iterator(&current);
    }
    for (ull i = current.current->power; i < prev_power; ++i)
    {
        *result *= point;
    }
    return OK;
}

Status diff(List a, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    List tmp;
    null_list(&tmp);
    Status st = create_list(&tmp);
    iterator current = head(a);
    iterator last = tail(a);
    while (!st && !is_equal(current, last))
    {
        if (current.current->power > 0)
        {
            ull power = current.current->power-1;
            ll coefficient = current.current->coefficient * current.current->power;
            st = insert_item(&tmp, tail(tmp), power, coefficient);
        }
        next_iterator(&current);
    }
    if (st)
    {
        destruct_list(&tmp);
        return st;
    }
    destruct_list(res);
    *res = tmp;
    return OK;
}

Status cmps(List a, List b, List* res)
{
    if (res == NULL)
    {
        return INVALID_INPUT;
    }
    List tmp;
    null_list(&tmp);
    Status st = create_list(&tmp);
    ull prev_power = 0;
    iterator current = head(a);
    iterator last = tail(a);
    while (!st && !is_equal(current, last))
    {
        for (ull i = current.current->power; i < prev_power; ++i)
        {
            List help;
            null_list(&help);
            if (!st)
            {
                st = mult(tmp, b, &help);
            }
            destruct_list(&tmp);
            tmp = help;
        }
        List help, help_2;
        null_list(&help);
        null_list(&help_2);
        if (!st)
        {
            st = create_list(&help);
        }
        if (!st)
        {
            st = create_list(&help_2);
        }
        if (!st)
        {
            st = insert_item(&help_2, tail(help_2), 0, current.current->coefficient);
        }
        if (!st)
        {
            st = add(tmp, help_2, &help);
        }
        destruct_list(&help_2);
        destruct_list(&tmp);
        tmp = help;
        prev_power = current.current->power;
        next_iterator(&current);
    }
    for (ull i = 0; i < prev_power; ++i)
    {
        List help;
        null_list(&help);
        if (!st)
        {
            st = mult(tmp, b, &help);
        }
        destruct_list(&tmp);
        tmp = help;
    }
    if (st)
    {
        destruct_list(&tmp);
        return st;
    }
    destruct_list(res);
    *res = tmp;
    return OK;

}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        return OPENING_ERROR;
    }
    List summator;
    null_list(&summator);
    Status st = create_list(&summator);
    while (!st && !feof(input)) {
        Command_status current_command;
        char *curr_command = NULL;
        char *curr_poly_1 = NULL;
        char *curr_poly_2 = NULL;

        int full_args_command = 1;
        ll x = 0;
        ll res = 0;
        List list_1, list_2;
        int args_cnt = 0;

        st = get_expression(input, &curr_poly_1, &curr_poly_2, &curr_command);
        st = st ? st : get_command(curr_command, &current_command,
                   &args_cnt, curr_poly_1, curr_poly_2,&full_args_command);
        null_list(&list_1);
        null_list(&list_2);
        if (st != END_OF_FILE) {
            switch (current_command) {
                case ADD:
                case SUB:
                case MULT:
                case DIV:
                case MOD:
                case CMPS: {
                    if (full_args_command && !st) {
                        st = get_polynomial(&list_1, curr_poly_1);
                        st = st ? st : get_polynomial(&list_2, curr_poly_2);
                    } else if (!full_args_command && !st) {
                        st = get_polynomial(&list_1, curr_poly_1);
                        st = st ? st : copy_list(summator, &list_2);
                    }
                    break;
                }
                case EVAL: {
                    if (full_args_command && !st) {
                        char *check_ptr;
                        x = strtoll(curr_poly_1, &check_ptr, 10);
                        st = st ? st : (*check_ptr == '\0' ? OK : INVALID_INPUT);
                    }
                    break;
                }
                case DIFF: {
                    if (full_args_command && !st) {
                        st = st ? st : get_polynomial(&list_1, curr_poly_1);
                    } else if (!full_args_command && !st) {
                        full_args_command = 0;
                        list_1 = summator;
                    }
                    break;
                }
                default:
                    st = INVALID_INPUT;
                    break;
            }
        }
        free(curr_poly_1);
        free(curr_poly_2);
        switch (current_command)
        {
            case ADD:
                st = st ? st : add(list_1, list_2, &summator);
                break;
            case SUB:
                st = st ? st : sub(list_1, list_2, &summator);
                break;
            case MULT:
                st = st ? st : mult(list_1, list_2, &summator);
                break;
            case DIV:
                st = st ? st : Div(list_1, list_2, &summator);
                break;
            case MOD:
                st = st ? st : mod(list_1, list_2, &summator);
                break;
            case EVAL:
                st = st ? st : eval(summator, x, &res);
                break;
            case DIFF:
                st = st ? st : diff(list_1, &summator);
                break;
            case CMPS:
                st = st ? st : cmps(list_1, list_2, &summator);
                break;
            default:
                st = st;
                break;
        }
        if (!st)
        {
            if (current_command == EVAL)
            {
                printf("The value at point x = %lld is %lld\n", x, res);
            }
            else
            {
                print_list(summator);
            }
        }
        destruct_list(&list_1);
        destruct_list(&list_2);
    }
    fclose(input);
    destruct_list(&summator);
    if (st)
    {
        errors(st);
        return st;
    }
    return 0;
}