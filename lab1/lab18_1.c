#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

void Usage()
{
    printf("Program for making some magic with files\n");
    printf("Enter filename1.txt fileout.txt\n");
}

typedef enum
{
    OK,
    INVALID_INPUT,
    NOT_OK,
    BAD_ALLOC,
    OVERFLOWED
} Status;

int is_separated(char ch)
{
    return ch == ' ' || ch == '\n' || ch == '\t';
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

int char_to_int(char ch)
{
    if (isdigit(ch))
    {
        return ch - '0';
    }
    if (isalpha(ch))
    {
        return ch - 'A' + 10;
    }
    return -1;
}

Status convert_to_dec(char* string, int base_of_number, int* result)
{
    *result = 0;
    int temp;
    int length = strlen(string);
    for (int i = 0; i < length; i++)
    {
        temp = char_to_int(string[i]);
        if (*result > (INT_MAX - temp) / base_of_number)
        {
            return OVERFLOWED;
        }
        *result *= base_of_number;
        *result += temp;
    }
    return OK;
    
}

Status file_processing(FILE* file_in, FILE* file_out)
{   
    char ch = ' ';
    char min;
    int size = 2;
    int base_of_number;
    int res;
    int current_pos = 0;
    char* string;
    char* temp_string;
    while (is_separated(ch))
    {
        ch = getc(file_in);
    }
    while (ch != EOF)
    {
        min = -1;
        size = 2;
        current_pos = 0;
        string = (char*)malloc(sizeof(char) * size);
        if (string == NULL)
        {
            return BAD_ALLOC;
        }
        
        while (!is_separated(ch) && ch != EOF)
        {
            if (current_pos > size - 2)
            {
                size *= 2;
                temp_string = realloc(string, size);
                if (temp_string == NULL)
                {
                    free(string);
                    return BAD_ALLOC;
                }
                string = temp_string;
            }
            if (!isalnum(ch))
            {
                free(string);
                return INVALID_INPUT;
            }
            if (isalpha(ch))
            {
                ch = toupper(ch);
            }
            if (ch > min)
            {
                min = ch;
            }
            string[current_pos] = ch;
            current_pos++;
            ch = getc(file_in);
        }

        string[current_pos] = '\0';
        int zero_flag = 1;
        int position = 0;

        for (int i = 0; string[i] && zero_flag; i++)
        {
            if (string[i] != '0')
            {
                zero_flag = 0;
            }
            position = i;
        }

        if (zero_flag == 1)
        {
            string[0] = '0';
            string[1] = '\0';
        }
        if (position != 0)
        {   
            for (int i = 0; string[position + i]; i++)
            {
                string[i] = string[i+position];
                string[i+1] = '\0';
            }
        }

        base_of_number = char_to_int(min) + 1;

        if (base_of_number < 2)
        {
            base_of_number = 2;
        }
        if (convert_to_dec(string, base_of_number, &res) != OK)
        {
            free(string);
            return OVERFLOWED;
        }

        fprintf(file_out, "%s %d %d", string, base_of_number, res);
        free(string);

        while (is_separated(ch))
        {
            ch = getc(file_in);
        }
        if (ch != EOF)
        {
            fprintf(file_out, "\n");
        }
    }
    
    return OK;
}

int main(int argc, char* argv[])
{
    Usage();
    
    if (argc != 3)
    {
        printf("Invalid input, check the amount of entered arguments\n");
        return INVALID_INPUT;
    }

    if (filename_checker(argv[1]) != OK || filename_checker(argv[2]) != OK)
    {
        printf("Invalid filename\n");
        return INVALID_INPUT;
    }

    FILE* file_in;
    FILE* file_out;

    if ((file_in = fopen(argv[1], "r")) == NULL)
    {
        printf ("Input file cannot be opened\n");
        return NOT_OK;
    }
    if ((file_out = fopen(argv[2], "w")) == NULL)
    {
        printf("Output file cannot be opened\n");
        fclose(file_in);
        return NOT_OK;
    }
    Status checker = file_processing(file_in, file_out);
    if (checker != OK)
    {   
        fclose(file_in);
        fclose(file_out);
        switch (checker)
        {
        case OVERFLOWED:
            printf("Overflow\n");
            return OVERFLOWED;
        case INVALID_INPUT:
            printf ("Invalid input\n");
            return INVALID_INPUT;
        default:
            printf("Something went wrong, check data \n");
            return NOT_OK;
        }
    }

    fclose(file_in);
    fclose(file_out);
    return 0;
}