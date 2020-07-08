#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/Generator/CartesianWellGenerator.hpp"
#include <cgnslib.h>

#define TOLERANCE 1.0e-12

struct CartesianWellGeneratorFixture {
    CartesianWellGeneratorFixture() {
        SpecialCgnsReader specialCgnsReader(this->inputPath);
        this->gridData = specialCgnsReader.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/CartesianWellGenerator/275v_160e.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string cartesianWellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/CartesianWellGenerator/CartesianWellGenerator.json";
};

FixtureTestSuite(CartesianWellGeneratorSuite, CartesianWellGeneratorFixture)

TestCase(CartesianWellGeneratorTest) {
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}), 0);
    checkEqual(std::count_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c.front() == BAR_2;}), 0);
    checkEqual(this->gridData->connectivities.size(), 352u);
    checkEqual(this->gridData->sections.size(), 7u);

    CartesianWellGenerator cartesianWellGenerator(this->gridData, this->cartesianWellGeneratorScript);

    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}), 1);
    checkEqual(std::count_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c.front() == BAR_2;}), 6);
    checkEqual(this->gridData->connectivities.size(), 358u);
    checkEqual(this->gridData->sections.size(), 8u);

    checkClose(this->gridData->coordinates[261][2], 0.8, TOLERANCE);
    checkClose(this->gridData->coordinates[252][2], 0.7, TOLERANCE);
    checkClose(this->gridData->coordinates[243][2], 0.6, TOLERANCE);
    checkClose(this->gridData->coordinates[234][2], 0.5, TOLERANCE);
    checkClose(this->gridData->coordinates[225][2], 0.4, TOLERANCE);
    checkClose(this->gridData->coordinates[216][2], 0.3, TOLERANCE);
    checkClose(this->gridData->coordinates[207][2], 0.2, TOLERANCE);

    std::vector<std::vector<int>> expected{
        {3, 261, 252, 352},
        {3, 252, 243, 353},
        {3, 243, 234, 354},
        {3, 234, 225, 355},
        {3, 225, 216, 356},
        {3, 216, 207, 357}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqualCollections(connectivities[352].cbegin(), connectivities[352].cend(), expected[ 0].cbegin(), expected[ 0].cend());
    checkEqualCollections(connectivities[353].cbegin(), connectivities[353].cend(), expected[ 1].cbegin(), expected[ 1].cend());
    checkEqualCollections(connectivities[354].cbegin(), connectivities[354].cend(), expected[ 2].cbegin(), expected[ 2].cend());
    checkEqualCollections(connectivities[355].cbegin(), connectivities[355].cend(), expected[ 3].cbegin(), expected[ 3].cend());
    checkEqualCollections(connectivities[356].cbegin(), connectivities[356].cend(), expected[ 4].cbegin(), expected[ 4].cend());
    checkEqualCollections(connectivities[357].cbegin(), connectivities[357].cend(), expected[ 5].cbegin(), expected[ 5].cend());

    auto well = this->gridData->sections.back();
    checkEqual(well.name, "WELL");
    checkEqual(well.begin, 352);
    checkEqual(well.end, 358);
}

TestSuiteEnd()
