#pragma once

#ifndef __CUCKOO_SEARCH_H__
#define __CUCKOO_SEARCH_H__

#include "stdafx.h"
#include <fstream>


template <std::size_t number_of_nests, class T>
class CuckooSearch
{
	int bestnest_idx, worstnest_idx;
	const double discovery_chance = 0.25;
	//JUST FOR STATUS PRINT
	double current_new_fitness;

public:
	CuckooSearch();
	~CuckooSearch() {};
	void run(int rounds = 1);

private:
	T nests[number_of_nests];
	void challenge_random_cuckoo();
	inline bool got_discovered() { return (double)rand() / RAND_MAX < discovery_chance; };
	void update_worstnest_idx();
	void empty_worst_nest();
};


template <std::size_t number_of_nests, class T>
CuckooSearch<number_of_nests, T>::CuckooSearch()
{
	std::srand(static_cast<unsigned int>(time(NULL)));

	//init first nest
	std::cout << "Initializing nest #1..." << std::flush;
	nests[0].init();
	std::cout << " Finished" << std::endl;
	worstnest_idx = bestnest_idx = 0;

	//init other nests and check if better/worse
	for (int i = 1; i < number_of_nests; i++) {
		std::cout << "Initializing nest #" << i+1 << "..." << std::flush;
		nests[i].init();
		std::cout << " Finished" << std::endl;
		if (nests[i].fitness > nests[worstnest_idx].fitness) worstnest_idx = i;
		if (nests[i].fitness < nests[bestnest_idx].fitness) bestnest_idx = i;
	}
}


template <std::size_t number_of_nests, class T>
void CuckooSearch<number_of_nests, T>::run(int rounds)
{
	std::ofstream out("CS_lin318_with_local.dat");

	for (int i = 0; i < rounds; i++) {
		challenge_random_cuckoo();
		if (got_discovered()) empty_worst_nest();
		if (!(i % 21)) {
			std::cout << "#" << i / 21 << ", Best: " << nests[bestnest_idx].fitness << ", Worst: " << nests[worstnest_idx].fitness << std::endl;//", New: " << current_new_fitness <<", step: " << stepsize << std::endl;		
			out << i / 21 << " " << nests[bestnest_idx].fitness << " " << nests[worstnest_idx].fitness << std::endl;//", New: " << current_new_fitness <<", step: " << stepsize << std::endl;		
																																				//std::cout << nests[bestnest_idx] << std::endl;
		}
	}
	out.close();
}


template <std::size_t number_of_nests, class T>
void CuckooSearch<number_of_nests, T>::challenge_random_cuckoo()
{
	int origin = //bestnest_idx;
		rand() % number_of_nests;
	int competitor = rand() % number_of_nests;
	T new_nest = nests[origin].create_new();
	current_new_fitness = new_nest.fitness;
	if (new_nest.fitness < nests[competitor].fitness) {
		//std::cout << "REPLACING #" << competitor << "/" << nests[competitor].fitness << " with " << new_nest.fitness << " from " << nests[origin].fitness << ", delta: " << nests[origin].fitness - new_nest.fitness << std::endl;
		nests[competitor] = new_nest;
		if (new_nest.fitness < nests[bestnest_idx].fitness) bestnest_idx = competitor;
		if (competitor == worstnest_idx) update_worstnest_idx();
	}
}


template <std::size_t number_of_nests, class T>
void CuckooSearch<number_of_nests, T>::update_worstnest_idx()
{
	double worst_fitness = nests[0].fitness;
	worstnest_idx = 0;
	
	for (int i = 1; i < number_of_nests; i++) {
		if (nests[i].fitness > worst_fitness) {
			worstnest_idx = i;
			worst_fitness = nests[i].fitness;
		}
	}
}


template <std::size_t number_of_nests, class T>
void CuckooSearch<number_of_nests, T>::empty_worst_nest()
{
	//std::cout << "EMPTY" << std::endl;
	int origin = bestnest_idx;
		//rand() % number_of_nests;
	double old_fitness = nests[worstnest_idx].fitness, new_fitness;
	nests[worstnest_idx] = nests[origin].create_new();
	new_fitness = nests[worstnest_idx].fitness;
	if (new_fitness < old_fitness) {
		if (new_fitness < nests[bestnest_idx].fitness) bestnest_idx = worstnest_idx;
		update_worstnest_idx();
	}
}


#endif //__CUCKOO_SEARCH_H__
