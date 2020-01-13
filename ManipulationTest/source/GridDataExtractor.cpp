#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/GridDataExtractor.hpp"
#include <cgnslib.h>

#undef TOLERANCE
#define TOLERANCE 1.0e-6

struct GridDataExtractorFixture : public FixtureCgnsReader {
    GridDataExtractorFixture() : FixtureCgnsReader("Manipulation/GridDataExtractor/125v_64e.cgns") {}

    std::string gridDataExtractorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/GridDataExtractor/GridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorSuite, GridDataExtractorFixture)

TestCase(GridDataTest) {
    GridDataExtractor gridDataExtractor(this->gridData, this->gridDataExtractorScript);

    checkEqual(this->gridData->coordinates.size(), 125u);

    {
        auto connectivities = this->gridData->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 64);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 120);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 0);
    }

    {
        auto sections = this->gridData->sections;
        checkEqual(sections.size(), 9u);

        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 2);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 7);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 0);

        {
            auto section = sections[1];
            checkEqual(section.name, std::string("NONPAY"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 8);
            checkEqual(section.end, 64);
        }

        {
            auto section = sections[8];
            checkEqual(section.name, std::string("INTERFACE"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 160);
            checkEqual(section.end, 184);
        }
    }
}

TestCase(ExtractTest) {
    GridDataExtractor gridDataExtractor(this->gridData, this->gridDataExtractorScript);
    auto extract = gridDataExtractor.extract;

    checkEqual(extract->coordinates.size(), 27u);
    {
        auto connectivities = extract->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 8);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 24);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 0);
    }

    {
        auto sections = extract->sections;
        checkEqual(sections.size(), 2u);

        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 0);

        {
            auto section = sections[0];
            checkEqual(section.name, std::string("PAY"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 0);
            checkEqual(section.end, 8);
        }

        {
            auto section = sections[1];
            checkEqual(section.name, std::string("INTERFACE"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 8);
            checkEqual(section.end, 32);
        }
    }

    {
        std::vector<std::array<double, 3>> expected{
            {2.5000e-01, 2.5000e-01, 2.5000e-01},
            {7.5000e-01, 2.5000e-01, 2.5000e-01},
            {7.5000e-01, 7.5000e-01, 2.5000e-01},
            {2.5000e-01, 7.5000e-01, 2.5000e-01},
            {2.5000e-01, 2.5000e-01, 7.5000e-01},
            {7.5000e-01, 2.5000e-01, 7.5000e-01},
            {7.5000e-01, 7.5000e-01, 7.5000e-01},
            {2.5000e-01, 7.5000e-01, 7.5000e-01},
            {2.5000e-01, 5.0000e-01, 2.5000e-01},
            {5.0000e-01, 2.5000e-01, 2.5000e-01},
            {7.5000e-01, 5.0000e-01, 2.5000e-01},
            {5.0000e-01, 7.5000e-01, 2.5000e-01},
            {2.5000e-01, 5.0000e-01, 7.5000e-01},
            {5.0000e-01, 2.5000e-01, 7.5000e-01},
            {7.5000e-01, 5.0000e-01, 7.5000e-01},
            {5.0000e-01, 7.5000e-01, 7.5000e-01},
            {7.5000e-01, 2.5000e-01, 5.0000e-01},
            {7.5000e-01, 7.5000e-01, 5.0000e-01},
            {2.5000e-01, 2.5000e-01, 5.0000e-01},
            {2.5000e-01, 7.5000e-01, 5.0000e-01},
            {2.5000e-01, 5.0000e-01, 5.0000e-01},
            {7.5000e-01, 5.0000e-01, 5.0000e-01},
            {5.0000e-01, 2.5000e-01, 5.0000e-01},
            {5.0000e-01, 7.5000e-01, 5.0000e-01},
            {5.0000e-01, 5.0000e-01, 2.5000e-01},
            {5.0000e-01, 5.0000e-01, 7.5000e-01},
            {5.0000e-01, 5.0000e-01, 5.0000e-01}
        };

        auto coordinates = extract->coordinates;
        checkEqual(coordinates.size(), 27u);
        checkCloseCollection(coordinates[ 0], expected[ 0], TOLERANCE);
        checkCloseCollection(coordinates[ 1], expected[ 1], TOLERANCE);
        checkCloseCollection(coordinates[ 2], expected[ 2], TOLERANCE);
        checkCloseCollection(coordinates[ 3], expected[ 3], TOLERANCE);
        checkCloseCollection(coordinates[ 4], expected[ 4], TOLERANCE);
        checkCloseCollection(coordinates[ 5], expected[ 5], TOLERANCE);
        checkCloseCollection(coordinates[ 6], expected[ 6], TOLERANCE);
        checkCloseCollection(coordinates[ 7], expected[ 7], TOLERANCE);
        checkCloseCollection(coordinates[ 8], expected[ 8], TOLERANCE);
        checkCloseCollection(coordinates[ 9], expected[ 9], TOLERANCE);
        checkCloseCollection(coordinates[10], expected[10], TOLERANCE);
        checkCloseCollection(coordinates[11], expected[11], TOLERANCE);
        checkCloseCollection(coordinates[12], expected[12], TOLERANCE);
        checkCloseCollection(coordinates[13], expected[13], TOLERANCE);
        checkCloseCollection(coordinates[14], expected[14], TOLERANCE);
        checkCloseCollection(coordinates[15], expected[15], TOLERANCE);
        checkCloseCollection(coordinates[16], expected[16], TOLERANCE);
        checkCloseCollection(coordinates[17], expected[17], TOLERANCE);
        checkCloseCollection(coordinates[18], expected[18], TOLERANCE);
        checkCloseCollection(coordinates[19], expected[19], TOLERANCE);
        checkCloseCollection(coordinates[20], expected[20], TOLERANCE);
        checkCloseCollection(coordinates[21], expected[21], TOLERANCE);
        checkCloseCollection(coordinates[22], expected[22], TOLERANCE);
        checkCloseCollection(coordinates[23], expected[23], TOLERANCE);
        checkCloseCollection(coordinates[24], expected[24], TOLERANCE);
        checkCloseCollection(coordinates[25], expected[25], TOLERANCE);
        checkCloseCollection(coordinates[26], expected[26], TOLERANCE);
    }

    {
        std::vector<std::vector<int>> expected{
            {17, 20, 19,  7, 12, 26, 23, 15, 25, 0},
            {17, 26, 23, 15, 25, 21, 17,  6, 14, 1},
            {17, 18, 20, 12,  4, 22, 26, 25, 13, 2},
            {17, 22, 26, 25, 13, 16, 21, 14,  5, 3},
            {17,  8,  3, 19, 20, 24, 11, 23, 26, 4},
            {17, 24, 11, 23, 26, 10,  2, 17, 21, 5},
            {17,  0,  8, 20, 18,  9, 24, 26, 22, 6},
            {17,  9, 24, 26, 22,  1, 10, 21, 16, 7},
            { 7,  7, 19, 20, 12,  8},
            { 7, 12, 20, 18,  4,  9},
            { 7, 19,  3,  8, 20, 10},
            { 7, 20,  8,  0, 18, 11},
            { 7,  2, 17, 21, 10, 12},
            { 7, 10, 21, 16,  1, 13},
            { 7, 17,  6, 14, 21, 14},
            { 7, 21, 14,  5, 16, 15},
            { 7,  4, 18, 22, 13, 16},
            { 7, 18,  0,  9, 22, 17},
            { 7, 13, 22, 16,  5, 18},
            { 7, 22,  9,  1, 16, 19},
            { 7,  6, 17, 23, 15, 20},
            { 7, 17,  2, 11, 23, 21},
            { 7, 15, 23, 19,  7, 22},
            { 7, 23, 11,  3, 19, 23},
            { 7,  0,  8, 24,  9, 24},
            { 7,  8,  3, 11, 24, 25},
            { 7,  9, 24, 10,  1, 26},
            { 7, 24, 11,  2, 10, 27},
            { 7,  7, 12, 25, 15, 28},
            { 7, 15, 25, 14,  6, 29},
            { 7, 12,  4, 13, 25, 30},
            { 7, 25, 13,  5, 14, 31}
        };

        auto connectivities = extract->connectivities;
        checkEqual(connectivities.size(), 32u);
        checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
        checkEqualCollections(connectivities[ 1].cbegin(), connectivities[ 1].cend(), expected[ 1].cbegin(), expected[ 1].cend());
        checkEqualCollections(connectivities[ 2].cbegin(), connectivities[ 2].cend(), expected[ 2].cbegin(), expected[ 2].cend());
        checkEqualCollections(connectivities[ 3].cbegin(), connectivities[ 3].cend(), expected[ 3].cbegin(), expected[ 3].cend());
        checkEqualCollections(connectivities[ 4].cbegin(), connectivities[ 4].cend(), expected[ 4].cbegin(), expected[ 4].cend());
        checkEqualCollections(connectivities[ 5].cbegin(), connectivities[ 5].cend(), expected[ 5].cbegin(), expected[ 5].cend());
        checkEqualCollections(connectivities[ 6].cbegin(), connectivities[ 6].cend(), expected[ 6].cbegin(), expected[ 6].cend());
        checkEqualCollections(connectivities[ 7].cbegin(), connectivities[ 7].cend(), expected[ 7].cbegin(), expected[ 7].cend());
        checkEqualCollections(connectivities[ 8].cbegin(), connectivities[ 8].cend(), expected[ 8].cbegin(), expected[ 8].cend());
        checkEqualCollections(connectivities[ 9].cbegin(), connectivities[ 9].cend(), expected[ 9].cbegin(), expected[ 9].cend());
        checkEqualCollections(connectivities[10].cbegin(), connectivities[10].cend(), expected[10].cbegin(), expected[10].cend());
        checkEqualCollections(connectivities[11].cbegin(), connectivities[11].cend(), expected[11].cbegin(), expected[11].cend());
        checkEqualCollections(connectivities[12].cbegin(), connectivities[12].cend(), expected[12].cbegin(), expected[12].cend());
        checkEqualCollections(connectivities[13].cbegin(), connectivities[13].cend(), expected[13].cbegin(), expected[13].cend());
        checkEqualCollections(connectivities[14].cbegin(), connectivities[14].cend(), expected[14].cbegin(), expected[14].cend());
        checkEqualCollections(connectivities[15].cbegin(), connectivities[15].cend(), expected[15].cbegin(), expected[15].cend());
        checkEqualCollections(connectivities[16].cbegin(), connectivities[16].cend(), expected[16].cbegin(), expected[16].cend());
        checkEqualCollections(connectivities[17].cbegin(), connectivities[17].cend(), expected[17].cbegin(), expected[17].cend());
        checkEqualCollections(connectivities[18].cbegin(), connectivities[18].cend(), expected[18].cbegin(), expected[18].cend());
        checkEqualCollections(connectivities[19].cbegin(), connectivities[19].cend(), expected[19].cbegin(), expected[19].cend());
        checkEqualCollections(connectivities[20].cbegin(), connectivities[20].cend(), expected[20].cbegin(), expected[20].cend());
        checkEqualCollections(connectivities[21].cbegin(), connectivities[21].cend(), expected[21].cbegin(), expected[21].cend());
        checkEqualCollections(connectivities[22].cbegin(), connectivities[22].cend(), expected[22].cbegin(), expected[22].cend());
        checkEqualCollections(connectivities[23].cbegin(), connectivities[23].cend(), expected[23].cbegin(), expected[23].cend());
        checkEqualCollections(connectivities[24].cbegin(), connectivities[24].cend(), expected[24].cbegin(), expected[24].cend());
        checkEqualCollections(connectivities[25].cbegin(), connectivities[25].cend(), expected[25].cbegin(), expected[25].cend());
        checkEqualCollections(connectivities[26].cbegin(), connectivities[26].cend(), expected[26].cbegin(), expected[26].cend());
        checkEqualCollections(connectivities[27].cbegin(), connectivities[27].cend(), expected[27].cbegin(), expected[27].cend());
        checkEqualCollections(connectivities[28].cbegin(), connectivities[28].cend(), expected[28].cbegin(), expected[28].cend());
        checkEqualCollections(connectivities[29].cbegin(), connectivities[29].cend(), expected[29].cbegin(), expected[29].cend());
        checkEqualCollections(connectivities[30].cbegin(), connectivities[30].cend(), expected[30].cbegin(), expected[30].cend());
        checkEqualCollections(connectivities[31].cbegin(), connectivities[31].cend(), expected[31].cbegin(), expected[31].cend());
    }
}

TestSuiteEnd()

struct GridDataExtractorWithWellFixture : public FixtureCgnsReader {
    GridDataExtractorWithWellFixture() : FixtureCgnsReader("Manipulation/GridDataExtractorWithWell/12523v_57072e.cgns") {}

    std::string gridDataExtractorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/GridDataExtractorWithWell/GridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorWithWellSuite, GridDataExtractorWithWellFixture)

TestCase(GridDataExtractorWithWellTest) {
    GridDataExtractor gridDataExtractor(this->gridData, this->gridDataExtractorScript);
    boost::shared_ptr<GridData> extract = gridDataExtractor.extract;

    checkEqual(extract->coordinates.size(), 2886u);

    {
        auto connectivities = extract->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}),    0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 1848);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}),  924);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}),    0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}),    0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}),    0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 77);
    }

    {
        auto sections = extract->sections;
        checkEqual(sections.size(), 2u);

        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 0);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 1);

        {
            auto section = sections[0];
            checkEqual(section.name, std::string("BODY_WELL"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 0);
            checkEqual(section.end, 2772);
        }

        {
            auto section = sections[1];
            checkEqual(section.name, std::string("LINE_WELL"));
            checkEqual(section.dimension, 1);
            checkEqual(section.begin, 2772);
            checkEqual(section.end, 2849);
        }
    }
}

TestSuiteEnd()
