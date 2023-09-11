// -------------------------------------------------------
//! @file Sorting.cpp
//! @brief Declarations of functions related to sorting
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Sorting.h"
#include "Onegin.h"

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

                --second_string_end;
            }

            if (*first_string_end == *second_string_end)
            {
                return EQUAL;
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

                --first_string_end;
            }

            if (*first_string_end == *second_string_end)
            {
                return EQUAL;
            }
        }
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
        unsigned long long *value_1_ptr = (unsigned long long *) value_1;
        unsigned long long *value_2_ptr = (unsigned long long *) value_2;
        temp = *(value_1_ptr + counter);
        *(value_1_ptr + counter) = *(value_2_ptr + counter);
        *(value_2_ptr + counter) = temp;
    }

    char byte = 0;

    for (size_t element = 0; element < type_size - counter * sizeof(unsigned long long); ++element)
    {
        char *value_1_ptr = (char *) value_1;
        char *value_2_ptr = (char *) value_2;
        byte = *(value_1_ptr + counter + element);
        *(value_1_ptr + counter + element) = *(value_2_ptr + counter + element);
        *(value_2_ptr + counter + element) = byte;
    }
}

int Partition(void *data, size_t elements_number, size_t type_size,
              int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    char *middle    = (char *) data;
    char *beginning = (char *) data;
    char *ending    = (char *) data + (elements_number - 1) * type_size;

    while (true)
    {
        while (comparator(beginning, middle) < 0)
        {
            beginning += type_size;
        }

        while (comparator(ending, middle) > 0)
        {
            ending -= type_size;
        }

        if (beginning >= ending)
        {
            return (ending - (char* ) data) / type_size;
        }

        // if (beginning == middle)
        // {
        //     middle = ((char *) ending - (char *) beginning + (char *) middle);
        // }
        // else if (ending == middle)
        // {
        //     middle = ((char *) beginning - (char *) ending  + (char *) middle);
        // }

        Swap(beginning, ending, type_size);

        ending -= type_size;
        beginning += type_size;
    }
}

void QuickSort(void *data, size_t elements_number, size_t type_size,
               int (*comparator)(const void *, const void *))
{
    assert(data != nullptr);
    assert(comparator != nullptr);

    if (elements_number > 1)
    {
        size_t border = Partition(data, elements_number, type_size, comparator);

        QuickSort(((char *) data), border, type_size, comparator);
        QuickSort(((char *) data + type_size * (border + 1)), elements_number - border - 1, type_size, comparator);
    }
}
