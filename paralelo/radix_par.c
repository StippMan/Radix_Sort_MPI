#include <mpi.h>
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

// global constants definitions

#define b 32	   // number of bits for integer
#define g 2		   // group of bits for each scan
#define N b / g	   // number of passes
#define B (1 << g) // number of buckets, 2^g

uint32_t num_elements;
FILE *input_file;
char *input_filename = NULL;

typedef struct
{
	size_t count;
	uint32_t *data;
} array;
static int print_array(array to_print)
{
	size_t i = 0;

	for (i = 0; i < to_print.count; ++i)
	{
		printf("%u \n", to_print.data[i]);
	}

	return 0;
}
int init_array(uint32_t *array)
{
	for (uint32_t i = 0; i < num_elements; i++)
	{
		fscanf(input_file, "%u", (array + i));
		fscanf(input_file, "\n");
	}

	return EXIT_SUCCESS;
}

static void global_radix(array scratch, array unsorted, array counting_array, uint32_t mask, uint32_t offset, uint32_t rank, uint32_t elements_per_proc)
{
	register size_t i = 0;

	for (i = unsorted.count - 1; i > 0; --i)
	{
		scratch.data[--counting_array.data[(unsorted.data[i] & mask) >> offset]] = unsorted.data[i];
	}
	assert(i == 0);
	scratch.data[counting_array.data[(unsorted.data[i] & mask) >> offset] - 1] = unsorted.data[i];
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
int radix_counting_sort(array unsorted, uint32_t rank, uint32_t world_size)
{
	array scratch = {unsorted.count, NULL};
	scratch.data = malloc(unsorted.count * sizeof(uint32_t));

	if (!scratch.data)
	{
		return -1;
	}

	uint32_t stackmem[B];
	array counting_array = {B, stackmem};
	uint32_t mask = 0b11, offset = 0;
	uint32_t allCounts[B][world_size];
	uint32_t l_B = B / world_size;
	MPI_Request req;
	MPI_Status stat;
	do
	{
		for (size_t j = 0; j < B; j++)
		{
			allCounts[j][rank] = 0;
		}

		count(unsorted, counting_array, mask, offset);
		radix(scratch, unsorted, counting_array, mask, offset);

		MPI_Allgather(counting_array.data, B, MPI_INTEGER, allCounts[rank], B, MPI_INTEGER, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0)
		{
			uint32_t *temp_arr = malloc(num_elements * sizeof(uint32_t));
		}

		for (size_t i = 0; i < rank; i++)
		{
			for (size_t j = 0; j < rank; i++)
			{
				uint32_t block = malloc(allCounts[j][i] * sizeof(uint32_t));
			}
		}
		MPI_Gather();

		array tmp = scratch;
		scratch = unsorted;
		unsorted = tmp;
		mask <<= 2;
		offset += 2;
	} while (mask);

	free(scratch.data);
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
	uint32_t rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
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
		if (rank == 0)
		{
			printf("Arquivo %s nao pode ser aberto!\n", argv[1]);
			MPI_Finalize();
			return EXIT_FAILURE;
		}
	}
	array test_input = {num_elements, data};

	uint32_t element_per_proc = num_elements / size;

	if (element_per_proc < 1)
	{
		if (rank == 0)
		{
			printf("Number of elements must be >= number of processes!");
		}
		MPI_Finalize();
		return EXIT_FAILURE;
	}

	uint32_t *localInput = (uint32_t *)malloc(element_per_proc * sizeof(uint32_t));

	MPI_Scatter(test_input.data, element_per_proc, MPI_INTEGER, localInput, element_per_proc, MPI_INTEGER, 0, MPI_COMM_WORLD);

	array local = {element_per_proc, localInput};

	radix_counting_sort(local, rank, size);

	uint32_t *output;
	if (rank == 0)
	{
		output = (uint32_t *)malloc(num_elements * sizeof(uint32_t));
	}
	array outputArray = {num_elements, output};

	MPI_Gather(local.data, element_per_proc, MPI_INTEGER, &output[rank * element_per_proc], element_per_proc, MPI_INTEGER, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		print_array(outputArray);
	}

	MPI_Finalize();

	return 0;
}