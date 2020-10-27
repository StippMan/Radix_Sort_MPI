#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define DIGITS 10 // the number of digits in base 10

int num_elements = 16;
int max_value = 15;
FILE *input_file;
char *input_filename = NULL;

typedef struct
{
	int id, inicio, fim;
	struct kdnode *array;
} thread_arg, *ptr_thread_arg;

// int init_array(int *array)
// {
// 	for (int i = 0; i < num_elements; i++)
// 	{
// 		fscanf(input_file, "%lu", &array[i]);
// 		fscanf(input_file, "\n");
// 	}

// 	return EXIT_SUCCESS;
// }

void counting_sort(int local_array[], int count_return[], int n_elements, int max_value){

	int output[n_elements];
	int* count_array = calloc(max_value+1, sizeof(int));

	for (int i = 0; i < max_value+1; i++)
	{
		++count_array[local_array[i]]; 
	}
	count_return[0] = count_array[0];
	for (int i = 1; i <= max_value; i++)
	{
		count_array[i] += count_array[i-1];
		count_return[i] = count_array[i];
	}

	for (int i = 0; i < max_value+1; i++)
	{
		output[count_array[local_array[i]]-1] = local_array[i];
		--count_array[local_array[i]];
	}

	for (int i = 0; i < max_value+1; ++i) 
		local_array[i] = output[i]; 

}


struct timeval begin, end;

int main(int argc, char *argv[])
{

	int array[16] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
	// int *array = malloc(num_elements * sizeof(int));
    MPI_Init(&argc, &argv);
    int num_processes;
    int process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
	int elements_per_rank = num_elements/num_processes;

	// if(process_rank == 0){
	// 	srand(time(NULL));

	// 	int opt;
	// 	while ((opt = getopt(argc, argv, "n:b:f:")) != -1)
	// 	{
	// 		switch (opt)
	// 		{
	// 		case 'n':
	// 			num_elements = strtoul(optarg, NULL, 0);
	// 			break;
	// 		case 'b':
	// 			max_value = strtoul(optarg, NULL, 0);
	// 			break;
	// 		case 'f':
	// 			input_filename = optarg;
	// 			break;
	// 		default:
	// 			fprintf(stderr, "Modo de usar: %s [-n numero de elemento]\
	// 							[-b maior elemento da entrada]\
	// 							[-f nome do arquivo de entrada] ...\n",
	// 							argv[0]);
	// 			exit(EXIT_FAILURE);
	// 		}
	// 	}
		// input_file = fopen("1mil.txt", "r");
		// if (input_file == NULL || init_array(array) != EXIT_SUCCESS)
		// {
		// 	printf("File %s could not be opened!\n", argv[1]);
		// 	return EXIT_FAILURE;
		// }
		// for (int i = 0; i < 10; i++)
		// {
		// 	printf("%lu ",array[i]);
		// }
		// printf("\n");
	// }
	

	int* local_array = malloc(elements_per_rank*sizeof(int));
	int* count_array = malloc(elements_per_rank*sizeof(int));
		
	MPI_Scatter(array, elements_per_rank, MPI_INT, local_array, elements_per_rank, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	// MPI_Gather(local_array, elements_per_rank, MPI_INT, array, elements_per_rank, MPI_INT, 0, MPI_COMM_WORLD);

	int n_bits_per_iteration = log2(num_processes);
	int n_iterations = 32/n_bits_per_iteration;
	for (int i = 0; i < n_iterations; i++)
	{

		counting_sort(local_array, count_array, elements_per_rank, max_value);

	}
	if(process_rank == 0){
		
		for (int i = 0; i < num_elements; i++)
		{
			printf("%d \n",array[i]);
		}
		
	}
	// gettimeofday(&begin, 0);

	// int i = 0b01010101000010010101010100001001;
	// int masked_i = i & mask;


	// sort array
	// radix(array, size);

	// if (1) {
	// 	printf("Sorted array contents:\n");
	// 	for (int i = 0; i < num_elements; i++) {
	// 		printf("%lu\n", *(array + i));
	// 	}
	// }

	// gettimeofday(&end, 0);
	// long seconds = end.tv_sec - begin.tv_sec;
	// long microseconds = end.tv_usec - begin.tv_usec;
	// double elapsed = seconds + microseconds * 1e-6;

	// printf("Time measured: %.3f seconds.\n", elapsed);

	// free(array); // free memory
  	MPI_Finalize();
	return 0;
}