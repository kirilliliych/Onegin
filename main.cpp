// -------------------------------------------------------
//! @file main.cpp
//! @brief Main function.
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Onegin.cpp"
#include "Sorting.cpp"

int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "RUS");

    struct Text input_text = {0,0,0,0};

    if (TextInput(&input_text) == INPUT_MISTAKE)
    {
        return INPUT_MISTAKE;
    }

    SortAndPrint(&input_text, DirectComparator, qsort, PrintText, "Directly sorted text\n\n");

    SortAndPrint(&input_text, ReverseComparator, QuickSort, PrintText, "\nReverse sorted text\n\n");

    SortAndPrint(&input_text, ReverseComparator, QuickSort, PrintBuffer, "\nOriginal text\n\n");

    FreeMemory(&input_text);

    return 0;
}
