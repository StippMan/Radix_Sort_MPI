#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define DIGITS 10 // the number of digits in base 10

unsigned long num_elements;
unsigned long max_value;
FILE *input_file;
char *input_filename = NULL;

typedef struct
{
	int id, inicio, fim;
	struct kdnode *array;
} thread_arg, *ptr_thread_arg;

int init_array(unsigned long *array)
{
	for (unsigned long i = 0; i < num_elements; i++)
	{
		fscanf(input_file, "%lu", (array + i));
		fscanf(input_file, "\n");
	}

	return EXIT_SUCCESS;
}

// get number of digits in a given unsigned long integer
int getNumOfDigits(unsigned long n)
{
	return floor(log10(abs(n))) + 1;
}

// get the highest amount of digits in a given array
int getMostDigits(unsigned long *a, unsigned long size)
{
	int max = getNumOfDigits(*a); // initialize max as digits of first index in a
	for (unsigned long i = 0; i < size / sizeof(unsigned long); i++)
	{
		if (getNumOfDigits(*(a + i)) > max)
		{                                   // if greater than max
			max = getNumOfDigits(*(a + i)); // update max
		}
	}
	return max;
}

// perform Radix sort
void radix(unsigned long *a, unsigned long size)
{
	// length of array
	unsigned long len = size / sizeof(unsigned long);
	
	unsigned long m = DIGITS, n = 1;

	unsigned long *working[DIGITS];  // array of pointers for digits 0-9
	unsigned long sizes[DIGITS];     // the amount of memory to be allocated to each index in working
	unsigned long available[DIGITS]; // used to keep track of next available index to store a value in working[]

	int mostDigits = getMostDigits(a, size); // get the most amount of sig figs in array
	// while not every significant digit has been covered, continue to sort
	while (getNumOfDigits(n) <= mostDigits)
	{

		// initialize sizes and available to 0s
		for (int i = 0; i < DIGITS; i++)
		{
			sizes[i] = 0;
			available[i] = 0;
		}

		// calculate memory needed for each index of working
		for (unsigned long i = 0; i < len; i++)
		{
			int ind = (int)(*(a + i) % m) / n; // get index
			sizes[ind]++;                      // increment amount of memory needed in that index
		}

		// allocate that memory in working[]
		for (int i = 0; i < DIGITS; i++)
		{
			working[i] = malloc(sizes[i] * sizeof(unsigned long));
		}

		// add values to working[]
		for (unsigned long i = 0; i < len; i++)
		{
			int ind = (int)(*(a + i) % m) / n;           // get index in working array
			*(working[ind] + available[ind]) = *(a + i); // add value to next available position in working[ind]
			available[ind]++;                            // increment the available index for that position, so future values will not overwrite
		}

		// transfer values in new order back to a
		unsigned long index = 0;
		// for every digit in working[]
		for (int digit = 0; digit < DIGITS; digit++)
		{
			// for every index under that digit
			for (unsigned long i = 0; i < sizes[digit]; i++)
			{
				*(a + index) = *(working[digit] + i); // add value to a
				index++;
			}
			free(working[digit]); // free memory
		}

		// increment m and n values
		m *= DIGITS;
		n *= DIGITS;
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	struct timeval begin, end;
	int opt;
	while ((opt = getopt(argc, argv, "n:b:f:")) != -1)
	{
		switch (opt)
		{
		case 'n':
			num_elements = strtoul(optarg, NULL, 0);
			break;
		case 'b':
			max_value = strtoul(optarg, NULL, 0);
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

	unsigned long size = num_elements * sizeof(unsigned long);
	unsigned long *array = malloc(size);

	input_file = fopen(input_filename, "r");
	if (input_file == NULL || init_array(array) != EXIT_SUCCESS)
	{
		printf("File %s could not be opened!\n", argv[1]);
		return EXIT_FAILURE;
	}

	gettimeofday(&begin, 0);

	// sort array
	radix(array, size);

	// if (1) {
	// 	printf("Sorted array contents:\n");
	// 	for (unsigned long i = 0; i < num_elements; i++) {
	// 		printf("%lu\n", *(array + i));
	// 	}
	// }
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double elapsed = seconds + microseconds * 1e-6;

	printf("Time measured: %.3f seconds.\n", elapsed);

	free(array); // free memory
	return 0;
}