// CuckooSearch.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#include "SimpleNest.h"
#include "TspNest.h"
#include "CuckooSearch.h"
//#include "LevyRnd.h"

using namespace std;

int main()
{
	cout << "Starting cuckoo search demo..." << endl;

	/*cout << "Test LevyRnd..." << endl;

	LevyRnd levy(1.5, 0.69657450255769679272122);

	double s = 1000;

	cout << "Uscaled:" << endl;
	for (int i = 0; i < 1000; i++) cout << levy.rnd() << endl;

	int a;
	cin >> a;

	cout << "Scaled by 0.01:" << endl;
	LevyRnd levy2 = levy * 0.01;
	for (int i = 0; i < 1000; i++) cout << levy2.rnd() << endl;*/

	//cin >> a;

	TspNest::load_locations("C:\\Programming\\cpp\\cuckoo\\visual-studio-version\\CuckooSearch\\CuckooSearch\\tsp_maps\\berlin52.xml"); //lin318.xml");
	
	//CuckooSearch<25, TspNest> cs;

	TspNest tnest;
	tnest.init();
	cout << "original: " << endl;
	cout << tnest << endl;
	//TEST DOUBLE BRIDGE
	cout << "modified: " << endl;
	cout << tnest.double_bridge() << endl;
	//END TEST DOUBLE BRIDGE

	//TEST TWO_OPT
	cout << "modified: " << endl;
	cout << tnest.two_opt(5, 20) << endl;
	cout << "modified 2: " << endl;
	cout << tnest.two_opt(20, 5) << endl;
	//END TEST TWO_OPT

	//SimpleNest::dim = 15;

	//CuckooSearch<25, SimpleNest> cs;
	//cs.run(25000);

	cout << "Finished!" << endl;
	return 0;
}

