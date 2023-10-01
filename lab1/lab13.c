#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    CREATE_ERROR,
    NO_ROOTS,
    NOT_OK
} Errors;

void Usage()
{
    printf("Tretiakov Nikita Mikhailovich M8O-213B-22\n");
    printf("Flag q solves different quadratic equations, contains flag and 4 double arguments, first of which is Eps\n");
    printf("Flag m finds out if the second number is divided by first, contains flag and 2 integer arguments that are not equal to zero\n");
    printf("Flag t finds out if the right triangle exists, contains flag and 4 double arguments, first of which is Eps\n");
}

Errors is_number(char s[])
{
    int cnt = 0;
    if (s[0] != '-')
    {
        for (int i = 0; i < strlen(s); i++)
        {
            if (s[i] == '.')
            {
                cnt++;
            }
            if (s[i] != '.')
            {
                if (!isdigit(s[i]))
                {
                    return INVALID_INPUT;
                }
            }
            if (cnt > 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }

    else
    {
        for (int i = 1; i < strlen(s); i++)
        {
            if (s[i] == '.')
            {
                cnt++;
            }
            if (s[i] != '.')
            {
                if (!isdigit(s[i]))
                {
                    return INVALID_INPUT;
                }
            }
            if (cnt > 1)
            {
                return INVALID_INPUT;
            }
        }
        
    }
    return OK;
}

double char_to_double(char s[])
{
    
    double n = atof(s);
    return n;
}

int char_to_int(char s[])
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            return 0;
        }
    }
    
    int n = atoi(s);
    return n;
}

Errors flag_checker(char *argv[])
{
    if (strlen(argv[1]) != 2)
        {
            return INVALID_INPUT;
        }
    else if (*argv[1] != '-' && *argv[1] != '/')
        {
            return INVALID_INPUT;
        }
    else if (argv[1][1] != 'q' && argv[1][1] != 'm' && argv[1][1] != 't')
        {
            return INVALID_INPUT;
        }
    return OK;
}

Errors arguments_checker(int argc, char* argv[])
{
    if ((argv[1][1] == 'q' && argc != 6))
    {
        return INVALID_INPUT;
    }
    if (argv[1][1] == 'm' && argc != 4)
    {
        return INVALID_INPUT;
    }
    if (argv[1][1] == 't' && argc != 6)
    {
        return INVALID_INPUT;
    }
    return OK;
}

Errors divisible(int a, int b)
{
    if (a % b)
    {
        return NOT_OK;
    }
    return OK;
}

Errors is_a_triange(double Eps, double a, double b, double c)
{
    if (fabs((a * a + b * b) - c * c) < Eps)
    {
        return OK;
    }
    else if (fabs((a * a + c * c) - b * b) < Eps)
    {
        return OK;
    }
    else if (fabs((b * b + c * c) - c * c) < Eps)
    {
        return OK;
    }
    return NOT_OK;
}

Errors permutations(double* array_in, int* the_same, double*** array_out, double Eps, double* x1, double *x2)
{

    double** arr = (double**)malloc(sizeof(double*) * 6);
    if (arr == NULL)
    {
        return CREATE_ERROR;
    }
    int counter = 0;
    for (int i = 0; i < 6; i++)
    {
        arr[i] = (double*)malloc(sizeof(double) * 3);
        if (arr[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(arr[j]);
            }
            free(arr);
            return CREATE_ERROR;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        arr[i][0] = array_in[i/2];
        arr[i][1] = array_in[((i+3)/2) % 3];
        arr[i][2] = array_in[(5 - i)%3];
    }

    for (int i = 0; i < 6; i++)
    {
        the_same[i] = 1;
    }
    
    for (int i = 0; i < 6; i++)
    {
        if (the_same[i] == 0)
        {
            continue;
        }

        for (int j = i+1; j < 6; j++)
        {   
            if (the_same[j] == 0)
            {
                continue;
            }

            for (int k = 0; k < 3; k++)
            {
                if (fabs(arr[i][k] - arr[j][k]) < Eps)
                {
                    counter++;
                }
            }

            if (counter == 3)
            {
                the_same[j] = 0;
            }
            counter = 0; 
        }
             
    }

    
    *array_out = arr;
    return OK;
    
}


int main(int argc, char* argv[])
{
    Usage();
    switch (flag_checker(argv))
    {
    case OK:
        break;
    case INVALID_INPUT:
        printf("Flag consists of \"-\" or \"/\" and a letter\n");
        return 1;
    default:
        printf("Something went wrong :)\n");
        return 2;

    }

    switch (arguments_checker(argc, argv))
    {
    case INVALID_INPUT:
        printf("Check the amount of entered arguments\n");
        return 3;
    case OK:
        break;
    default:
        printf("Something went wrong :)\n");
        return 2;
    }

    switch (argv[1][1])
    {
    case 'q':
    {
        if ((is_number(argv[2]) == INVALID_INPUT) || (is_number(argv[3]) == INVALID_INPUT) || (is_number(argv[4]) == INVALID_INPUT) || (is_number(argv[5]) == INVALID_INPUT))
        {
            printf("Invalid input\n");
            return INVALID_INPUT;
        }
        double Eps = char_to_double(argv[2]);
        if (Eps <= 0)
        {
            printf("Eps must be greater than zero\n");
            return INVALID_INPUT;
        }
        double x1, x2;
        int counter = 0;
        double *array_in = (double*)malloc(sizeof(double) * 3);

        if (array_in == NULL)
        {
            return CREATE_ERROR;
        }
        if (fabs(char_to_double(argv[3])) < Eps)
        {
            printf("Not a quadratic equation\n");
            return INVALID_INPUT;
        }
        array_in[0] = char_to_double(argv[3]);;
        array_in[1] = char_to_double(argv[4]);
        array_in[2] = char_to_double(argv[5]);
        int* the_same = (int*)malloc(sizeof(int) * 6);
        if (the_same == NULL)
        {
            free(array_in);
            return CREATE_ERROR;
        }
        double **array_out;

        switch (permutations(array_in, the_same, &array_out, Eps, &x1, &x2))
        {
            case CREATE_ERROR:
                free(array_in);
                free(the_same);
                printf("Memory error");
                return 4;
            case OK:
            {
                for (int i = 0; i < 6; i++)
                {   
                    if (the_same[i] == 1)
                    {
                        double discriminant = (array_out[i][1] * array_out[i][1]) - 4 * array_out[i][0] * array_out[i][2];
                        if (discriminant < -Eps)
                        {
                            printf("There are no roots in %fx^2 + %fx + %f\n", array_out[i][0], array_out[i][1], array_out[i][2]);
                        }
                        else if (fabs(discriminant) < Eps)
                        {
                            discriminant = 0;
                            printf("The root of %fx^2 + %fx + %f is %f\n", array_out[i][0], array_out[i][1], array_out[i][2], -array_out[i][1]/ (2.0 * array_out[i][0]));
                        }
                        else if (fabs(discriminant) > Eps)
                        {
                            
                            double a = (-array_out[i][1] + sqrt(discriminant));
                            double b = (-array_out[i][1] - sqrt(discriminant));
                            double c = 2.0 * array_out[i][0];
                            
                            printf("The roots of %fx^2 + %fx + %f are %f\tand\t%f\n", array_out[i][0], array_out[i][1], array_out[i][2], a/c, b/c);
                        
                        }
                        

                        
                    }
                }
                break;
            
            }
            default:
                printf("Something went wrong\n");
                break;
        }   
        free(array_in);
        free(array_out);
        free(the_same);
        break;
    }
   // End of case 'q'; 

   case 'm':
    {
        if ((is_number(argv[2]) == INVALID_INPUT) || (is_number(argv[3]) == INVALID_INPUT))
        {
            printf("Invalid input\n");
            return INVALID_INPUT;
        }
        if (char_to_int(argv[2]) == 0 || char_to_int(argv[3]) == 0)
        {
            printf("Wrong input\n");
            return INVALID_INPUT;
        }
        int a = char_to_int(argv[2]);
        int b = char_to_int(argv[3]);
        if (divisible(a,b) == OK)
            {
                printf("Second number is not divided by first\n");
                return OK;
            }
        if (divisible(a,b) == NOT_OK)
        {
            printf("Yes, second number is divided by first\n");
            return OK;
        }
        break;

    }
    //end of case m

    case 't':
    {
        if ((is_number(argv[2]) == INVALID_INPUT) || (is_number(argv[3]) == INVALID_INPUT) || (is_number(argv[4]) == INVALID_INPUT) || (is_number(argv[5]) == INVALID_INPUT))
        {
            printf("Invalid input\n");
            return INVALID_INPUT;
        }

        if (char_to_int(argv[3]) == 0 || char_to_int(argv[4]) == 0 || char_to_int(argv[5]) == 0)
        {
            printf("Not a triangle\n");
            return INVALID_INPUT;
        }

        double Eps = char_to_double(argv[2]);
        double a = char_to_double(argv[3]);
        double b = char_to_double(argv[4]);
        double c = char_to_double(argv[5]);

        if (is_a_triange(Eps, a, b, c) == OK)
        {
            printf("Exists\n");
            return OK;
        }
        else
        {
            printf("Does not exist\n");
            return OK;
        }

        break;
    }

    default:
        printf("Something went wrong\n");
        break;
    }
    
    return 0;
}
