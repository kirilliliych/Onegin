#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define test printf("%d\n", __LINE__);
const int SMALLER = -1;                                              // means first string is lexicografically smaller than second
const int BIGGER  =  1;                                              // means first string is lexicografically bigger  than second
const char INPUT_FILE_1[]  = "Onegin.txt";
const char INPUT_FILE_2[]  = "Hamlet.txt";
const char OUTPUT_FILE_1[] = "Onegin_sorted.txt";
const char OUTPUT_FILE_2[] = "Hamlet_sorted.txt";

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

int TextInput(Text *input_text);
char *GetTextFromFile(FILE *file, Text *input_text);
size_t GetFileSize(FILE *file);
struct String *PlacePointers(Text *input_text);
void Swap(void *value_1, void *value_2, size_t type_size);
bool IsCyrillic(char letter);
bool IsLatin(char letter);
bool IsLetter(char letter);
void QuickSort(void *data, size_t lines_number, size_t type_size, int (*comparator)(const void *, const void *));
void PrintText(FILE *file, Text *input_text);
void PrintBuffer(FILE *file, Text *input_text);
int DirectComparator(const void *first_string, const void *second_string);
int ReverseComparator(const void *first_string, const void *second_string);
void FreeMemory(Text *input_text);
int sortingnumbers(const void *num1, const void *num2);
int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "RUS");

    struct Text input_text = {0,0,0,0};

    if (TextInput(&input_text) == -1)
    {
        return -1;
    }

    QuickSort(input_text.lines, input_text.lines_number, sizeof(String), DirectComparator);
    //qsort(input_text.lines, input_text.lines_number, sizeof(String), DirectComparator);
    int mas[10] = {10,9,8,7,6,5,4,3,2,1};
    /*int a = 5;
    int b = 10;
    Swap(&a, &b, sizeof(int));
    printf("%d, %d\n", a, b);
    */
    /*QuickSort(mas, 10, sizeof(int), sortingnumbers);
    for(int i = 0; i < 10; i++)
    {
        printf("%d ", mas[i]);
    }
    printf("\n"); */
    FILE *output = fopen("Text_sorted.txt", "w");
    assert(output != nullptr);

    fprintf(output, "Directly sorted text\n\n");
    PrintText(output, &input_text);
    fclose(output);
    QuickSort(&input_text.lines, input_text.lines_number, sizeof(String), ReverseComparator);
    //qsort(input_text.lines, input_text.lines_number, sizeof(String), ReverseComparator);

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

int TextInput(Text *input_text)
{
    assert (input_text != nullptr);

    FILE *input_from_file = fopen(INPUT_FILE_1, "r");

    if (input_from_file == nullptr)
    {
        printf("ERROR: file was not found");
        return -1;
    }

    input_text->buffer = GetTextFromFile(input_from_file, input_text);
    input_text->lines = PlacePointers(input_text);

    fclose(input_from_file);

    return 0;
}

size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

char *GetTextFromFile(FILE *file, Text *input_text)
{
    assert (file != nullptr);
    assert (input_text != nullptr);

    input_text->file_size = GetFileSize(file) + 2;

    char *buffer = (char*) calloc(input_text->file_size, sizeof(char));

    input_text->file_size = fread(buffer, sizeof(char), input_text->file_size, file) + 2;

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

int DirectComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    char *first_string_begin  = (((String *) first_string) ->str);
    char *second_string_begin = (((String *) second_string)->str);

    while ((*first_string_begin != '\n') && (*second_string_begin != '\n'))
    {

        if (!IsLetter(*first_string_begin))
        {
            ++first_string_begin;
        }

        if (!IsLetter(*second_string_begin))
        {
            ++second_string_begin;
        }

        if ((IsLetter(*first_string_begin)) && (IsLetter(*second_string_begin)) && (*first_string_begin == *second_string_begin))
        {
            ++first_string_begin;
            ++second_string_begin;
        }

        if ((IsLetter(*first_string_begin)) && (IsLetter(*second_string_begin)) && (*first_string_begin != *second_string_begin))
        {
            break;
        }
    }

    return *first_string_begin - *second_string_begin;
}

int ReverseComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    char *first_string_begin  = (((String *) first_string) ->str);
    char *first_string_end    = (((String *) first_string) ->str) + (((String *) first_string) ->len) - 1;
    char *second_string_begin = (((String *) second_string)->str);
    char *second_string_end   = (((String *) second_string)->str) + (((String *) second_string)->len) - 1;

    while ((first_string_end != first_string_begin) && (second_string_end != second_string_begin))
    {
        if (!IsLetter(*first_string_end))
        {
            --first_string_end;
        }

        if (!IsLetter(*second_string_end))
        {

            --second_string_end;
        }

        if ((IsLetter(*first_string_end)) && (IsLetter(*second_string_end)) && (*first_string_end == *second_string_end))
        {
            --first_string_end;
            --second_string_end;
        }

        if ((IsLetter(*first_string_end)) && (IsLetter(*second_string_end)) && (*first_string_end != *second_string_end))
        {
            break;
        }
    }

    if (first_string_end == first_string_begin)
    {
        if (second_string_end != second_string_begin)
        {
            return SMALLER;
        }
    }

    if (second_string_end == second_string_begin)
    {
        return BIGGER;
    }

    return *first_string_end - *second_string_end;
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

    char byte = 0;

    for (size_t element = 0; element < type_size - counter * sizeof(unsigned long long); ++element)
    {
        byte = *((char *) value_1 + counter + element);
        *((char *) value_1 + counter + element) = *((char *) value_2 + counter + element);
        *((char *) value_2 + counter + element) = byte;
    }
}

bool IsCyrillic(char letter)
{
    return (((letter >= 'À') && (letter <= 'ß')) || ((letter >= 'à') && (letter <= 'ÿ')));
}

bool IsLatin(char letter)
{
    return (((letter >= 'A') && (letter <= 'Z')) || ((letter >= 'a') && (letter <= 'z')));
}

bool IsLetter(char letter)
{
    return ((IsCyrillic(letter)) || (IsLatin(letter)));
}
/*void QuickSort(void *data, size_t lines_number, size_t type_size, int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    size_t left = 0;
    size_t right = lines_number - 1;
    size_t middle_element_displacement = type_size * lines_number / 2;
    printf("middle_element_displacement is: %d\n", middle_element_displacement / type_size);
    while (left <= right)
    {

        while (comparator(((char *) data + type_size * left), ((char *) data + middle_element_displacement)) < 0)
        {
            printf("left: %d\n", left);
            ++left;
        }
        printf("passed left\n");
        while (comparator(((char *) data + type_size * right), ((char *) data + middle_element_displacement)) > 0)
        {
            printf("right: %d\n", right);
            --right;
        }
        printf("passed right\n");
        if (left <= right)
        {
            Swap(((char *) data + type_size * left), ((char *) data + type_size * right), type_size);
            printf("swapped: %d, %d\n", *((int *)((char *) data + type_size * left)), *((int *)((char *) data + type_size * right)));
            ++left;
            --right;
        }
    }

    if (right > 0)
    {
        printf("worked\n");
        printf("right_border: %d\n", right+1);
        QuickSort((char *) data, right + 1, type_size, comparator);
    }

    if (left < lines_number - 1)
    {
        printf("worked2\n");
        QuickSort(((char *) data + type_size * left), lines_number - left, type_size, comparator);
    }
}
*/
int Partition (void *data, size_t left, size_t right, size_t type_size, int (*comparator)(const void *, const void *))
{
    size_t middle_element_displacement = (left + right) / 2;

    //char middle_element = *((char *) data + type_size * (left + right) / 2);
    while (left < right)
    {
        while (comparator(((char *) data + type_size * left),  ((char *) data + type_size * middle_element_displacement)) < 0)
        {

            ++left;
            //printf("Left moved: %d\n", left);
        }

        while (comparator(((char *) data + type_size * right), ((char *) data + type_size * middle_element_displacement)) > 0)
        {
            --right;
            //printf("Right moved: %d\n", right);
        }
        //test;
        //printf("left: %d\n", left);
        //printf("right: %d\n", right);

        if (left >= right)
        {
            //test;
            break;
        }

        if ((left == middle_element_displacement) || (right == middle_element_displacement))
        {
            middle_element_displacement = right + left - middle_element_displacement;
        }

        Swap(((char *) data + type_size * left), ((char *) data + type_size * right), type_size);

        ++left;
        --right;
    }
    printf("middle: %d\n", middle_element_displacement);
    return middle_element_displacement;
}
void QuickSort(void *data, size_t elements_number, size_t type_size, int (*comparator)(const void *, const void *))
{
    printf("in: %d\n", elements_number);
    size_t left = 0;
    size_t right = elements_number - 1;

    if ((left < right) && ((int) elements_number > 1))
    {
        size_t border = Partition(data, left, right, type_size, comparator);

        printf("%d %d %d\n", elements_number, border - 1, elements_number - border);

        QuickSort(((char *) data), border - 1, type_size, comparator);
        QuickSort(((char *) data + type_size * border), elements_number - border - 1, type_size, comparator);
    }
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

void FreeMemory(struct Text *input_text)
{
    free(input_text->buffer);
    free(input_text->lines);

    input_text->buffer = nullptr;
    input_text->lines =  nullptr;
}
int sortingnumbers(const void *num1, const void *num2)
{
    //printf("Num1 is: %d\n", *((int *)num1));
    //printf("Num2 is: %d\n", *((int *)num2));
    //printf("Result is: %d\n", *((int *)num1) - *((int *)num2));
    return *((int *)num1) - *((int *)num2);
}
