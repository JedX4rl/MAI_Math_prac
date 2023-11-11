#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    OK,
    BAD_ALLOC,
    INVALID_INPUT,
    OPENING_ERROR
}Status;

typedef enum
{
    NAME,
    SURNAME,
    GROUP
}Search_status;

void errors(Status status)
{
    switch (status)
    {
        case OK:
            return;
        case BAD_ALLOC:
            printf("Allocation error\n");
            return;
        case INVALID_INPUT:
            printf("Invalid input\n");
            return;
        case OPENING_ERROR:
            printf("File cannot be opened\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

typedef struct Student
{
    unsigned id;
    char name[16];
    char surname[16];
    char group[20];
    unsigned char* grades;
} Student;

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



Status id_is_valid(char* str)
{
    size_t length = strlen(str);
    if (length == 0)
    {
        return INVALID_INPUT;
    }
    for (int i = 0; i < length; ++i)
    {
        if(!isdigit(str[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status is_valid(char* str, int flag)
{
    if (str == NULL)
    {
        return INVALID_INPUT;
    }
    size_t length = strlen(str);
    if (length == 0)
    {
        return INVALID_INPUT;
    }
    if (flag == 0)
    {
        for (int i = 0; i < length; ++i)
        {
            if (!isalpha(str[i]))
            {
                return INVALID_INPUT;
            }
        }
    }
    return OK;
}

int compare_by_id(const void* a, const void* b)
{
    unsigned id_1 = ((Student*)a)->id;
    unsigned id_2 = ((Student*)b)->id;
    if (id_1 == id_2)
    {
        return 0;
    }
    return id_1 < id_2 ? -1 : 1;
}

int compare_by_surname(const void* a, const void* b)
{
    char* surname_1 = ((Student*)a)->surname;
    char* surname_2 = ((Student*)b)->surname;
    return strcmp(surname_1, surname_2);
}

int compare_by_name(const void* a, const void* b)
{
    char* name_1 = ((Student*)a)->name;
    char* name_2 = ((Student*)b)->name;
    return strcmp(name_1, name_2);
}

int compare_by_group(const void* a, const void* b)
{
    char* group_1 = ((Student*)a)->group;
    char* group_2 = ((Student*)b)->group;
    return strcmp(group_1, group_2);
}

Status print_student(Student student)
{
    printf("%u ", student.id);
    printf("%s ", student.name);
    printf("%s ", student.surname);
    printf("%s ", student.group);
    printf("%hhu %hhu %hhu %hhu %hhu\n", student.grades[0], student.grades[1], student.grades[2], student.grades[3], student.grades[4]);
    return OK;
}

Status print_all_students(Student* students, int amount)
{
    for (int i = 0; i < amount; ++i)
    {
        print_student(students[i]);
    }
    return OK;
}

Status search_by_id(Student* students, int amount_of_students, Student* found_stud, const char* tmp)
{
    if (students == NULL || tmp == NULL || found_stud == NULL)
    {
        return INVALID_INPUT;
    }
    Status st = OK;
    char* ending = NULL;
    long id = strtol(tmp, &ending, 10);
    if (*ending != '\0')
    {
        st = INVALID_INPUT;
    }
    if (!st)
    {
        for (int i = 0; i < amount_of_students; ++i)
        {
            if (id == students[i].id)
            {
                *found_stud = students[i];
                break;
            }
        }
    }
    return st;
}

Status search_by_flag(Student* students, int amount_of_students, Student** found_studs, const char* str, Search_status flag, int* res_size)
{
    if (students == NULL || str == NULL || found_studs == NULL)
    {
        return INVALID_INPUT;
    }
    int cnt = 0;
    int max = 2;
    Status st = OK;
    *found_studs = (Student*)malloc(sizeof(*students) * max);
    if (*found_studs == NULL)
    {
        st = BAD_ALLOC;
    }
    if (!st)
    {
        for (int i = 0; i < amount_of_students; ++i)
        {
            if ((flag == NAME && !strcmp(str, students[i].name)) || (flag == SURNAME && !strcmp(str, students[i].surname))
                    || (flag == GROUP && !strcmp(str, students[i].group)))
            {
                if (cnt == max)
                {
                    max *= 2;
                    st = my_realloc(found_studs, max);
                    if (st)
                    {
                        st = BAD_ALLOC;
                        free(*found_studs);
                        break;
                    }
                }
                found_studs[cnt++] = &students[i];
            }
        }
    }
    *res_size = cnt;
    return st;
}


Status read_students(FILE* file, int* amount_of_students, Student** students)
{
    if (file == NULL || students == NULL || amount_of_students == NULL)
    {
        return INVALID_INPUT;
    }
    int cnt = 0;
    int size = 2;
    Student* in_func = (Student*)malloc(sizeof(Student) * 2);
    if (in_func == NULL)
    {
        fclose(file);
        return BAD_ALLOC;
    }
    Status st = OK;
    while(!feof(file) && !st)
    {
        Student current_student;
        current_student.grades = (unsigned char*)malloc(sizeof(unsigned char) * 5);
        if (current_student.grades == NULL)
        {
            free(in_func);
            st = BAD_ALLOC;
        }
        if (!st)
        {
            if (fscanf(file, " %u %15[^ ] %15[^ ] %19[^ ] %hhu %hhu %hhu %hhu %hhu",
                        &(current_student.id), current_student.name, current_student.surname, current_student.group,
                        &(current_student.grades[0]), &(current_student.grades[1]), &(current_student.grades[2]),
                        &(current_student.grades[3]), &(current_student.grades[4])) != 9)
            {
                free(in_func);
                st = INVALID_INPUT;
            }
            st = st ? st : is_valid(current_student.name, 0);
            st = st ? st : is_valid(current_student.surname, 0);
            st = st ? st : is_valid(current_student.group, 1);
        }
        if (!st)
        {
            if (cnt == size)
            {
                size *= 2;
                st = my_realloc(&in_func, sizeof(Student) * size);
                if (st != OK)
                {
                    free(*students);
                    st = BAD_ALLOC;
                }
            }
        }
        if (st)
        {
            free(current_student.grades);
        }
        if (!st)
        {
            in_func[cnt] = current_student;
            cnt++;
        }
    }
    if (st)
    {
        for (int i = 0; i < cnt; ++i)
        {
            free(in_func[i].grades);
        }
        free(*students);
        return st;
    }
    *amount_of_students = cnt;
    *students = in_func;
    return st;
}

Status write_to_file(FILE* file, const char* id, int amount, Student* students)
{
    if (file == NULL || id == NULL)
    {
        return INVALID_INPUT;
    }
    Student temp;
    Status st;
    st = search_by_id(students, amount, &temp, id);
    if (!st)
    {
        double average = (temp.grades[0] + temp.grades[1] + temp.grades[2] + temp.grades[3] + temp.grades[4]) / 5.0;
        fprintf(file, "%s %s %f", temp.surname, temp.name, average);
        fflush(file);
    }
    return st;
}

Status write_clever_studs(FILE* file, int amount, Student* students)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    Status status = OK;
    double sum = 0;
    for (int i = 0; i < amount; ++i)
    {
        double temp = (students[i].grades[0] + students->grades[1] + students->grades[2] + students->grades[3] + students->grades[4]) / 5.0;
        sum +=  temp;
    }
    double average = sum / amount;

    for (int i = 0; i < amount; ++i)
    {
        double temp = (students[i].grades[0] + students[i].grades[1] + students[i].grades[2] + students[i].grades[3] + students[i].grades[4]) / 5.0;
        if (temp > average)
        {
            fprintf(file, "%s %s", students[i].surname, students[i].name);
            fflush(file);
        }
    }
    return status;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }
    FILE* input;
    FILE* output;
    if ((input = fopen(argv[1], "r")) == NULL)
    {
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    if ((output = fopen(argv[2], "w")) == NULL)
    {
        fclose(input);
        errors(OPENING_ERROR);
        return OPENING_ERROR;
    }
    Student* students;
    int amount_of_students = 0;
    Status st = read_students(input, &amount_of_students, &students);
    fclose(input);
    if (st)
    {
        errors(st);
        fclose(output);
        return st;
    }

    if (!st)
    {
        printf("Available commands:\n"
               "exit, search_by_id, search_by_surname, search_by_name, search_by_group,\n"
               "print_all_to_file, print_smart,\n"
               "sort_by_id, sort_by_surname, sort_by_name, sort_by_group\n");
    }
    int running = 1;
    while (running && !st)
    {
        Status st1 = OK;
        char* command = NULL;
        printf("Enter your command: \n");
        st = get_str(&command);
        if (!st)
        {
            if (!strcmp(command, "exit"))
            {
                printf("Have a great day!\n");
                running = 0;
            }
            else if (!strcmp(command, "search_by_id"))
            {
                printf("Enter student's id:\n");
                char* id = NULL;
                Student found_st;
                st1 = get_str(&id);
                if (!st1)
                {
                    st1 = id_is_valid(id);
                }
                if (!st1)
                {
                    st1 = search_by_id(students, amount_of_students, &found_st, id);
                }
                if (!st1)
                {
                    print_student(found_st);
                }
                free(id);
            }
            else if (!strcmp(command, "search_by_name"))
            {
                printf("Enter student's name:\n");
                char* name = NULL;
                int cnt = 0;
                Student* found_st = NULL;
                st1 = get_str(&name);
                if (!st1)
                {
                    st1 = is_valid(name, 0);
                }
                if (!st1)
                {
                    st1 = search_by_flag(students, amount_of_students,&found_st, name, NAME, &cnt);
                }
                if (!st1)
                {
                    print_all_students(found_st, cnt);
                }
                free(name);
                free(found_st);
            }
            else if (!strcmp(command, "search_by_surname"))
            {
                printf("Enter student's surname:\n");
                char* surname = NULL;
                int cnt = 0;
                Student* found_st = NULL;
                st1 = get_str(&surname);
                if (!st1)
                {
                    st1 = is_valid(surname, 0);
                }
                if (!st1)
                {
                    st1 = search_by_flag(students, amount_of_students,&found_st, surname, SURNAME, &cnt);
                }
                if (!st1)
                {
                    print_all_students(found_st, cnt);
                }
                free(surname);
                free(found_st);
            }
            else if (!strcmp(command, "search_by_group"))
            {
                printf("Enter student's group:\n");
                char* group = NULL;
                int cnt = 0;
                Student* found_st = NULL;
                st1 = get_str(&group);
                if (!st1)
                {
                    st1 = is_valid(group, 1);
                }
                if (!st1)
                {
                    st1 = search_by_flag(students, amount_of_students,&found_st, group, GROUP, &cnt);
                }
                if (!st1)
                {
                    print_all_students(found_st, cnt);
                }
                free(group);
                free(found_st);
            }
            else if (!strcmp(command, "sort_by_id"))
            {
                if (!st1)
                {
                    qsort(students, amount_of_students, sizeof(Student), compare_by_id);
                    print_all_students(students, amount_of_students);
                }
            }
            else if (!strcmp(command, "sort_by_name"))
            {
                if (!st1)
                {
                    qsort(students, amount_of_students, sizeof(Student), compare_by_name);
                    print_all_students(students, amount_of_students);
                }
            }
            else if (!strcmp(command, "sort_by_surname"))
            {
                if (!st1)
                {
                    qsort(students, amount_of_students, sizeof(Student), compare_by_surname);
                    print_all_students(students, amount_of_students);
                }
            }
            else if (!strcmp(command, "sort_by_group"))
            {
                if (!st1)
                {
                    qsort(students, amount_of_students, sizeof(Student), compare_by_group);
                    print_all_students(students, amount_of_students);
                }
            }
            else if (!strcmp(command, "print_all_to_file"))
            {
                printf("Enter student's id:\n");
                char* id = NULL;
                int cnt = 0;
                st1 = get_str(&id);
                if (!st1)
                {
                    st1 = write_to_file(output, id, amount_of_students, students);
                }
                free(id);
            }
            else if (!strcmp(command, "print_smart"))
            {
                st1 = write_clever_studs(output, amount_of_students, students);
            }
            else
            {
                printf("Unknown command 0_0\n");
            }
            if (st1)
            {
                errors(st1);
            }
            free(command);
            printf("\n");
        }

    }
    for (int i = 0; i < amount_of_students; ++i)
    {
        free(students[i].grades);
    }
    free(students);
    fclose(output);
    return 0;
}
