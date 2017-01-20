#pragma once
#ifndef __SIMPLE_NEST_H__
#define __SIMPLE_NEST_H__


#include "NestTemplate.h"


extern double stepsize;


class SimpleNest :
	public NestTemplate<double, SimpleNest>
{
private:
	double calc_fitness();

public:
	static double max_val;
	static double min_val;

	static unsigned int dim;
	static double scaling_factor;

	SimpleNest();
	~SimpleNest();

	SimpleNest& init();
	SimpleNest operator+(LevyRnd& levy);
};

#endif //__SIMPLE_NEST_H__