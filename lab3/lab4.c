#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

typedef struct String
{
    char* str;
    int size;
} String;

typedef struct Address
{
    String city;
    String street;
    unsigned building_number;
    String building;
    unsigned apart_number;
    String idx;
} Address;

typedef struct Mail
{
    Address user;
    double weight;
    String post_id;
    String date_of_creation;
    String estimated_date_of_sent;
    String is_sent;
} Mail;

typedef struct Post
{
    int max_size;
    int tmp_size;
    Address post_building;
    Mail* mails;
} Post;


typedef enum
{
    OK,
    BAD_ALLOC,
    NULL_POINTER,
    INVALID_INPUT,
    NOT_OK
} Status;

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
        default:
            printf("Something went wrong\n");
            return;
    }
}

Status create_string(char* s, String* result)
{
    if (s == NULL)
    {
        result->size = 0;
        result->str = NULL;
        return OK;
    }
    int length = (int)strlen(s);
    result->str = (char*)malloc(sizeof(char) * (length + 1));
    if (result->str == NULL)
    {
        return BAD_ALLOC;
    }
    result->str[length] = '\0';
    for (int i = 0; i < length; ++i)
    {
        result->str[i] = s[i];
    }
    result->size = length;
    return OK;
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

Status destroy_string(String* string)
{
    if (string->str == NULL)
    {
        return NULL_POINTER;
    }
    free(string->str);
    string->str = NULL;
    string->size = 0;
    return OK;
}

int my_str_cmp(String str1, String str2)
{
    if (str1.str == NULL || str2.str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = str1.size;
    int length_2 = str2.size;
    int min = length_1;
    if (length_1 > length_2)
    {
        min = length_2;
    }
    for (int i = 0; i < min; ++i)
    {
        if (str1.str[i] != str2.str[i])
        {
            return str1.str[i] < str2.str[i] ? -1 : 1;
        }
    }
    return 0;
}

int compare_strings(String str1, String str2)
{
    if (str1.str == NULL || str2.str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = str1.size;
    int length_2 = str2.size;
    if (length_2 != length_1)
    {
        return 0;
    }
    return my_str_cmp(str1, str2) == 0;
}

Status my_strcpy(String* dest, String* src)
{
    if (src->str == NULL || dest->str == NULL)
    {
        return NULL_POINTER;
    }
    int length_1 = dest->size;
    int length_2 = src->size;

    for (int i = 0; i < length_2; ++i)
    {
        if (i == length_1)
        {
            break;
        }
        dest->str[i] = src->str[i];
    }
    dest->str[length_2] = '\0';
    return OK;
}

Status copy_str(String* dest, String* source)
{
    create_string(source->str, dest);
    return OK;
}

Status concatenation(String* str1, String* str2)
{
    int length = str1->size + str2->size;
    my_realloc(&(str1->str), length);
    for (int i = 0; i < length; ++i)
    {
        str1->str[str1->size + i] = str2->str[i];
    }
    str1->size = length;
    return OK;
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

void free_address(Address tmp)
{
    destroy_string(&(tmp.idx));
    destroy_string(&(tmp.street));
    destroy_string(&(tmp.building));
    destroy_string(&(tmp.city));
}

void free_mail(Mail* tmp)
{
    free_address(tmp->user);
    destroy_string(&(tmp->estimated_date_of_sent));
    destroy_string(&(tmp->date_of_creation));
    destroy_string(&(tmp->post_id));
    destroy_string(&(tmp->is_sent));
}

void free_post(Mail* mails, int* size)
{
    for (int i = 0; i < *size; ++i)
    {
        free_mail(&mails[i]);
    }
    *size = 0;
}

Status is_uint(char* str)
{
    if (str == NULL)
    {
        return INVALID_INPUT;
    }
    int length = (int)strlen(str);
    for (int i = 0; i < length; ++i)
    {
        if (!isdigit(str[i]))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

Status is_udouble(char* str)
{
    if (str == NULL)
    {
        return INVALID_INPUT;
    }
    int length = (int) strlen(str);
    int dots = 0;
    for (int i = 0; i < length; ++i)
    {
        char c = str[i];
        if (!isdigit(c))
        {
            if (c != '.')
            {
                return INVALID_INPUT;
            }
            dots++;
        }
    }
    if (dots > 1)
    {
        return INVALID_INPUT;
    }
    return OK;
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

Status create_post(Post* post, const Address address)
{
    if (post == NULL)
    {
        return NULL_POINTER;
    }

    post->post_building = address;
    post->tmp_size = 0;
    post->max_size = 10;
    post->mails = (Mail*)malloc(sizeof(Mail) * 10);
    if (post->mails == NULL)
    {
        post->max_size = 0;
        return BAD_ALLOC;
    }
    return OK;
}

Status create_post_address(Address* address)
{
    String city;
    String street;
    String building;
    String idx;
    Status st = create_string(NULL, &city);
    if (st != OK)
    {
        return st;
    }
    st = create_string(NULL, &street);
    if (st != OK)
    {
        return st;
    }
    st = create_string(NULL, &building);
    if (st != OK)
    {
        return st;
    }
    st = create_string(NULL, &idx);
    if (st != OK)
    {
        return st;
    }
    address->idx = idx;
    address->building = building;
    address->street = street;
    address->city = city;
    address->apart_number = 0;
    address->building_number = 0;
    return OK;
}

Status destroy_address(Address* address)
{
    if (address == NULL)
    {
        return NULL_POINTER;
    }
    destroy_string(&(address->city));
    destroy_string(&(address->street));
    destroy_string(&(address->building));
    destroy_string(&(address->idx));
    address->apart_number = 0;
    address->building_number = 0;
    return OK;
}

Status destroy_post(Post* post)
{
    if (post == NULL)
    {
        return NULL_POINTER;
    }
    for (int i = 0; i < post->tmp_size; ++i)
    {
        free_mail(&(post->mails[i]));
    }
    free(post->mails);
    destroy_address(&(post->post_building));
    return OK;
}

Status create_mail(Mail* mail)
{
    char *city = NULL, *street = NULL, *building_number = NULL, *building = NULL, *apart_number = NULL, *weight = NULL, *idx = NULL, *post_id = NULL;
    printf("Enter the mail info\n");
    printf("City: ");
    Status status;
    status = get_str(&city);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if (strlen(city) == 0)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Street: ");
    status = get_str(&street);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if (strlen(street) == 0)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Building number: ");
    status = get_str(&building_number);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if (strlen(building_number) == 0 || building_number[0] == '0')
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    if (is_uint(building_number) != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Building: ");
    status = get_str(&building);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    printf("Apartments number: ");
    status = get_str(&apart_number);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return BAD_ALLOC;
    }
    if (apart_number[0] == '0' || strlen(apart_number) == 0)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    if (is_uint(apart_number) != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Recipient idx: ");
    status = get_str(&idx);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if (is_uint(idx) != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    if (strlen(idx) != 6)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Weight: ");
    status = get_str(&weight);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if (is_udouble(weight) != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    printf("Mail id: ");
    status = get_str(&post_id);
    if (status != OK)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return status;
    }
    if ((is_uint(post_id) != OK) || (strlen(post_id) != 14))
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return INVALID_INPUT;
    }
    time_t timer = time(NULL);
    struct tm *tmp_time = localtime(&timer);
    char* time_field = (char*)malloc(sizeof(char) * 20);
    if (time_field == NULL)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return BAD_ALLOC;
    }
    char* est_time = (char*)malloc(sizeof(char) * 20);
    if (est_time == NULL)
    {
        free_all(8, city, street, building, idx, post_id, building_number, apart_number, weight);
        return BAD_ALLOC;
    }
    sprintf(time_field, "%02d:%02d:%04d %02d:%02d:%02d",
            tmp_time->tm_mday, tmp_time->tm_mon + 1, tmp_time->tm_year + 1900,
            tmp_time->tm_hour, tmp_time->tm_min, tmp_time->tm_sec);
    sprintf(est_time, "%02d:%02d:%04d %02d:%02d:%02d",
            tmp_time->tm_mday, tmp_time->tm_mon + 1, tmp_time->tm_year + 1900,
            tmp_time->tm_hour, tmp_time->tm_min + 1, tmp_time->tm_sec);
    status = create_string(city, &(mail->user.city));
    if (status != OK)
    {
        free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
        return status;
    }

    status = create_string(street, &(mail->user.street));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    mail->user.building_number = atoi(building_number);

    status = create_string(building, &(mail->user.building));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    mail->user.apart_number = atoi(apart_number);

    status = create_string(idx, &(mail->user.idx));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    mail->weight = atof(weight);

    status = create_string(post_id, &(mail->post_id));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    status = create_string(time_field, &(mail->date_of_creation));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    status = create_string(est_time, &(mail->estimated_date_of_sent));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    status = create_string("No", &(mail->is_sent));
    {
        if (status != OK)
        {
            free_all(9, city, street, building, idx, post_id, building_number, apart_number, weight, time_field);
            return status;
        }
    }

    free_all(10, city, street, building, idx, post_id, building_number, apart_number, weight, time_field, est_time);
    return OK;
}

Status compare(Mail mail1, Mail mail2)
{
    if (!compare_strings(mail1.user.idx, mail2.user.idx))
    {
        return my_str_cmp(mail1.user.idx, mail2.user.idx);
    }

    return my_str_cmp(mail1.post_id, mail2.post_id);
}

Status deliver_mail(Mail** mails, int* tmp_amount, int* max_amount)
{
    Mail mail;
    Status status = create_mail(&mail);
    if (status != OK)
    {
        return status;
    }
    if (*tmp_amount - 1 == *max_amount)
    {
        *max_amount *= 2;
        if (my_realloc(mails, *max_amount + 1) != OK)
        {
            free_mail(&mail);
            return BAD_ALLOC;
        }
    }
    if (*tmp_amount == 0)
    {
        (*mails)[*tmp_amount] = mail;
        (*tmp_amount)++;
        return OK;
    }
    int idx = 0;
    int k = compare(mail, (*mails)[idx]);
    while ((k > 0) && (idx < *tmp_amount))
    {
        k = compare(mail, (*mails)[idx]);
        if (k == -1)
        {
            break;
        }
        idx++;
    }
    for (int i = *tmp_amount - 1; i >= idx; --i)
    {
        (*mails)[i+1] = (*mails)[i];
    }
    (*mails)[idx] = mail;
    (*tmp_amount)++;
    return OK;
}

Status print_mail(Mail* mail)
{
    printf("Mail id: %s\n", mail->post_id.str);
    printf("Address of user:\n");
    printf("City: %s\n", mail->user.city.str);
    printf("Street: %s\n", mail->user.street.str);
    printf("Building number: %u\n", mail->user.building_number);
    printf("Building: %s\n", mail->user.building.str);
    printf("Apartment number: %u\n", mail->user.apart_number);
    printf("User post idx: %s\n", mail->user.idx.str);
    printf("Weight of mail: %.4f\n", mail->weight);
    printf("Creation time: %s\n", mail->date_of_creation.str);
    printf("Estimated receiving time: %s\n", mail->estimated_date_of_sent.str);
    printf("Mail is sent: %s\n\n", mail->is_sent.str);
    return OK;
}

Status find_mail(Mail** mails, int tmp_amount, String id, int* idx)
{
    for (int i = 0; i < tmp_amount; ++i)
    {
        Mail mail = (*mails)[i];
        String tmp = mail.post_id;
        if (my_str_cmp(id, tmp) == 0)
        {
            *idx = i;
            return OK;
        }
    }
    return NOT_OK;
}

Status delete_mail(Mail** mails, int* tmp_amount)
{
    printf("Enter the mail id you want to delete\n");
    String mail_id;
    int idx = 0;
    char* m_id = NULL;
    Status status = get_str(&m_id);
    if (status != OK)
    {
        return status;
    }
    status = create_string(m_id, &mail_id);
    if (status != OK)
    {
        free(m_id);
        return status;
    }
    status = find_mail(mails, *tmp_amount, mail_id, &idx);
    if (status != OK)
    {
        free(m_id);
        destroy_string(&mail_id);
        return status;
    }
    free_mail(mails[idx]);
    (*tmp_amount)--;
    for (int i = idx; i < *tmp_amount; ++i)
    {
        (*mails)[i] = (*mails)[i+1];
    }
    free(m_id);
    destroy_string(&mail_id);
    return OK;
}

Status searching(Mail** mails, int tmp_amount)
{
    printf("Enter mail id you want to find\n");
    char* id;
    int idx = 0;
    String mail_id;
    Status status = (get_str(&id));
    if (status != OK)
    {
        return status;
    }
    status = create_string(id, &mail_id);
    if (status != OK)
    {
        free(id);
        return status;
    }
    if (find_mail(mails, tmp_amount, mail_id, &idx) != OK)
    {
        free(id);
        destroy_string(&mail_id);
        return status;
    }
    print_mail(&(*mails)[idx]);
    free(id);
    destroy_string(&mail_id);
    return OK;
}

Status is_sent(Mail** mails, int tmp_amount)
{
    printf("Enter mail id that is sent\n");
    char* id;
    String mail_id;
    int idx = 0;
    Status status = (get_str(&id));
    if (status != OK)
    {
        return status;
    }
    status = create_string(id, &mail_id);
    if (status != OK)
    {
        free(id);
        return status;
    }
    if (find_mail(mails, tmp_amount, mail_id, &idx) != OK)
    {
        free(id);
        destroy_string(&mail_id);
        return status;
    }
    destroy_string(&((*mails)[idx].is_sent));
    status = create_string("Yes", &((*mails)[idx].is_sent));
    if (status != OK)
    {
        free(id);
        destroy_string(&mail_id);
        return status;
    }
    free(id);
    destroy_string(&mail_id);
    return OK;
}


int compare_mails(const void* a, const void* b)
{
    String str1 = ((Mail*)a)->date_of_creation;
    String str2 = ((Mail*)b)->date_of_creation;
    for (int i = 0; i < 4; ++i)
    {
        if (str1.str[i + 6] != str2.str[i + 6])
        {
            return str1.str[i + 6] < str2.str[i + 6] ? 1 : -1;
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        if (str1.str[i+3] != str2.str[i+3])
        {
            return str1.str[i + 3] < str2.str[i + 3] ? 1 : -1;
        }
    }
    return my_str_cmp(str1, str2);
}

int compare_time(String *a, String* b)
{
    for (int i = 0; i < 4; ++i)
    {
        if (a->str[i + 6] != b->str[i + 6])
        {
            return a->str[i + 6] < b->str[i + 6] ? -1 : 1;
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        if (a->str[i+3] != b->str[i+3])
        {
            return a->str[i + 3] < b->str[i + 3] ? -1 : 1;
        }
    }
    return my_str_cmp(*a, *b);
}

Status sort_mails(Mail** mails, int amount)
{
    qsort(*mails, amount, sizeof(String), compare_mails);
    return OK;
}

Status print_all_mails(Mail* mails, int tmp_size)
{
    for (int i = 0; i < tmp_size; ++i)
    {
        print_mail(&(mails[i]));
    }
    return OK;
}

Status print_sent_mails(Mail* mails, int tmp_size)
{
    int amount_of_sent = 0;
    String is_sent;
    Status status = create_string("Yes", &is_sent);
    if (status != OK)
    {
        return status;
    }
    Mail* sent_mails = (Mail*)malloc(sizeof(*mails) * tmp_size);
    if (sent_mails == NULL)
    {
        destroy_string(&is_sent);
        return BAD_ALLOC;
    }
    for (int i = 0; i < tmp_size; ++i)
    {
        Mail tmp = mails[i];
        if (my_str_cmp(tmp.is_sent, is_sent) == 0)
        {
            sent_mails[amount_of_sent] = mails[i];
            amount_of_sent++;
        }
    }
    sort_mails(&sent_mails, amount_of_sent);
    print_all_mails(sent_mails, amount_of_sent);
    destroy_string(&is_sent);
    free(sent_mails);
    return OK;
}

Status print_unsent_mails(Mail* mails, int tmp_size)
{
    int amount_of_sent = 0;
    char* current_time = (char*)malloc(sizeof(char) * 20);
    if (current_time == NULL)
    {
        return BAD_ALLOC;
    }
    time_t timer = time(NULL);
    struct tm *tmp_time = localtime(&timer);
    sprintf(current_time, "%02d:%02d:%04d %02d:%02d:%02d",
            tmp_time->tm_mday, tmp_time->tm_mon + 1, tmp_time->tm_year + 1900,
            tmp_time->tm_hour, tmp_time->tm_min, tmp_time->tm_sec);
    String is_sent;
    Status status = create_string(current_time, &is_sent);
    if (status != OK)
    {   free(current_time);
        return status;
    }
    Mail* unsent_mails = (Mail*)malloc(sizeof(*mails) * tmp_size);
    if (unsent_mails == NULL)
    {
        free(current_time);
        destroy_string(&is_sent);
        return BAD_ALLOC;
    }
    for (int i = 0; i < tmp_size; ++i)
    {
        Mail tmp = mails[i];
        if (compare_time(&is_sent, &(tmp.estimated_date_of_sent)) > 0)
        {
            unsent_mails[amount_of_sent] = tmp;
            amount_of_sent++;
        }
    }
    sort_mails(&unsent_mails, amount_of_sent);
    print_all_mails(unsent_mails, amount_of_sent);
    destroy_string(&is_sent);
    free(unsent_mails);
    free(current_time);
    return OK;
}

int main()
{
    char* tmp = NULL;
    Address post_addr;
    Status fck = create_post_address(&post_addr);
    if (fck != OK)
    {
        errors(fck);
        return fck;
    }
    Post post;
    String exit, add, delete, search, deliver, print_all, print_sent, print_unsent;
    Status st = create_post(&post, post_addr);
    Status st1;
    if (!st)
    {
        st = create_string("exit", &exit);
    }
    if (!st)
    {
        st = create_string("delete", &delete);
    }
    if (!st)
    {
        st = create_string("print_all", &print_all);
    }
    if (!st)
    {
        st = create_string("search", &search);
    }
    if (!st)
    {
        st = create_string("add", &add);
    }
    if (!st)
    {
        st = create_string("deliver", &deliver);
    }
    if (!st)
    {
        st = create_string("print_sent", &print_sent);
    }
    if (!st)
    {
        st = create_string("print_unsent", &print_unsent);
    }
    if (!st)
    {
        printf("Available commands: exit, add, delete, search, deliver, print_all, print_sent, print_unsent\n");
    }
    int running = 1;

    while (running && !st)
    {
        String command;
        printf("Enter your command: ");
        st = get_str(&tmp);
        st1 = create_string(tmp, &command);
        free(tmp);
        if (!st && !st1)
        {
            Status main_status = OK;
            if (compare_strings(command, exit))
            {
                printf("Have a great day!\n");
                running = 0;
            }
            else if (compare_strings(command, add))
            {
                main_status = deliver_mail(&(post.mails), &(post.tmp_size), &(post.max_size));
                if (!main_status)
                {
                    printf("Mail was added\n");
                }
            }
            else if (compare_strings(command, delete))
            {
                main_status = delete_mail(&(post.mails), &(post.tmp_size));
                if (!main_status)
                {
                    printf("Mail was deleted\n");
                }
            }
            else if (compare_strings(command, search))
            {
                main_status = searching(&(post.mails), post.tmp_size);
            }
            else if (compare_strings(command, deliver))
            {
                main_status = is_sent(&(post.mails), post.tmp_size);
                printf("Mail was delivered\n");
            }
            else if (compare_strings(command, print_all))
            {
                print_all_mails(post.mails, post.tmp_size);
            }
            else if (compare_strings(command, print_sent))
            {
                main_status = print_sent_mails(post.mails, post.tmp_size);
            }
            else if (compare_strings(command, print_unsent))
            {
                main_status = print_unsent_mails(post.mails, post.tmp_size);
            }
            else
            {
                printf("Unknown command 0_0\n");
            }
            if (main_status)
            {
                errors(main_status);
            }
            destroy_string(&command);
            printf("\n");
        }
    }
    destroy_string(&exit);
    destroy_string(&add);
    destroy_string(&search);
    destroy_string(&print_all);
    destroy_string(&print_unsent);
    destroy_string(&print_sent);
    destroy_string(&delete);
    destroy_string(&deliver);
    destroy_post(&post);

    if (st)
    {
        errors(st);
        return st;
    }
    if (st1)
    {
        errors(st1);
        return st1;
    }

    return 0;
}
