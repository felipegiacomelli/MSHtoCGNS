#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/Generator/RegionGenerator.hpp"
#include <cgnslib.h>

#include "MSHtoCGNS/Manipulation/GridDataExtractor.hpp"
#include "MSHtoCGNS/Manipulation/MultipleBasesCgnsCreator.hpp"

struct RegionGeneratorFixtureI {
    RegionGeneratorFixtureI() {
        CgnsReader cgnsReader(this->inputPath);
        this->gridData = cgnsReader.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RegionGenerator/125v_64e_1r.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string regionGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RegionGenerator/RegionGenerator1.json";
};

FixtureTestSuite(RegionGeneratorSuiteI, RegionGeneratorFixtureI)

TestCase(RegionGeneratorTest) {
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);

    RegionGenerator regionGenerator(this->gridData, this->regionGeneratorScript);

    checkEqual(this->gridData->sections.size(), 8u);
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& s){return s.dimension == 3;}), 2);

    {
        auto section = this->gridData->sections[0];
        checkEqual(section.name, std::string("PAY"));
        checkEqual(section.dimension, 3);
        checkEqual(section.begin, 0);
        checkEqual(section.end, 8);
    }

    {
        auto section = this->gridData->sections[1];
        checkEqual(section.name, std::string("NONPAY"));
        checkEqual(section.dimension, 3);
        checkEqual(section.begin, 8);
        checkEqual(section.end, 64);
    }
}

TestSuiteEnd()

struct RegionGeneratorFixtureII {
    RegionGeneratorFixtureII() {
        CgnsReader cgnsReader(this->inputPath);
        this->gridData = cgnsReader.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RegionGenerator/125v_64e_2r.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string regionGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RegionGenerator/RegionGenerator2.json";
};

FixtureTestSuite(RegionGeneratorSuiteII, RegionGeneratorFixtureII)

TestCase(RegionGeneratorTest) {
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 3;}), 2);

    RegionGenerator regionGenerator(this->gridData, this->regionGeneratorScript);

    checkEqual(this->gridData->sections.size(), 10u);
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& s){return s.dimension == 3;}), 4);

    // {
    //     auto section = this->gridData->sections[0];
    //     checkEqual(section.name, std::string("PAY"));
    //     checkEqual(section.dimension, 3);
    //     checkEqual(section.begin, 0);
    //     checkEqual(section.end, 8);
    // }

    // {
    //     auto section = this->gridData->sections[1];
    //     checkEqual(section.name, std::string("NONPAY"));
    //     checkEqual(section.dimension, 3);
    //     checkEqual(section.begin, 8);
    //     checkEqual(section.end, 64);
    // }
}

TestSuiteEnd()
