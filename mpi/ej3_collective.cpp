#include <mpi.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#include <algorithm>
#include <sstream>

#define TAG 10
#define TEXT_SIZE 3000000

using namespace std;

std::size_t count ( const std::string& text, const std::string& word )
{
    std::istringstream input(text);
    return (std::count(std::istream_iterator<std::string>(input),
        std::istream_iterator<std::string>(), word));
}
// Code by: http://stackoverflow.com/questions/4197786/find-repeated-words-number-in-c


int main(int argc, char *argv[])
{
	int MPI_Rank , MPI_Size ; 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD , &MPI_Rank );
	MPI_Comm_size(MPI_COMM_WORLD , &MPI_Size );

	int local = 3;
	int total ;

	int nsize = TEXT_SIZE-1;
	char buffer[TEXT_SIZE];
	char short_buffer[TEXT_SIZE/(MPI_Size-1)];

	int total_words;

	const char word[] = "delicate";

	if(MPI_Rank == 0){
		printf("p 0\n");
		ifstream in_file;
		in_file.open("big.txt");
		in_file.read(buffer,nsize);
		in_file.close();
	}

	int part = (nsize+1)/(MPI_Size);
	MPI_Scatter(buffer , part , MPI_CHAR,  short_buffer, part, MPI_CHAR, 0, MPI_COMM_WORLD);

	// printf("%s process %d\n", short_buffer, MPI_Rank);

	int repeated = count(short_buffer , word);

	MPI_Reduce(&repeated, &total_words, 1, MPI_INT , MPI_SUM, 0, MPI_COMM_WORLD);

	if(MPI_Rank == 0){
		printf("total words = %d\n", total_words);
	}

	MPI_Finalize();


	return 0;
}

