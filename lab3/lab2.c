#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    BAD_ALLOC,
    NOT_OK
} Status;

typedef struct Vector
{
    double* coords;
} Vector;

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
        case NOT_OK:
            printf("Not OK\n");
            return;
        default:
            printf("Something went wrong\n");
            return;
    }
}

void print_vector(Vector* arr, int size) {
    printf("<");
    for (int i = 0; i < size; i++) {
        printf("%.3lf", arr->coords[i]);
        if (i != size - 1) {
            printf(", ");
        }
    }
    printf(">");
    printf("\n");
}

Status get_inf_norm(double* res, Vector* v, int dimension, int p)
{
    *res = v->coords[0];
    for (int i = 0; i < dimension; ++i)
    {
        *res = fabs(fmax(*res, fabs(v->coords[i])));
    }
    return OK;
}

Status get_golder_norm(double* res, Vector *v, int dimension, int p)
{
    *res = 0;
    if (p < 1)
    {
        return INVALID_INPUT;
    }
    for (int i = 0; i < dimension; ++i)
    {
        double tmp_res = 0;
        tmp_res = pow(fabs(v->coords[i]), p);
        *res += tmp_res;
    }
    *res = pow(*res, 1.0 / p);
    return OK;
}

Status multiply_matrix(double** A, double* x, double** res, int size_of_matrix, int size_of_vector)
{
    if (size_of_matrix != size_of_vector)
    {
        return INVALID_INPUT;
    }
    (*res) = (double*)malloc(sizeof(double) * size_of_vector);
    if (*res == NULL)
    {
        return BAD_ALLOC;
    }
    for (int i = 0; i < size_of_matrix; ++i)
    {
        (*res)[i] = 0.0;
        for (int j = 0; j < size_of_matrix; ++j)
        {
            (*res)[i] += A[i][j] * x[j];
        }
    }
    return OK;
}

Status get_matrix_norm(double* res, Vector* v, int dimension, int p)
{
    double* mult_res = NULL;
    double** A = (double**)malloc(sizeof(double*) * dimension);
    if (A == NULL)
    {
        return BAD_ALLOC;
    }
    for (int i = 0; i < dimension; ++i)
    {
        A[i] = (double*)malloc(sizeof(double) * dimension);
        if (A[i] == NULL)
        {
            for (int j = 0; j < i; ++j)
            {
                free(A[j]);
            }
            free(A);
            return BAD_ALLOC;
        }
    }
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            if (i == j)
            {
                A[i][j] = 1;
            }
            else
            {
                A[i][j] = 0;
            }
        }
    }
    Status mult_status = multiply_matrix(A, v->coords, &mult_res, dimension, dimension);
    if (mult_status == BAD_ALLOC)
    {
        for (int i = 0; i < dimension; ++i)
        {
            free(A[i]);
        }
        free(A);
        free(mult_res);
        return mult_status;
    }
    *res = 0;
    for (int i = 0; i < dimension; ++i)
    {
        *res += mult_res[i] * v->coords[i];
    }
    for (int i = 0; i < dimension; ++i)
    {
        free(A[i]);
    }
    free(A);
    free(mult_res);
    *res = sqrt(*res);
    return OK;
}

Status get_max_vector(int dimension, Vector** max_vectors, double eps, int* size, int p, Status norm(double *res, Vector* v, int dimension, int p), int cnt, ...)
{
    if (1 > cnt || dimension < 1)
    {
        return INVALID_INPUT;
    }
    *size = cnt;
    double max = 0.0;
    (*max_vectors) = (Vector*)malloc(sizeof(Vector) * cnt);
    if (*max_vectors == NULL)
    {
        return BAD_ALLOC;
    }
    va_list ptr;
    va_start(ptr, cnt);
    Status norm_status;
    int max_idx = 0;
    for (int i = 0; i < cnt; ++i)
    {
        Vector tmp = va_arg(ptr, Vector);
        double tmp_length = 0;
        norm_status = norm(&tmp_length, &tmp, dimension, p);
        if (norm_status != OK)
        {
            return norm_status;
        }
        if ((tmp_length - max) > eps)
        {
            max = tmp_length;
            max_idx = 1;
            (*max_vectors)[0] = tmp;
        }
        else if (fabs(tmp_length - max) < eps)
        {
            (*max_vectors)[max_idx++] = tmp;
        }
    }
    *size = max_idx;
    va_end(ptr);
    return OK;
}


int main(int argc, char* argv[])
{
    const double eps = 100;
    Vector* maximums = NULL;
    Vector vector1, vector2, vector3;
    int dimension = 3;
    vector1.coords = (double*)malloc(dimension * sizeof(double));
    vector1.coords[0] = 1.0;
    vector1.coords[1] = 1.0;
    vector1.coords[2] = 1.0;

    vector2.coords = (double*)malloc(dimension * sizeof(double));
    vector2.coords[0] = 2.0;
    vector2.coords[1] = 7.0;
    vector2.coords[2] = 6.0;

    vector3.coords = (double*)malloc(dimension * sizeof(double));
    vector3.coords[0] = 2.0;
    vector3.coords[1] = 7.0;
    vector3.coords[2] = -6.0;

    int cnt = 3;
    int size;
    int p = 2;

    Status status = get_max_vector(dimension, &maximums, eps, &size, p, get_matrix_norm, cnt, vector1, vector2, vector3);
    if (status != OK)
    {
        errors(status);
        free(vector1.coords);
        free(vector2.coords);
        free(vector3.coords);
        free(maximums);
        return status;
    }

    for (int i = 0; i < size; i++)
    {
        Vector tmp = maximums[i];
        print_vector(&tmp, dimension);
    }
    free(vector1.coords);
    free(vector2.coords);
    free(vector3.coords);
    free(maximums);
    return 0;
}