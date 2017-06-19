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
sort_array (unsigned int size) 
{
    unsigned int i,j;
    int temp;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void
display_array (unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++)
        printf("%d,", array[i]);
}

int 
main (int argc, char *argv[])
{
    unsigned int size = 20;
    fill_array (size);
    sort_array (size);
    display_array (size);

    free (array);
    return 0;
}


