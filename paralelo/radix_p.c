#include <mpi.h>
#include <stdio.h>


int num_elements;


int main(int argc, char** argv) {

	// int opt;
	// while ((opt = getopt(argc, argv, "n:d:t:f:")) != -1) {
	// 	switch (opt) {
	// 	case 'n':
	// 	num_elements = strtoul(optarg, NULL, 0);
	// 	break;
	// 	case 'd':
	// 	dimensions = strtoul(optarg, NULL, 0);
	// 	break;
	// 	case 't':
	// 	num_threads = strtoul(optarg, NULL, 0);
	// 	break;
	// 	case 'f':
	// 	input_filename = optarg;
	// 	break;
	// 	default:
	// 	fprintf(stderr,
	// 			"Modo de usar: %s [-n numero de elemento][-d numero de "
	// 			"dimensoes][-t numero de threads][-f nome do arquivo de entrada] ...\n",
	// 			argv[0]);
	// 	exit(EXIT_FAILURE);
	// 	}
	// }

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);


	
	//paralelo aq



	// Finalize the MPI environment.
	MPI_Finalize();
}