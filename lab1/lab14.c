#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    OK,
    MEMORY_ERROR,
    N_FLAG,
    INVALID_INPUT
}Status;

Status flag_checker(int argc, char* argv[])
{   
    if (argc < 3)
    {
        return INVALID_INPUT;
    }

    if (*argv[1] != '-' && *argv[1] != '/')
    {
        return INVALID_INPUT;
    }

    if (argv[1][1] == 'n')
        {
            if (argc != 4 || (argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' && argv[1][2] != 'a'))
            {
                return INVALID_INPUT;
            }
            return N_FLAG;
        }
    else
        if (argc != 3)
            {
                return INVALID_INPUT;
            }
    return OK;
}

Status filename_checker(char* file_in)
{
    int length = strlen(file_in);
    int counter = 0;
    for (int i = 0; i < length; i++)
    {
        if (file_in[i] == '.')
        {
            counter++;
        }
    }
    if (counter != 1)
    {
        return INVALID_INPUT;
    }

    if (file_in[length - 4] == '.' && file_in[length - 2] == 'x' && file_in[length - 1] == 't' && file_in[length - 3] == 't')
    {
        return OK;
    }
    return INVALID_INPUT;
    
}

char* processing_file_1(char* file_in, char* file_out)
{
    int length = strlen(file_in);
    int i = 0;
    int counter = 0;
    while (file_in[i] != '.' && i < length)
    {
        counter++;
        i++;
    }
    file_out[0] = 'o';
    file_out[1] = 'u';
    file_out[2] = 't';
    file_out[3] = '_';
    for (int i = 0; i < length; i++)
    {
        file_out[i+4] = file_in[i];
    }
    
    return file_out;
    
}

Status flag_d(FILE* file_in, FILE* file_out)
{
    if (file_in == NULL || file_out == NULL)
    {
        return INVALID_INPUT;
    }

    char ch;
    while ((ch = fgetc(file_in)) != EOF)
    {
        if (!isdigit(ch))
        {
            fputc(ch, file_out);
        }
    }
    return OK;

}

Status flag_i(FILE* file_in, FILE* file_out)
{
    if (file_in == NULL || file_out == NULL)
    {
        return INVALID_INPUT;
    }

    char ch;
    int counter = 0;
    while ((ch = fgetc(file_in)) != EOF)
    {
        if (isalpha(ch))
        {
            counter++;
        }
        if (ch == '\n')
        {
            fputc(counter + '0', file_out);
            fputc('\n', file_out);
            counter = 0;
        }
    }
    fputc(counter + '0', file_out);
    return OK;
}

Status flag_s(FILE* file_in, FILE* file_out)
{
    if (file_in == NULL || file_out == NULL)
    {
        return INVALID_INPUT;
    }

    char ch;
    double counter = 0;
    while ((ch = fgetc(file_in)) != EOF)
    {
        
        if (!isdigit(ch) && !isalpha(ch) && ch != ' ' && ch != '\n' && ch >= 0)
        {
            counter++;
        }
        if (ch < 0)
        {
            counter += 0.5;
        }
        if (ch == '\n')
        {
            fputc(counter + '0', file_out);
            fputc('\n', file_out);
            counter = 0;
        }
    }
    fputc(counter + '0', file_out);
    return OK;
}

Status flag_a(FILE* file_in, FILE* file_out)
{
    if (file_in == NULL || file_out == NULL)
    {
        return INVALID_INPUT;
    }

    char ch;
    while ((ch = fgetc(file_in)) != EOF)
    {
        if (!isdigit(ch) && ch != '\n')
        {
            fprintf(file_out, "%X", ch);
        }
        if (isdigit(ch))
        {
            fputc(ch, file_out);
        }
        if (ch == '\n')
        {
            fputc('\n', file_out);
        }
    }
    return OK;
    
}

int main(int argc, char* argv[])
{
    Status flag = flag_checker(argc, argv);
    if (flag == INVALID_INPUT)
    {
        printf("Invalid input, check the amount of entered arguments\n");
        return INVALID_INPUT;
    }
    char* filename_in = argv[2];
    if (filename_checker(filename_in) == INVALID_INPUT)
        {
            printf("Incorrect filename\n");
            return INVALID_INPUT;
        }
    int length_filename_1 = strlen(filename_in);
    char* filename_out;
    int another_name = 0;


    switch (flag)
    {
    case OK:
        //-------Creating an _out.txt file-------//
        another_name = 1;
        filename_out = (char*)malloc(sizeof(char) * (length_filename_1 + 5));
        if (filename_out == NULL)
        {
            return MEMORY_ERROR;
        }
        filename_out[-1] = '\0';
        filename_out = processing_file_1(filename_in, filename_out);
        //----------Done----------//
        
        break;
    case N_FLAG:
        filename_out = argv[3];
        if (filename_checker(filename_in) == INVALID_INPUT || filename_checker(filename_out) == INVALID_INPUT)
        {
            printf("Incorrect filename\n");
            return INVALID_INPUT;
        }
        break;
    case INVALID_INPUT:
        printf("Invalid input\n");
        return 1;

    default:
        printf("Something went wrong\n");
        return 2;
    }

    FILE* file_in;
    FILE* file_out;

    if ((file_in = fopen(filename_in, "r")) == NULL)
    {
        printf("Input file cannot be opened\n");
    }

    else if ((file_out = fopen(filename_out, "w")) == NULL)
    {
        fclose(file_in);
        printf("Output file cannot be opened\n");
    }

    if (another_name)
    {
        switch (argv[1][1])
        {
        case 'd':
            flag_d(file_in, file_out);
            break;
        case 'i':
            flag_i(file_in, file_out);
            break;
        case 's':
            flag_s(file_in, file_out);
            break;
        case 'a':
            flag_a(file_in, file_out);
            break;
        default:
            break;
        }
    }

    switch (argv[1][2])
    {
    case 'd':
        flag_d(file_in, file_out);
        break;
    case 'i':
        flag_i(file_in, file_out);
        break;
    case 's':
        flag_i(file_in, file_out);
        break;
    case 'a':
        flag_a(file_in, file_out);
        break;
    default:
        break;
    }

    if (another_name)
    {
        free(filename_out);
    }

    fclose(file_in);
    fclose(file_out);
    return 0;
}