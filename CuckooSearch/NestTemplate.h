#pragma once

#ifndef __NEST_TEMPLATE_H__
#define __NEST_TEMPLATE_H__


#include "stdafx.h"
#include "LevyRnd.h"


template <typename T, class I>
class NestTemplate
{
private:
	virtual double calc_fitness() = 0;

public:
	static LevyRnd levy;
	std::vector<T> nest;
	double fitness;

	NestTemplate();
	NestTemplate(const NestTemplate<T, I>& orig_nest);
	virtual I& operator=(const I& orig_nest);
	virtual ~NestTemplate();
	virtual I create_new();

	virtual I& init() = 0;
	virtual I operator+(LevyRnd& levy) = 0;
	
	T& operator[](std::size_t i);
	friend I operator+(LevyRnd& levy, I& nest);

	friend std::ostream& operator<<(std::ostream& os, I& os_nest) {
		os << "[";
		for (int i = 0; i < os_nest.nest.size(); i++) {
			os << os_nest.nest[i] << ((i < os_nest.nest.size() - 1) ? ", " : "]");
		}
		os << ", fitness: " << os_nest.fitness;
		return os;
	}
};


template<typename T, class I> LevyRnd NestTemplate<T, I>::levy;


template<typename T, class I> NestTemplate<T, I>::NestTemplate()
{
	nest.resize(I::dim);
}


template<typename T, class I> NestTemplate<T, I>::NestTemplate(const NestTemplate<T, I>& orig_nest)
{
	nest.resize(orig_nest.nest.size());
	for (int i = 0; i < nest.size(); i++) {
		nest[i] = orig_nest.nest[i];
	}
	fitness = orig_nest.fitness;
}


template<typename T, class I> I& NestTemplate<T, I>::operator=(const I& orig_nest)
{
	nest.resize(orig_nest.nest.size());
	for (int i = 0; i < nest.size(); i++) {
		nest[i] = orig_nest.nest[i];
	}
	fitness = orig_nest.fitness;
	return *static_cast<I*>(this);
}


template<typename T, class I> NestTemplate<T, I>::~NestTemplate()
{
}


template<typename T, class I>
T& NestTemplate<T, I>::operator[](std::size_t i)
{
	return nest[i];
}


template<typename T, class I>I NestTemplate<T, I>::create_new() {
	return *this + I::scaling_factor * levy;
}


template<typename T, class I> I operator+(LevyRnd& levy, I& nest)
{
	return nest + levy;
}

#endif //__NEST_TEMPLATE_H__