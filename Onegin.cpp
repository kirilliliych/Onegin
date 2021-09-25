// -------------------------------------------------------
//! @file Onegin.cpp
//! @brief Declarations of functions not related to sorting
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Onegin.h"

void HelpIfNeed(int args_num, const char **args)
{
    assert(args != nullptr);

    for (int check_args = 0; check_args < args_num; ++check_args)
    {
        if (strcmp(args[check_args], "-help") == 0)
        {
            printf("This is a text sorter.\n\n"
                   "Sorts the given file in lexicographical order.\n"
                   "First from the beginning, than from the end.\n"
                   "Both variants are put in one file, after that"
                   " the original text is put there as well.\n\n");
        }
    }
}
int GetFileName(Text *input_text)
{
    assert(input_text != nullptr);

    printf("Enter name of file you want to sort with its extension: ");

    if (!scanf("%256s", input_text->file_name))
    {
        return CHECK_FAILED;
    }
    else
    {
        return CHECK_OK;
    }
}

int TextInput(Text *input_text)
{
    assert(input_text != nullptr);

    if (GetFileName(input_text) == CHECK_FAILED)
    {
        printf("ERROR int TextInput(): file name was not input\n");
        return INPUT_ERROR;
    }

    input_text->file_ptr = fopen(input_text->file_name, "r");

    if (input_text->file_ptr == NULL)
    {
        printf("ERROR int TextInput(): file was not found");
        return INPUT_ERROR;
    }

    input_text->buffer = GetTextFromFile(input_text);
    input_text->lines = PlacePointers(input_text);

    fclose(input_text->file_ptr);

    return 0;
}

size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

char *GetTextFromFile(Text *input_text)
{
    assert(input_text != nullptr);

    input_text->file_size = GetFileSize(input_text->file_ptr) + 2;

    char *buffer = (char*) calloc(input_text->file_size, sizeof(char));

    input_text->file_size = fread(buffer, sizeof(char), input_text->file_size, input_text->file_ptr) + 2;

    buffer[input_text->file_size - 2] = '\n';

    size_t string_counter = 0;

    for (size_t checking_strings = 0; checking_strings < input_text->file_size; ++checking_strings)
    {
        if ((buffer[checking_strings]) == '\n')
        {
            ++string_counter;
        }
    }

    input_text->lines_number = string_counter;

    buffer[input_text->file_size - 1] = '\0';

    return buffer;
}

String *PlacePointers(Text *input_text)
{
    assert(input_text != nullptr);

    struct String *strings = (String *) calloc(input_text->lines_number, sizeof(String));

    char *pointer_end = input_text->buffer;
    size_t counter = 0;

    for (char *pointer_begin = input_text->buffer; *pointer_begin != '\0'; ++pointer_begin)
    {
        if (*pointer_begin == '\n')
        {
            strings[counter].str = pointer_end;
            strings[counter].len = pointer_begin - pointer_end;
            pointer_end = pointer_begin + 1;

            ++counter;
        }
    }

    return strings;
}

bool IsCyrillic(char letter)
{
    return (((letter >= 'À') && (letter <= 'ß')) || ((letter >= 'à') && (letter <= 'ÿ')));
}

int ToLowerCyrillic(char letter)
{
    if ((letter >= 'À') && (letter <= 'ß'))
    {
        return letter + ('à' - 'À');
    }
    else return letter;

}

bool IsLatin(char letter)
{
    return (((letter >= 'A') && (letter <= 'Z')) || ((letter >= 'a') && (letter <= 'z')));
}

int ToLower(char letter)
{
    if (IsLatin(letter))
    {
        return tolower(letter);
    }
    else if (IsCyrillic(letter))
    {
        return ToLowerCyrillic(letter);
    }
    else
    {
        return letter;
    }
}

bool IsLetter(char letter)
{
    return ((IsCyrillic(letter)) || (IsLatin(letter)));
}

void OutputCtor(Text *input_text, char *file_name_output)
{
    strcpy(file_name_output, input_text->file_name);

    int dot_checker = 0;
    int counter = 0;
    const char *end_of_output = "_sorted.txt";

    while (file_name_output[dot_checker] != '.')
    {
        ++dot_checker;
    }

    while (end_of_output[counter] != '\0')
    {
        file_name_output[dot_checker + counter] = end_of_output[counter];
        ++counter;
    }

    file_name_output[dot_checker + counter] = '\0';
}

void PrintText(FILE *file, Text *input_text)
{
    assert(file != nullptr);
    assert(input_text != nullptr);

    for (unsigned long long strings_index = 0; strings_index < input_text->lines_number; ++strings_index)
    {
        for (unsigned long long char_index = 0; char_index <= (input_text->lines)[strings_index].len; ++char_index)
        {
            fputc(*((input_text->lines)[strings_index].str + char_index), file);
        }
    }
}

void PrintBuffer(FILE *file, Text *input_text)
{
    assert(file != nullptr);
    assert(input_text != nullptr);

    fputs(input_text->buffer, file);
}

void SortAndPrint(Text *input_text, int (comparator)(const void *, const void *),
                 void (sorting_function)(void *data, size_t number_of_elements,
                 size_t type_size, int (comparator)(const void *, const void *)),
                 void (output_function)(FILE* file, Text *text),
                 const char *file_name_output, const char *mode, const char *message)
{
    sorting_function(input_text->lines, input_text->lines_number, sizeof(*(input_text->lines)), comparator);

    FILE *output = nullptr;

    if (strcmp(mode, "w") == 0)
    {
        output = fopen(file_name_output, "w");
    }
    else if (strcmp(mode, "a") == 0)
    {
        output = fopen(file_name_output, "a");
    }

    assert(output != nullptr);

    fprintf(output, message);
    output_function(output, input_text);
    fclose(output);
}

void FreeMemory(Text *input_text)
{
    free(input_text->buffer);
    free(input_text->lines);

    input_text->buffer = nullptr;
    input_text->lines  = nullptr;
}
