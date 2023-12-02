#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>

#define BUFF_SIZE 256

typedef enum
{
    OK,
    BAD_ALLOC,
    NULL_POINTER,
    INVALID_INPUT,
    NOT_OK,
    OPENING_ERROR
} Status;

typedef enum
{
    START,
    INTERMEDIATE,
    FINISH,
    ERROR
} Stop;

typedef struct Route
{
    char* name;
    char* arrival_time;
    char* depart_time;
    Stop type;
    struct Route* next;
} Route;

typedef struct Bus
{
    Route* routes;
    char* number;
    struct Bus* next;
} Bus;

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

Status free_all(int cnt, ...)
{
    if (cnt < 1)
    {
        return INVALID_INPUT;
    }
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = 0; i < cnt; ++i)
    {
        void* tmp = va_arg(ptr, void*);
        free(tmp);
    }
    va_end(ptr);
    return OK;
}

Status not_empty_str(char* str)
{
    if (str == NULL)
    {
        return INVALID_INPUT;
    }
    if (!strlen(str))
    {
        return INVALID_INPUT;
    }
    return OK;
}

Status date_is_valid(char* str)
{
    if (str == NULL)
    {
        return INVALID_INPUT;
    }
    if (strlen(str) != 19)
    {
        return INVALID_INPUT;
    }
    int is_leap_year = 0;
    int day, month, year, hours, minutes, seconds;
    sscanf(str, "%d.%d.%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    if ((year % 400 == 0 ||(year % 4==0 && year % 100 != 0)))
    {
        is_leap_year = 1;
    }
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31 ||
        ((month == 4 || month == 6 || month == 9 || month == 11) && (day > 30)) ||
        (month == 2 && (is_leap_year ? day > 29 : day > 28)) ||
        hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
    {
        return INVALID_INPUT;
    }

    return OK;
}

Status find_bus(const char* number, Bus* head, Bus** found)
{
    if (number == NULL)
    {
        return INVALID_INPUT;
    }
    if (head == NULL)
    {
        return NOT_OK;
    }
    Bus* tmp = head;
    while (tmp != NULL)
    {
        if (!strcmp(number, tmp->number))
        {
            *found = tmp;
            return OK;
        }
        tmp = tmp->next;
    }
    return NOT_OK;
}

Status create_bus(char* number, Bus** bus)
{
    if (number == NULL)
    {
        return INVALID_INPUT;
    }
    *bus = (Bus*)malloc(sizeof(Bus));
    if (*bus == NULL)
    {
        return BAD_ALLOC;
    }
    (*bus)->number = number;
    (*bus)->next = NULL;
    (*bus)->routes = NULL;
    return OK;
}

Stop get_stop_status(const char* info)
{
    if (!strcmp(info, "Start"))
    {
        return START;
    }
    if (!strcmp(info, "Mid"))
    {
        return INTERMEDIATE;
    }
    if (!strcmp(info, "End"))
    {
        return FINISH;
    }
    return ERROR;
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

int compare_time(const char* a, const char* b)
{
    if (a == NULL || b == NULL)
    {
        return 0;
    }
    if (a == NULL)
    {
        return -1;
    }
    if (b == NULL)
    {
        return 1;
    }
    int a_day, a_month, a_year, a_hour, a_minute, a_sec;
    int b_day, b_month, b_year, b_hour, b_minute, b_sec;
    sscanf(a, "%d.%d.%d %d:%d:%d", &a_day, &a_month, &a_year, &a_hour, &a_minute, &a_sec);
    sscanf(b, "%d.%d.%d %d:%d:%d", &b_day, &b_month, &b_year, &b_hour, &b_minute, &b_sec);
    if (a_year != b_year)
    {
        return a_year > b_year ? 1 : -1;
    }
    if (a_month != b_month)
    {
        return a_month > b_month ? 1 : -1;
    }
    if (a_day != b_day)
    {
        return a_day > b_day ? 1 : -1;
    }
    if (a_hour != b_hour)
    {
        return a_hour > b_hour ? 1 : -1;
    }
    if (a_minute > b_minute)
    {
        return a_minute > b_minute ? 1 : -1;
    }
    return a_sec > b_sec ? 1 : -1;
}

Status add_route(Route** routes, char* name, char* arrival_time, char* depart_time, Stop type)
{
    if (name == NULL || arrival_time == NULL || depart_time == NULL)
    {
        return INVALID_INPUT;
    }
    Route* new = (Route*)malloc(sizeof(Route));
    if (new == NULL)
    {
        return BAD_ALLOC;
    }
    new->name = strdup(name);
    new->arrival_time = strdup(arrival_time);
    new->depart_time = strdup(depart_time);
    new->type = type;
    new->next = NULL;
    if (new->name == NULL || new->arrival_time == NULL || new->depart_time == NULL)
    {
        free_all(4, new, new->name, new->depart_time, new->arrival_time);
        return BAD_ALLOC;
    }
    if (*routes == NULL || compare_time(new->arrival_time, (*routes)->arrival_time) < 0)
    {
        new->next = *routes;
        *routes = new;
        return OK;
    }
    Route* curr = *routes;
    while (curr->next != NULL && compare_time(new->arrival_time, curr->next->arrival_time) >= 0)
    {
        curr = curr->next;
    }
    new->next = curr->next;
    curr->next = new;
    return OK;
}

Status add_bus(Bus** buses, Bus* new)
{
    if (new == NULL)
    {
        return INVALID_INPUT;
    }
    if (*buses == NULL)
    {
        *buses = new;
    }
    else if ((*buses)->next == NULL)
    {
        (*buses)->next = new;
    }
    else
    {
        Bus* curr = *buses;
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new;
    }
    return OK;
}

Status read_from_file(char* argv[], int cnt, Bus** buses)
{
    Status st = OK;
    for (int i = 1; i < cnt; ++i)
    {
        const char* filename = argv[i];
        FILE* in = fopen(filename, "r");
        if (in == NULL)
        {
            return OPENING_ERROR;
        }
        char* current_line = NULL;
        size_t length = 0;
        int status;
        while ((status = getline(&current_line, &length, in)) != -1)
        {
            char* number = NULL;
            char* name = NULL;
            char* arrival_time = NULL;
            char* arrival_date = NULL;
            char* depart_time = NULL;
            char* depart_date = NULL;
            char* stop = NULL;
            if (status == -1)
            {
                free(current_line);
                fclose(in);
                return BAD_ALLOC;
            }
            number = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (number == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            name = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (name == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            arrival_time = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (arrival_time == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            depart_time = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (depart_time == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            arrival_date = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (arrival_date == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            depart_date = (char*)malloc(sizeof(char) * BUFF_SIZE);
            if (depart_date == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            stop = (char*)malloc(sizeof(char) * 20);
            if (stop == NULL)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }

            if (sscanf(current_line, "%s %s %s %s %s %s %s", number, name, arrival_date, arrival_time, depart_date, depart_time, stop) != 7)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return BAD_ALLOC;
            }
            sprintf(arrival_date, "%s %s", arrival_date, arrival_time);
            sprintf(depart_date, "%s %s", depart_date, depart_time);
            free(arrival_time);
            free(depart_time);
            arrival_time = NULL;
            depart_time = NULL;
            if (not_empty_str(number) || date_is_valid(arrival_date) || date_is_valid(depart_date))
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return INVALID_INPUT;
            }
            Bus* new = NULL;
            Status find = find_bus(number, *buses, &new);
            int is_first = 0;
            if (find == INVALID_INPUT)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return status;
            }
            if (find == NOT_OK)
            {
                is_first = 1;
            }

            if (is_first)
            {
                st = create_bus(number, &new);
                if (st)
                {
                    free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                    fclose(in);
                    return st;
                }
            }
            Stop type = get_stop_status(stop);
            if (type == ERROR)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return NOT_OK;
            }
            st = add_route(&new->routes, name, arrival_date, depart_date, type);
            if (st != OK)
            {
                free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                fclose(in);
                return st;
            }
            if (is_first || new == NULL)
            {
                st = add_bus(buses, new);
                if (st != OK)
                {
                    free_all(8, current_line, number, name, arrival_time, depart_time, stop, arrival_date, depart_date);
                    fclose(in);
                    return st;
                }
            }
            free(current_line);
            current_line = NULL;
        }
        if (current_line != NULL)
        {
            free(current_line);
        }
        fclose(in);
    }
    return OK;
}

Status print_buses(Bus* buses, char* out_name)
{
    if (buses == NULL || out_name == NULL)
    {
        return INVALID_INPUT;
    }
    Bus* current = buses;
    FILE* out = fopen(out_name, "w");
    if (out == NULL)
    {
        return OPENING_ERROR;
    }
    while (current != NULL)
    {
        int idx = 0;
        fprintf(out, "Number: %s", current->number);
        Route* curr = current->routes;
        while (curr != NULL)
        {
            if (curr->type == START)
            {
                fprintf(out, "\nRoute %d: ", idx);
                idx++;
            }
            else
            {
                fprintf(out, "-> ");
            }
            fprintf(out, "%s ", curr->name);
            curr = curr->next;
        }
        current = current->next;
        fprintf(out, "\n");
    }
    fflush(out);
    fclose(out);
    return OK;
}

Status print_info(Bus* buses)
{
    if (buses == NULL)
    {
        return INVALID_INPUT;
    }
    Bus* current = buses;
    int idx = 0;
    printf("Number: %s", current->number);
    Route* curr = current->routes;
    while (curr != NULL)
    {
        if (curr->type == START)
        {
            printf("\nRoute %d: ", idx);
            idx++;
        }
        else
        {
            printf("-> ");
        }
        printf("%s ", curr->name);
        curr = curr->next;
    }
    printf("\n");
    return OK;
}

Status free_routes(Route* routes)
{
    if (routes == NULL)
    {
        return OK;
    }
    Route* curr = routes;
    while (curr != NULL)
    {
        free(curr->name);
        free(curr->arrival_time);
        free(curr->depart_time);
        Route *tmp = curr->next;
        curr->name = NULL;
        curr->arrival_time = NULL;
        curr->depart_time = NULL;
        free(curr);
        curr = tmp;
    }
    return OK;
}

Status free_buses(Bus* buses)
{
    if (buses == NULL)
    {
        return OK;
    }
    Bus* current = buses;
    while (current != NULL)
    {
        Bus* tmp = current;
        free_routes(tmp->routes);
        free(current);
        current = tmp;
    }
    return OK;
}

int amount_of_stops(Route* routes)
{
    if (routes == NULL)
    {
        return 0;
    }
    int counter = 0;
    Route* current = routes;
    while (current != NULL)
    {
        counter++;
        current = current->next;
    }
    return counter;
}

int amount_of_routes(Route* routes)
{
    if (routes == NULL)
    {
        return 0;
    }
    int counter = 0;
    Route* current = routes;
    while (current != NULL)
    {
        if (current->type == FINISH)
        {
            counter++;
        }
        current = current->next;
    }
    return counter;
}

Status time_difference(char* a, char* b, time_t* res)
{
    int a_day, a_month, a_year, a_hour, a_minute, a_sec;
    int b_day, b_month, b_year, b_hour, b_minute, b_sec;
    sscanf(a, "%d.%d.%d %d:%d:%d", &a_day, &a_month, &a_year, &a_hour, &a_minute, &a_sec);
    sscanf(b, "%d.%d.%d %d:%d:%d", &b_day, &b_month, &b_year, &b_hour, &b_minute, &b_sec);
    struct tm date_1 = {0};
    struct tm date_2 = {0};

    date_1.tm_year = a_year - 1900;
    date_1.tm_mon = a_month - 1;
    date_1.tm_mday = a_day;
    date_1.tm_hour = a_hour;
    date_1.tm_min = a_minute;
    date_1.tm_sec = a_sec;

    date_2.tm_year = b_year - 1900;
    date_2.tm_mon = b_month - 1;
    date_2.tm_mday = b_day;
    date_2.tm_hour = b_hour;
    date_2.tm_min = b_minute;
    date_2.tm_sec = b_sec;

    time_t a_time_in_sec = mktime(&date_1);
    time_t b_time_in_sec = mktime(&date_2);
    time_t difference = difftime(b_time_in_sec, a_time_in_sec);
    *res = difference;
    return OK;
}

Status bus_with_the_biggest_amount_of_routes(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* curr = buses;
    int max = 0;
    int current = 0;
    while (curr != NULL)
    {
        current = amount_of_routes(curr->routes);
        if (current > max)
        {
            max = current;
            *res = curr;
        }
        curr = curr->next;
    }
    return OK;
}

Status bus_with_the_least_amount_of_routes(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* curr = buses;
    int min = 100000;
    int current = 0;
    while (curr != NULL)
    {
        current = amount_of_routes(curr->routes);
        if (current <= min)
        {
            min = current;
            *res = curr;
        }
        curr = curr->next;
    }
    return OK;
}

Status bus_with_the_longest_way(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* current = buses;
    int max = 0;
    int curr = 0;
    while (current != NULL)
    {
        curr = amount_of_stops(current->routes);
        if (curr > max)
        {
            max = curr;
            *res = current;
        }
        current = current->next;
    }
    return OK;
}

Status bus_with_the_shortest_way(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* current = buses;
    int min = 100000;
    int curr = 0;
    while (current != NULL)
    {
        curr = amount_of_stops(current->routes);
        if (curr <= min)
        {
            min = curr;
            *res = current;
        }
        current = current->next;
    }
    return OK;
}

Status bus_with_the_biggest_route(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    int tmp = 0;
    int tmp_size = 0;
    int max_size = 0;
    Bus* current_bus = buses;
    while (current_bus != NULL)
    {
        Route* current_route = current_bus->routes;
        while(current_route != NULL)
        {
            tmp++;
            if (current_route->type == FINISH)
            {
                tmp_size = tmp;
                tmp = 0;
                if (tmp_size > max_size)
                {
                    max_size = tmp_size;
                    *res = current_bus;
                }
            }
            current_route = current_route->next;
        }

        current_bus = current_bus->next;
    }

    return OK;
}

Status bus_with_the_shortest_route(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    int tmp = 0;
    int tmp_size = 0;
    int min_size = 1000;
    Bus* current_bus = buses;
    while (current_bus != NULL)
    {
        Route* current_route = current_bus->routes;
        while(current_route != NULL)
        {
            tmp++;
            if (current_route->type == FINISH)
            {
                tmp_size = tmp;
                tmp = 0;
                if (tmp_size < min_size)
                {
                    min_size = tmp_size;
                    *res = current_bus;
                }
            }
            current_route = current_route->next;
        }

        current_bus = current_bus->next;
    }

    return OK;
}

Status the_longest_stop(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* current = buses;
    time_t max = 0;
    time_t tmp = 0;
    if (buses->next == NULL)
    {
        *res = buses;
        return OK;
    }
    while (current != NULL)
    {
        Route* curr_route = current->routes;
        while (curr_route != NULL)
        {
            time_difference(curr_route->depart_time, curr_route->arrival_time, &tmp);
            if (tmp < max)
            {
                max = tmp;
                *res = current;
            }
            curr_route = curr_route->next;
        }
        current = current->next;
    }
    return OK;
}

Status the_fastest_stop(Bus* buses, Bus** res)
{

    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* current = buses;
    time_t min = -1110;
    time_t tmp = 0;
    if (buses->next == NULL)
    {
        *res = buses;
        return OK;
    }
    while (current != NULL)
    {
        Route* curr_route = current->routes;
        while (curr_route != NULL)
        {
            time_difference(curr_route->depart_time, curr_route->arrival_time, &tmp);
            if (tmp >= min)
            {
                min = tmp;
                *res = current;
            }
            curr_route = curr_route->next;
        }
        current = current->next;
    }
    return OK;
}

Status the_slowpoke(Bus* buses, Bus** res)
{
    if (buses == NULL)
    {
        *res = NULL;
        return OK;
    }
    Bus* current = buses;
    time_t max = 0;
    time_t tmp = 0;
    if (buses->next == NULL)
    {
        *res = buses;
        return OK;
    }
    while (current != NULL)
    {
        time_t result = 0;
        Route* curr_route = current->routes;
        while (curr_route != NULL)
        {
            time_difference(curr_route->depart_time, curr_route->arrival_time, &tmp);
            result += tmp;
            curr_route = curr_route->next;
        }
        if (result < max)
        {
            max = result;
            *res = current;
        }
        current = current->next;
    }
    return OK;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        errors(INVALID_INPUT);
        return INVALID_INPUT;
    }

    Bus* list_of_buses = NULL;
    Status st = read_from_file(argv, argc, &list_of_buses);
    if (st)
    {
        errors(st);
        return st;
    }

    if (!st)
    {
        printf("Available commands:\n"
               "exit, biggest_amt_of_routes, least_amt_of_routes, longest_way, shortest_way,\n"
               "biggest_route, least_route, print_all_to_file,\n"
               "biggest_stop, fastest_stop, slowpoke\n");
    }
    int running = 1;
    while (running && !st)
    {
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
            else if (!strcmp(command, "biggest_amt_of_routes"))
            {
                Bus* found_bus;

                st = bus_with_the_biggest_amount_of_routes(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "least_amt_of_routes"))
            {
                Bus* found_bus;

                st = bus_with_the_least_amount_of_routes(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "longest_way"))
            {
                Bus* found_bus;

                st = bus_with_the_longest_way(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "shortest_way"))
            {
                Bus* found_bus;

                st = bus_with_the_shortest_way(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "biggest_route"))
            {
                Bus* found_bus;

                st = bus_with_the_biggest_route(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "least_route"))
            {
                Bus* found_bus;

                st = bus_with_the_shortest_route(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "fastest_stop"))
            {
                Bus* found_bus;

                st = the_fastest_stop(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "longest_stop"))
            {
                Bus* found_bus;

                st = the_longest_stop(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "slowpoke"))
            {
                Bus* found_bus;

                st = the_slowpoke(list_of_buses, &found_bus);

                if (found_bus != NULL)
                {
                    print_info(found_bus);
                }
                else
                {
                    printf("The list of buses is empty!\n");
                }
            }
            else if (!strcmp(command, "print_all_to_file"))
            {
                char* file_name_out = NULL;
                printf("Enter the name of file you want to print all buses in:\n");
                st = get_str(&file_name_out);
                if (!st)
                {
                    print_buses(list_of_buses, file_name_out);
                }
                free(file_name_out);
            }
            else
            {
                printf("Unknown command 0_0\n");
            }
            free(command);
        }
    }
    if (st)
    {
        errors(st);
    }
    free_buses(list_of_buses);
    return st;
}
