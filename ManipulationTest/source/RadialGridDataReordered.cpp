#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/RadialGridDataReordered.hpp"
#include <cgnslib.h>
#include <cmath>

#undef TOLERANCE
#define TOLERANCE 1.0e-4

struct RadialGridDataReorderedFixture : public FixtureCgnsReader {
    RadialGridDataReorderedFixture() : FixtureCgnsReader("Manipulation/RadialGridDataReordered/370v_324e.cgns") {}

    double calculateDistance(std::array<double, 3> a, std::array<double, 3> b) {
        return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
    }
};

FixtureTestSuite(RadialGridDataReorderedSuite, RadialGridDataReorderedFixture)

TestCase(RadialGridDataReorderedTest) {
    RadialGridDataReordered radialGridDataReordered(this->gridData);

    auto reordered = radialGridDataReordered.reordered;

    checkEqual(reordered->coordinates.size(), 370u);

    {
        auto connectivities = reordered->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}),   0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 216);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 108);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}),   0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}),  24);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 156);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}),   9);
    }

    {
        auto sections = reordered->sections;
        checkEqual(sections.size(), 5u);

        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 3);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 1);

        {
            auto section = sections[0];
            checkEqual(section.name, std::string("BODY_WELL"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 0);
            checkEqual(section.end, 324);
        }

        {
            auto section = sections[1];
            checkEqual(section.name, std::string("BOUNDARY_SURFACE"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 324);
            checkEqual(section.end, 432);
        }

        {
            auto section = sections[2];
            checkEqual(section.name, std::string("BOUNDARY_BOTTOM"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 432);
            checkEqual(section.end, 468);
        }

        {
            auto section = sections[3];
            checkEqual(section.name, std::string("BOUNDARY_TOP"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 468);
            checkEqual(section.end, 504);
        }

        {
            auto section = sections[4];
            checkEqual(section.name, std::string("LINE_WELL"));
            checkEqual(section.dimension, 1);
            checkEqual(section.begin, 504);
            checkEqual(section.end, 513);
            std::vector<int> expected{0, 37, 74, 111, 148, 185, 222, 259, 296, 333};
            checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
        }
    }

    int shift = 0;
    {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 12; ++j)
                checkEqual(reordered->connectivities[shift++].front(), PENTA_6);

            for (int j = 0; j < 24; ++j)
                checkEqual(reordered->connectivities[shift++].front(), HEXA_8);
        }

        for (int i = 0; i < 108; ++i) {
            checkEqual(reordered->connectivities[shift++].front(), QUAD_4);
        }

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 12; ++j) {
                checkEqual(reordered->connectivities[shift++].front(), TRI_3);
            }

            for (int j = 0; j < 24; ++j) {
                checkEqual(reordered->connectivities[shift++].front(), QUAD_4);
            }
        }

        for (int i = 0; i < 9; ++i) {
            checkEqual(reordered->connectivities[shift++].front(), BAR_2);
        }
    }

    for (int i = 0; i < 37; ++i) {
        checkSmall(reordered->coordinates[37 * 0 + i][2], TOLERANCE);
        checkClose(reordered->coordinates[37 * 1 + i][2], 5.5555555, TOLERANCE);
        checkClose(reordered->coordinates[37 * 2 + i][2], 11.111111, TOLERANCE);
        checkClose(reordered->coordinates[37 * 3 + i][2], 16.666666, TOLERANCE);
        checkClose(reordered->coordinates[37 * 4 + i][2], 22.222222, TOLERANCE);
        checkClose(reordered->coordinates[37 * 5 + i][2], 27.777777, TOLERANCE);
        checkClose(reordered->coordinates[37 * 6 + i][2], 33.333333, TOLERANCE);
        checkClose(reordered->coordinates[37 * 7 + i][2], 38.888888, TOLERANCE);
        checkClose(reordered->coordinates[37 * 8 + i][2], 44.444444, TOLERANCE);
        checkClose(reordered->coordinates[37 * 9 + i][2], 49.999999, TOLERANCE);
    }

    for (int i = 0; i < 37; ++i) {
        checkClose(fabs(reordered->coordinates[37 * 0 + i][2] - reordered->coordinates[37 *  1 + i][2]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 1 + i][2] - reordered->coordinates[37 *  2 + i][2]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 2 + i][2] - reordered->coordinates[37 *  3 + i][2]), 5.5555555555555536e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 3 + i][2] - reordered->coordinates[37 *  4 + i][2]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 4 + i][2] - reordered->coordinates[37 *  5 + i][2]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 5 + i][2] - reordered->coordinates[37 *  6 + i][2]), 5.5555555555555500e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 6 + i][2] - reordered->coordinates[37 *  7 + i][2]), 5.5555555555555642e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 7 + i][2] - reordered->coordinates[37 *  8 + i][2]), 5.5555538601345447e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 8 + i][2] - reordered->coordinates[37 *  9 + i][2]), 5.5555572509765625e+00, TOLERANCE);
    }

    for (int i = 0; i < 37; ++i) {
        checkClose(this->calculateDistance(reordered->coordinates[37 * 0 + i], reordered->coordinates[37 *  1 + i]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 1 + i], reordered->coordinates[37 *  2 + i]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 2 + i], reordered->coordinates[37 *  3 + i]), 5.5555555555555536e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 3 + i], reordered->coordinates[37 *  4 + i]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 4 + i], reordered->coordinates[37 *  5 + i]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 5 + i], reordered->coordinates[37 *  6 + i]), 5.5555555555555500e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 6 + i], reordered->coordinates[37 *  7 + i]), 5.5555555555555642e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 7 + i], reordered->coordinates[37 *  8 + i]), 5.5555538601345447e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 8 + i], reordered->coordinates[37 *  9 + i]), 5.5555572509765625e+00, TOLERANCE);
    }
}

TestSuiteEnd()
