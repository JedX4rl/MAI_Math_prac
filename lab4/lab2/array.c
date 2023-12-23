//
// Created by Никита Третьяков on 21.12.2023.
//

#include "array.h"

Status create_array(Array** arr, char ch)
{
    if (arr == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (!isalpha(ch))
    {
        return INVALID_ARGUMENT;
    }
    *arr = (Array*)malloc(sizeof(Array));
    if ((*arr) == NULL)
    {
        return BAD_ALLOC;
    }
    (*arr)->name = toupper(ch);
    (*arr)->size = 0;
    (*arr)->capacity = 256;
    (*arr)->data = (int*)malloc(sizeof(int) * (*arr)->capacity);
    if ((*arr)->data == NULL)
    {
        free(*arr);
        return BAD_ALLOC;
    }
    return OK;
}

Status add_item(Array** arr, char name, int number)
{
    if (*arr == NULL)
    {
        Status st = create_array(arr, name);
        if (st)
        {
            return st;
        }
    }
    (*arr)->data[(*arr)->size] = number;
    (*arr)->size++;
    if ((*arr)->size + 1 == (*arr)->capacity)
    {
        int* tmp = realloc((*arr)->data, sizeof(int) * (*arr)->capacity * 2);
        if (tmp == NULL)
        {
            return BAD_ALLOC;
        }
        (*arr)->data = tmp;
        (*arr)->capacity *= 2;
    }
    return OK;
}

void free_array(Array* arr)
{
    if (arr == NULL)
    {
        return;
    }
    free(arr->data);
    free(arr);
}

Status delete_items(Array* arr, int idx, int cnt)
{
    if (cnt < 1 || idx > arr->size || arr == NULL)
    {
        return INVALID_ARGUMENT;
    }
    int size = arr->size;
    for (int i = idx; i < size; ++i)
    {
        if (i + cnt < size)
        {
            arr->data[i] = arr->data[i+cnt];
        }
    }
    int* tmp = NULL;
    if (cnt < size)
    {
        tmp = realloc(arr->data, sizeof(int) * (size - cnt));
        if (tmp == NULL)
        {
            return BAD_ALLOC;
        }
        arr->size -= cnt;
    }
    else
    {
        tmp = realloc(arr->data, sizeof(int) * idx);
        if (tmp == NULL)
        {
            return BAD_ALLOC;
        }
        arr->size = idx;
    }
    arr->data = tmp;
    return OK;
}

Status save(Array* arr, const char* filename)
{
    if (arr == NULL || filename == NULL)
    {
        return INVALID_ARGUMENT;
    }
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        return OPENING_ERROR;
    }
    for (int i = 0; i < arr->size; ++i)
    {
        fprintf(file, "%d ", arr->data[i]);
    }
    fclose(file);
    return OK;
}

Status load(Array** arr, char name, const char* filename)
{
    if (filename == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (*arr == NULL)
    {
        Status st = create_array(arr, name);
        if (st)
        {
            return st;
        }
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return OPENING_ERROR;
    }
    int number = 0;
    while (fscanf(file, "%d ", &number) == 1)
    {
        Status st = add_item(arr, name, number);
        if (st)
        {
            free_array(*arr);
            return st;
        }
    }
    fclose(file);
    return OK;
}

Status concatenation(Array* a, Array* b)
{
    if (b == NULL || b->size == 0)
    {
        return INVALID_ARGUMENT;
    }
    int size = b->size;
    for (int i = 0; i < size; ++i)
    {
        Status st = add_item(&a, a->name, b->data[i]);
        if (st)
        {
            free_array(a);
            free_array(b);
            return st;
        }
    }
    return OK;
}

void stats(Array* a)
{
    if (a == NULL)
        return;
    printf("Array %c\n", a->name);
    printf("Size %d\n", a->size);
    if (!a->size)
    {
        return;
    }
    int size = a->size;
    int max_cnt = 0;
    int most_common = a->data[0];
    for (int i = 1; i < size; ++i)
    {
        int cnt = 1;
        for (int j = i + 1; j < size; ++j)
        {
            if (a->data[i] == a->data[j])
            {
                cnt++;
            }
            if (cnt > max_cnt)
            {
                max_cnt = cnt;
                most_common = a->data[i];
            }
            else if (cnt == max_cnt)
            {
                most_common = max(most_common, a->data[i]);
            }
        }
    }
    printf("Most common: %d\n", most_common);
    int sum = a->data[0];
    int max_element = a->data[0];
    int min_element = a->data[0];
    int min_idx = 0;
    int max_idx = 0;
    for (int i = 1; i < size; ++i)
    {
        sum += a->data[i];
        if (a->data[i] > max_element)
        {
            max_element = a->data[i];
            max_idx = i;
        }
        if (a->data[i] < min_element)
        {
            min_element = a->data[i];
            min_idx = i;
        }
    }
    double res = sum / size;
    printf("Max: %d, index: %d\n", max_element, max_idx);
    printf("Min: %d, index: %d\n", min_element, min_idx);
    printf("Mean: %lf\n", res);
    double max_sigma = 0;
    for (int i = 0; i < size; ++i)
    {
        double curr = fabs(a->data[i] - max_sigma);
        if (curr - max_sigma > 1e-12)
        {
            max_sigma = curr;
        }
    }
    printf("Sigma: %lf\n", max_sigma);
    printf("\n");
}

Array* find_array(Array** storage, int size, char name)
{
    if (storage == NULL)
    {
        return NULL;
    }
    int left = 0;
    int right = size - 1;
    while (left <= right)
    {
        int middle = (left + right) / 2;
        char curr = storage[middle]->name;
        if (curr < name)
        {
            left = middle + 1;
        }
        else if (curr > name)
        {
            right = middle - 1;
        }
        else
        {
            return storage[middle];
        }
    }
    return NULL;
}

Status Free(Array* array)
{
    if (array == NULL)
    {
        return OK;
    }
    free(array->data);
    array->data = (int*)malloc(sizeof(int) * 256);
    if (array->data == NULL)
    {
        return BAD_ALLOC;
    }
    array->size = 0;
    return OK;
}

void print(Array* array, char name, char* mode)
{
    if (array == NULL || mode == NULL)
    {
        return;
    }
    if (!strcmp(mode, "all"))
    {
        for (int i = 0; i < array->size; ++i)
        {
            printf("%d ", array->data[i]);
        }
        printf("\n");
    }
    else
    {
        int left, right;
        int read = sscanf(mode, "%d %d", &left, &right);
        if (read != 2)
        {
            read = sscanf(mode, "%d", &left);
            if (read != 1)
            {
                return;
            }
            printf("%d ", array->data[left]);
        }
        else
        {
            if (left > right || left > array->size || right > array->size)
            {
                return;
            }
            for (int i = left; i <= right; ++i)
            {
                printf("%d ", array->data[i]);
            }
            printf("\n");
        }
    }
}

Status copy(Array* a, int start, int end, Array** b)
{
    if (a == NULL || *b == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (start > a->size)
    {
        return INVALID_INPUT;
    }
    int upper = min(a->size, end);
    for (int i = start; i < upper; ++i)
    {
        Status st = add_item(b, (*b)->name, a->data[i]);
        if (st)
        {
            return st;
        }
    }
    return OK;
}

int compare_to_max(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}
int compare_to_min(const void* a, const void* b)
{
    return (*(int*)b - *(int*)a);
}
int my_rand(int min, int max)
{
    return min + rand() % (max - min + 1);
}
int random_compare(const void* a, const void* b)
{
    return rand() % 3 - 1;
}

Status rand_array(Array** array, char name, char* args)
{
    if (args == NULL || *args == '\0')
    {
        return INVALID_ARGUMENT;
    }
    if (*array == NULL)
    {
        Status st = create_array(array, name);
    }
    int cnt, lb, rb;
    int read = sscanf(args, "%d %d %d", &cnt, &lb, &rb);
    if (read != 3)
    {
        free(*array);
        return INVALID_INPUT;
    }
    for (int i = 0; i < cnt; ++i)
    {
        int number = my_rand(lb, rb);
        Status st = add_item(array, name, number);
        if (st)
        {
            free(*array);
            return st;
        }
    }
    return OK;
}

void free_storage(Array** storage, int size)
{
    if (storage == NULL)
    {
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        free_array(storage[i]);
    }
    free(storage);
}

void sort(Array* a, char mode)
{
    if (a == NULL)
    {
        return;
    }
    if (mode == '+')
    {
        qsort(a->data, a->size, sizeof(int), compare_to_max);
    }
    else if (mode == '-')
    {
        qsort(a->data, a->size, sizeof(int), compare_to_min);
    }
    else
    {
        return;
    }
}

void Shuffle(Array* a)
{
    if (a == NULL)
    {
        return;
    }
    qsort(a->data, a->size, sizeof(int), random_compare);
}

Status add_to_storage(Array** storage, Array* a, int* capacity)
{
    if (storage == NULL || a == NULL || capacity == NULL)
    {
        return INVALID_ARGUMENT;
    }
    if (*capacity == 0)
    {
        storage[*capacity] = a;
        (*capacity)++;
    }
    else
    {
        int idx = 0;
        while (idx < *capacity && a->name > storage[idx]->name)
        {
            idx++;
        }
        for (int i = *capacity; i > idx; --i)
        {
            storage[i] = storage[i - 1];
        }
        storage[idx] = a;
        (*capacity)++;
    }
    return OK;
}

