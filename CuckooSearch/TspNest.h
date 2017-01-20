#pragma once
#ifndef __TSP_NEST_H__
#define __TSP_NEST_H__

#include "NestTemplate.h"

//extern double stepsize;

class TspNest :
	public NestTemplate<unsigned int, TspNest>
{
private:
	double calc_fitness();
	double two_opt_gain(unsigned int first_idx, unsigned int second_idx);
	double double_bridge_gain(unsigned int idx1, unsigned int idx2, unsigned int idx3, unsigned int idx4);

public:
	static double scaling_factor;
	static unsigned int dim;
	static std::vector<std::vector<std::pair<unsigned int, double> > > tsp_ordered_list;
	static std::vector<std::vector<double> > tsp_map;
	static bool load_locations(const char* filename);

	TspNest();
	TspNest& init();
	~TspNest();
	TspNest operator+(LevyRnd& levy);

	TspNest& TspNest::two_opt(unsigned int first_idx, unsigned int second_idx);
	TspNest& two_opt();
	TspNest& TspNest::double_bridge(unsigned int idx1, unsigned int idx2, unsigned int idx3, unsigned int idx4);
	TspNest& double_bridge();
	TspNest& to_local_optimum();
};

#endif //__TSP_NEST_H__