#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include <cgnslib.h>

CgnsCreator::CgnsCreator(boost::shared_ptr<GridData> gridData, std::string outputPath, bool createInConstructor) : gridData(gridData), output(outputPath) {
    this->setDimensions();
    this->setupFile();
    if (createInConstructor)
        this->create();
}

void CgnsCreator::setDimensions() {
    this->cellDimension = this->gridData->dimension;

    this->sizes[0] = this->gridData->coordinates.size();

    const auto& cs = this->gridData->connectivities;

    std::vector<int> elementTypes;
    if (this->gridData->dimension == 2)
        elementTypes = std::vector<int>{TRI_3, TRI_6, TRI_10, QUAD_4, QUAD_9, QUAD_8};
    else
        elementTypes = std::vector<int>{TETRA_4, TETRA_10, TETRA_20, HEXA_8, HEXA_27, HEXA_20, HEXA_64, PENTA_6, PENTA_18, PENTA_15, PYRA_5, PYRA_14, PYRA_13};

    this->sizes[1] = std::count_if(cs.cbegin(), cs.cend(), [&](const auto& c){return hasElement(elementTypes.cbegin(), elementTypes.cend(), c[0]);});

    this->sizes[2] = 0;
}

void CgnsCreator::setupFile() {
    if (this->output.extension() == std::string(".cgns")) {
        if (boost::filesystem::exists(this->output))
            boost::filesystem::remove_all(this->output);
        this->fileName = boost::replace_all_copy(this->output.string(), "//", "/");
    }
    else {
        auto directory(boost::filesystem::absolute(this->output) / boost::str(boost::format("/%iv_%ie/") % this->sizes[0] % this->sizes[1]));
        if (!boost::filesystem::exists(directory))
            boost::filesystem::create_directory(directory);
        this->fileName = boost::replace_all_copy(directory.string(), "//", "/") + std::string("Grid.cgns");
    }
    cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsCreator::create() {
    this->writeBase();
    this->writeZone();
    this->writeCoordinates();
    this->buildGlobalConnectivities();
    this->writeSections(this->gridData->sections);
}

void CgnsCreator::writeBase() {
    if (cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write base", cg_get_error()));
}

void CgnsCreator::writeZone() {
    if (cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), this->sizes, Unstructured, &this->zoneIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write zone", cg_get_error()));
}

void CgnsCreator::writeCoordinates() {
    std::vector<double> coordinatesX(this->sizes[0]);
    std::vector<double> coordinatesY(this->sizes[0]);
    std::vector<double> coordinatesZ(this->sizes[0]);
    for (int c = 0; c < this->sizes[0]; ++c) {
        coordinatesX[c] = this->gridData->coordinates[c][0];
        coordinatesY[c] = this->gridData->coordinates[c][1];
        coordinatesZ[c] = this->gridData->coordinates[c][2];
    }

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", &coordinatesX[0], &this->coordinateIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write CoordinateX", cg_get_error()));

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", &coordinatesY[0], &this->coordinateIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write CoordinateY", cg_get_error()));

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", &coordinatesZ[0], &this->coordinateIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write CoordinateZ", cg_get_error()));
}

void CgnsCreator::buildGlobalConnectivities() {
    this->global = this->gridData->connectivities;

    for (auto& connectivity : this->global)
        connectivity.pop_back();

    for (auto& connectivity : this->global)
        for (auto vertex = connectivity.begin() + 1; vertex != connectivity.end(); ++vertex)
            *vertex += 1;
}

void CgnsCreator::writeSections(std::vector<SectionData> sections) {
    for (auto section : sections) {
        std::transform(section.name.begin(), section.name.end(), section.name.begin(), ::toupper);

        this->setElementType(section.begin, section.end);

        if (this->elementType != MIXED) {
            this->writeSection(section.begin, section.end, section.name);
        }
        else {
            std::vector<int> offsets;
            offsets.reserve(section.end - section.begin + 1);
            offsets.emplace_back(0);

            for (auto element = this->global.begin() + section.begin; element != this->global.begin() + section.end; ++element) {
                offsets.emplace_back(offsets.back() + element->size());
            }

            this->writePolySection(section.begin, section.end, section.name, offsets);
        }
    }
}

void CgnsCreator::setElementType(int begin, int end) {
    this->elementType = MIXED;
    int target = (this->global.cbegin() + begin)->front();
    if (std::all_of(this->global.cbegin() + begin, this->global.cbegin() + end, [=](const auto& c){return c.front() == target;}))
        this->elementType = target;
}

void CgnsCreator::writeSection(int begin, int end, std::string name) {
    std::vector<int> connectivities;
    connectivities.reserve(std::accumulate(this->global.begin() + begin, this->global.begin() + end, 0, [](int s, const auto& c){return s + c.size();}));
    append(this->elementType, this->global.cbegin() + begin, this->global.cbegin() + end, std::back_inserter(connectivities));

    this->elementEnd += end - begin;

    if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, name.c_str(), ElementType_t(this->elementType), this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write section " + name, cg_get_error()));

    this->elementStart = this->elementEnd + 1;
}

void CgnsCreator::writePolySection(int begin, int end, std::string name, const std::vector<int>& offsets) {
    std::vector<int> connectivities;
    connectivities.reserve(std::accumulate(this->global.begin() + begin, this->global.begin() + end, 0, [](int s, const auto& c){return s + c.size();}));
    append(this->global.cbegin() + begin, this->global.cbegin() + end, std::back_inserter(connectivities));

    this->elementEnd += end - begin;

    if (cg_poly_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, name.c_str(), MIXED, this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &offsets[0], &this->sectionIndex))
        throw std::runtime_error(error(__PRETTY_FUNCTION__, "Could not write poly section " + name, cg_get_error()));

    this->elementStart = this->elementEnd + 1;
}

std::string CgnsCreator::getFileName() const {
    return this->fileName;
}

CgnsCreator::~CgnsCreator() {
    cg_close(this->fileIndex);
}
