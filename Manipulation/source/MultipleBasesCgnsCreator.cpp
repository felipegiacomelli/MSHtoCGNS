#include "MSHtoCGNS/Manipulation/MultipleBasesCgnsCreator.hpp"
#include <cgnslib.h>

MultipleBasesCgnsCreator::MultipleBasesCgnsCreator(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::vector<std::string> zoneNames, std::string outputPath) :
    CgnsCreator(gridDatas.front(), outputPath, false), gridDatas(gridDatas), baseNames(baseNames), zoneNames(zoneNames) {
    this->create();
}

void MultipleBasesCgnsCreator::create() {
    for (unsigned i = 0; i < this->gridDatas.size(); ++i) {
        this->gridData = this->gridDatas[i];
        this->baseName = boost::to_upper_copy(this->baseNames[i]);
        this->zoneName = boost::to_upper_copy(this->zoneNames[i]);

        this->setDimensions();
        this->writeBase();
        this->writeZone();
        this->writeCoordinates();
        this->buildGlobalConnectivities();
        this->writeSections(gridData->regions);
        this->writeSections(gridData->boundaries);
        this->writeSections(gridData->wells);

        this->global.clear();
        this->elementStart = 1;
        this->elementEnd = 0;
    }
}
