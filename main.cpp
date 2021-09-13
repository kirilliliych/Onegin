#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct String
{
    char *str;
    size_t len;
};

struct Text
{
    char *buffer;
    struct String *lines;
    size_t lines_number;
    size_t file_size;
};

void TextInput(FILE *file, struct Text *input_text);
char *CreateBuffer(FILE *file, size_t *lines_number, size_t *file_size);
size_t GetFileSize(FILE *file);
struct String *PlacePointers(char *buffer, size_t lines_number);

int DirectComparator(const void *first_string, const void *second_string);
int ReversedComparator(const void *first_string, const void *second_string);

int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "ru_RU.cp1251");

    FILE *input = fopen(argv[1], "r");
    if(input == nullptr)
    {
         printf("ERROR, the file was not found");

         return -1;
    }

    struct Text input_text;

    TextInput(input, &input_text);
    fclose(input);

    //qsort();

    FILE *output = fopen("Text_sorted.txt", "w");
    assert(output != nullptr);

    fprintf(output, "Directly sorted text\n\n");
    //TextPrint(output);
    fclose(output);

   // QuickSort();

    output = fopen("Text_sorted.txt", "a");
    assert(output != nullptr);

    fprintf(output, "\nReverse sorted text\n\n");
//    TextPrint(output);

    fprintf(output, "\n Original text\n\n");
 //   PrintBuffer(input_text.buffer);
    fclose(output);

    return 0;
}

void TextInput (FILE *file, struct Text *input_text)
{
    assert (file != nullptr);
    assert (input_text != nullptr);

    input_text->buffer = CreateBuffer(file, &input_text->lines_number, &input_text->file_size);
    input_text->lines = PlacePointers(input_text->buffer, input_text->lines_number);

}

size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

char *CreateBuffer(FILE *file, size_t *lines_number, size_t *file_size)
{
    assert (file != nullptr);
    assert (lines_number != nullptr);
    assert (file_size != nullptr);
                                                               // и что тут нужно сделать ++
    *(file_size)++ = GetFileSize(file);
    rewind(file);

    char *buffer = (unsigned char*) calloc(*file_size, sizeof(char));

    fread(buffer, sizeof(char), *file_size, file);
    *(buffer + (int)(*file_size)) = '\0';                              // не уверен, что в св€зи с ниженаписанным нужна эта строчка

    size_t string_counter = 0;

    for (size_t counting_strings = 0; counting_strings < *file_size; ++counting_strings)
    {
        if (*(buffer + counting_strings) == '\n')
        {
            ++string_counter;
        }
    }

    *lines_number = string_counter;
    *file_size -= string_counter;                             // warning!!! поправка на размер файла сделана в предположении, что '/r' в конце строки при считывании с помощью fread удал€ютс€, поэтому необходимо из размера файла вычесть количество символов равное количеству строк в файле
    *(buffer + (int)(*file_size)) = '\0';                             // в том же самом предположении дл€ нахождени€ конца "полезного" буфера

    return buffer;
}

struct String *PlacePointers(char *buffer, size_t lines_number)
{
    assert(buffer != nullptr);

    struct String *strings = (struct String*) calloc(lines_number, sizeof(struct String));

    char *pointer_begin = buffer;
    char *pointer_end = buffer;
    size_t counter = 0;

    while (*pointer_begin != '\0')
    {
        while (*pointer_begin != '\n')
        {
            ++pointer_begin;
        }
        (strings + counter)->str = pointer_end;
        (strings + counter)->len = pointer_begin - pointer_end;

        pointer_end = pointer_begin + 1;
        ++pointer_begin;
    }

    return strings;
}

int DirectComparator (const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    unsigned char *first_string_begin  = (((struct String*) first_string) ->str);
    unsigned char *first_string_end    = (((struct String*) first_string) ->str) + (((struct String*) first_string) ->len);
    unsigned char *second_string_begin = (((struct String*) second_string)->str);
    unsigned char *second_string_end   = (((struct String*) second_string)->str) + (((struct String*) second_string)->len);

    while ((!isalpha((int) *first_string_begin))  && (*first_string_begin  != *first_string_end))
    {
        ++first_string_begin;
    }

    while ((!isalpha((int) *second_string_begin)) && (*second_string_begin != *second_string_end))
    {
        ++second_string_begin;
    }

    while ((*first_string_begin == *second_string_begin) && (*first_string_begin != *first_string_end) && (*second_string_begin != *second_string_end))
    {
        ++first_string_begin;
        ++second_string_begin;

        while ((!isalpha((int) *first_string_begin))  && (*first_string_begin  != ' ') && (*first_string_begin != *first_string_end))
        {
            ++first_string_begin;
        }

        while ((!isalpha((int) *second_string_begin)) && (*second_string_begin != ' ') && (*second_string_begin != *second_string_end))
        {
            ++second_string_begin;
        }
    }

    return (((int) *first_string_begin) - ((int) *second_string_begin));
}

int ReversedComparator (const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    unsigned char *first_string_begin  = (((struct String*) first_string) ->str);
    unsigned char *first_string_end    = (((struct String*) first_string) ->str) + (((struct String*) first_string) ->len);
    unsigned char *second_string_begin = (((struct String*) second_string)->str);
    unsigned char *second_string_end   = (((struct String*) second_string)->str) + (((struct String*) second_string)->len);

    while ((!isalpha((int) *first_string_end))  && (*first_string_begin  != *first_string_end))
    {
        --first_string_end;
    }

    while ((!isalpha((int) *second_string_end)) && (*second_string_begin != *second_string_end))
    {
        --second_string_end;
    }

    while ((*first_string_end == *second_string_end) && (*first_string_begin != *first_string_end) && (*second_string_begin != *second_string_end))
    {
        --first_string_end;
        --second_string_end;

        while ((!isalpha((int) *first_string_end))  && (*first_string_end  != ' ') && (*first_string_begin != *first_string_end))
        {
            --first_string_end;
        }

        while ((!isalpha((int) *second_string_end)) && (*second_string_end != ' ') && (*second_string_begin != *second_string_end))
        {
            --second_string_end;
        }
    }

    return (((int) *first_string_end) - ((int) *second_string_end));
}
