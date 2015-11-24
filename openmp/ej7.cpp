#include <iostream>
#include <cstdio>
#include <omp.h>
#include <cstdlib>

#include <vector>

using namespace std;

// double trap(){

// 	cout<<"calling trap rule" << endl;
// 	double left;
// 	double right;

// 	int trap_count;
// 	double base_len;
// 	double estimate, x;


// }

double f(double x){
	return x*2;
}


void Trap(double a, double b, int n , double* global_result_p){

	double h, x, my_result;
	double local_a, local_b;

	int local_n;
	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();

	h = (b-a)/n;
	local_n = n/thread_count;
	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n*h;

	my_result = (f(local_a) + f(local_b))/2.0;
	for(int i = 1; i <= local_n-1; ++i){
		x = local_a + i*h;
		my_result += f(x);
	}

	my_result = my_result*h;

	#pragma omp critical
	*global_result_p += my_result;


}

void hello(void){

	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();

	printf("Hello from thread %d of %d\n", my_rank, thread_count);
}




int main(int argc, char* argv[]){

	double global_result = 0.0;
	double a,b;
	int n;

	int thread_count = strtol(argv[1] , NULL , 10);

	cout << " We can trust in one-thread result because there no exist critical sections.
" << endl;

	return 0;
		
}

We can trust in one-thread result because there no exist critical sections.

