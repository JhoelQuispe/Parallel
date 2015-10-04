#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#define DIM 4
#define MAX_IN_MAT 10

int thread_count;

void create_matrix(int**&);
void fill_matrix(int**&);
void print_matrix(int**);
void fill_vector(int*);
void print_vector(int*);

void* Hello(void*);
void* Product_Mat_Vec(void*);

int **mat;
int vec[DIM];
int result[DIM];

int main(int argc, char*argv[]){

	create_matrix(mat);
    fill_matrix(mat);
    print_matrix(mat);

    fill_vector(vec);
    print_vector(vec);
    

	long thread;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);
	
	thread_handles = static_cast<pthread_t*>(malloc(thread_count * sizeof(pthread_t)));
	for(thread = 0; thread < thread_count; ++thread){
		pthread_create(&thread_handles[thread], NULL, Product_Mat_Vec, (void*) thread);
	}

	
	for(thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
	}

	free(thread_handles);
	
	print_vector(result);

	return 0;

}

void *Product_Mat_Vec(void* rank){
    long my_rank = (long) rank;
	// printf("Multiply from thread %ld of %d\n", my_rank, thread_count);
	// print_vector(mat[my_rank]);
	double t_sum = 0.0;
	for (int i = 0; i < DIM; ++i)
	{
		// printf("%d , %ld , %d, %d\n", i , my_rank, vec[i] , mat[my_rank][i]);	
		t_sum += vec[i]*mat[my_rank][i];
	}
	result[my_rank] = t_sum;

    return NULL;
}

void create_matrix(int**&mat){
	mat = new int*[DIM];
	for(int i = 0; i < DIM; ++i){
		mat[i] = new int[DIM];	
	}
}

void fill_matrix(int **&mat){
    for(int i = 0; i < DIM; ++i){
        for(int j = 0; j < DIM; ++j){
            mat[i][j] = rand()%MAX_IN_MAT;
        }
    }
}

void print_matrix(int **mat){
    for(int i = 0; i < DIM; ++i){
        for(int j = 0; j < DIM; ++j){
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void fill_vector(int *vec){
    for(int i = 0; i < DIM; ++i){
        vec[i] = rand()%MAX_IN_MAT;
    }
}

void print_vector(int *vec){
    for(int i = 0; i < DIM; ++i){
        printf("%d\n", vec[i]);
    }
    printf("\n");
}
