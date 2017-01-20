#pragma once

#ifndef __LEVYRND_H__
#define __LEVYRND_H__

#include "stdafx.h"

class LevyRnd
{
	static bool seeded;
	const double TWO_PI = 2.*M_PI,
		MIN_DOUBLE_LIMIT = std::numeric_limits<double>::min();
	double scaling_factor = 1.;
	double z0, z1, u, v, w, beta, inverse_beta, sigma;
	bool generate;

public:
	LevyRnd(double beta, double sigma);
	LevyRnd() : LevyRnd(1.5, 0.69657450255769679272122) {};
	~LevyRnd();
	double rnd();
	LevyRnd operator*(double factor);
	friend LevyRnd operator*(double factor, LevyRnd& levy);

private:
	double normalRnd();
};

#endif //__LEVYRND_H__