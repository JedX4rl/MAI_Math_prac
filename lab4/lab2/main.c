#include <stdio.h>
#include "my_lib.h"
#include "array.h"

void menu()
{
    printf("Load <Name, filename>; - load numbers from file to Array\n");
    printf("Save <Name, filename>; - save numbers from Array to file\n");
    printf("Rand <Name, count, lb, rb>; - fill array by random numbers in [lb, rb]\n");
    printf("Concat <Name_1, Name_2>; - concat two arrays\n");
    printf("Free(Name); - free array\n");
    printf("Remove <Name, amount, index>; - delete amount numbers after index in Array\n");
    printf("Copy <Name_1, start, end, Name_2>; - copy elements from Name_1 to Name_2 in [start, end]\n");
    printf("Sort <Name, A+>; - sort array ascending\n");
    printf("Sort <Name, A->; - sort array descending\n");
    printf("Shuffle <Name>; - shuffle array\n");
    printf("Stats <Name>; - print info about Array\n");
    printf("Print <Name, index>; - print elements of Name[idx]\n");
    printf("Print <Name, start, end>; - print elements of range [start, end]\n");
    printf("Print <Name, all>; - to print all elements of Name\n");
    printf("Exit; - exit\n");
    printf("<--------------->\n");
}

Status commands(Array** storage, int* size_of_storage, char* in)
{
    if (storage == NULL || size_of_storage == NULL || in == NULL)
    {
        return INVALID_ARGUMENT;
    }
    char *checker = strchr(in, ';');
    if (checker == NULL)
    {
        return INVALID_INPUT;
    }
    int size = (int) strlen(in);
    int idx = 0;
    char *command = (char *) malloc(sizeof(char) * size);
    if (command == NULL)
    {
        return BAD_ALLOC;
    }
    char *arguments = (char *) malloc(sizeof(char) * size);
    if (arguments == NULL)
    {
        free(command);
        command = NULL;
        return BAD_ALLOC;
    }
    int finished = 0;
    for (int i = 0; i < size; ++i)
    {
        char ch = in[i];
        if (!isspace(ch))
        {
            if (ch == ',' || ch == ';')
            {
                continue;
            }
            if (!finished)
            {
                command[idx] = ch;
                idx++;
            } else
            {
                arguments[idx] = ch;
                idx++;
            }
        }
        if (isspace(ch))
        {
            if (finished)
            {
                arguments[idx] = ch;
                idx++;
            }
            if (idx != 0 && !finished)
            {
                command[idx] = '\0';
                idx = 0;
                finished = 1;
            }
        }
    }
    if (finished)
    {
        arguments[idx] = '\0';
        idx = 0;
    } else
    {
        command[idx] = '\0';
        idx = 0;
    }
    char first_arr, second_arr;
    int size_of_arguments = (int)strlen(arguments);
    Array *tmp = NULL;
    Status st = 0;
    int to_add = 0;
    if (!strcmp(command, "Load"))
    {
        char* filename = (char*)malloc(sizeof(char) * size);
        if (filename == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        first_arr = arguments[0];
        for (int i = 2; i < size_of_arguments; i++)
        {
            filename[idx] = arguments[i];
            idx++;
        }
        filename[idx] = '\0';
        idx = 0;
        tmp = find_array(storage, *size_of_storage, first_arr);
        if (tmp == NULL)
        {
            to_add = 1;
        }
        st = load(&tmp, first_arr, filename);
        if (st)
        {
            free(filename);
            free(command);
            free(arguments);
            return st;
        }
        free(filename);
        if (to_add)
        {
            add_to_storage(storage, tmp, size_of_storage);
        }
    }
    else if (!strcmp(command, "Save"))
    {
        char* filename = (char*)malloc(sizeof(char) * size);
        if (filename == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        first_arr = arguments[0];
        for (int i = 2; i < size_of_arguments; i++)
        {
            filename[idx] = arguments[i];
            idx++;
        }
        filename[idx] = '\0';
        idx = 0;
        tmp = find_array(storage, *size_of_storage, first_arr);
        st = save(tmp, filename);
        if (st)
        {
            free(filename);
            free(command);
            free(arguments);
            return st;
        }
        free(filename);
    }
    else if (!strcmp(command, "Rand"))
    {
        char* two_arguments = (char*)malloc(sizeof(char) * size);
        if (two_arguments == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        first_arr = arguments[0];
        for (int i = 2; i < size_of_arguments; i++)
        {
            two_arguments[idx] = arguments[i];
            idx++;
        }
        two_arguments[idx] = '\0';
        idx = 0;
        tmp = find_array(storage, *size_of_storage, first_arr);
        if (tmp == NULL)
        {
            to_add = 1;
        }
        st = rand_array(&tmp, first_arr, two_arguments);
        if (st)
        {
            free(two_arguments);
            free(command);
            free(arguments);
            return st;
        }
        free(two_arguments);
        if (to_add)
        {
            add_to_storage(storage, tmp, size_of_storage);
        }
    }
    else if (!strcmp(command, "Concat"))
    {
        int read = sscanf(arguments, "%c %c", &first_arr, &second_arr);
        if (read != 2)
        {
            free(command);
            free(arguments);
            return INVALID_INPUT;
        }
        tmp = find_array(storage, *size_of_storage, first_arr);
        Array* tmp2 = find_array(storage, *size_of_storage, second_arr);
        st = concatenation(tmp, tmp2);
        if (st)
        {
            free(command);
            free(arguments);
            return st;
        }
    }
    else if (!strncmp(command, "Free", 4))
    {
        printf("%s\n", command);
        first_arr = command[5];
        printf("Letter %c\n", first_arr);
        tmp = find_array(storage, *size_of_storage, first_arr);
        st = Free(tmp);
        if (st)
        {
            free(command);
            free(arguments);
            return st;
        }
    }
    else if (!strcmp(command, "Remove"))
    {
        char* two_args = (char*)malloc(sizeof(char) * size);
        if (two_args == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        first_arr = arguments[0];
        for (int i = 2; i < size_of_arguments; i++)
        {
            two_args[idx] = arguments[i];
            idx++;
        }
        two_args[idx] = '\0';
        idx = 0;
        int start, end;
        int read = sscanf(two_args, "%d %d", &start, &end);
        if (read != 2)
        {
            free(command);
            free(arguments);
            free(two_args);
            return INVALID_INPUT;
        }
        tmp = find_array(storage, *size_of_storage, first_arr);
        st = delete_items(tmp, start, end);
        if (st)
        {
            free(two_args);
            free(command);
            free(arguments);
            return st;
        }
        free(two_args);
    }
    else if (!strcmp(command, "Copy"))
    {
        char* two_args = (char*)malloc(sizeof(char) * size);
        if (two_args == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        first_arr = arguments[0];
        for (int i = 2; i < size_of_arguments; i++)
        {
            two_args[idx] = arguments[i];
            idx++;
        }
        two_args[idx] = '\0';
        idx = 0;
        int start, end;
        int read = sscanf(two_args, "%d %d %c", &start, &end, &second_arr);
        if (read != 3)
        {
            free(command);
            free(arguments);
            free(two_args);
            return INVALID_INPUT;
        }
        tmp = find_array(storage, *size_of_storage, first_arr);
        Array* tmp2 = find_array(storage, *size_of_storage, second_arr);
        if (tmp2 == NULL)
        {
            st = create_array(&tmp2, second_arr);
            to_add = 1;
        }
        st = copy(tmp, start, end, &tmp2);
        if (st)
        {
            free(two_args);
            free(command);
            free(arguments);
            return st;
        }
        free(two_args);
        if (to_add)
        {
            add_to_storage(storage, tmp2, size_of_storage);
        }
    }
    else if (!strcmp(command, "Sort"))
    {
        first_arr = arguments[0];
        tmp = find_array(storage, *size_of_storage, first_arr);
        sort(tmp, arguments[1]);
    }
    else if (!strcmp(command, "Shuffle"))
    {
        first_arr = arguments[0];
        tmp = find_array(storage, *size_of_storage, first_arr);
        Shuffle(tmp);
    }
    else if (!strcmp(command, "Stats"))
    {
        first_arr = arguments[0];
        tmp = find_array(storage, *size_of_storage, first_arr);
        stats(tmp);
    }
    else if (!strcmp(command, "Print"))
    {

        first_arr = arguments[0];
        tmp = find_array(storage, *size_of_storage, first_arr);
        char* two_args = (char*)malloc(sizeof(char) * size);
        if (two_args == NULL)
        {
            free(command);
            free(arguments);
            return BAD_ALLOC;
        }
        for (int i = 2; i < size_of_arguments; i++)
        {
            two_args[idx] = arguments[i];
            idx++;
        }
        two_args[idx] = '\0';
        if (!strcmp(two_args, "all") && !isnumber(*two_args))
        {
            st = INVALID_INPUT;
        }
        if (!st)
        {
            print(tmp, first_arr, two_args);
        }
        free(two_args);
    }
    else if (!strcmp(command, "Exit"))
    {
        free(command);
        free(arguments);
        return EXIT;
    }
    else
    {
        free(command);
        free(arguments);
        return INVALID_INPUT;
    }
    free(command);
    free(arguments);
    return OK;
}

int main()
{
    Array** storage = (Array**)malloc(sizeof(Array) * 64);
    if (storage == NULL)
    {
        return BAD_ALLOC;
    }
    int size = 0;
    Status st = OK;
    menu();
    while (st != EXIT)
    {
        char* str = NULL;
        st = get_str(&str);
        st = st ? st : commands(storage, &size, str);
        if (st != EXIT)
        {
            errors(st);
            st ? printf("Try again!\n") : printf("Success\n");
        }
        free(str);
        str = NULL;
    }
    free_storage(storage, size);
    printf("Have a great day!\n");
    return OK;
}
