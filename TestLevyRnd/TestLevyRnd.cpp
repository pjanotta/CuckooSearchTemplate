// TestLevyRnd.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <fstream>
#include "LevyRnd.h"

using namespace std;

int main()
{
	cout << "Test LevyRnd ..." << endl;

	ofstream data("LevyHistogram.dat");
	int hist[100000] = {0};
	int outliers = 0, smallVals = 0;
	LevyRnd levy2, levy = levy2;
	

	for (int i = 0; i < 100000000; i++) {
		double rand_val = levy.rnd();
		if (rand_val >= -5 && rand_val < 5) {
			if (fabs(rand_val) <= 1) smallVals++;
			hist[(int)(rand_val * 10000 + 50000)]++;
		}
		else outliers++;
		if ((i % 5000) == 0) cout << i << " " << rand_val << endl;
	}

	for (int i = 0; i < 100000; i++) data << (double)i / 10000. - 5 << " " << hist[i] << endl;
	cout << "Outliers: " << outliers << endl;
	cout << "Smaller 1: " << smallVals << endl;
	data.close();

	return 0;
}

