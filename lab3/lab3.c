#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    OK,
    NOT_OK,
    BAD_ALLOC,
    OPENING_ERROR,
    INVALID_INPUT
} Status;

typedef struct Employee
{
    unsigned id;
    char name[256];
    char surname[256];
    double salary;
} Employee;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case NOT_OK:
            printf("Not OK\n");
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}
Status my_realloc(void** ptr, int size)
{
    void* new_ptr = realloc(*ptr, size);
    if (new_ptr != NULL)
    {
        *ptr = new_ptr;
        return OK;
    }
    return BAD_ALLOC;
}
void print_vector(Employee member)
{
    printf("%u %s %s %lf\n", member.id, member.name, member.surname, member.salary);
}

Status input_checker(int argc, char* argv[])
{
    if ((argc != 4) || ((*argv[2] != '-') && (*argv[2] != '/')))
    {
        return INVALID_INPUT;
    }
    if (argv[2][1] != 'a' && argv[2][1] != 'd')
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status read_from_file(FILE* file, Employee** employees, int* capacity)
{
    *capacity = 16;
    (*employees) = (Employee*)malloc(sizeof(Employee) * (*capacity));
    if ((*employees) == NULL)
    {
        return BAD_ALLOC;
    }
    int idx = 0;
    char* string = NULL;
    bool start = true;
    Status mem;
    int cont;
    while ((cont = getline(&string, &(size_t){0}, file)) != -1)
    {
        if (cont == -1)
        {
            free(string);
            return BAD_ALLOC;
        }
        Employee tmp;
        if (!sscanf(string, "%u %s %s %lf", &(tmp.id), tmp.name, tmp.surname, &(tmp.salary)))
        {
            free(string);
            return BAD_ALLOC;
        }
        (*employees)[idx] = tmp;
        idx++;

        if (*capacity - 1 == idx) {
            *capacity *= 2;
            mem = my_realloc(employees, *capacity);
            if (mem == BAD_ALLOC) {
                free(string);
                free(*employees);
            }
        }
        free(string);
        string = NULL;
        start = true;
    }
    free(string);
    *capacity = idx;
    return OK;
}

int comparator(const void* a, const void* b)
{

    double eps = 1e-12;
    double salary_difference = ((Employee*)a)->salary - ((Employee*)b)->salary;
    if (salary_difference > eps)
    {
        return 1;
    }
    else if (salary_difference < eps)
    {
        return -1;
    }
    int surname_difference = strcmp(((Employee*)a)->surname, ((Employee*)b)->surname);
    if (!surname_difference)
    {
        return surname_difference;
    }
    int name_difference = strcmp(((Employee*)a)->name, ((Employee*)b)->name);
    if (!name_difference)
    {
        return name_difference;
    }
    return (((Employee*)a)->id - ((Employee*)b)->id);
}

Status write_to_file(FILE* out, Employee* employees, int size, Status order)
{
    if (order == OK)
    {
        for (int i = 0; i < size; ++i)
        {
            Employee temp = employees[i];
            fprintf(out, "%d %s %s %lf", temp.id, temp.name, temp.surname, temp.salary);
            if (i != size - 1)
            {
                fprintf(out, "\n");
            }
        }
    }
    else
    {
        for (int i = size - 1; i >= 0; --i)
        {
            Employee temp = employees[i];
            fprintf(out, "%d %s %s %lf", temp.id, temp.name, temp.surname, temp.salary);
            if (i != 0)
            {
                fprintf(out, "\n");
            }
        }
    }
    return OK;
}

int main(int argc, char* argv[])
{
    Status status = input_checker(argc, argv);
    char* flag = argv[2];
    if (status != OK)
    {
        errors(status);
        return status;
    }
    char* file_in_name = argv[1];
    char* file_out_name = argv[3];
    FILE* file_in;
    FILE* file_out;
    if ((file_in = fopen(file_in_name, "r")) == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    if ((file_out = fopen(file_out_name, "w")) == NULL)
    {
        fclose(file_in);
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }

    Employee* employees = NULL;

    int capacity = 0;
    Status read_status = read_from_file(file_in, &employees, &capacity);
    if (read_status != OK)
    {
        errors(read_status);
        free(employees);
        fclose(file_in);
        fclose(file_out);
        return read_status;
    }

    Status sort;
    if (flag[1] == 'a')
    {
        sort = OK;
    }
    else
    {
        sort = NOT_OK;
    }

    qsort(employees, capacity, sizeof(Employee), comparator);
    write_to_file(file_out, employees, capacity, sort);
    fclose(file_in);
    fclose(file_out);
    free(employees);

    return 0;
}
