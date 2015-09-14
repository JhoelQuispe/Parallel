#include <iostream>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc , char* argv[]){
	int rank , size , guinea_pig; 

	MPI_Init (&argc , &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  	MPI_Comm_size (MPI_COMM_WORLD, &size);        

	int tag = 1;
	MPI_Status status;

	if(rank == 0){
		guinea_pig = 3;
		MPI_Send(&guinea_pig , 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
		MPI_Recv(&guinea_pig, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD, &status);
		guinea_pig *= 2;
		printf("Process %d: val = %d\n", rank, guinea_pig);
	}
	if(rank != 0 ){
		MPI_Recv(&guinea_pig, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
		guinea_pig *= 2;
		printf("Process %d: val = %d\n", rank, guinea_pig) ;
		MPI_Send(&guinea_pig, 1, MPI_INT, (rank+1)%size, tag, MPI_COMM_WORLD);
	}
  	
//	printf( "Hello world from process %d of %d\n", rank, size );

	MPI_Finalize();  	
}