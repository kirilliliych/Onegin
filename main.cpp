// -------------------------------------------------------
//! @file main.cpp
//! @brief Main function.
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Onegin.h"
#include "Sorting.h"

int main(int argc, const char *argv[])
{
    setlocale(LC_ALL, "RUS");

    HelpIfNeed(argc, argv);

    Text input_text = {};
    int number_of_files = 0;

    printf("Enter how many files you want to sort: ");
    scanf("%d", &number_of_files);

    while (number_of_files < 1)
    {
        printf("ERROR, number of files must be more than 0\n");

        printf("Enter how many files you want to sort: ");
        scanf("%d", &number_of_files);
    }

    while (number_of_files > 0)
    {
        if (TextInput(&input_text) == INPUT_ERROR)
        {
            return INPUT_ERROR;
        }

        char file_name_output[MAX_NAME_SIZE] = "";
        OutputCtor(&input_text, file_name_output);

        const char *mode = "w";

        char message_1[] = "Directly sorted text\n\n";
        SortAndPrint(&input_text, DirectComparator, QuickSort, PrintText, file_name_output, mode, message_1);

        mode = "a";

        char message_2[] = "\nReverse sorted text\n\n";
        SortAndPrint(&input_text, ReverseComparator, QuickSort, PrintText, file_name_output, mode, message_2);

        char message_3[] = "\nOriginal text\n\n";
        SortAndPrint(&input_text, ReverseComparator, QuickSort, PrintBuffer, file_name_output, mode, message_3);

        FreeMemory(&input_text);

        --number_of_files;

        if (number_of_files > 0)
        {
            printf("It is sorted, next!\n");
        }
    }

    return 0;
}
