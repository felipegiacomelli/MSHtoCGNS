#include <BoostInterface/Test.hpp>
#include <Utilities/Vector.hpp>

#define TOLERANCE 1e-12

TestCase(linearize_matrix_via_iterators) {
    std::vector<std::vector<double>> a = {{-10.0, 10.0, 20.0},
                                          { 30.0, 40.0, 50.0},
                                          { 60.0, 70.0, 80.0}};

    std::vector<double> b;
    append(a.cbegin(), a.cend(), std::back_inserter(b));

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