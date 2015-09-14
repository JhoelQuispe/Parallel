#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

void show_str(char*arr , int size){
	for(int _= 0 ; _<size; ++_){
		printf("%c", arr[_]);
	}
	printf("\n");
}

int prl_count_words(char* arr, int size){
	int wrds = 0;
	for(int i = 0 ; i < size; ++i){
		if(arr[i] == ' '){
			++wrds;
		}
	}
	return wrds;
}

int main(int argc, char *argv[])
{


	int MPI_Rank , MPI_Size ; 


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD , &MPI_Rank );
	MPI_Comm_size(MPI_COMM_WORLD , &MPI_Size );

	MPI_Status status;
	int tag = 10;

	int words;
	// hola como| estas
	// hola com|o estas
	// hola como |estas
	int nsize = 59;
	char buffer[60	];

	int part = (nsize+1)/(MPI_Size-1);

	if(MPI_Rank == 0){
		ifstream in_file;
		in_file.open("text.txt");
		in_file.read(buffer,nsize);
		in_file.close();
		buffer[59] = ' ';

		int f_buff_pos = 0;
		cout<<buffer<<endl;
		for(int i = 1 ; i < MPI_Size; ++i){
			MPI_Send(&buffer[f_buff_pos], part, MPI_CHAR, i, tag, MPI_COMM_WORLD );
			f_buff_pos += part;
		}
		int total_words = 0;
		for(int i = 1; i < MPI_Size; ++i){
			MPI_Recv(&words, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD , &status);
			total_words += words;
		}
		printf("total words: %d\n", total_words);
	}
	else
	{
		MPI_Recv(&buffer , part , MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		//show_str(buffer, part);
		words = prl_count_words(buffer, part);
		//printf("%d\n", words);
		MPI_Send(&words, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

	}

	MPI_Finalize();

	return 0;
}