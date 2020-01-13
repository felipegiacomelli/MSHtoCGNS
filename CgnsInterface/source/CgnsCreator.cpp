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

    if (this->gridData->dimension == 2)
        this->sizes[1] = std::count_if(cs.begin(), cs.cend(), [](const auto& c){return c[0] == TRI_3 || c[0] == QUAD_4;});
    else
        this->sizes[1] = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TETRA_4 || c[0] == HEXA_8 || c[0] == PENTA_6 || c[0] == PYRA_5;});

    this->sizes[2] = 0;
}

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
