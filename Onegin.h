// -------------------------------------------------------
//! @file Onegin.h
//! @brief Prototypes & documentation for Onegin.cpp
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

const int SMALLER = -1;                                              // means first string is lexicografically smaller than second
const int BIGGER  =  1;                                              // means first string is lexicografically bigger  than second
const int INPUT_MISTAKE    = -2;                                     // means there is a mistake in input
const char INPUT_FILE_1[]  = "Onegin.txt";
const char INPUT_FILE_2[]  = "Hamlet.txt";
const char OUTPUT_FILE_1[] = "Onegin_sorted.txt";
const char OUTPUT_FILE_2[] = "Hamlet_sorted.txt";

// String struct, contains string and its length
struct String
{
    char *str;
    size_t len;
};

// Text struct, contains information about input text
struct Text
{
    char *buffer;
    struct String *lines;
    size_t lines_number;
    size_t file_size;
};

/** Fills text struct
 *
 * @param [out] input_text - pointer to text struct
 *
 * @return 0 if input is ok and -1 if it is not
 */
int TextInput(Text *input_text);

/** Creates buffer and fills it with text, counts number of lines in it
 *
 * @param [in]  file - pointer to input file
 * @param [out] input_text - pointer to text struct
 *
 * @return pointer to the buffer with text
 *
 */
char *GetTextFromFile(FILE *file, Text *input_text);

/** Finds out size of input file
 *
 * @param [in] file - pointer to input file
 *
 * @return Size of file
 *
 */
size_t GetFileSize(FILE *file);

/** Places a pointer in the buffer at the beginning of each line in the text. Also counts the length of these lines
 *
 * @param [out] input_text - pointer to text_struct
 *
 * @return Pointer to array of filled in string struct
 *
 */
struct String *PlacePointers(Text *input_text);

/** Finds out if symbol is from cyrillic alphabet
 *
 * @param [in] letter - checked symbol
 *
 * @return True if it is from cyrillic alphabet and false if it is not
 *
 */
bool IsCyrillic(char letter);

/** Makes cyrillic letter a lowercase letter
 *
 * @param [in] letter - transformed letter
 *
 * @return Code of lowercase letter of cyrillic alphabet
 *
 */
int ToLowerCyrillic(char letter);

/** Finds out if symbol is from latin alphabet
 *
 * @param [in] letter - checked symbol
 *
 * @return True if it is from latin alphabet and false if it is not
 *
 */
bool IsLatin(char letter);

/** Makes letter a lowercase letter
 *
 * @param [in] letter - transformed letter
 *
 * @return Code of lowercase letter of cyrillic or latin alphabet
 *
 */
int ToLower(char letter);

/** Finds out if symbol is from cyrillic or latin alphabet
 *
 * @param [in] letter - checked symbol
 *
 * @return True if it is from cyrillic or latin alphabet and false if it is not
 *
 */
bool IsLetter(char letter);

/** Sorts array and outputs it to the file according to parameters
 *
 * @param [out] input_text pointer to struct that contains information about input text
 * @param [in]  sorting function - pointer to function that sorts array in a certain way
 * @param [in]  type_size - size of array's element
 * @param [in]  comparator - pointer to comparing function
 * @param [in]  output_function - pointer to function that outputs data to the file
 * @param [in]  message - string that is output to the file
 *
 */
void SortAndPrint(Text *input_text, int (comparator)(const void *, const void *),
                 void (sorting_function)(void *data, size_t number_of_elements,
                 size_t type_size, int (comparator)(const void *, const void *)),
                 void (output_function)(FILE* file, Text *text), char message[]);

/** Frees memory used by buffer and array of string struct
 *
 * @param [in] input_text - pointer to the text struct
 *
 */
void FreeMemory(Text *input_text);

/** Prints the result text to a file after sorting elements
 *
 * @param [in]  file - pointer to output file
 * @param [out] input_text - pointer to text struct
 *
 */
void PrintText(FILE *file, Text *input_text);

/** Prints text from buffer to a file
 *
 * @param [in] file - pointer to output file
 * @param [in] input_text - pointer to text struct
 *
 */
void PrintBuffer(FILE *file, Text *input_text);

