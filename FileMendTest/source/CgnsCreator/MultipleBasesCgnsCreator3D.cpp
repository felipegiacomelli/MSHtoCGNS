#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"
#include "MSHtoCGNS/FileMend/CgnsCreator/MultipleBasesCgnsCreator3D.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct MultipleBasesCgnsCreator3DFixture {
    MultipleBasesCgnsCreator3DFixture() {
        MshReader3D mshReader3D(this->inputPath);

        boost::property_tree::ptree script;
        boost::property_tree::read_json(this->multipleBasesScript, script);
        GridDataExtractor gridDataExtractor(mshReader3D.gridData, script.get_child("ScriptGridDataExtractor"));

        this->gridDatas.emplace_back(mshReader3D.gridData);
        this->gridDatas.emplace_back(gridDataExtractor.extract);

        MultipleBasesCgnsCreator3D multipleBasesCgnsCreator3D(this->gridDatas, this->baseNames, "./");

        this->filePath = multipleBasesCgnsCreator3D.getFileName();
        cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
    }

    ~MultipleBasesCgnsCreator3DFixture() {
        cg_close(this->fileIndex);
        boost::filesystem::remove_all(this->filePath);
    };

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/4x4x4_2x2x2.msh";
    std::string multipleBasesScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/MultipleBasesCgnsCreator3D/ScriptMultipleBases.json";
    std::vector<boost::shared_ptr<GridData>> gridDatas;
    std::vector<std::string> baseNames{"ROCK", "RESERVOIR"};

    std::string filePath;
    int fileIndex;
    char buffer[1024];
    int cellDimension, physicalDimension;
    int sizes[3];
    ElementType_t type;
    int elementStart;
    int elementEnd;
    int nbndry;
    int parent_flag;
};

FixtureTestSuite(MultipleBasesCgnsCreator3DSuite, MultipleBasesCgnsCreator3DFixture)

TestCase(Base1Case) {
    int numberOfBases;
    cg_nbases(this->fileIndex, &numberOfBases);
    checkEqual(numberOfBases, 2);

    int baseIndex = 1;
    cg_base_read(this->fileIndex, baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension);
    check(std::string(this->buffer) == "ROCK");

    int numberOfZones;
    cg_nzones(this->fileIndex, baseIndex, &numberOfZones);
    checkEqual(numberOfZones, 1);

    int zoneIndex = 1;
    cg_zone_read(this->fileIndex, baseIndex, zoneIndex, this->buffer, this->sizes);
    check(std::string(this->buffer) == "ROCK");
    checkEqual(this->sizes[0], 152);
    checkEqual(this->sizes[1], 72);
    checkEqual(this->sizes[2], 0);

    int numberOfSections;
    cg_nsections(this->fileIndex, baseIndex, zoneIndex, &numberOfSections);
    checkEqual(numberOfSections, 8);
}

TestCase(Base2Case) {
    int numberOfBases;
    cg_nbases(this->fileIndex, &numberOfBases);
    checkEqual(numberOfBases, 2);

    int baseIndex = 2;
    cg_base_read(this->fileIndex, baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension);
    check(std::string(this->buffer) == "RESERVOIR");

    int numberOfZones;
    cg_nzones(this->fileIndex, baseIndex, &numberOfZones);
    checkEqual(numberOfZones, 1);

    int zoneIndex = 1;
    cg_zone_read(this->fileIndex, baseIndex, zoneIndex, this->buffer, this->sizes);
    check(std::string(this->buffer) == "RESERVOIR");
    checkEqual(this->sizes[0], 27);
    checkEqual(this->sizes[1], 8);
    checkEqual(this->sizes[2], 0);

    int numberOfSections;
    cg_nsections(this->fileIndex, baseIndex, zoneIndex, &numberOfSections);
    checkEqual(numberOfSections, 7);
}

TestSuiteEnd()
