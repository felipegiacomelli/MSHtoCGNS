#include <BoostInterface/Test.hpp>
#include <Utilities/Vector.hpp>

#define TOLERANCE 1e-12

TestCase(c_array_from_vector_1d) {
	std::vector<double> a = {0.0, 10.0, 20.0};
	double* b = determine_array_1d<double>(a);
	checkClose(b[0],  0.0, TOLERANCE);
	checkClose(b[1], 10.0, TOLERANCE);
	checkClose(b[2], 20.0, TOLERANCE);

	int* c = determine_array_1d<int>(a);
	checkEqual(c[0],  0.0);
	checkEqual(c[1], 10.0);
	checkEqual(c[2], 20.0);

	delete b; 
	delete c;
}

TestCase(c_array_from_vector_2d) {
	std::vector<std::vector<double>> a = {{-10.0, 10.0, 20.0},
							 			  { 30.0, 40.0, 50.0},
	                         			  { 60.0, 70.0, 80.0}};

	double* b = determine_array_1d<double>(a);
	checkClose(b[0], -10.0, TOLERANCE);
	checkClose(b[1],  10.0, TOLERANCE);
	checkClose(b[2],  20.0, TOLERANCE);
	checkClose(b[3],  30.0, TOLERANCE);
	checkClose(b[4],  40.0, TOLERANCE);
	checkClose(b[5],  50.0, TOLERANCE);
	checkClose(b[6],  60.0, TOLERANCE);
	checkClose(b[7],  70.0, TOLERANCE);
	checkClose(b[8],  80.0, TOLERANCE);
}