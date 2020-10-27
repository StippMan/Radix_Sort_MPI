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

int main(int argc, char** argv) {

	int array[8] = {0,1,2,3,4,5,6,7};
	
    // Initialize the MPI environment

    // Get the number of processes
    MPI_Init(NULL, NULL);
    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Get the rank of the process


    // Get the name of the processor
	int* local_array = (int*)malloc(2*sizeof(int));
	MPI_Scatter(array,2,MPI_INT,local_array,2,MPI_INT, 0, MPI_COMM_WORLD);
    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

	for (int i = 0; i < 2; i++)
	{
		printf("rank %d: %d \n",world_rank,local_array[i]);
	}


	local_array[0] = 99;

	MPI_Gather(local_array, 2, MPI_INT, array, 2, MPI_INT, 0, MPI_COMM_WORLD);
	

	if(world_rank == 0){

		printf("array after: ");
		for (int i = 0; i < 8; i++)
		{
			printf("%d ",array[i]);
		}
		printf("\n");
	}
    // Finalize the MPI environment.
    MPI_Finalize();

}