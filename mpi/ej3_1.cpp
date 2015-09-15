#include <mpi.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#include <algorithm>
#include <sstream>

#define TEXT_SIZE 100

using namespace std;

std::size_t count ( const std::string& text, const std::string& word )
{
    std::istringstream input(text);
    return (std::count(std::istream_iterator<std::string>(input),
        std::istream_iterator<std::string>(), word));
}
// Code by: http://stackoverflow.com/questions/4197786/find-repeated-words-number-in-c
void show_str(char*arr , int size){
	for(int _= 0 ; _<size; ++_){
		printf("%c", arr[_]);
	}
	printf("\n");
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
	int nsize = TEXT_SIZE-1;
	char buffer[TEXT_SIZE];
	int repeated;

	const char word[] = "archivo";


	//char query[7] = {''}; 

	int part = (nsize+1)/(MPI_Size-1);
	char substr [2*part]; 
	for(int i = 0 ; i < 2*part ; i++){
		substr[i] = ' ';
	}

	if(MPI_Rank == 0){
		ifstream in_file;
		in_file.open("text.txt");
		in_file.read(buffer,nsize);
		in_file.close();
		buffer[TEXT_SIZE-1] = ' ';

		int f_buff_pos = 0;
		//cout<<buffer<<endl;

		int position_i = 0;
		int position_f = part;

		printf("\n");

		

		for(int i = 1 ; i < MPI_Size; ++i){

			//cout<<"proceso "<<i<<", pos "<<position_f<<endl;
			for(int m = 0 ; m < 2*part ; m++){
				substr[m] = ' ';
			}

			int k = 0;
			for(; k < part && k+position_i < TEXT_SIZE-1; k++){
				substr[k] = buffer[k+position_i];
			}

			// cout<<"paso el primero"<<endl;
			while(buffer[k+position_i] != ' '){
			//	printf("%c",buffer[position_f] );
				substr[k] = buffer[k+position_i];
				k++;
			}
			position_i += k;

	
			// show_str(substr , 2*part);
				
			MPI_Send(&substr, 2*part, MPI_CHAR, i, tag, MPI_COMM_WORLD );

		}


		int total_words = 0;
		for(int i = 1; i < MPI_Size; ++i){
			MPI_Recv(&words, 1, MPI_INT, i, tag, MPI_COMM_WORLD , &status);
			total_words += words;
		}
		printf("total words: %d\n", total_words);
	}
	else{


		MPI_Recv(&substr , 2*part , MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		// show_str(substr, 2*part);
		repeated = count(substr , word);
		printf("%d\n", repeated);
		// words = prl_count_words(buffer, part);
		//printf("%d\n", words);
		MPI_Send(&repeated, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);


	}

	return 0;
}
