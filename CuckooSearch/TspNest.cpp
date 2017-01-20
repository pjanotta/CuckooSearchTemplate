#include "stdafx.h"
#include "TspNest.h"
#include "LevyRnd.h"
#include "tinyxml2.h"
#include <algorithm>
#include <utility>
#include <iterator>

using namespace std;
using namespace tinyxml2;

//double stepsize;


unsigned int TspNest::dim = 0;
double TspNest::scaling_factor = 1;
vector<vector<pair<unsigned int, double> > > TspNest::tsp_ordered_list;
vector<vector<double> > TspNest::tsp_map;


bool TspNest::load_locations(const char* filename)
{
	int vertex_out = 0, vertex_in = 0, number_of_vertices = 0;
	double costs;
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);
	XMLElement *pEdge, *pElem;

	if (eResult != XML_SUCCESS) {
		cerr << "Error: " << eResult << endl;
		cerr << xmlDoc.GetErrorStr1() << endl;
		cerr << xmlDoc.GetErrorStr2() << endl;

		return false;
	}
	else cout << "Successfully loaded XML file" << endl;

	XMLNode *pRoot = xmlDoc.FirstChildElement("travellingSalesmanProblemInstance");
	XMLElement *pGraph = pRoot->FirstChildElement("graph");

	pElem = pGraph->FirstChildElement("vertex");

	while (pElem != NULL) {
		vector<pair<unsigned int, double> > indexed_costs;
		vector<double> out_costs;
		vertex_in = 0;
		number_of_vertices++;
		pEdge = pElem->FirstChildElement("edge");
		while (pEdge != NULL) {
			if (vertex_out == vertex_in) {
				out_costs.push_back(0);
				vertex_in++;
				continue;
			} 
			else {
				pEdge->QueryDoubleAttribute("cost", &costs);
				indexed_costs.push_back(pair<unsigned int, double>(static_cast<unsigned int>(vertex_in), costs) );
				out_costs.push_back(costs);
				vertex_in++;
			}
			pEdge = pEdge->NextSiblingElement("edge");
		}
		sort(indexed_costs.begin(), indexed_costs.end(), [](pair<unsigned int, double>& first_pair, pair<unsigned int, double>& second_pair) { return first_pair.second < second_pair.second; });
		tsp_ordered_list.push_back(indexed_costs);
		pElem = pElem->NextSiblingElement("vertex");
		if (pElem == NULL) out_costs.push_back(0); // Add self-reference of last vertex
		tsp_map.push_back(out_costs);
		vertex_out++;
	}
	dim = number_of_vertices;
	return true;
}


TspNest::TspNest() : NestTemplate<unsigned int, TspNest>()
{
}


TspNest& TspNest::init()
{
	unsigned int current_location = rand() % dim;
	unsigned int next_location = current_location;
	unsigned int first_location = nest[0] = current_location;
	double total_costs = 0;
	vector<bool> unvisited_location(dim, true);
	unvisited_location[first_location] = false;

	for (unsigned int i = 1; i < dim; i++) {
		double min_costs;
		do {
/* 
 * OPTION1 init via Levy Flights in ordered neighboor ranks */
			int jmin = floor(fabs(0.3*levy.rnd()));
			for (unsigned int j = jmin; j < dim - 1; j++) {
				if (unvisited_location[tsp_ordered_list[current_location][j].first]) { //Never the case due to ordering: min_costs > tsp_map[i][j].second
					next_location = tsp_ordered_list[current_location][j].first;
					min_costs = tsp_ordered_list[current_location][j].second;
					break;
				}
			}
/**/
/*
 * OPTION2 init with completely random permutations *
			int j = rand() % (dim-1);
			if (unvisited_location[tsp_ordered_list[current_location][j].first]) {
				next_location = tsp_ordered_list[current_location][j].first;
				min_costs = tsp_ordered_list[current_location][j].second;
				break;
			}
*/
		} while (current_location == next_location);
		nest[i] = next_location;
		unvisited_location[next_location] = false;
		total_costs += min_costs;
		current_location = next_location;
	}
	total_costs += find_if(tsp_ordered_list[current_location].begin(), tsp_ordered_list[current_location].end(), [first_location](pair<unsigned int, double>& candidate) { return candidate.first == first_location; })->second;
	fitness = total_costs;

	std::cout << " Finding local optimum..." << std::flush;
	this->to_local_optimum();

	return *this;
}

TspNest::~TspNest()
{
}


double TspNest::calc_fitness()
{
	unsigned int start_nest = nest[0], current_nest = start_nest, new_nest;
	double total_costs = 0;

	for (unsigned int i = 0; i < dim - 1; i++) {
		new_nest = nest[i + 1];
		total_costs += tsp_map[current_nest][new_nest];
		current_nest = new_nest;
	}
	total_costs += tsp_map[current_nest][start_nest];

	return this->fitness = total_costs;
}


TspNest TspNest::operator+(LevyRnd& levy)
{
	TspNest new_nest(*this);
	unsigned int steps = static_cast<unsigned int>(ceil(fabs(levy.rnd())));
	if (steps >= 5) {
		//Double Bridge
		new_nest.double_bridge();
	}
	else {
		for (; steps > 0; steps--) {
			new_nest.two_opt();
		}
	}

	new_nest.to_local_optimum();

	return new_nest;
}


double TspNest::two_opt_gain(unsigned int first_idx, unsigned int second_idx) {
	return tsp_map[nest[first_idx ? first_idx - 1 : dim - 1]][nest[second_idx]]
		- tsp_map[nest[first_idx ? first_idx - 1 : dim - 1]][nest[first_idx]]
		+ tsp_map[nest[first_idx]][nest[(second_idx + 1) % dim]]
		- tsp_map[nest[second_idx]][nest[(second_idx + 1) % dim]];
}


TspNest& TspNest::two_opt(unsigned int first_idx, unsigned int second_idx)
{
	if (first_idx > second_idx) {
		int tmp_idx = first_idx;
		first_idx = second_idx;
		second_idx = tmp_idx;
	}
	vector<unsigned int> new_route;
	for (int i = (second_idx + 1) % dim; i != first_idx; i = (i + 1) % dim) {
		new_route.push_back(nest[i]);
	}

	for (int i = second_idx; i != first_idx; i = i ? i - 1 : dim - 1) {
		new_route.push_back(nest[i]);
	}
	new_route.push_back(nest[first_idx]);

	this->fitness += two_opt_gain(first_idx, second_idx);
	nest = new_route;

	//std::cout << *this << std::endl;

	double new_fitness = fitness;
	if (fabs(new_fitness - this->calc_fitness()) >= 0.1) {
		int a;
		std::cout << "New fitness: " << new_fitness << ", recalced fitness: " << fitness << endl;
		std::cin >> a;
	}

	return *this;
}


TspNest& TspNest::two_opt()
{
	/*
	int second_idx;
	do { second_idx = rand() % dim; } while (second_idx == first_idx || abs(second_idx - first_idx) >= dim - 2);
	*/

	int first_idx = rand() % dim;
	unsigned int origin = (first_idx != 0 ? first_idx-1 : dim-1), second_idx;
	double search_distance = tsp_ordered_list[origin][2].second * fabs(levy.rnd());

	if (tsp_ordered_list[origin][0].second > search_distance 
		&& (tsp_ordered_list[origin][0].first != first_idx && abs(static_cast<int>(tsp_ordered_list[origin][0].first - first_idx)) < dim - 2))
	{
		second_idx = tsp_ordered_list[origin][0].first;
	}
	else {
		for (unsigned int i = 0; i < dim - 1; i++) {
			if (i == dim - 2) {
				if (tsp_ordered_list[origin][0].first != first_idx && abs(static_cast<int>(tsp_ordered_list[origin][0].first - first_idx)) < dim - 2) {
					second_idx = tsp_ordered_list[origin][dim - 2].first;
				}
				else {
					do { second_idx = rand() % dim; } while (second_idx == first_idx || abs(static_cast<int>(second_idx - first_idx)) >= dim - 2);
				}
				break;
			}
			if (tsp_ordered_list[origin][i + 1].second > search_distance
				&& tsp_ordered_list[origin][i + 1].first != first_idx
				&& abs(static_cast<int>(tsp_ordered_list[origin][i+1].first - first_idx)) < dim - 2) 
			{
				second_idx = tsp_ordered_list[origin][i + 1].first;
				break;
			}
		}
	}

	return two_opt(first_idx, second_idx);
}


double TspNest::double_bridge_gain(unsigned int idx1, unsigned int idx2, unsigned int idx3, unsigned int idx4) {
	return  tsp_map[nest[idx1]][nest[idx3 + 1]] - tsp_map[nest[idx1]][nest[idx1 + 1]]
		+ tsp_map[nest[idx2]][nest[(idx4 + 1) % dim]] - tsp_map[nest[idx2]][nest[idx2 + 1]]
		+ tsp_map[nest[idx3]][nest[idx1 + 1]] - tsp_map[nest[idx3]][nest[idx3 + 1]]
		+ tsp_map[nest[idx4]][nest[idx2 + 1]] - tsp_map[nest[idx4]][nest[(idx4 + 1) % dim]];
}


TspNest& TspNest::double_bridge(unsigned int idx1, unsigned int idx2, unsigned int idx3, unsigned int idx4)
{
	vector<unsigned int> new_route;
	unsigned int i;
	for (i = 0; i <= idx1; i++) new_route.push_back(nest[i]);
	for (i = idx3 + 1; i <= idx4; i++) new_route.push_back(nest[i]);
	for (i = idx2 + 1; i <= idx3; i++) new_route.push_back(nest[i]);
	for (i = idx1 + 1; i <= idx2; i++) new_route.push_back(nest[i]);
	for (i = idx4 + 1; i < dim; i++) new_route.push_back(nest[i]);

	fitness += double_bridge_gain(idx1, idx2, idx3, idx4);
	nest = new_route;
	return *this;
}


TspNest& TspNest::double_bridge()
{
	vector<unsigned int> indices;
	unsigned int idx = rand() % dim, idx_size, i, j;
	indices.push_back(idx);
	for (i = 0; i < 3; ++i) {
		idx = rand() % dim;
		for (j = 0, idx_size = static_cast<unsigned int>(indices.size()); j < idx_size; ++j) {
			if (idx == indices[j] || idx == (indices[j] + 1) % dim) {
				i--;
				break;
			}
			if (j == idx_size - 1) {
				indices.push_back(idx);
			}
		}
	}
	sort(indices.begin(), indices.end());
	return double_bridge(indices[0], indices[1], indices[2], indices[3]);
}

TspNest & TspNest::to_local_optimum()
{
	double gain, new_gain;
	unsigned int first_idx, second_idx;

	while(true) {

	//unsigned int number_of_optimizations = ceil(fabs(levy.rnd()));
	//for (unsigned int i = 0; i < number_of_optimizations; i++) {

	//std::cout << "." << std::flush;
		gain = 0;
		for (unsigned int i = 0; i < dim - 1; i++) {
			for (unsigned int j = i + 1; j < dim && j - i < dim - 2; j++) {
				new_gain = -two_opt_gain(i, j);
				if (new_gain > gain) {
					gain = new_gain;
					first_idx = i;
					second_idx = j;
				}
			}
		}
		if (gain == 0) {
			break;
		}
		else {
			this->two_opt(first_idx, second_idx);
		}
	}

	return *this;
}
