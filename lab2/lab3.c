#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef enum
{
    OK,
    NOT_OK
} Status;

Status file_name_info(char* filename)
{
    printf("File: %s\n", filename);
    return  OK;
}

Status substring_info(int position, int num_of_str)
{
    printf("Substring found in line: %d on a position: %d\n", num_of_str, position);
    return  OK;
}

Status is_sub_string(FILE** file, char* str)
{
    int str_len = (int)strlen(str);
    int position = 0;
    int num_of_str = 1;
    int nothing = 1;
    int temp = 0;
    int flag_eof = 0, flag_n = 0;
    char c;

    while ((c = fgetc(*file)) != EOF && flag_eof == 0)
    {
        if (flag_n == 1)
        {
            position = 0;
            num_of_str++;
        }
        temp = 0;
        flag_n = 0;
        if (c == '\n')
        {
            flag_n = 1;
        }
        while (c == str[temp] && temp < str_len)
        {
            c = fgetc(*file);
            if (c == EOF)
            {
                flag_eof = 1;
            }
            temp++;
        }
        if (temp == str_len)
        {
            substring_info(position , num_of_str);
            nothing = 0;
        }
        if (temp > 1)
        {
            fseek(*file, -temp, SEEK_CUR);
            position++;
        }
        else if (temp == 1)
        {
            fseek(*file, -1, SEEK_CUR);
        }
        position++;
    }
    if (nothing == 1)
    {
        return NOT_OK;
    }
    return  OK;
}

Status file_changer(char* string, ...)
{
    va_list(files);
    va_start(files, string);
    va_list(filenames);
    va_start(filenames, string);
    FILE* file;

    while ((file = fopen(va_arg(files, char*), "r")) != NULL)
    {
        char* filename = va_arg(filenames, char*);
        file_name_info(filename);
        if (is_sub_string(&file, string) != OK)
        {
            printf("Nothing found\n");
        }
        fclose(file);
    }
    va_end(filenames);
    return OK;
}
int main()
{
    file_changer("\nabc", "/Users/nikitatretakov/Uni/Labs/MathPracticum/lab3/file1.txt", "/Users/nikitatretakov/Uni/Labs/MathPracticum/lab3/file2.txt");
    return 0;
}
