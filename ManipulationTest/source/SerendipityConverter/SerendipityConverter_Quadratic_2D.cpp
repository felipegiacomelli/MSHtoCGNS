#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include "MSHtoCGNS/Manipulation/SerendipityConverter.hpp"
#include <cgnslib.h>

struct Quadratic2DFixture : public FixtureCgnsReader {
    Quadratic2DFixture() : FixtureCgnsReader("Manipulation/SerendipityConverter/25v_4e_quadratic.cgns") {}
};

FixtureTestSuite(Quadratic2DSuite, Quadratic2DFixture)

TestCase(Quadratic2DTest) {
    SerendipityConverter serendipityConverter(this->gridData);
    auto serendipity{serendipityConverter.serendipity};

    {
        auto cs = serendipity->connectivities;
        checkEqual(cs.size(), 12u);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == TRI_10;}), 0);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_9;}), 0);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_8;}), 4);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == BAR_3;}), 8);
    }

    std::vector<std::array<double, 3>> expected{
        {0.000, 0.000, 0.000},
        {1.000, 0.000, 0.000},
        {1.000, 1.000, 0.000},
        {0.000, 1.000, 0.000},
        {0.500, 0.000, 0.000},
        {0.250, 0.000, 0.000},
        {0.750, 0.000, 0.000},
        {1.000, 0.500, 0.000},
        {1.000, 0.250, 0.000},
        {1.000, 0.750, 0.000},
        {0.500, 1.000, 0.000},
        {0.750, 1.000, 0.000},
        {0.250, 1.000, 0.000},
        {0.000, 0.500, 0.000},
        {0.000, 0.750, 0.000},
        {0.000, 0.250, 0.000},
        {0.500, 0.500, 0.000},
        {0.500, 0.250, 0.000},
        {0.250, 0.500, 0.000},
        {0.500, 0.750, 0.000},
        {0.750, 0.500, 0.000}
    };

    checkEqual(serendipity->coordinates.size(), 21u);
    checkCloseCollection(serendipity->coordinates[ 0], expected[ 0], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 1], expected[ 1], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 2], expected[ 2], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 3], expected[ 3], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 4], expected[ 4], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 5], expected[ 5], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 6], expected[ 6], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 7], expected[ 7], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 8], expected[ 8], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[ 9], expected[ 9], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[10], expected[10], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[11], expected[11], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[12], expected[12], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[13], expected[13], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[14], expected[14], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[15], expected[15], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[16], expected[16], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[17], expected[17], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[18], expected[18], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[19], expected[19], TOLERANCE);
    checkCloseCollection(serendipity->coordinates[20], expected[20], TOLERANCE);
}

TestSuiteEnd()
