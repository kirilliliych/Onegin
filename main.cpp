#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

const int SMALLER = -1;                                              // means first string is lexicografically smaller than second
const int BIGGER  =  1;                                              // means first string is lexicografically bigger  than second
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

void TextInput(FILE *file, Text *input_text);
char *CreateBuffer(FILE *file, size_t *lines_number, size_t *file_size);
size_t GetFileSize(FILE *file);
struct String *PlacePointers(char *buffer, size_t lines_number);
void Swap(void *value_1, void *value_2, size_t type_size);
bool IsCyrillic (char letter);
void QuickSort(void *data, size_t lines_number, size_t type_size, int (*comparator)(const void *, const void *));
void PrintText(FILE *file, Text *input_text);
void PrintBuffer(FILE *file, Text *input_text);
int DirectComparator(const void *first_string, const void *second_string);
int ReverseComparator(const void *first_string, const void *second_string);
void FreeMemory(struct Text *input_text);

int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "ru_RU.cp1251");

    FILE *input = fopen("Onegin.txt", "r");
    if(input == nullptr)
    {
         printf("ERROR, the file was not found");

         return -1;
    }

    struct Text input_text;

    TextInput(input, &input_text);
    fclose(input);
    printf("%d\n", __LINE__);
    qsort(input_text.lines, input_text.lines_number, sizeof(struct String), DirectComparator);
    printf("%d\n", __LINE__);
    //QuickSort(input_text.lines, input_text.lines_number, sizeof(struct String), DirectComparator);
    FILE *output = fopen("Text_sorted.txt", "w");
    assert(output != nullptr);

    fprintf(output, "Directly sorted text\n\n");
    PrintText(output, &input_text);
    fclose(output);

    //QuickSort(&input_text.lines, input_text.lines_number, sizeof(struct String), ReverseComparator);
    qsort(input_text.lines, input_text.lines_number, sizeof(struct String), ReverseComparator);

    output = fopen("Text_sorted.txt", "a");
    assert(output != nullptr);

    fprintf(output, "\nReverse sorted text\n\n");
    PrintText(output, &input_text);

    fprintf(output, "\nOriginal text\n\n");
    PrintBuffer(output, &input_text);
    fclose(output);

    FreeMemory(&input_text);

    return 0;
}

void TextInput(FILE *file, Text *input_text)
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

    *file_size = GetFileSize(file) + 1;
    rewind(file);

    char *buffer = (char*) calloc(*file_size, sizeof(char));

    fread(buffer, sizeof(char), *file_size, file);

    size_t string_counter = 0;

    for (size_t checking_strings = 0; checking_strings < *file_size; ++checking_strings)
    {
        if (*(buffer + checking_strings) == '\n')
        {
            ++string_counter;
        }

    }

    string_counter++;
    *file_size -= string_counter;
    *lines_number = string_counter;
    *(buffer + *file_size) = '\0';

    return buffer;
}

struct String *PlacePointers(char *buffer, size_t lines_number)
{
    assert(buffer != nullptr);

    struct String *strings = (struct String *) calloc(lines_number, sizeof(struct String));

    char *pointer_begin = buffer - 1;
    char *pointer_end = buffer;
    size_t counter = 0;

    while (*pointer_begin != '\0')
    {
        ++pointer_begin;

        while ((*pointer_begin != '\n') && (*pointer_begin != '\0'))
        {
            ++pointer_begin;
        }
        (strings + counter)->str = pointer_end;
        (strings + counter)->len = pointer_begin - pointer_end;

        ++counter;
        pointer_end = pointer_begin + 1;
    }

    *pointer_begin = '\n';

    return strings;
}

int DirectComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);
    char *first_string_begin  = (((struct String *) first_string) ->str);
    char *second_string_begin = (((struct String *) second_string)->str);

    while (!IsCyrillic(*first_string_begin)  && !isalpha(*first_string_begin)  && (*first_string_begin  != '\n'))
    {
        ++first_string_begin;
    }

    while (!IsCyrillic(*second_string_begin) && !isalpha(*second_string_begin) && (*second_string_begin != '\n'))
    {

        ++second_string_begin;
    }

    while ((*first_string_begin == *second_string_begin) && (*first_string_begin != '\n') && (*second_string_begin != '\n'))
    {
        ++first_string_begin;
        ++second_string_begin;

        while (!IsCyrillic(*first_string_begin)  && !isalpha(*first_string_begin)  && (*first_string_begin  != '\n'))
        {
            ++first_string_begin;
        }

        while (!IsCyrillic(*second_string_begin) && !isalpha(*second_string_begin) && (*second_string_begin != '\n'))
        {
            ++second_string_begin;
        }
    }

    return (((int) *first_string_begin) - ((int) *second_string_begin));
}

int ReverseComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    char *first_string_begin  = (((struct String *) first_string) ->str);
    char *first_string_end    = (((struct String *) first_string) ->str) + (((struct String *) first_string) ->len) - 1;
    char *second_string_begin = (((struct String *) second_string)->str);
    char *second_string_end   = (((struct String *) second_string)->str) + (((struct String *) second_string)->len) - 1;

    while (!IsCyrillic(*first_string_end)  && !isalpha(*first_string_end)  && (first_string_end  != first_string_begin))
    {
        --first_string_end;
    }

    while (!IsCyrillic(*second_string_end) && !isalpha(*second_string_end) && (second_string_end != second_string_begin))
    {
        --second_string_end;
    }

    while ((*first_string_end == *second_string_end) && (first_string_end != first_string_begin) && (second_string_end != second_string_begin))
    {
        --first_string_end;
        --second_string_end;

        while (!IsCyrillic(*first_string_end)  && !isalpha(*first_string_end)  && (first_string_end  != first_string_begin))
        {
            --first_string_end;
        }

        while (!IsCyrillic(*second_string_end) && !isalpha(*second_string_end) && (second_string_end != second_string_begin))
        {
            --second_string_end;
        }
    }

    if (first_string_end == first_string_begin)
    {
        if (second_string_end != second_string_begin)
        {
            return SMALLER;
        }
        else
        {
            if (*first_string_end == *second_string_end)
            {
                return 0;
            }
            else
            {
                return (((int) *first_string_end) - ((int) *second_string_end));
            }
        }
    }

    if (*second_string_end == *second_string_begin)
    {
        return BIGGER;
    }

    return (((int) *first_string_end) - ((int) *second_string_end));
}

void Swap(void *value_1, void *value_2, size_t type_size)
{
    assert(value_1 != nullptr);
    assert(value_2 != nullptr);

    unsigned long long temp = 0;
    size_t counter = 0;

    for (counter = 0; counter < type_size / sizeof(unsigned long long); ++counter)
    {
        temp = *((unsigned long long *) value_1 + counter);
        *((unsigned long long *) value_1 + counter) = *((unsigned long long *) value_2 + counter);
        *((unsigned long long *) value_2 + counter) = temp;
    }

    if (counter * sizeof(unsigned long long) != type_size)
    {
        char byte = 0;

        for (size_t element = 0; element < type_size - counter * sizeof(unsigned long long); ++element)
        {
            byte = *((char *) value_1 + counter + element);
            *((char *) value_1 + counter + element) = *((char *) value_2 + counter + element);
            *((char *) value_2 + counter + element) = byte;
        }
    }
}

bool IsCyrillic(char letter)
{
    return ((letter >= 'À') && (letter <= 'ÿ'));
}

void QuickSort(void *data, size_t lines_number, size_t type_size, int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    size_t left = 0;
    size_t right = lines_number - 1;
    size_t middle_element_displacement = type_size * lines_number / 2;

    while (left <= right)
    {

        printf("%d\n", __LINE__);
        while (comparator(((char *) data + type_size * left),  ((char *) data + middle_element_displacement)) < 0)
        {
            ++left;
        }
        printf("%d\n", __LINE__);
        while (comparator(((char *) data + type_size * right), ((char *) data + middle_element_displacement)) > 0)
        {
            --right;
        }
        printf("%d\n", __LINE__);
        if (left <= right)
        {
            Swap(((char *) data + type_size * left), ((char *) data + type_size * right), type_size);

            ++left;
            --right;
        }
    }
    printf("%d\n", __LINE__);
    if (right > 0)
    {
        QuickSort((char *) data, right + 1, type_size, comparator);
    }

    if (left < lines_number)
    {
        QuickSort(((char *) data + type_size * left), lines_number - left, type_size, comparator);
    }

}

void PrintText(FILE *file, Text *input_text)
{
    assert(file != nullptr);
    assert(input_text != nullptr);

    for (size_t strings_index = 0; strings_index < input_text->lines_number; ++strings_index)
    {
        for (size_t char_index = 0; char_index <= (input_text->lines)[strings_index].len; ++char_index)
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

void FreeMemory(struct Text *input_text)
{
    free(input_text->buffer);
    free(input_text->lines);

    input_text->buffer = nullptr;
    input_text->lines =  nullptr;
}

