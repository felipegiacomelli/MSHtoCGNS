#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include <cgnslib.h>

CgnsCreator::CgnsCreator(boost::shared_ptr<GridData> gridData, std::string folderPath) : gridData(gridData), folderPath(folderPath) {}

void CgnsCreator::setupFile() {
    if (this->folderPath.extension() == std::string(".cgns")) {
        if (boost::filesystem::exists(this->folderPath))
            boost::filesystem::remove_all(this->folderPath);
        this->fileName = this->folderPath.string();
    }
    else {
        boost::filesystem::path folderName(boost::filesystem::absolute(this->folderPath).string() + std::string("/") + std::to_string(this->sizes[0]) + std::string("v_") + std::to_string(this->sizes[1]) + "e/");
        if (!boost::filesystem::exists(folderName))
            boost::filesystem::create_directory(folderName);
        this->fileName = folderName.string() + std::string("Grid.cgns");
    }
    cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsCreator::initialize() {
    this->writeBase();
    this->writeZone();
    this->writeCoordinates();
    this->buildGlobalConnectivities();
    this->writeSections();
}

void CgnsCreator::writeBase() {
    if (cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write base");
}

void CgnsCreator::writeZone() {
    if (cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), this->sizes, Unstructured, &this->zoneIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write zone");
}

void CgnsCreator::buildGlobalConnectivities() {
    for (auto i = this->gridData->tetrahedrons.cbegin(); i != this->gridData->tetrahedrons.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->hexahedrons.cbegin(); i != this->gridData->hexahedrons.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->prisms.cbegin(); i != this->gridData->prisms.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->pyramids.cbegin(); i != this->gridData->pyramids.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }

    for (auto i = this->gridData->triangles.cbegin(); i != this->gridData->triangles.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->quadrangles.cbegin(); i != this->gridData->quadrangles.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }

    for (auto i = this->gridData->lines.cbegin(); i != this->gridData->lines.cend(); ++i) {
        this->globalConnectivities.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
    }

    std::stable_sort(this->globalConnectivities.begin(), this->globalConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
    for (unsigned i = 0; i < this->globalConnectivities.size(); ++i)
        this->globalConnectivities[i].pop_back();
}

void CgnsCreator::writeSections() {
    this->writeRegions();
    this->writeBoundaries();
}

std::string CgnsCreator::getFileName() const {
    return this->fileName;
}

CgnsCreator::~CgnsCreator() {
    cg_close(this->fileIndex);
}
