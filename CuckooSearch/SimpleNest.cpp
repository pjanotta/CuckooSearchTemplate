#include "stdafx.h"
#include "SimpleNest.h"
#include "LevyRnd.h"


double SimpleNest::min_val = -5.0;
double SimpleNest::max_val = 5.0;
unsigned int SimpleNest::dim = 15;
double SimpleNest::scaling_factor = 0.01;


double stepsize;


SimpleNest::SimpleNest() : NestTemplate<double, SimpleNest>()
{
}


SimpleNest& SimpleNest::init()
{
	nest.resize(dim);
	for (unsigned int i = 0; i < SimpleNest::dim; i++) {
		nest[i] = (double)rand() / RAND_MAX * (max_val - min_val) + min_val;
	}
	calc_fitness();
	return *this;
}


SimpleNest::~SimpleNest()
{
}


double SimpleNest::calc_fitness()
{
	double total_of_squares = 0;
	for (int i = 0, dim = (int)nest.size(); i < dim; i++) {
		total_of_squares += nest[i]*nest[i];
	}
	return this->fitness = sqrt(total_of_squares);
}


SimpleNest SimpleNest::operator+(LevyRnd& levy)
{
	SimpleNest new_nest(*this);
	double levy_total = 0;
	for (int i = 0, dim = (int)nest.size(); i < dim; i++) {
		double levy_val = levy.rnd();
		levy_total += levy_val*levy_val;
		new_nest.nest[i] += levy_val;
		if (new_nest.nest[i] > max_val) new_nest.nest[i] = max_val;
		else if (new_nest.nest[i] < min_val) new_nest.nest[i] = min_val;
	}
	stepsize = sqrt(levy_total);
	new_nest.calc_fitness();
	return new_nest;
}
