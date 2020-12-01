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

    checkEqual(this->gridData->coordinates.size(), 25u);
    {
        auto cs = this->gridData->connectivities;
        checkEqual(cs.size(), 12u);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == TRI_10;}), 0);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_9;}), 4);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == BAR_3;}), 8);
    }

    auto serendipity{serendipityConverter.serendipity};
    checkEqual(serendipity->coordinates.size(), 21u);
    {
        auto cs = serendipity->connectivities;
        checkEqual(cs.size(), 12u);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == TRI_10;}), 0);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_9;}), 0);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_8;}), 4);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == BAR_3;}), 8);
    }

    for (auto c : gridData->connectivities) {
        for (auto v : c) {
            printf(" %2i", v);
        }
        printf("\n");
    }
    printf("\n");

    for (auto c : serendipity->connectivities) {
        for (auto v : c) {
            printf(" %2i", v);
        }
        printf("\n");
    }


    CgnsCreator cgnsCreator(serendipity, "/home/felipe");
}

TestSuiteEnd()
