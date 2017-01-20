// CuckooSearch.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#include "SimpleNest.h"
#include "TspNest.h"
#include "CuckooSearch.h"

using namespace std;

int main()
{
	cout << "Starting cuckoo search demo..." << endl;

/*	//SimpleNest::dim = 15;
*/
	TspNest::load_locations("lin318.xml"); 

	CuckooSearch<25, TspNest> cs;
	cs.run(500);

	cout << "Finished!" << endl;
	return 0;
}

