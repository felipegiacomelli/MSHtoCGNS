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
        std::string filePath = multipleBasesCgnsCreator3D.getFileName();
        cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
    }

    ~MultipleBasesCgnsCreator3DFixture() {
        cg_close(this->fileIndex);
        // boost::filesystem::remove_all(this->filePath);
    };

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/4x4x4_2x2x2.msh";
    std::string multipleBasesScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/MultipleBasesCgnsCreator3D/ScriptMultipleBases.json";
    std::vector<boost::shared_ptr<GridData>> gridDatas;
    std::vector<std::string> baseNames{"ROCK", "RESERVOIR"};

    std::string filePath;
    int fileIndex;
    char name[1024];
    ElementType_t type;
    int elementStart;
    int elementEnd;
    int nbndry;
    int parent_flag;
};

FixtureTestSuite(MultipleBasesCgnsCreator3DSuite, MultipleBasesCgnsCreator3DFixture)

TestCase(MultipleBasesCgnsCreator3DCase) {
}

TestSuiteEnd()
