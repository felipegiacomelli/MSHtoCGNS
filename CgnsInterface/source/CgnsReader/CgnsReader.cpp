#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include <cgnslib.h>

CgnsReader::CgnsReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
    this->readNumberOfBases();
    this->readBase();
    this->readZone();
    this->readNumberOfSections();
    this->readNumberOfBoundaries();
    this->createGridData();
}

void CgnsReader::checkFile() {
    if (!boost::filesystem::exists(this->filePath.parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path " + this->filePath.parent_path().string() + " does not exist");

    if (!boost::filesystem::exists(this->filePath))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    if (cg_is_cgns(boost::filesystem::absolute(this->filePath).c_str(), &this->fileType))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The file is not a valid cgns file");

    if (cg_open(boost::filesystem::absolute(this->filePath).c_str(), CG_MODE_READ, &this->fileIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not open the file " + boost::filesystem::absolute(this->filePath).string());

    if (cg_version(this->fileIndex, &this->fileVersion))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read file version");

    if (this->fileVersion <= 3.10)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(this->fileVersion) + ") is older than 3.11");
}

void CgnsReader::readNumberOfBases() {
    if (cg_nbases(this->fileIndex, &this->numberOfBases))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of bases");

    if (this->numberOfBases < 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has no base");
}

void CgnsReader::readBase() {
    if (cg_base_read(this->fileIndex, this->baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read base");
}

void CgnsReader::readZone() {
    if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of zones");

    if (this->zoneIndex != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has more than one zone");

    ZoneType_t zoneType;
    if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &zoneType))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read zone type");

    if (zoneType != Unstructured)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Only unstructured zones are supported");

    if (cg_zone_read(this->fileIndex, this->baseIndex, this->zoneIndex, this->buffer, this->sizes))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read zone");
}

void CgnsReader::readNumberOfSections() {
    if (cg_nsections(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfSections))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of sections");
}

void CgnsReader::readNumberOfBoundaries() {
    if (cg_nbocos(this->fileIndex, this->baseIndex, this->zoneIndex, &this->numberOfBoundaries))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of boundaries");
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
        if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section elements");

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

CgnsReader::~CgnsReader() {
    cg_close(this->fileIndex);
}
