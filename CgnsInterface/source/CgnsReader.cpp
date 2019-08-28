#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include <cgnslib.h>

CgnsReader::CgnsReader(std::string filePath, bool readInConstructor) : CgnsOpener(filePath, "Read") {
    this->createGridData();
    if (readInConstructor) {
        this->read();
    }
}

void CgnsReader::createGridData() {
    this->gridData = boost::make_shared<GridData>();
    this->gridData->dimension = this->cellDimension;
}

void CgnsReader::read() {
    this->readNumberOfSections();
    this->readCoordinates();
    this->readSections();
    this->buildGlobalConnectivities();
    this->findVertices(this->gridData->regions);
    this->findVertices(this->gridData->boundaries);
    this->findVertices(this->gridData->wells);
}

void CgnsReader::readNumberOfSections() {
    if (cg_nsections(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfSections))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of sections");
}

void CgnsReader::readCoordinates() {
    std::vector<double> coordinatesX(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &this->one, this->sizes, &coordinatesX[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateX");

    std::vector<double> coordinatesY(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &this->one, this->sizes, &coordinatesY[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateY");

    int numberOfCoordinates;
    if (cg_ncoords(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfCoordinates))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of coordinates");

    std::vector<double> coordinatesZ(this->sizes[0]);
    if (numberOfCoordinates == 3)
        if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateZ", RealDouble, &this->one, this->sizes, &coordinatesZ[0]))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateZ");

    this->gridData->numberOfLocalVertices = this->sizes[0];
    this->gridData->coordinates.reserve(this->sizes[0]);
    for (int c = 0; c < this->sizes[0]; ++c) {
        this->gridData->coordinates.emplace_back(std::array<double, 3>{coordinatesX[c], coordinatesY[c], coordinatesZ[c]});
    }
}

void CgnsReader::readSections() {
    for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; ++sectionIndex) {
        ElementType_t elementType;
        int lastBoundaryElement, parentFlag;
        if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &this->elementStart, &this->elementEnd, &lastBoundaryElement, &parentFlag))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");

        if (this->skipSection(std::string(this->buffer), elementType))
            continue;

        int size;
        if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

        std::vector<int> connectivities(size);
        std::vector<int> offsets(this->elementEnd - this->elementStart + 2);
        if (elementType == MIXED)
            cg_poly_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], &offsets[0], nullptr);
        else
            if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section " + this->buffer + " elements");

        this->addConnectivities(connectivities, elementType);

        if (elementType == MIXED)
            this->addEntity(connectivities[0]);
        else
            this->addEntity(elementType);
    }
}

bool CgnsReader::skipSection(std::string, int) {
    return false;
}

void CgnsReader::addEntity(int elementType) {
    if (this->cellDimension == 2) {
        if (elementType == TRI_3 || elementType == QUAD_4)
            this->addRegion();
        else if (elementType == BAR_2)
            this->addBoundary();
    }
    else if (this->cellDimension == 3) {
        if (elementType == TETRA_4 || elementType == HEXA_8 || elementType == PENTA_6 || elementType == PYRA_5)
            this->addRegion();
        else if (elementType == TRI_3 || elementType == QUAD_4)
            this->addBoundary();
        else if (elementType == BAR_2)
            this->addWell();
    }
}

void CgnsReader::addRegion() {
    this->gridData->regions.emplace_back(EntityData{boost::to_upper_copy(std::string(this->buffer)), this->elementStart - 1, this->elementEnd});
}

void CgnsReader::addBoundary() {
    this->gridData->boundaries.emplace_back(EntityData{boost::to_upper_copy(std::string(this->buffer)), this->elementStart - 1, this->elementEnd});
}

void CgnsReader::addWell() {
    this->gridData->wells.emplace_back(EntityData{boost::to_upper_copy(std::string(this->buffer)), this->elementStart - 1, this->elementEnd});
}

void CgnsReader::addConnectivities(const std::vector<int>& connectivities, int elementType) {
    int numberOfVertices;
    if (cg_npe(ElementType_t(elementType), &numberOfVertices))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

    int numberOfElements = this->elementEnd - this->elementStart + 1;

    switch (elementType) {
        case MIXED : {
            int position = 0;
            for (int e = 0; e < numberOfElements; ++e) {
                cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                std::vector<int> element(numberOfVertices);
                for (int k = 0; k < numberOfVertices; ++k) {
                    element[k] = connectivities[position + 1 + k] - 1;
                }
                element.emplace_back(this->elementStart - 1 + e);
                switch (connectivities[position]) {
                    case TETRA_4:
                        this->addConnectivity(this->gridData->tetrahedrons, element);
                        break;
                    case HEXA_8:
                        this->addConnectivity(this->gridData->hexahedrons, element);
                        break;
                    case PENTA_6:
                        this->addConnectivity(this->gridData->prisms, element);
                        break;
                    case PYRA_5:
                        this->addConnectivity(this->gridData->pyramids, element);
                        break;
                    case TRI_3:
                        this->addConnectivity(this->gridData->triangles, element);
                        break;
                    case QUAD_4:
                        this->addConnectivity(this->gridData->quadrangles, element);
                        break;
                }
                position += numberOfVertices + 1;
            }
            break;
        }
        case TETRA_4:
            addConnectivity(this->gridData->tetrahedrons, connectivities, numberOfElements, numberOfVertices);
            break;
        case HEXA_8:
            addConnectivity(this->gridData->hexahedrons, connectivities, numberOfElements, numberOfVertices);
            break;
        case PENTA_6:
            addConnectivity(this->gridData->prisms, connectivities, numberOfElements, numberOfVertices);
            break;
       case PYRA_5:
            addConnectivity(this->gridData->pyramids, connectivities, numberOfElements, numberOfVertices);
            break;
        case TRI_3:
            addConnectivity(this->gridData->triangles, connectivities, numberOfElements, numberOfVertices);
            break;
        case QUAD_4:
            addConnectivity(this->gridData->quadrangles, connectivities, numberOfElements, numberOfVertices);
            break;
        case BAR_2:
            addConnectivity(this->gridData->lines, connectivities, numberOfElements, numberOfVertices);
            break;
        default:
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Section " + std::string(this->buffer) + " element type " + std::to_string(elementType) + " not supported");
    }
}

void CgnsReader::buildGlobalConnectivities() {
    this->global.reserve(this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size() + this->gridData->triangles.size() + this->gridData->quadrangles.size() + this->gridData->lines.size());

    append(this->gridData->tetrahedrons, this->global);
    append(this->gridData->hexahedrons, this->global);
    append(this->gridData->prisms, this->global);
    append(this->gridData->pyramids, this->global);
    append(this->gridData->triangles, this->global);
    append(this->gridData->quadrangles, this->global);
    append(this->gridData->lines, this->global);

    std::sort(this->global.begin(), this->global.end(), [](const auto& a, const auto& b){return a.back() < b.back();});
}

void CgnsReader::findVertices(std::vector<EntityData>& entities) {
    for (auto& entity : entities) {
        std::set<int> vertices;
        for (auto position = this->global.cbegin() + entity.begin; position != this->global.cbegin() + entity.end; ++position) {
            vertices.insert(position->cbegin(), position->cend() - 1);
        }
        entity.vertices = std::vector<int>{vertices.begin(), vertices.end()};
    }
}

int CgnsReader::readSolutionIndex(std::string solutionName) {
    int numberOfSolutions;
    if (cg_nsols(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfSolutions))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of solutions");

    int solutionIndex;
    for (solutionIndex = 1; solutionIndex <= numberOfSolutions; ++solutionIndex) {
        GridLocation_t gridLocation;
        if (cg_sol_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionIndex, this->buffer, &gridLocation))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read solution " + std::to_string(solutionIndex) + " information");

        if (solutionName.compare(this->buffer) == 0)
            break;
    }

    return solutionIndex;
}

std::vector<double> CgnsReader::readField(int solutionIndex, std::string fieldName) {
    int dataDimension, solutionEnd;
    if (cg_sol_size(this->fileIndex, this->baseIndex, this->zoneIndex, solutionIndex, &dataDimension, &solutionEnd))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read solution " + std::to_string(solutionIndex));

    int solutionStart = 1;
    std::vector<double> field(solutionEnd);
    if (cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionIndex, fieldName.c_str(), RealDouble, &solutionStart, &solutionEnd, &field[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read permanent field '" + fieldName + "'' in solution " + std::to_string(solutionIndex));

    return field;
}

std::vector<double> CgnsReader::readField(std::string solutionName, std::string fieldName){
    return this->readField(this->readSolutionIndex(solutionName), fieldName);
}

int CgnsReader::readNumberOfTimeSteps() {
    int numberOfTimeSteps;
    if (cg_biter_read(this->fileIndex, this->baseIndex, this->buffer, &numberOfTimeSteps))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not base iterative data information");

    return numberOfTimeSteps;
}

std::vector<double> CgnsReader::readTimeInstants() {
    if (cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, nullptr))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not go to base iterative data");

    int arrayIndex = 1;
    DataType_t dataType;
    int dataDimension, dimensionVector;
    if (cg_array_info(arrayIndex, this->buffer, &dataType, &dataDimension, &dimensionVector))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read array information");

    std::vector<double> timeInstants(dimensionVector);
    if (cg_array_read(arrayIndex, &timeInstants[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read array");

    return timeInstants;
}
