#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <iterator>
#include <algorithm>
#include <sstream>

#define TAG 10
#define TEXT_SIZE 3000000

using namespace std;

void Odd_even_sort(int a[] , int n){
	int phase, i, tmp;
	for (phase = 0; phase < n ; ++phase){
		if(phase%2 == 0){
			for(i = 1; i<n; i+=2){
				if(a[i-1] > a[i]){
					swap(a[i],a[i-1]);
				}
			}
		}
		else{
			for(i = 1; i < n-1; i+=2){
				if(a[i] > a[i+1]){
					swap(a[i],a[i+1]);
				}
			}
		}
	}
}

void set_vector(int*&a, int n){

	// a = {15,11,9,16,3,14,8,7,4,6,12,10,5,2,13,1};

	// for(int i = 0 ; i < n ; ++i){
	// 	a[i] = random()%100;
	// }
}

void show_vector(const int*a, int n){
	for(int i = 0 ; i < n ; ++i){
		printf("%d ", a[i]);
	}
	printf("\n");
}

int compute_partner(int rank , int phase){
	if(phase%2==0)
		if (rank%2 == 0)
			return rank+1;
		else
			return rank-1;
	else
		if(rank%2 == 0)
			return rank -1;
		else 
			if(rank+1>3)
				return -1;
			else
				return rank+1;

}

int main(int argc, char *argv[])
{
	int MPI_Rank , MPI_Size ; 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD , &MPI_Rank );
	MPI_Comm_size(MPI_COMM_WORLD , &MPI_Size );
	
	MPI_Status status;

	int vec[16] = {15,11,9,16, 3,14,8,7, 4,6,12,10, 5,2,13,1};

	int local_size = 16/MPI_Size;

	int local_vec[local_size*2] = {0};
	int aux_vec[local_size];
	int vec_to_sort[local_size*2];

	MPI_Scatter(&vec , 16/MPI_Size, MPI_INT, &local_vec, 16/MPI_Size, MPI_INT,  0, MPI_COMM_WORLD);

	Odd_even_sort(local_vec, local_size);

	for(int phase=0 ; phase < local_size; ++phase){
		int partner = compute_partner(MPI_Rank, phase);
		if(partner != -1){
			// printf("rank , partner,  phase %d,%d,%d\n", MPI_Rank, partner , phase);
		
			if(partner < MPI_Rank){
				MPI_Send(&local_vec , local_size, MPI_INT, partner, TAG, MPI_COMM_WORLD);
				MPI_Recv(&local_vec, local_size, MPI_INT, partner, TAG, MPI_COMM_WORLD, &status);
			}
			else{
				MPI_Recv(&local_vec[local_size], local_size, MPI_INT, partner, TAG, MPI_COMM_WORLD, &status);
				Odd_even_sort(local_vec , local_size*2);
				MPI_Send(&local_vec[local_size], local_size, MPI_INT, partner, TAG, MPI_COMM_WORLD);
			}
		}

		MPI_Gather(&local_vec, local_size, MPI_INT, vec, local_size, MPI_INT, 0, MPI_COMM_WORLD);
		if(MPI_Rank == 0)	show_vector(vec , 16);

	}
	// MPI_Gather(&local_vec, local_size, MPI_INT, vec, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	// if(MPI_Rank == 0)	show_vector(vec , 16);

	MPI_Finalize();


	return 0;
}

