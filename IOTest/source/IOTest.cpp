#define BOOST_TEST_MODULE IOTestModule

#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader2D.hpp>
#include <IO/GridReader3D.hpp>

#define TOLERANCE 1e-12

struct Fixture2D {
	Fixture2D() {
		GridReader2D gridReader2D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/5n_8e.msh");
		this->gridData = gridReader2D.getGridData();
	}

	~Fixture2D() = default;

	GridData gridData;
};

FixtureTestSuite(Suite2D, Fixture2D)
	TestCase(Coordinates) {
		auto coordinates = this->gridData.coordinates;

		check(coordinates.size() == 5);

		checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
		checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
		checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
		checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
		checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
	}
TestSuiteEnd()