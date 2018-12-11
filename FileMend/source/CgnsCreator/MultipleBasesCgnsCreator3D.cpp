#include "MSHtoCGNS/FileMend/MultipleBasesCgnsCreator3D.hpp"
#include <cgnslib.h>

MultipleBasesCgnsCreator3D::MultipleBasesCgnsCreator3D(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::string folderPath) : CgnsCreator3D(nullptr, folderPath, false), gridDatas(gridDatas), baseNames(baseNames), firstCall(true) {
    this->initialize();
}

void MultipleBasesCgnsCreator3D::initialize() {
    for (unsigned i = 0; i < this->gridDatas.size(); i++) {
        this->gridData = this->gridDatas[i];

        this->checkDimension();
        this->setDimensions();

        if (this->firstCall) {
            this->firstCall = false;
            this->setupFile();
        }

        std::transform(this->baseNames[i].begin(), this->baseNames[i].end(), this->baseNames[i].begin(), ::toupper);
        this->baseName = this->baseNames[i];

        std::transform(this->baseNames[i].begin(), this->baseNames[i].end(), this->baseNames[i].begin(), ::toupper);
        this->zoneName = this->baseNames[i];

        this->writeBase();
        this->writeZone();
        this->writeCoordinates();
        this->buildGlobalConnectivities();
        this->writeSections();

        this->globalConnectivities.clear();
        this->globalConnectivities.clear();
        this->globalConnectivities.clear();

        this->elementStart = 1;
        this->elementEnd = 0;
    }
}
