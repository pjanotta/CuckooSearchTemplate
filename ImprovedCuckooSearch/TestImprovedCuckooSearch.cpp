// CuckooSearch.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#include "SimpleNest.h"
#include "TspNest.h"
#include "ImprovedCuckooSearch.h"

using namespace std;

int main()
{
	cout << "Starting cuckoo search demo..." << endl;

	TspNest::load_locations("lin318.xml"); //burma14.xml"); //berlin52.xml");

	ImprovedCuckooSearch<25, TspNest> cs;
	cs.run(500);

	cout << "Finished!" << endl;
	return 0;
}

