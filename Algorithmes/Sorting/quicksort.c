#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

int *array;

void
fill_array (unsigned int size) 
{
    unsigned int i;

    if (array != NULL)
        free (array);

    array = (int *) malloc (sizeof(int)*size);
    srand (time (NULL));

    for (i = 0; i < size; i++)
    {
        array[i] = rand () % 100;
    }
}
 

/*
unsigned int 
parse_opt (int argc, char *argv[])
{
    static struct option long_options[] = {
        {"length", 
*/

void 
sort_array (int *ar, unsigned int size) 
{
    unsigned int i;
    printf("Before : ");
    for (i = 0; i < size; i++)
        printf("%d,", ar[i]);
    printf("\n");
    
    if (size <= 1)
        return;

    if (size == 2)
    {
        if (ar[1] > ar[0])
            return;
        int temp = ar[1];
        ar[1] = ar[0];
        ar[0] = temp;
        return;
    }

    int *pivot = &ar[0];
    int *left = &ar[1];
    int *right = &ar[size - 1];
    int size_left = 0;

    while (left != right)
    {
        if (*left <= *pivot)
        {
            left++;
            size_left++;
            continue;
        }
        if (*right > *pivot)
        {
            right--;
            continue;
        }
        int temp = *left;
        *left = *right;
        *right = temp;
    }

    if (*left < *pivot)
        size_left++;

    if (size_left == size - 1)
    {
        int temp = *right;
        *right = *pivot;
        *pivot = temp;
    }
    else if (size_left > 0)
    {
        right--;
        int temp = *right;
        *right = *pivot;
        *pivot = temp;
    }
    printf("After : ");
    for (i = 0; i < size; i++)
        printf("%d,", ar[i]);
    printf("\n");
    sort_array (&ar[0], size_left);
    sort_array (left, size - size_left - 1);
    
    return;      
}

void
display_array (int *ar, unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++)
        printf("%d,", ar[i]);
    printf("\n");
}

int 
main (int argc, char *argv[])
{
    unsigned int size = 20;
    fill_array (size);
    sort_array (array, size);
    display_array (array, size);

    free (array);
    return 0;
}


