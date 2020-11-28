#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/SerendipityConverter.hpp"
#include <cgnslib.h>

struct SerendipityConverterFixture : public FixtureCgnsReader {
    SerendipityConverterFixture() : FixtureCgnsReader("Manipulation/SerendipityConverter/49v_6e_cubic.cgns") {}
};

FixtureTestSuite(SerendipityConverterSuite, SerendipityConverterFixture)

TestCase(SerendipityConverterTest) {
    SerendipityConverter serendipityConverter(this->gridData);

    checkEqual(this->gridData->coordinates.size(), 49u);
    {
        auto cs = this->gridData->connectivities;
        checkEqual(cs.size(), 14u);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == TRI_10;}), 4);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == QUAD_16;}), 2);
        checkEqual(std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c.front() == BAR_4;}), 8);
    }

    auto serendipity{serendipityConverter.serendipity};



}

TestSuiteEnd()
