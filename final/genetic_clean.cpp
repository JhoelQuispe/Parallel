#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread>


#define POPULATION 1000
#define GENERATIONS 50
#define P_MUTATION 2
#define P_CROSSOVER 80
#define ELITISM 10

using namespace std;


double mean_cost(int node, vector<double> p1, vector<double> p2, vector<double> p3) {
	return ( p1[node] + p2[node] + p3[node] ) / 3.0;
}


	struct nn
	{
		double rank;
		int id;
		bool scheduled; 
		int processor_assigned;
	};


bool compare_by_rank(nn &a, nn &b) {
	return a.rank > b.rank;
}

double EST(int node, vector<double> pr, double com[10][10], double avail, vector<double> aft, double current_processor, vector<nn> ranks){
	if(node == 0){
		return 0;
	}
	else {

		vector<double> ready_time;
		for(int i = 0; i < 10; i++){
			if(com[i][node] > 0){
				if (current_processor == ranks[i].processor_assigned){
					ready_time.push_back(aft[i]);
				}
				else {
					ready_time.push_back(com[i][node] + aft[i]);
				}
			}
		}

		return max(avail, *max_element(ready_time.begin(), ready_time.end()) );

	}
}



double EST_(int node, vector<double> pr, double com[10][10], double avail, vector<double> aft, double current_processor, vector<int> &chromo){
	if(node == 0){
		return 0;
	}
	else {

		vector<double> ready_time;
		for(int i = 0; i < 10; i++){
			if(com[i][node] > 0){
				if (current_processor == chromo[i]){
					ready_time.push_back(aft[i]);
				}
				else {
					ready_time.push_back(com[i][node] + aft[i]);
				}
			}
		}

		return max(avail, *max_element(ready_time.begin(), ready_time.end()) );

	}
}



double calculate_fitness ( vector<int> chromosome, vector< vector<double> > &P) {

	// It's the same than eft.



	cout << "calculate_fitness" << endl;
	double procesor_time [3];

	for (int i = 10; i < 20; ++i)
	{
		cout<<chromosome[i] << "-";
		cout<< P[chromosome[chromosome[i]]][chromosome[i]]  <<",";

		procesor_time[chromosome[chromosome[i]]] += P[chromosome[chromosome[i]]][chromosome[i]];

	}

	cout << endl;

	for (int i = 0; i < 3; ++i)
	{
		cout << procesor_time[i] << " ";
	}

	cout << endl;

}

double calculate_fitness_2 ( vector<int> chromosome, vector< vector<double> > &P, double com[10][10] ) {

	double avail[3] = {0.};
	vector<double> Veft(10,0.);

	for (int i = 10; i < 20; ++i){
		double est = EST_(chromosome[i], P[chromosome[chromosome[i]]], com, avail[chromosome[chromosome[i]]], Veft, chromosome[chromosome[i]], chromosome);
		double eft = est + P[chromosome[chromosome[i]]][chromosome[i]];
		avail[chromosome[chromosome[i]]] = eft;
		Veft[chromosome[i]] = eft;

	}

	return (double)*max_element(Veft.begin(), Veft.end());
}

void select_chromosomes (vector<double> &V_fitness, vector<int> &selected_parents){
	for(int i = 0; i < POPULATION; ++i) {
		int indiv_1 = rand()%POPULATION;
		int indiv_2 = rand()%POPULATION;
		
		selected_parents[ (V_fitness[indiv_1] < V_fitness[indiv_2]) ? indiv_1 : indiv_2 ] += 1; 
		
	}
}

void mutation (vector< vector<int> > &population, double com[10][10]) {


	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < POPULATION; ++i) {

		int prob = rand()%100;
		if(prob < P_MUTATION) {
	
			int new_processor = rand()%3;
			int selected_gen = rand()%10; // only processors
			population[i][selected_gen] = new_processor;


			int selected_gen1 = rand()%10+10;
			int selected_gen2 = rand()%10+10;

			swap(population[i][selected_gen1], population[i][selected_gen2]);

			for(auto gen: population[i]) cout << gen << ' ';
			vector <int> posiciones;
			int iterator = 10;

			while(iterator < 20){

				int maximo = 10;
			
				for (int j = 0; j < 10; ++j) {
					if (com[j][population[i][iterator]] > 0 ) {
						for (int ii = 10; ii < 20; ++ii)
						{	
							if (population[i][ii] == j){
								posiciones.push_back(ii);
							}
						}
					}
				}

				if(posiciones.size() > 0){
					maximo = (int)(*max_element(posiciones.begin(), posiciones.end()) ) + 1;
				}

				posiciones.clear();

				if(maximo > iterator){
					// shift
					vector <double> new_chromo(20);

					for (int k = 0; k < iterator; ++k) {
						new_chromo[k] = population[i][k];
					}
					for (int k = iterator+1; k < maximo; ++k) {
						new_chromo[k-1] = population[i][k];
					}
					new_chromo[maximo-1] = population[i][iterator];
					for(int k = maximo; k < 20; ++k) {
						new_chromo[k] = population[i][k];
					}

					for (int gg = 0; gg < 20; ++gg)
					{
						population[i][gg] = new_chromo[gg];
					}

				}
				else{
					iterator++ ;
				}
				// iterator++ ;


			}

		}

	}


}

void elitism (vector<double> V_fitness, vector<int> elite_population, vector< vector<int> > &new_population, vector< vector<int> > &old_population) {

	vector<int> index(V_fitness.size(), 0);

	for (int i = 0 ; i != index.size(); ++i) {
		index[i] = i;
	}

	sort(index.begin(), index.end(),
		[&](const int& a, const int& b) {
			return (V_fitness[a] < V_fitness[b]);
		}
	);

	for (int i = 0; i < ELITISM; ++i)
	{

	
		for(int j = 0; j < 20; ++j) {
			new_population[i][j] = old_population[index[i]][j];
		}

	}


}

void crossover (vector<int> &selected_parents, vector< vector<int> > &population, vector< vector<int> > &new_population, double com[10][10]) {
	
	int n_crossover = POPULATION/2;

	int iterator_i = 0;
	int iterator_j = POPULATION-1;

	int k = 0;

	#pragma parallel for num_threads(4)
	for (int i = 0; i < n_crossover; ++i, k+=2) {

		int c_prob = rand()%100;

		int parent_1, parent_2;

		while (selected_parents[iterator_i] == 0 ) {
			iterator_i++;
		}
		parent_1 = iterator_i;

		while (selected_parents[iterator_j] == 0 ) {
			iterator_j--;
		}
		parent_2 = iterator_j;

		selected_parents[iterator_i]--;
		selected_parents[iterator_j]--;


		if ( c_prob < P_CROSSOVER ) {

			//////////////////////////////////////////

			int cut_point = rand()%(10-1);
			int side = rand()%2;

			for (int j = 0; j <= cut_point; ++j)
			{
				new_population[k][j] = population[parent_1][j];
				new_population[k+1][j] = population[parent_2][j];	
			}

			for (int j = cut_point+1 + 10*side; j < 20; ++j)
			{
				new_population[k][j] = population[parent_2][j];
				new_population[k+1][j] = population[parent_1][j];
			}

		}

		else {
			for (int j = 0; j < 20; ++j) {
				new_population[k][j] = population[parent_1][j];
				new_population[k+1][j] = population[parent_2][j];
			}
		}

	}


	vector<int> elementos_faltantes;
	vector<int> elementos_sobrantes;

	int ggwp = 0;

	#pragma parallel for num_threads(4)
	for (int i = 0; i < POPULATION; ++i) {

		for ( int j = 0; j < 10; ++j) {
			int ite = 10;
			int veces = 0;
			while(ite < 20) {
				if (new_population[i][ite] == j) {
					veces += 1;
					if (veces > 1) {
						elementos_sobrantes.push_back(ite);
					}
				}
				ite++;
			}

			if(veces == 0) {
				elementos_faltantes.push_back(j);
			}
		} 

		sort(elementos_sobrantes.begin(), elementos_sobrantes.end());

		for(int j = 0; j < elementos_faltantes.size(); ++j) {
			new_population[i][elementos_sobrantes[j]] = elementos_faltantes[j];
		}	

		elementos_sobrantes.clear();
		elementos_faltantes.clear();

	}

	for (int i = 0; i < POPULATION; ++i) {

		vector <int> posiciones;
		int iterator = 10;

		while(iterator < 20){

			int maximo = 10;
		
			for (int j = 0; j < 10; ++j) {
				if (com[j][new_population[i][iterator]] > 0 ) {
					for (int ii = 10; ii < 20; ++ii)
					{	
						if (new_population[i][ii] == j){
							posiciones.push_back(ii);
						}
					}
				}
			}


			if(posiciones.size() > 0){
				maximo = (int)(*max_element(posiciones.begin(), posiciones.end()) ) + 1;
			}

			posiciones.clear();

			if(maximo > iterator){
				// shift
				vector <double> new_chromo(20);

				for (int k = 0; k < iterator; ++k) {
					new_chromo[k] = new_population[i][k];
				}
				for (int k = iterator+1; k < maximo; ++k) {
					new_chromo[k-1] = new_population[i][k];
				}
				new_chromo[maximo-1] = new_population[i][iterator];

				for(int k = maximo; k < 20; ++k) {
					new_chromo[k] = new_population[i][k];
				}

				for (int gg = 0; gg < 20; ++gg)
				{
					new_population[i][gg] = new_chromo[gg];
				}

			}
			else{
				iterator++ ;
			}


		}

	}

}



void copy_population(vector< vector<int> > &old_population, vector< vector<int> > &new_population) {
	for (int i = 0; i < POPULATION; ++i) {
		for (int j = 0; j < 20; ++j) {
			new_population[i][j] = old_population[i][j];
		}
	}
}

int main(){


	vector <double> vec;

	vector < vector<double> > P;


	vector<double> P1 = {14,13,11,13,12,13,7,5,18,21};
	vector<double> P2 = {16,19,13,8,13,16,15,11,12,7};
	vector<double> P3 = {9,18,19,17,10,9,11,14,20,16};

	P.push_back(P1);
	P.push_back(P2);
	P.push_back(P3);


	double com[10][10] = {0};


	vector <double> avail(3);

	for (int i = 0; i < 3; ++i)
	{
		avail[i] = 0; 
	}

	com[0][1] = 18;
	com[0][2] = 12;
	com[0][3] = 9;
	com[0][4] = 11;
	com[0][5] = 14;
	com[1][7] = 19;
	com[1][8] = 16;
	com[2][6] = 23;
	com[3][7] = 27;
	com[3][8] = 23;
	com[4][8] = 13;
	com[5][7] = 15;
	com[6][9] = 17;
	com[7][9] = 11;
	com[8][9] = 13;



	for (int i = 0; i < 10; ++i)
	{
		vec.push_back(i);
	}

	vector <nn> ranks(10);

	for(int i = 0; i < 10; i++){
		ranks[i].rank = 0.;
		ranks[i].id = i;
		ranks[i].scheduled = false;
		ranks[i].processor_assigned = -1;
	}



	ranks[9].rank = mean_cost(9, P[0], P[1], P[2]);


	for (int i = 8; i >= 0; --i ) {
		vector <double> c_aux;
		// ranks [8] = mean_cost(i, p1, p2, p3)
		for(int j = 0; j < 10; ++j) {
			if (com[i][j] > 0) {
				c_aux.push_back(com[i][j] + ranks[j].rank);
			}
		} 
		ranks[i].rank = mean_cost(i, P[0], P[1], P[2]) + *max_element(c_aux.begin(), c_aux.end());
	}


	sort(ranks.begin(), ranks.end(), compare_by_rank);

	// while unscheduled

	vector<double> eft(10,0);

	vector<double> processor_order(10,0);

	for(int i = 0; i < 10; i++) {

		vector <double> aux_eft(3);

		for(int p = 0; p < P.size(); ++p){
			// cout << "p->" << p << endl;

			double est = EST(ranks[i].id, P[p], com, avail[p], eft, p, ranks);
			double eft = est + P[p][ranks[i].id];
			eft += 0;
			aux_eft[p] = eft; 

		}


		int min_eft = *min_element(aux_eft.begin(), aux_eft.end());
		int assigned = (int)(min_element(aux_eft.begin(), aux_eft.end()) - aux_eft.begin());
		printf("assigned: %d\n", assigned);
		ranks[ranks[i].id].processor_assigned = assigned;
		avail[assigned] = min_eft ;

		processor_order[ranks[i].id] = assigned;

		eft[ranks[i].id] = min_eft;

	}


	/// INIT POPULATION
	vector< vector<int> > population;

	vector< vector<int> > new_population;

	for (int i = 0; i < POPULATION; ++i)
	{
		vector<int> new_chromosome(20,0);
		new_population.push_back(new_chromosome);
	}

	for(int i = 0; i < POPULATION; ++i) {
		vector<int> indiv(20,0) ;
		for (int j = 0; j < 10; j++) {
			indiv[j] = processor_order[j];	
		}
		for (int j = 0; j < 10; j++) {
			indiv[10+j] = ranks[j].id;
		}
		population.push_back(indiv);
	}

	vector<double> V_fitness(POPULATION);


	for(int generation = 0; generation < GENERATIONS; ++generation) {

		cout << "GENERATION: " <<generation << endl;

		vector<int> selected_parents(POPULATION, 0);

		#pragma parallel for num_threads(4);
		for(int i = 0; i < POPULATION; ++i) {
			double fitness = calculate_fitness_2(population[i], P, com);
			V_fitness[i] = fitness;
		}

		// cout << "Fitness" << endl;
		// for( auto fitness : V_fitness){
		// 	cout << fitness << " ";
		// }

		// cout << endl;

		vector<int> elite_population(ELITISM);


		select_chromosomes( V_fitness, selected_parents );  // tournament selection

		copy_population(population, new_population);

		crossover (selected_parents, population, new_population, com);

		mutation (new_population, com);

		elitism(V_fitness, elite_population, new_population, population);

		random_shuffle(new_population.begin(), new_population.end());

		copy_population(new_population, population);

	}


	for(int i = 0; i < POPULATION; ++i) {
		double fitness = calculate_fitness_2(population[i], P, com);
		V_fitness[i] = fitness;
	}

	cout << "Fitness" << endl;
	for( auto fitness : V_fitness){
		cout << fitness << " ";
	}


	cout << endl;

	for(int i = 0; i < 20; ++i) {
		cout << population[0][i] << " ";
	}

	double fitness = calculate_fitness_2(population[0], P, com);


	return 0;
}
