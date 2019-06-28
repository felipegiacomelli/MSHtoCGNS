#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include <cgnslib.h>

CgnsReader::CgnsReader(std::string filePath) : CgnsOpener(filePath, "Read") {
    this->readNumberOfSections();
    this->createGridData();
}

void CgnsReader::readNumberOfSections() {
    if (cg_nsections(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfSections))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of sections");
}

void CgnsReader::createGridData() {
    this->gridData = boost::make_shared<GridData>();
    this->gridData->dimension = this->cellDimension;
}

void CgnsReader::readSections() {
    for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; ++sectionIndex) {
        {
            ElementType_t elementType;
            int lastBoundaryElement, parentFlag;
            if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &this->elementStart, &this->elementEnd, &lastBoundaryElement, &parentFlag))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");
            this->elementType = elementType;
        }

        if (this->skipSection())
            continue;

        int size;
        if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

        std::vector<int> connectivities(size);
        std::vector<int> offsets(this->elementEnd - this->elementStart + 2);
        if (this->elementType == MIXED)
            cg_poly_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], &offsets[0], nullptr);
        else
            if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section " + this->buffer + " elements");

        this->addConnectivities(connectivities);

        if (this->elementType == MIXED)
            this->addEntity(connectivities[0]);
        else
            this->addEntity(this->elementType);
    }
}

void CgnsReader::addRegion(std::string&& name, int begin, int end) {
    this->gridData->regions.emplace_back(RegionData());
    std::transform(name.begin(), name.end(), std::back_inserter(this->gridData->regions.back().name), ::toupper);
    this->gridData->regions.back().begin = begin;
    this->gridData->regions.back().end = end;
}

void CgnsReader::addBoundary(std::string&& name, int begin, int end) {
    this->gridData->boundaries.emplace_back(BoundaryData());
    std::transform(name.begin(), name.end(), std::back_inserter(this->gridData->boundaries.back().name), ::toupper);
    this->gridData->boundaries.back().begin = begin;
    this->gridData->boundaries.back().end = end;
}

void CgnsReader::addConnectivities(const std::vector<int>& connectivities) {
    int numberOfVertices;
    if (cg_npe(ElementType_t(this->elementType), &numberOfVertices))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

    int numberOfElements = this->elementEnd - this->elementStart + 1;

    switch (this->elementType) {
        case MIXED : {
            int position = 0;
            for (int e = 0; e < numberOfElements; ++e) {
                cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                std::vector<int> element(numberOfVertices);
                for (int k = 0; k < numberOfVertices; ++k)
                    element[k] = connectivities[position + 1 + k] - 1;
                element.emplace_back(this->elementStart - 1 + e);
                switch (connectivities[position]) {
                    case TETRA_4: {
                        this->addConnectivity(this->gridData->tetrahedrons, element);
                        break;
                    }
                    case HEXA_8: {
                        this->addConnectivity(this->gridData->hexahedrons, element);
                        break;
                    }
                    case PENTA_6: {
                        this->addConnectivity(this->gridData->prisms, element);
                        break;
                    }
                    case PYRA_5: {
                        this->addConnectivity(this->gridData->pyramids, element);
                        break;
                    }
                    case TRI_3: {
                        this->addConnectivity(this->gridData->triangles, element);
                        break;
                    }
                    case QUAD_4: {
                        this->addConnectivity(this->gridData->quadrangles, element);
                        break;
                    }
                }
                position += numberOfVertices + 1;
            }
            break;
        }
        case TETRA_4: {
            addConnectivity(this->gridData->tetrahedrons, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        case HEXA_8: {
            addConnectivity(this->gridData->hexahedrons, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        case PENTA_6: {
            addConnectivity(this->gridData->prisms, connectivities, numberOfElements, numberOfVertices);
            break;
        }
       case PYRA_5: {
            addConnectivity(this->gridData->pyramids, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        case TRI_3: {
            addConnectivity(this->gridData->triangles, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        case QUAD_4: {
            addConnectivity(this->gridData->quadrangles, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        case BAR_2: {
            addConnectivity(this->gridData->lines, connectivities, numberOfElements, numberOfVertices);
            break;
        }
        default:
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Section " + std::string(this->buffer) + " element type " + std::to_string(elementType) + " not supported");
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
