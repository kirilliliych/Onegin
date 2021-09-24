// -------------------------------------------------------
//! @file Sorting.cpp
//! @brief Declarations of functions related to sorting
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Sorting.h"

int DirectComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    char *first_string_begin  = ((const String *) first_string) ->str;
    char *second_string_begin = ((const String *) second_string)->str;

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

        if ((IsLetter(*first_string_begin)) && (IsLetter(*second_string_begin)) &&
           (ToLower(*first_string_begin) == ToLower(*second_string_begin)))
        {
            ++first_string_begin;
            ++second_string_begin;
        }

        if ((IsLetter(*first_string_begin)) && (IsLetter(*second_string_begin)) &&
           (ToLower(*first_string_begin) != ToLower(*second_string_begin)))
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

    String *first =  (String *) first_string;
    String *second = (String *) second_string;

    size_t minlen = (first->len > second->len) ? second->len : first->len;

    for (int checking_strings = 1; checking_strings < minlen; ++checking_strings)
    {
        if (ToLower(first->str[first->len - checking_strings]) > ToLower(second->str[second->len - checking_strings]))
        {
            return BIGGER;
        }
        else if (ToLower(second->str[second->len - checking_strings]) > ToLower(first->str[first->len - checking_strings]))
        {
            return SMALLER;
        }
    }

    if (first->len == second->len)
    {
        return 0;
    }

    return (first->len > second->len) ? BIGGER : SMALLER;
}

/*int ReverseComparator(const void *first_string, const void *second_string)
{
    assert(first_string  != nullptr);
    assert(second_string != nullptr);

    char *first_string_begin  = (((const String *) first_string) ->str);
    char *first_string_end    = (((const String *) first_string) ->str) + (((const String *) first_string) ->len) - 1;
    char *second_string_begin = (((const String *) second_string)->str);
    char *second_string_end   = (((const String *) second_string)->str) + (((const String *) second_string)->len) - 1;

    while ((first_string_end > first_string_begin) && (second_string_end > second_string_begin))
    {
        if (!IsLetter(*first_string_end))
        {
            --first_string_end;
            continue;
        }

        if (!IsLetter(*second_string_end))
        {
            --second_string_end;
            continue;
        }

        if (*first_string_end == *second_string_end)
        {
            --first_string_end;
            --second_string_end;
        }
        else
        {
            break;
        }
    }

    if (first_string_end == first_string_begin)
    {
        if (second_string_end != second_string_begin)
        {
            while (second_string_end > second_string_begin)
            {
                if (IsLetter(*second_string_end))
                {
                    return SMALLER;
                }
            }

            if (*first_string_end == *second_string_end)
            {
                return 0;
            }
        }
    }

    if (second_string_end == second_string_begin)
    {
        if (first_string_end != first_string_begin)
        {
            while (first_string_end > first_string_begin)
            {
                if (IsLetter(*first_string_end))
                {
                    return BIGGER;
                }
            }

            if (*first_string_end == *second_string_end)
            {
                return 0;
            }
        }
    }

    return *first_string_end - *second_string_end;
}
 */

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

int Partition(void *data, size_t elements_number, size_t type_size,
              int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    void *middle = data;
    void *beginning = data;
    void *ending = (char *) data + (elements_number - 1) * type_size;

    while (true)
    {
        while (comparator(beginning, middle) < 0)
        {
            beginning = (char *) beginning + type_size;
        }

        while (comparator(ending, middle) > 0)
        {
            ending = (char *) ending - type_size;
        }

        if (beginning >= ending)
        {
            return ((char *) ending - (char *) data) / type_size;
        }

        if (beginning == middle)
        {
            middle = ((char *) ending - (char *) beginning + (char *) middle);
        }
        else if (ending == middle)
        {
            middle = ((char *) beginning - (char *) ending  + (char *) middle);
        }

        Swap(beginning, ending, type_size);

        ending = (char *) ending - type_size;
        beginning = (char *) beginning + type_size;
    }
}

void QuickSort(void *data, size_t elements_number, size_t type_size,
               int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    if ((int) elements_number > 1)
    {
        size_t border = Partition(data, elements_number, type_size, comparator);

        QuickSort(((char *) data), border, type_size, comparator);
        QuickSort(((char *) data + type_size * (border + 1)), elements_number - border - 1, type_size, comparator);
    }
}
