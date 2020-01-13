#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct SpecialCgnsReaderFixture {
    SpecialCgnsReaderFixture() {
        SpecialCgnsReader specialCgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
        this->gridData = specialCgnsReader3D.gridData;
    }

    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(SpecialCgnsReaderSuite, SpecialCgnsReaderFixture)

TestCase(SpecialCgnsReaderCase) {
    {
        std::vector<std::vector<int>> expected{
            {10, 12522,   187,  8793,  3980,     0},
            {10, 12150, 10771, 10772,  3620, 53351},
            {17, 11573, 11567, 10039, 10052, 11574, 11568, 10041, 10054, 54300},
            {17,  8689,  8690,  8692,  8691,  8693,  8694,  8696,  8695, 56147},
            {14,  9471, 10275, 10278,  9484, 10287, 10290, 56148},
            {14,  9770, 10554, 11061,  9783, 10566, 11067, 57071},
            {12,  8796,  8794,  8793,  8795, 12522, 53352},
            {12, 10060, 10047, 10044, 10057, 11575, 54299},
            { 5,  8688,   911,  1025, 57072},
            { 5,  8815,  8893,  8702, 59933},
            { 7,  8946,  8948,  8698,  8694, 59934},
            { 7,  8811,  8889,  8891,  8813, 59957}
        };

        auto connectivities = this->gridData->connectivities;
        checkEqual(connectivities.size(), 59958u);
        checkEqualCollections(connectivities[    0].cbegin(), connectivities[    0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
        checkEqualCollections(connectivities[53351].cbegin(), connectivities[53351].cend(), expected[ 1].cbegin(), expected[ 1].cend());
        checkEqualCollections(connectivities[54300].cbegin(), connectivities[54300].cend(), expected[ 2].cbegin(), expected[ 2].cend());
        checkEqualCollections(connectivities[56147].cbegin(), connectivities[56147].cend(), expected[ 3].cbegin(), expected[ 3].cend());
        checkEqualCollections(connectivities[56148].cbegin(), connectivities[56148].cend(), expected[ 4].cbegin(), expected[ 4].cend());
        checkEqualCollections(connectivities[57071].cbegin(), connectivities[57071].cend(), expected[ 5].cbegin(), expected[ 5].cend());
        checkEqualCollections(connectivities[53352].cbegin(), connectivities[53352].cend(), expected[ 6].cbegin(), expected[ 6].cend());
        checkEqualCollections(connectivities[54299].cbegin(), connectivities[54299].cend(), expected[ 7].cbegin(), expected[ 7].cend());
        checkEqualCollections(connectivities[57072].cbegin(), connectivities[57072].cend(), expected[ 8].cbegin(), expected[ 8].cend());
        checkEqualCollections(connectivities[59933].cbegin(), connectivities[59933].cend(), expected[ 9].cbegin(), expected[ 9].cend());
        checkEqualCollections(connectivities[59934].cbegin(), connectivities[59934].cend(), expected[10].cbegin(), expected[10].cend());
        checkEqualCollections(connectivities[59957].cbegin(), connectivities[59957].cend(), expected[11].cbegin(), expected[11].cend());

        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 53352);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 1848);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 924);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}), 948);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 2862);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 24);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 0);
    }

    {
        auto sections = this->gridData->sections;
        checkEqual(sections.size(), 8u);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 2);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 6);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 0);
    }
}

TestSuiteEnd()
