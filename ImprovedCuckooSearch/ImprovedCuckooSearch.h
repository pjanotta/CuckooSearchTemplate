#pragma once

#ifndef __IMPROVED_CUCKOO_SEARCH_H__
#define __IMPROVED_CUCKOO_SEARCH_H__

#include "stdafx.h"
#include <algorithm>
#include <fstream>

template <std::size_t number_of_nests, class T>
class ImprovedCuckooSearch
{
	int bestnest_idx, worstnest_idx;
	const double discovery_chance = 0.2;
	const double smart_cuckoo_fraction = 0.6;
	//JUST FOR STATUS PRINT
	double current_new_fitness;

public:
	ImprovedCuckooSearch();
	~ImprovedCuckooSearch() {};
	void run(int rounds = 1);

private:
	T nests[number_of_nests];
	void smart_cuckoo_search();
	void challenge_random_cuckoo();
	void challenge_cuckoo(unsigned int origin, unsigned int competitor);
	//inline bool got_discovered() { return (double)rand() / RAND_MAX < discovery_chance; };
	void update_worstnest_idx();
	void replace_worst_nests();
};


template <std::size_t number_of_nests, class T>
ImprovedCuckooSearch<number_of_nests, T>::ImprovedCuckooSearch()
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
void ImprovedCuckooSearch<number_of_nests, T>::run(int rounds)
{
	for (int i = 0; i < rounds; i++) {
		smart_cuckoo_search();
		challenge_random_cuckoo();
		replace_worst_nests();
		std::cout << "#" << i << ", Best: " << nests[bestnest_idx].fitness << ", Worst: " << nests[worstnest_idx].fitness << std::endl;//", New: " << current_new_fitness <<", step: " << stepsize << std::endl;		
	}
}

template<std::size_t number_of_nests, class T>
void ImprovedCuckooSearch<number_of_nests, T>::challenge_cuckoo(unsigned int origin, unsigned int competitor)
{
	T new_nest = nests[origin].create_new();
	if (new_nest.fitness < nests[competitor].fitness) {
		if (new_nest.fitness < nests[bestnest_idx].fitness) {			
			bestnest_idx = competitor;
		}
		nests[competitor] = new_nest;
		if (competitor == worstnest_idx) update_worstnest_idx();
	}
}


template<std::size_t number_of_nests, class T>
void ImprovedCuckooSearch<number_of_nests, T>::smart_cuckoo_search()
{
	std::vector<unsigned int> nest_idcs(number_of_nests);
	std::vector<unsigned int> rnd_vals(number_of_nests);
	for (unsigned int i = 0; i < number_of_nests; i++) {
		rnd_vals[i] = rand();
		nest_idcs[i] = i;
	}
    std::sort(nest_idcs.begin(), nest_idcs.end(), [=](const unsigned int &idx1, const unsigned int &idx2)->bool { return rnd_vals[idx1] > rnd_vals[idx2]; });

	for (unsigned int i = 0; i < static_cast<unsigned int>(smart_cuckoo_fraction * number_of_nests); i++) {
		challenge_cuckoo(nest_idcs[i], nest_idcs[i]);
	}

}



template <std::size_t number_of_nests, class T>
inline void ImprovedCuckooSearch<number_of_nests, T>::challenge_random_cuckoo()
{
	challenge_cuckoo(bestnest_idx, rand() % number_of_nests);
}

template <std::size_t number_of_nests, class T>
void ImprovedCuckooSearch<number_of_nests, T>::update_worstnest_idx()
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
void ImprovedCuckooSearch<number_of_nests, T>::replace_worst_nests()
{
	std::vector<unsigned int> nest_idcs(number_of_nests);
	for (unsigned int i = 0; i < number_of_nests; i++) nest_idcs[i] = i;
	
	std::sort(nest_idcs.begin(), nest_idcs.end(), [=](const unsigned int &idx1, const unsigned int &idx2) { return nests[idx1].fitness > nests[idx2].fitness; });

	for (unsigned int i = 0; i < static_cast<unsigned int>(discovery_chance * number_of_nests); i++) {
		nests[nest_idcs[i]] = nests[bestnest_idx].create_new().double_bridge().to_local_optimum();
		if (nests[nest_idcs[i]].fitness < nests[bestnest_idx].fitness) {
			bestnest_idx = nest_idcs[i];
		}
	}
	update_worstnest_idx();
}


#endif //__IMPROVED_CUCKOO_SEARCH_H__
