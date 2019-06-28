#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include <cgnslib.h>

CgnsCreator::CgnsCreator(boost::shared_ptr<GridData> gridData, std::string outputPath) : gridData(gridData), output(outputPath) {}

void CgnsCreator::setupFile() {
    if (this->output.extension() == std::string(".cgns")) {
        if (boost::filesystem::exists(this->output))
            boost::filesystem::remove_all(this->output);
        this->fileName = this->output.string();
    }
    else {
        boost::filesystem::path folderName(boost::filesystem::absolute(this->output).string() + std::string("/") + std::to_string(this->sizes[0]) + std::string("v_") + std::to_string(this->sizes[1]) + "e/");
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
    this->global.reserve(this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size() + this->gridData->triangles.size() + this->gridData->quadrangles.size() + this->gridData->lines.size());

    for (auto i = this->gridData->tetrahedrons.cbegin(); i != this->gridData->tetrahedrons.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->hexahedrons.cbegin(); i != this->gridData->hexahedrons.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->prisms.cbegin(); i != this->gridData->prisms.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->pyramids.cbegin(); i != this->gridData->pyramids.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }

    for (auto i = this->gridData->triangles.cbegin(); i != this->gridData->triangles.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }
    for (auto i = this->gridData->quadrangles.cbegin(); i != this->gridData->quadrangles.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }

    for (auto i = this->gridData->lines.cbegin(); i != this->gridData->lines.cend(); ++i) {
        this->global.emplace_back(std::vector<int>());
        std::transform(i->cbegin(), i->cend(), std::back_inserter(this->global.back()), [](auto x){return x + 1;});
    }

    std::sort(this->global.begin(), this->global.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
    for (unsigned i = 0; i < this->global.size(); ++i)
        this->global[i].pop_back();
}

void CgnsCreator::writeSections() {
    this->writeRegions();
    this->writeBoundaries();
}

void CgnsCreator::setElementType(int begin, int end, std::unordered_map<unsigned,int> sizeType) {
    this->elementType = MIXED;
    for (auto entry : sizeType)
        if (std::all_of(this->global.begin() + begin, this->global.begin() + end, [=](const auto& c){return c.size() == entry.first;}))
            this->elementType = entry.second;
}

void CgnsCreator::writeSection(int begin, int end, std::string name) {
    std::vector<int> connectivities;
    connectivities.reserve(std::accumulate(this->global.begin() + begin, this->global.begin() + end, 0, [](int s, const auto& c){return s + c.size();}));
    append(this->global.begin() + begin, this->global.begin() + end, std::back_inserter(connectivities));

    this->elementEnd += end - begin;

    if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, name.c_str(), ElementType_t(this->elementType), this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write section " + name);

    this->elementStart = this->elementEnd + 1;
}

void CgnsCreator::writePolySection(int begin, int end, std::string name, const std::vector<int>& offsets) {
    std::vector<int> connectivities;
    connectivities.reserve(std::accumulate(this->global.begin() + begin, this->global.begin() + end, 0, [](int s, const auto& c){return s + c.size();}));
    append(this->global.begin() + begin, this->global.begin() + end, std::back_inserter(connectivities));

    this->elementEnd += end - begin;

    if (cg_poly_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, name.c_str(), MIXED, this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &offsets[0], &this->sectionIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write poly section " + name);

    this->elementStart = this->elementEnd + 1;
}

std::string CgnsCreator::getFileName() const {
    return this->fileName;
}

CgnsCreator::~CgnsCreator() {
    cg_close(this->fileIndex);
}
