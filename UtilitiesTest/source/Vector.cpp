#include <BoostInterface/Test.hpp>
#include <Utilities/Vector.hpp>

#define TOLERANCE 1e-12

TestCase(c_array_from_vector_1d) {
	std::vector<double> a = {0.0, 1.0, 2.0};
	double* b = determine_array_1d<double>(a);
	checkClose(b[0], 0.0, TOLERANCE);
	checkClose(b[1], 1.0, TOLERANCE);
	checkClose(b[2], 2.0, TOLERANCE);

	int* c = determine_array_1d<int>(a);
	checkEqual(c[0], 0);
	checkEqual(c[1], 1);
	checkEqual(c[2], 2);

	delete b; 
	delete c;
}

TestCase(c_array_from_vector_2d) {
	std::vector<std::vector<double>> a = {{0.0, 1.0, 2.0},
							 			  {3.0, 4.0, 5.0},
	                         			  {6.0, 7.0, 8.0}};

	double* b = determine_array_1d<double>(a.cbegin(), a.cend());
	checkClose(b[0], 0.0, TOLERANCE);
	checkClose(b[1], 1.0, TOLERANCE);
	checkClose(b[2], 2.0, TOLERANCE);
	checkClose(b[3], 3.0, TOLERANCE);
	checkClose(b[4], 4.0, TOLERANCE);
	checkClose(b[5], 5.0, TOLERANCE);
	checkClose(b[6], 6.0, TOLERANCE);
	checkClose(b[7], 7.0, TOLERANCE);
	checkClose(b[8], 8.0, TOLERANCE);

	int* c = determine_array_1d<int>(a.cbegin(), a.cend());
	checkEqual(c[0], 0.0);
	checkEqual(c[1], 1.0);
	checkEqual(c[2], 2.0);
	checkEqual(c[3], 3.0);
	checkEqual(c[4], 4.0);
	checkEqual(c[5], 5.0);
	checkEqual(c[6], 6.0);
	checkEqual(c[7], 7.0);
	checkEqual(c[8], 8.0);

	unsigned* d = determine_array_1d<unsigned>(a.cbegin(), a.cbegin()+1);
	checkEqual(d[0], 0.0);
	checkEqual(d[1], 1.0);
	checkEqual(d[2], 2.0);

	float* e = determine_array_1d<float>(a.cbegin()+1, a.cbegin()+2);
	checkClose(e[0], 3.0, TOLERANCE);
	checkClose(e[1], 4.0, TOLERANCE);
	checkClose(e[2], 5.0, TOLERANCE);

	delete b; 
	delete c;
	delete d;
	delete e;
}