#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    OK,
    INVALID_INPUT,
    FLAG_R,
    FLAG_A,
    NOT_OK
} Status;

void Usage()
{
    printf("Program makes some magic with txt files depends on flag (r or a)\n");
    printf("Flag /r or -r: filename1.txt filename2.txt fileout.txt\n");
    printf("Flag /a or -a: filename1.txt fileout.txt\n");
}

Status input_checker(int argc, char* argv[])
{
    if (argc < 2)
    {
        return INVALID_INPUT;
    }
    if (strlen(argv[1]) != 2)
    {
        return INVALID_INPUT;
    }
    if (*argv[1] != '-' && *argv[1] != '/')
    {
        return INVALID_INPUT;
    }
    if (argv[1][1] != 'r' && argv[1][1] != 'a')
    {
        return INVALID_INPUT;
    }
    if (argc == 5 && argv[1][1] == 'r')
    {
        return FLAG_R;
    }
    if (argc == 4 && argv[1][1] == 'a')
    {
        return FLAG_A;
    }
    return INVALID_INPUT;
}

Status filename_checker(char* file_in)
{
    int length = strlen(file_in);
    if (length < 5)
    {
        return INVALID_INPUT;
    }
    int counter = 0;
    for (int i = 0; i < length; i++)
    {
        if (file_in[i] == '.')
        {
            counter++;
        }
        if (!isdigit(file_in[i]) && !isalpha(file_in[i]) && file_in[i] != '.')
        {
            return INVALID_INPUT;
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

int is_separated(char ch)
{
    if (ch == ' ' || ch == '\n' || ch == '\t')
    {
        return 1;
    }
    return 0;
}

Status flag_r(FILE* file1, FILE* file2, FILE* file_out)
{
    char ch_1 = ' ';
    char ch_2 = ' ';

    while (is_separated(ch_1))
    {
        ch_1 = getc(file1);
    }
    while (is_separated(ch_2))
    {
        ch_2 = getc(file2);
    }

    int change_input_file;

    if (ch_1 != EOF)
    {
        change_input_file = 1;
    }
    else 
    {
        change_input_file = 0;
    }

    while (ch_1 != EOF || ch_2 != EOF)
    {
        if (change_input_file)
        {
            while (!is_separated(ch_1) && ch_1 != EOF)
            {
                fputc(ch_1, file_out);
                ch_1 = getc(file1);
            }
            while (is_separated(ch_1))
            {
                ch_1 = getc(file1);
            }
        }
        else
        {
            while (!is_separated(ch_2) && ch_2 != EOF)
            {
                fputc(ch_2, file_out);
                ch_2 = getc(file2);
            }
            while (is_separated(ch_2))
            {
                ch_2 = getc(file2);
            }
        }
        if (change_input_file)
        {
            change_input_file = (ch_2 == EOF) ? 1 : 0;

        }
        else
        {
            change_input_file = (ch_1 != EOF) ? 1 : 0;
        }
        if (ch_1 != EOF || ch_2 != EOF)
        {
            fputc(' ', file_out);
        }
    }

    return OK;

}

Status flag_a(FILE* file1, FILE* file_out)
{
    int counter = 1;
    char ch = ' ';
    int result;

    while(is_separated(ch))
    {
        ch = getc(file1);
    }
    while (ch != EOF)
    {
        while (!is_separated(ch) && ch != EOF)
        {
            if (counter % 10 == 0)
            {
                if (isalpha(ch))
                {
                    ch = tolower(ch);
                    for (int i = 3; i >= 0; i--)
                    {
                        result = (ch & (3 << 2 * i)) >> 2 * i;
                        fprintf(file_out, "%d", result);
                    }
                }
                else
                    fputc(ch, file_out);
                
            }
            else if (counter % 2 == 0 && counter % 10 != 0)
                {
                    if(isalpha(ch))
                    {
                        ch = tolower(ch);
                    }
                    fputc(ch, file_out);
                }

            else if (counter % 5 == 0 && counter % 10 != 0)
            {
                for (int i = 2; i >= 0; i--)
                {
                    result = (ch & (7 << 3 * i)) >> 3 * i;
                    fprintf(file_out, "%d", result);
                }
                
            }

            else
            {
                fputc(ch, file_out);
            }

            ch = getc(file1);
        }

        while (is_separated(ch))
        {
            ch = getc(file1);
        }

        counter++;

        if (ch != EOF)
        {
            fputc(' ', file_out);
        }

    }
    return OK;
}



int main(int argc, char* argv[])
{

    switch (input_checker(argc, argv))
    {
    case INVALID_INPUT:
        Usage();
        printf("Invalid input, check entered arguments\n");
        return INVALID_INPUT;
        break;
    case FLAG_R:
        if (filename_checker(argv[2]) == INVALID_INPUT || filename_checker(argv[3]) == INVALID_INPUT || filename_checker(argv[4]) == INVALID_INPUT)
        {
            Usage();
            printf("Incorrect filenames\n");
            return INVALID_INPUT;
        }
        break;
    case FLAG_A:
        if (filename_checker(argv[2]) == INVALID_INPUT || filename_checker(argv[3]) == INVALID_INPUT)
            {
                Usage();
                printf("Incorrect filenames\n");
                return INVALID_INPUT;
            }
            break;
    default:
        break;
    }

    FILE* file1;
    FILE* file2;
    FILE* file_out;

    switch (argv[1][1])
    {
    case 'r':
        if ((file1 = fopen(argv[2], "r")) == NULL)
        {
            printf("file1 cannot be opened\n");
            return NOT_OK;
        }
        if ((file2 = fopen(argv[3], "r")) == NULL)
        {
            printf("file2 cannot be opened\n");
            fclose(file1);
            return NOT_OK;
        }
        if ((file_out = fopen(argv[4], "w")) == NULL)
        {
            printf("file_out cannot be opened\n");
            fclose(file1);
            fclose(file2);
            return NOT_OK;
        }
        if (flag_r(file1, file2, file_out) != OK)
        {
            return NOT_OK;
        }
        fclose(file1);
        fclose(file2);
        fclose(file_out);
        break;
    case 'a':
        if ((file1 = fopen(argv[2], "r")) == NULL)
        {
            printf ("file1 cannot be opened\n");
            return NOT_OK;
        }
        if ((file_out = fopen(argv[3], "w")) == NULL)
        {
            printf("file_out cannot be opened\n");
            fclose(file1);
            return NOT_OK;
        }
        if (flag_a(file1, file_out) != OK)
        {
            return NOT_OK;
        }
        fclose(file1);
        fclose(file_out);
        break;
    }
    
    return 0;
}