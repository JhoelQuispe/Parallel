#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#define DIM 4
#define MAX_IN_MAT 8

using namespace std;

void create_matrix(int**&mat){
	cout<<"create matrix" <<endl;
	mat = new int*[DIM];
	mat[0] = new int[DIM*DIM];
	for(int i = 1 ; i < DIM ; ++i){
		mat[i] = new int[DIM];
	}
}

void fill_random_matrix(int**&mat){
	cout<<"fill random"<<endl;
	for(int i = 0 ; i < DIM ; ++i){
		for(int j = 0 ; j < DIM ; ++j){
			mat[i][j] = rand()%MAX_IN_MAT;
		}
	}
}

void print_matrix( int **mat ){
	for(int i = 0 ; i < DIM ; ++i){
		for(int j = 0 ; j < DIM ; ++j){
			printf("%d\t", mat[i][j]);
			// cout<<&mat[i][j]<<"\t";
		}
		printf("\n");
	}
}

void fill_random_vector(int*vec){
	for(int i = 0 ; i < DIM ; ++i){
		vec[i] = rand()%MAX_IN_MAT;
	}
}

void print_vector(int*vec){
	for(int i =  0 ; i < DIM ; ++i){
		printf("%d\n", vec[i]);
		// cout<<&vec[i]<<endl;
	}
}

int product_vector(int*vecX , int*vecY){
	int result = 0;
	for(int i = 0 ; i < DIM ; ++i){
		result += vecX[i]*vecY[i];
	}
	return result;
}

int main(int argc, char *argv[]){

	int MPI_Rank , MPI_Size ; 

	MPI_Init(&argc, &argv);

	MPI_Comm_rank( MPI_COMM_WORLD, &MPI_Rank);
	MPI_Comm_size( MPI_COMM_WORLD, &MPI_Size);

	int **mat;

	int vecA[DIM];
	int vecR[DIM];

	int vecAux[DIM];

	mat = new int *[DIM];

	if(MPI_Rank == 0){		

		// create_matrix(mat);
		mat[0] = new int [DIM * DIM];
        for (unsigned int i = 1; i < DIM; i++) {
            mat[i] = mat[i - 1] + DIM;
        }

		// print_matrix(mat);

		fill_random_matrix(mat);
		print_matrix(mat);

		fill_random_vector(vecA);

	}

	MPI_Scatter(mat[0], DIM, MPI_INT, vecAux, DIM,  MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vecA, DIM, MPI_INT, 0, MPI_COMM_WORLD);

	int value = product_vector(vecAux , vecA);

	MPI_Gather(&value, 1, MPI_INT, vecR, 1, MPI_INT, 0, MPI_COMM_WORLD );

	if(MPI_Rank == 0) {
		printf("Resultado:\n");
		print_vector(vecR);
	}

	MPI_Finalize();


	return 0;

}