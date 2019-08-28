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

    if (this->gridData->dimension == 2)
        this->sizes[1] = this->gridData->triangles.size() + this->gridData->quadrangles.size();
    else
        this->sizes[1] = this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size();

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
    this->writeSections(this->gridData->regions);
    this->writeSections(this->gridData->boundaries);
    this->writeSections(this->gridData->wells);
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
    this->global.reserve(this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size() + this->gridData->triangles.size() + this->gridData->quadrangles.size() + this->gridData->lines.size());

    append(TETRA_4, this->gridData->tetrahedrons, this->global);
    append(HEXA_8, this->gridData->hexahedrons, this->global);
    append(PENTA_6, this->gridData->prisms, this->global);
    append(PYRA_5, this->gridData->pyramids, this->global);
    append(TRI_3, this->gridData->triangles, this->global);
    append(QUAD_4, this->gridData->quadrangles, this->global);
    append(BAR_2, this->gridData->lines, this->global);

    std::sort(this->global.begin(), this->global.end(), [](const auto& a, const auto& b){return a.back() < b.back();});

    for (auto& connectivity : this->global)
        connectivity.pop_back();

    for (auto& connectivity : this->global)
        for (auto vertex = connectivity.begin() + 1; vertex != connectivity.end(); ++vertex)
            *vertex += 1;
}

void CgnsCreator::writeSections(std::vector<EntityData> entities) {
    for (auto entity : entities) {
        std::transform(entity.name.begin(), entity.name.end(), entity.name.begin(), ::toupper);

        this->setElementType(entity.begin, entity.end);

        if (this->elementType != MIXED) {
            this->writeSection(entity.begin, entity.end, entity.name);
        }
        else {
            std::vector<int> offsets;
            offsets.reserve(entity.end - entity.begin + 1);
            offsets.emplace_back(0);

            for (auto element = this->global.begin() + entity.begin; element != this->global.begin() + entity.end; ++element) {
                offsets.emplace_back(offsets.back() + element->size());
            }

            this->writePolySection(entity.begin, entity.end, entity.name, offsets);
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
