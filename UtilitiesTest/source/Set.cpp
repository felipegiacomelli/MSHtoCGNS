#include <BoostInterface/Test.hpp>
#include <Utilities/Set.hpp>

#define TOLERANCE 1e-12

TestCase(c_array_from_set) {
	std::vector<double> a = {-10.0, 10.0, 20.0, 30.0};
	std::set<double> b(a.cbegin(), a.cend());
	
	double* c = determine_array_1d<double>(b);
	checkClose(c[0], -10.0, TOLERANCE);
	checkClose(c[1],  10.0, TOLERANCE);
	checkClose(c[2],  20.0, TOLERANCE);
	checkClose(c[3],  30.0, TOLERANCE);

	delete c;
}