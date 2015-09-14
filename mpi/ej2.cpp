// Need 5 Processes

#include <mpi.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

void fill_vector(int* arr, int size){
	for(int i = 0 ; i < size ; ++i){
		arr[i] = rand()%100;
	}
}

void noob_fill_vector(int* arr, const int size){
	for(int i = 0 ; i < size ; ++i){
		arr[i] = i;
	}
}

void show_vector(int* arr, int size){
	for(int i = 0; i < size; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char  *argv[])
{
	int rank, size;
	int tag = 10;
	MPI_Status status;

	MPI_Init(&argc , &argv);
	MPI_Comm_rank(MPI_COMM_WORLD , &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int size_arr = 12;
	int arr[size_arr]; 
	


	if(rank == 0){
		printf("process: %d\n", rank);
		noob_fill_vector(arr, size_arr );
		show_vector(arr , size_arr);
		int f_arr_pos = 0;
		for(int i = 1 ; i < size ; ++i){
			MPI_Send(&arr[f_arr_pos], size_arr/(size-1), MPI_INT, i , tag, MPI_COMM_WORLD);
			f_arr_pos += size_arr/(size-1);
		}
	}

	if(rank != 0){
		int *part;
		MPI_Recv(&arr, size_arr/(size-1), MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		printf("process: %d\n", rank);
		show_vector(arr, size_arr/(size-1));
	}

	MPI_Finalize();

	return 0;
}