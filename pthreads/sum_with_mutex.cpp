#include <cstdio>
#include <vector>
#include <cstdlib>
#include <pthread.h>

int thread_count;
double linear_pi(int);
void* calculate_pi_thread(void*);
int precision; 
int part;
double total;
pthread_mutex_t mutex_p;

int main(int argc, char *argv[])
{
	thread_count = strtol(argv[1], NULL, 10);
	std::vector<pthread_t> thread_handles(thread_count);
	pthread_mutex_init(&mutex_p, NULL);

	precision = 10000000;
	part = precision / thread_count;
	total = 0;

	for (long thread = 0; thread < thread_count; ++thread)
	{
		pthread_create(&thread_handles[thread], NULL, calculate_pi_thread, (void*)thread);
	}

	for (long thread = 0; thread < thread_count; ++thread)
	{
		pthread_join(thread_handles[thread], NULL);
	}
	total *= 4;
	printf("result: %f\n", total);
	pthread_mutex_destroy(&mutex_p);

	return 0;
}

void* calculate_pi_thread(void* rank){
	double local_sum = 0;
	long my_rank = (long) rank;
	int begin = my_rank*part;
	int end = (my_rank+1)*part;
	short factor = 1;

	if(begin%2==0) factor = 1;
	else factor = -1;

	for (int i = begin; i < end; ++i, factor=-factor)
	{
		local_sum += factor*(1.0/(2*i+1));
	}
	pthread_mutex_lock(&mutex_p);
	total += local_sum;
	pthread_mutex_unlock(&mutex_p);	

	return NULL;
}

double linear_pi(int precision){

	double r = 0.;
	short factor = 1;
	for (int i = 0; i < precision; ++i , factor=-factor)
	{
		r += factor*(1.0/(2*i+1));
	}
	r *= 4;
	return r;
}