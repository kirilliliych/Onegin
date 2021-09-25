// -------------------------------------------------------
//! @file Sorting.h
//! @brief Prototypes & documentation for Sorting.cpp
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#ifndef SORTING_H
#define SORTING_H

#include <assert.h>
#include <cstring>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/** Compares lines in lexicographical order from the beginning
 *
 * @param [in] first_string  - pointer to the first  compared line
 * @param [in] second_string - pointer to the second compared line
 *
 * @return Number < 0 if first line is smaller, 0 if they are equal and > 0 if first line is bigger
 *
 */
int DirectComparator(const void *first_string, const void *second_string);

/** Compares lines in lexicographical order from the end
 *
 * @param [in] first_string  - pointer to the first  compared line
 * @param [in] second_string - pointer to the second compared line
 *
 * @return SMALLER if first line is smaller, EQUAL if they are equal and BIGGER if first line is bigger
 *
 */
int ReverseComparator(const void *first_string, const void *second_string);

/** Swaps two elements
 *
 * @param [out] value_1 - pointer to the first  element
 * @param [out] value_2 - pointer to the second element
 * @param [in]  type_size - size of the type of elements
 *
 */
void Swap(void *value_1, void *value_2, size_t type_size);

/** Divides array of elements into two parts
 *
 * @param [out] data  - pointer to the array that will be divided
 * @param [in]  elements_number - number of elements in the whole input array
 * @param [in]  type_size - size of type of elements in the array
 * @param [in]  comparator - pointer to comparing function
 *
 * @return Position of element that divides two new arrays
 *
 */
int Partition(void *data, size_t elements_number, size_t type_size,
              int (comparator)(const void *, const void *));

/** Sorts elements
 *
 * @param [out] data - pointer to the array that will be sorted
 * @param [in]  elements_number - number of elements that will be sorted
 * @param [in]  type_size - size of type of elements in the array
 * @param [in]  comparator - pointer to comparing function
 *
 */
void QuickSort(void *data, size_t elements_number, size_t type_size,
               int (*comparator)(const void *, const void *));

#endif
