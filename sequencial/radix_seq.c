#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

uint32_t num_elements;
FILE *input_file;
char *input_filename = NULL;

typedef struct
{
    size_t count;
    uint32_t *data;
} array;

int init_array(uint32_t *array)
{
    for (uint32_t i = 0; i < num_elements; i++)
    {
        fscanf(input_file, "%u", (array + i));
        fscanf(input_file, "\n");
    }

    return EXIT_SUCCESS;
}

static void radix(array scratch, array unsorted, array counting_array, uint32_t mask, uint32_t offset)
{
    register size_t i = 0;

    for (i = unsorted.count - 1; i > 0; --i)
    {
        scratch.data[--counting_array.data[(unsorted.data[i] & mask) >> offset]] = unsorted.data[i];
    }
    assert(i == 0);
    scratch.data[counting_array.data[(unsorted.data[i] & mask) >> offset] - 1] = unsorted.data[i];
}

static void count(array to_count, array counting_array, uint32_t mask, uint32_t offset)
{
    memset(counting_array.data, 0, counting_array.count * sizeof(uint32_t));
    register size_t i = 0;

    for (i = 0; i < to_count.count; ++i)
    {
        counting_array.data[(to_count.data[i] & mask) >> offset]++;
    }

    for (i = 1; i < counting_array.count; ++i)
    {
        counting_array.data[i] += counting_array.data[i - 1];

        if (counting_array.data[i] >= to_count.count)
        {
            return;
        }
    }
}
int radix_counting_sort(array unsorted)
{
    array scratch = {unsorted.count, NULL};
    scratch.data = malloc(unsorted.count * sizeof(uint32_t));

    if (!scratch.data)
    {
        return -1;
    }

    uint32_t stackmem[256];
    array counting_array = {256, stackmem};
    uint32_t mask = 0xF, offset = 0;

    do
    {
        count(unsorted, counting_array, mask, offset);
        radix(scratch, unsorted, counting_array, mask, offset);
        array tmp = scratch;
        scratch = unsorted;
        unsorted = tmp;
        mask <<= 0b0100;
        offset += 0b0100;
        printf("%i\n", CHAR_BIT);

    } while (mask);

    free(scratch.data);
    return 0;
}

static int print_array(array to_print)
{
    size_t i = 0;

    for (i = 0; i < to_print.count; ++i)
    {
        printf("%u \n", to_print.data[i]);
    }

   
    return 0;
}

static int verify_sorted(array sorted)
{
    size_t i = 0;

    for (i = 1; i < sorted.count; ++i)
    {
        if (sorted.data[i] < sorted.data[i - 1])
        {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "n:f:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            num_elements = strtoul(optarg, NULL, 0);
            break;
        case 'f':
            input_filename = optarg;
            break;
        default:
            fprintf(stderr, "Modo de usar: %s [-n numero de elemento][-d numero de dimensoes][-f nome do arquivo de entrada] ...\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    uint32_t *data = malloc(num_elements * sizeof(uint32_t));
    input_file = fopen(input_filename, "r");
    if (input_file == NULL || init_array(data) != EXIT_SUCCESS)
    {
        printf("Arquivo %s nao pode ser aberto!\n", argv[1]);
        return EXIT_FAILURE;
    }

    array test_input = {num_elements, data};
    radix_counting_sort(test_input);
    // verify_sorted(test_input);
    print_array(test_input);

    return 0;
}