#include "stdafx.h"
#include "LevyRnd.h"


bool LevyRnd::seeded = false;


LevyRnd::LevyRnd(double beta, double sigma)
{
	if (!seeded) {
		std::srand(static_cast<unsigned int>(time(NULL)));
		seeded = true;
	}

	this->beta = beta;
	this->inverse_beta = 1. / beta;
	this->sigma = sigma;
}


LevyRnd::~LevyRnd()
{
}


double LevyRnd::normalRnd()
{
	static bool generate;
	generate = !generate;

	//if (!generate)
	//	return z1;

	double u1, u2;

	do {
		u1 = (double)rand() / RAND_MAX;
		u2 = (double)rand() / RAND_MAX;
	} while (u1 <= MIN_DOUBLE_LIMIT);

	if (-log(u1) > 0) {
		z0 = sqrt(-2.0 * log(u1)) * cos(TWO_PI * u2);
	}
	else {
		z0 = 0; // z1 = 0;
	}
	return z0;
}

double LevyRnd::rnd()
{
	u = normalRnd();
	do {
		v = normalRnd();
	} while (v == 0);

	double result = scaling_factor * u * sigma / pow(fabs(v), inverse_beta);
	return result;
}

LevyRnd LevyRnd::operator*(double factor)
{
	LevyRnd new_levy(beta, sigma);
	new_levy.scaling_factor = factor * scaling_factor;
	return new_levy;
}

LevyRnd operator*(double factor, LevyRnd& levy)
{
	return levy * factor;
}
