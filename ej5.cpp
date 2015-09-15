#include <mpi.h>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define DIM 4


using namespace std;


void create_matrix(int**&mat){
	cout<<"create matrix"<<endl;
	mat = new int*[DIM];	
	for(int i = 0 ; i < DIM ; ++i ){
		cout<<i<<endl;
		mat[i] = new int[DIM];
		
	}
}

void fill_random_matrix(int**&mat){
	cout<<"fill random"<<endl;
	for(int i = 0 ; i < DIM ; ++i){
		for(int j = 0 ; j < DIM ; ++j ){
			mat[i][j] = rand()%5;
		}
	}
}

void print_matrix(int**mat){
	cout<<"show matrix"<<endl;
	for(int i = 0 ; i < DIM; ++i){
		for(int j = 0 ; j < DIM ; ++j){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

 
int main(int argc, char *argv[])
{
	// srand(time(0));
	int **mat;
	
	int vecA[DIM];
	int vecB[DIM];

	MPI_Init(&argc , &argv);

	int MPI_Rank , MPI_Size;
	int tag_send = 8;
	int tag_recv = 9;

	MPI_Comm_rank(MPI_COMM_WORLD , &MPI_Rank);
	MPI_Comm_size(MPI_COMM_WORLD , &MPI_Size);

	MPI_Status status ;

	// printf("soy el proceso %d\n", MPI_Rank );

	if(MPI_Rank == 0){

		create_matrix(mat);
		fill_random_matrix(mat);
		print_matrix(mat);	

		for(int i = 0 ; i < MPI_Size ; i++){
			MPI_Send(&mat[i] , DIM , MPI_INT , i , tag_send, MPI_COMM_WORLD);
		}
	}
	else{
		printf("im process %d\n", MPI_Rank);
		MPI_Recv(&vecA , DIM , MPI_INT , 0 , tag_send , MPI_COMM_WORLD , &status);
	
		// calculate column
		for(int i = 0 ; i < DIM ; ++i){
			for(int j = 0 ; j < DIM ; ++j)
			vecB[i] = mat
		}
	}

	MPI_Finalize();



	return 0;
}
