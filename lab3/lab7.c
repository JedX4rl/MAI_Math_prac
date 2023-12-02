#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NAME_SIZE 16
#define EPS 1e-12

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
    END_OF_FILE
} Status;

typedef enum
{
    SURNAME,
    NAME,
    MIDDLE_NAME,
    BDAY,
    GENDER,
    SALARY,
    DELETE
} Commands;

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
        case END_OF_FILE:
            printf("That's it! Have a nice day (.)(.)\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

typedef struct Liver
{
    int flag;
    char name[NAME_SIZE];
    char surname[NAME_SIZE];
    char middle_name[NAME_SIZE];
    char birthday[NAME_SIZE];
    char gender[2];
    double salary;
    struct Liver* next;
} Liver;

typedef struct
{
    Liver * head;
    Liver * tail;
    ull size;
} List;

typedef struct modification
{
    struct modification* prev;
    Liver* liver;
    char info[NAME_SIZE];
    double salary;
    Commands flag;
    struct modification* next;
}modification;

typedef struct modifications_list
{
    ull amt_of_modifications;
    ull size;
    char info[NAME_SIZE];
    struct modification* head;
    struct modification* tail;
} modifications_list;

Status create_modifications_list(modifications_list* list)
{
    if (list == NULL)
    {
        return INVALID_INPUT;
    }
    list->amt_of_modifications = 0;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return OK;
}


Status pop_back(modifications_list* tmp)
{
    if (tmp == NULL)
    {
        tmp->size = 0;
        tmp->amt_of_modifications = 0;
        return OK;
    }
    modification* current = tmp->head;
    if (tmp->size)
    {
        tmp->head = current->next;
        free(current);
        tmp->head->prev = NULL;
        tmp->amt_of_modifications--;
    }
    return OK;
}


Status push_front(modifications_list* tmp, Liver* liver, Commands flag, const char* info, double salary)
{
    if (tmp == NULL)
    {
        return INVALID_INPUT;
    }
    modification* new = (modification*)malloc(sizeof(modification));
    if (new == NULL)
    {
        return BAD_ALLOC;
    }
    new->flag = flag;
    new->liver = liver;
    if (flag == SALARY)
    {
        new->salary = salary;
    }
    else if (flag != DELETE)
    {
        strcpy(new->info, info);
    }
    if (tmp->tail != NULL)
    {
        tmp->tail->next = new;
        new->prev = tmp->tail;
    }
    else if (tmp->head == NULL)
    {
        tmp->head = new;
        new->prev = NULL;
    }
    tmp->tail = new;
    new->next = NULL;
    tmp->amt_of_modifications++;
    tmp->size++;
    return OK;
}

Status pop_front(modifications_list* tmp)
{
    modification* last = tmp->tail;
    if (tmp->amt_of_modifications == 0)
    {
        return OK;
    }
    if (tmp->amt_of_modifications == 1)
    {
        tmp->head = tmp->tail = NULL;
        tmp->amt_of_modifications = 0;
        return OK;
    }
    tmp->tail = last->prev;
    last->prev->next = NULL;
    tmp->amt_of_modifications--;
    return OK;
}

Status destruct_modifications_list(modifications_list* tmp)
{
    if (tmp->head == NULL)
    {
        return OK;
    }
    if (tmp->head == tmp->tail)
    {
        free(tmp->tail);
        tmp->head = tmp->tail = NULL;
    }
    else
    {
        modification* current = tmp->head;
        while (current != NULL)
        {
            tmp->head = current->next;
            free(current);
            current = current->next;
        }
    }
    tmp->amt_of_modifications = 0;
    return OK;
}

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

Status get_str(char** str)
{
    int max_size = 20;
    int temp_size = 0;
    *str = (char*)malloc(sizeof(char) * max_size);
    if (*str == NULL)
    {
        return BAD_ALLOC;
    }
    char c;
    while ((c = getchar()) != '\n')
    {
        if (temp_size + 1 == max_size)
        {
            max_size *= 2;
            if (my_realloc(str, max_size + 1) != OK)
            {
                free(*str);
                return BAD_ALLOC;
            }
        }
        (*str)[temp_size] = c;
        temp_size++;
    }
    (*str)[temp_size] = '\0';
    return OK;
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

    list->head = list->tail;
    list->tail = list->head;
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
    Liver* item = list->head;
    Liver* next_item = item->next;
    while (item->next != NULL)
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

int compare_age(const char* a, const char* b)
{
    if (a[6] > b[6])
    {
        return 1;
    }
    else if (a[6] < b[6])
    {
        return 0;
    }

    int a_age = 0;
    int b_age = 0;

    for (int i = 6; i < 10; ++i)
    {
        a_age = a_age * 10 + a[i] - '0';
        b_age = b_age * 10 + b[i] - '0';
    }

    if (a_age > b_age)
    {
        return 1;
    }
    else if (b_age < a_age)
    {
        return 0;
    }

    a_age = 0;
    b_age = 0;

    for (int i = 3; i < 5; ++i)
    {
        a_age = a_age * 10 + a[i] - '0';
        b_age = b_age * 10 + b[i] - '0';
    }

    if (a_age > b_age)
    {
        return 1;
    }
    else if (b_age < a_age)
    {
        return 0;
    }

    a_age = 0;
    b_age = 0;

    for (int i = 0; i < 2; ++i)
    {
        a_age = a_age * 10 + a[i] - '0';
        b_age = b_age * 10 + b[i] - '0';
    }

    if (a_age > b_age)
    {
        return 1;
    }
    else if (b_age < a_age)
    {
        return 0;
    }
    return 0;
}

Status push_back_list(List* livers, Liver* liver)
{
    if (livers == NULL || liver == NULL)
    {
        return INVALID_INPUT;
    }
    if (livers->tail != NULL)
    {
        livers->tail->next = liver;
    }
    if (livers->head == NULL)
    {
        livers->head = liver;
    }
    livers->tail = liver;
    liver->next = NULL;
    livers->size++;
    return OK;
}

Status push_front_list(List* livers, Liver* liver)
{
    if (livers == NULL || liver == NULL)
    {
        return INVALID_INPUT;
    }
    if (livers->head != NULL)
    {
        liver->next = livers->head;
    }
    if (livers->tail == NULL)
    {
        livers->tail = liver;
    }
    livers->head = liver;
    livers->size++;
    return OK;
}

Status add_liver(List* livers, Liver* liver)
{
    if (livers == NULL || liver == NULL)
    {
        return INVALID_INPUT;
    }
    liver->flag = 1;
    if (livers->size == 0)
    {
        push_back_list(livers, liver);
        return OK;
    }
    if (compare_age(livers->head->birthday, liver->birthday))
    {
        push_front_list(livers, liver);
        return OK;
    }
    Liver* current = livers->head;
    while (current->next != NULL)
    {
        if (compare_age(current->next->birthday, liver->birthday))
        {
            break;
        }
        current = current->next;
    }
    if (current->next == NULL)
    {
        push_back_list(livers, liver);
        return OK;
    }
    liver->next = current->next;
    current->next = liver;
    return OK;
}

void print_list(List list)
{
    Liver* item = list.head;
    if (list.size == 0)
    {
        printf("List is empty\n");
        return;
    }
    while (item != NULL)
    {
        if (item->flag)
        {
            if (strcmp(item->middle_name, "NONE"))
            {
                printf("%s %s %s %s %s %lf\n", item->name, item->surname, item->middle_name, item->birthday, item->gender, item->salary);
            }
            else
            {
                printf("%s %s %s %s %lf\n", item->name, item->surname, item->birthday, item->gender, item->salary);
            }
        }
        item = item->next;
    }
    printf("\n");
}

void print_liver(Liver* item)
{
    if (strcmp(item->middle_name, "NONE") != 0)
    {
        printf("%s %s %s %s %s %lf\n", item->name, item->surname, item->middle_name, item->birthday, item->gender, item->salary);
    }
    else
    {
        printf("%s %s %s %s %lf\n", item->name, item->surname, item->birthday, item->gender, item->salary);
    }
}

Status without_middle_name(Liver* liver, const char* str)
{
    if (liver == NULL || str == NULL)
    {
        return INVALID_INPUT;
    }
    if (sscanf(str, "%s %s %s %s %lf", liver->surname, liver->name, liver->birthday, liver->gender, &liver->salary) != 5)
    {
        return INVALID_INPUT;
    }
    strcpy(liver->middle_name, "NONE");
    return OK;
}

Status check_person(const char* str, Commands flag)
{
    if (str == NULL || strlen(str) > 14)
    {
        return INVALID_INPUT;
    }
    if (flag != MIDDLE_NAME && *str == '\0')
    {
        return INVALID_INPUT;
    }
    for (int i = 0; str[i]; ++i)
    {
        if (!isalpha(str[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status valid_birthday(const char* str)
{
    if (str == NULL || strlen(str) != 10)
    {
        return INVALID_INPUT;
    }
    if (str[2] != '.' && str[5] != '.')
    {
        return INVALID_INPUT;
    }
    if (!isdigit(str[0]) && !isdigit(str[1]) && !isdigit(str[3]) && !isdigit(str[4]))
    {
        return INVALID_INPUT;
    }
    for (int i = 6; i < 10; ++i)
    {
        if (!isdigit(str[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status get_flag(Commands* status, char ch)
{
    if (ch == 's')
    {
        *status = SALARY;
    }
    else if (ch == 'n')
    {
        *status = NAME;
    }
    else if (ch == 'S')
    {
        *status = SURNAME;
    }
    else if (ch == 'm')
    {
        *status = MIDDLE_NAME;
    }
    else if (ch == 'b')
    {
        *status = BDAY;
    }
    else if (ch == 'g')
    {
        *status = GENDER;
    }
    else if (ch == 'd')
    {
        *status = DELETE;
    }
    else
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status get_liver(List* livers, const char* ptr)
{
    if (livers == NULL || ptr == NULL)
    {
        return INVALID_INPUT;
    }
    Status st = OK;
    Commands flag;
    Liver* current = (Liver*)malloc(sizeof(Liver));
    if (current == NULL)
    {
        st = BAD_ALLOC;
    }
    if (sscanf(ptr, "%s %s %s %s %s %lf", current->surname, current->name, current->middle_name, current->birthday, current->gender, &(current->salary)) != 6)
    {
        st = st ? st: without_middle_name(current, ptr) == INVALID_INPUT ? st : OK;
    }
    if (!st)
    {
        st = check_person(current->surname, SURNAME);
    }
    if (!st)
    {
        st = check_person(current->name, SURNAME);
    }
    if (!st)
    {
        st = check_person(current->middle_name, MIDDLE_NAME);
    }
    if (!st)
    {
        st = valid_birthday(current->birthday);
    }
    if (!st)
    {
        st = add_liver(livers, current);
    }
    if (st)
    {
        free(current);
        return st;
    }
    return OK;
}

Status read_from_file(FILE* file, List* livers)
{
    if (file == NULL || livers == NULL)
    {
        return INVALID_INPUT;
    }
    char* tmp_str = (char*)malloc(sizeof(char) * 128);
    if (tmp_str == NULL)
    {
        return BAD_ALLOC;
    }
    int status = 1;
    size_t length = 0;
    Status st = OK;
    while (((status = getline(&tmp_str, &length, file)) != -1) && !st)
    {
        if (status)
        {
            st = get_liver(livers, tmp_str);
        }
    }
    free(tmp_str);
    return st;
}


Status change_liver(Liver* liver, modifications_list* modifications, const char* info, char* gender, double salary, Commands flag)
{
    if (liver == NULL || modifications == NULL)
    {
        return INVALID_INPUT;
    }
    Status st = OK;
    if (flag == NAME)
    {
        st = push_front(modifications, liver, flag, liver->name, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver %s changed name to %s successfully!\n", liver->surname , info);
            strcpy(liver->name, info);
        }
    }
    if (!st && flag == SURNAME)
    {
        st = push_front(modifications, liver, flag, liver->surname, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver %s changed surname to %s successfully!\n", liver->surname , info);
            strcpy(liver->surname, info);
        }
    }
    else if (!st && flag == MIDDLE_NAME)
    {
        st = push_front(modifications, liver, flag, liver->middle_name, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver %s changed middle name to %s successfully!\n", liver->surname , info);
            strcpy(liver->middle_name, info);
        }
    }
    else if (!st && flag == BDAY)
    {
        st = push_front(modifications, liver, flag, liver->birthday, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver %s changed birthday date to %s successfully!\n", liver->surname , info);
            strcpy(liver->birthday, info);
        }
    }
    else if (!st && flag == GENDER)
    {
        st = push_front(modifications, liver, flag, liver->gender, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver %s changed gender to %s successfully!??????\n", liver->surname , info);
            strcpy(liver->gender, gender);
        }
    }
    else if (!st && flag == SALARY)
    {
        st = push_front(modifications, liver, flag, liver->surname, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("Liver's %s salary changed to %s successfully!\n", liver->surname , info);
            liver->salary = salary;
        }
    }
    else if (!st && flag == DELETE)
    {
        liver->flag = 0;
        st = push_front(modifications, liver, flag, liver->surname, liver->salary);
        if (!st && !(modifications->size & 1))
        {
            st = pop_back(modifications);
        }
        if (!st)
        {
            printf("You deleted liver successfully!\n");
        }
    }
    return OK;
}

Status search_liver(List* livers, Liver* liver, Liver** result)
{
    if (livers == NULL || liver == NULL)
    {
        return INVALID_INPUT;
    }
    Liver* current = livers->head;
    while (current != NULL)
    {
        if (!strcmp(liver->surname, current->surname) && !strcmp(liver->name, current->name) && !strcmp(liver->middle_name, current->middle_name)
            && !strcmp(liver->birthday, current->birthday) && !strcmp(liver->name, current->name) && (fabs(liver->salary-current->salary) < EPS) && current->flag)
        {
            *result = current;
            return OK;
        }
        current = current->next;
    }
    return INVALID_INPUT;
}

Status undo(modifications_list* modifications)
{
    if (modifications == NULL || modifications->amt_of_modifications == 0)
    {
        return INVALID_INPUT;
    }
    modification* current = modifications->tail;

    switch (current->flag)
    {
        case DELETE:
            current->liver->flag = 1;
            break;
        case NAME:
            strcpy(current->liver->name, current->info);
            break;
        case SURNAME:
            strcpy(current->liver->surname, current->info);
            break;
        case MIDDLE_NAME:
            strcpy(current->liver->middle_name, current->info);
            break;
        case BDAY:
            strcpy(current->liver->birthday, current->info);
            break;
        case GENDER:
            strcpy(current->liver->gender, current->info);
            break;
        case SALARY:
            current->liver->salary = current->salary;
            break;
    }
    Status st = pop_front(modifications);
    return st;
}

Status write_to_file(List* livers)
{
    if (livers == NULL)
    {
        return INVALID_INPUT;
    }
    int size = 16;
    int cnt = 0;
    char* file_name = (char*)malloc(sizeof(char) * size);
    if (file_name == NULL)
    {
        return BAD_ALLOC;
    }
    Status st = OK;
    printf("Enter the path to the file\n");
    char c = getchar();
    while (c != '\n')
    {
        if (cnt == size - 1)
        {
            size *= 2;
            st = my_realloc(&file_name, sizeof(char) * size);
            if (st)
            {
                free(file_name);
                return st;
            }
        }
        file_name[cnt++] = c;
        c = getchar();
    }
    FILE* out = fopen(file_name, "w");
    if (out == NULL)
    {
        st = OPENING_ERROR;
    }
    if (!st)
    {
        Liver* current = livers->head;
        while (current != NULL)
        {
            if (current->flag)
            {
                if (strcmp(current->middle_name, "NONE") != 0)
                {
                    fprintf(out, "%s %s %s %s %s %lf\n", current->surname, current->name, current->middle_name, current->birthday, current->gender, current->salary);
                }
                else
                {
                    fprintf(out, "%s %s %s %s %lf\n", current->surname, current->name, current->birthday, current->gender, current->salary);
                }
            }
            current = current->next;
        }
    }
    fclose(out);
    free(file_name);
    return st;
}

Status create_liver(Liver** res)
{
    Status st = OK;
    Liver* needed = (Liver*)malloc(sizeof(Liver));
    if (needed == NULL)
    {
        st = BAD_ALLOC;
    }
    char* tmp;
    printf("Enter name of liver\n");
    st = get_str(&tmp);
    if (!st)
    {
        st = check_person(tmp, NAME);
        if (!st)
        {
            strcpy(needed->name, tmp);
        }
        free(tmp);
    }
    if (!st)
    {
        printf("Enter surname of liver\n");
        st = get_str(&tmp);
        if (!st)
        {
            st = check_person(tmp, SURNAME);
            if (!st)
            {
                strcpy(needed->surname, tmp);
            }
            free(tmp);
        }
    }
    if (!st)
    {
        printf("Enter middle_name of liver, in case of lack of it, enter NONE\n");
        st = get_str(&tmp);
        if (!st)
        {
            st = check_person(tmp, MIDDLE_NAME);
            if (!st)
            {
                strcpy(needed->middle_name, tmp);
            }
            free(tmp);
        }
    }
    if (!st)
    {
        printf("Enter birth date of liver\n");
        st = get_str(&tmp);
        if (!st)
        {
            st = valid_birthday(tmp);
            if (!st)
            {
                strcpy(needed->birthday, tmp);
            }
            free(tmp);
        }
    }
    if (!st)
    {
        printf("Enter gender of liver\n");
        st = get_str(&tmp);
        if (strlen(tmp) != 1)
        {
            st = INVALID_INPUT;
        }
        if (!st)
        {
            strcpy(needed->gender, tmp);
        }
    }
    if (!st)
    {
        printf("Enter salary of liver\n");
        st = get_str(&tmp);
        if (!st)
        {
            needed->salary = strtod(tmp, NULL);
            needed->flag = 1;
        }
    }
    if (st)
    {
        free(needed);
        return st;
    }
    *res = needed;
    return st;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return INVALID_INPUT;
    }
    Status st = OK;
    FILE* input = fopen(argv[1], "r");
    if (input == NULL)
    {
        st = OPENING_ERROR;
        errors(st);
        return st;
    }
    List livers;
    modifications_list modifications;
    if (!st)
    {
        st = create_modifications_list(&modifications);
    }
    st = null_list(&livers);
    if (!st)
    {
        st = create_list(&livers);
    }
    if (!st)
    {
        st = read_from_file(input, &livers);
    }
    int running = 1;
    while (!st && running)
    {
        printf("Available commands: exit, print_all, print_to_file, find_liver, delete_liver, change_name, change_surname, change_middle_name\n"
               "change_birth_date, change_gender, change_salary, undo\n");
        char* command = NULL;
        printf("Enter command: \n");
        st = get_str(&command);
        if (!st)
        {
            if (!strcmp(command, "exit"))
            {
                printf("Bye!\n");
                running = 0;
            }
            else if (!strcmp(command, "print_all"))
            {
                print_list(livers);
            }
            else if (!strcmp(command, "find_liver"))
            {
                Liver* res;
                Liver* needed;
                st = create_liver(&needed);
                if (!st)
                {
                    st = search_liver(&livers, needed, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        print_liver(res);
                    }
                }
                free(needed);
            }
            else if (!st && !strcmp(command, "delete_liver"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        change_liver(res, &modifications, NULL," ", 0, DELETE);
                    }
                    print_list(livers);
                    printf("\n");
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_name"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new name\n");
                        st = get_str(&tmp);
                        if (!st)
                        {
                            st = check_person(tmp, NAME);
                        }
                        if (!st)
                        {
                            change_liver(res, &modifications, tmp," ", 0, NAME);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_surname"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new surname\n");
                        st = get_str(&tmp);
                        if (!st)
                        {
                            st = check_person(tmp, SURNAME);
                        }
                        if (!st)
                        {
                            change_liver(res, &modifications, tmp," ", 0, SURNAME);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_birth_date"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new birth date\n");
                        st = get_str(&tmp);
                        if (!st)
                        {
                            st = valid_birthday(tmp);
                        }
                        if (!st)
                        {
                            change_liver(res, &modifications, tmp," ", 0, BDAY);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_middle_name"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new middle_name\n");
                        st = get_str(&tmp);
                        if (!st)
                        {
                            st = check_person(tmp, MIDDLE_NAME);
                        }
                        if (!st)
                        {
                            change_liver(res, &modifications, tmp," ", 0, MIDDLE_NAME);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_gender"))
            {
                printf("Are you sure?\n");
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new gender\n");
                        st = get_str(&tmp);
                        if (!st && strlen(tmp) != 1)
                        {
                            st = INVALID_INPUT;
                        }
                        if (!st)
                        {
                            change_liver(res, &modifications, tmp,tmp, 0, GENDER);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "change_salary"))
            {
                Liver* current;
                Liver* res;
                st = create_liver(&current);
                if (!st)
                {
                    st = search_liver(&livers, current, &res);
                    if (st != OK)
                    {
                        printf("Liver not found\n");
                    }
                    else
                    {
                        char* tmp;
                        printf("Enter new salary\n");
                        st = get_str(&tmp);
                        if (!st)
                        {
                            double income = strtod(tmp, NULL);
                            change_liver(res, &modifications, tmp,tmp, income, SALARY);
                        }
                        free(tmp);
                    }
                }
                free(current);
            }
            else if (!st && !strcmp(command, "undo"))
            {
                if (modifications.amt_of_modifications > 0)
                {
                    undo(&modifications);
                }
                else
                {
                    printf("The list of modifications is empty!\n");
                }
            }
            else if (!st && !strcmp(command, "print_to_file"))
            {
                st = write_to_file(&livers);
            }
        }
        free(command);
        if (st)
        {
            free(command);
            running = 0;
        }
    }
    if (st)
    {
        errors(st);
    }
    destruct_list(&livers);
    destruct_modifications_list(&modifications);
    fclose(input);
    return st;
}
