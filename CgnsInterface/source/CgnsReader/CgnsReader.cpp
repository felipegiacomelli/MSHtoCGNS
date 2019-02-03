#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include <cgnslib.h>

CgnsReader::CgnsReader(std::string filePath) : filePath(filePath) {
    this->checkFile();
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

void CgnsReader::readBase() {
    if (cg_nbases(this->fileIndex, &this->baseIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of bases");

    if (this->baseIndex != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has more than one base");

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

bool CgnsReader::skipSection(int) {
    return false;
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

void CgnsReader::addConnectivities(int elementType, int elementStart, int elementEnd, const std::vector<int> connectivities) {
    int numberOfVertices;
    if (cg_npe(ElementType_t(elementType), &numberOfVertices))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

    int numberOfElements = elementEnd - elementStart + 1;

    switch (ElementType_t(elementType)) {
        case MIXED : {
            int position = 0;
            for (int e = 0; e < numberOfElements; ++e) {
                cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                std::vector<int> element(numberOfVertices);
                for (int k = 0; k < numberOfVertices; ++k)
                    element[k] = connectivities[position + 1 + k] - 1;
                element.emplace_back(elementStart - 1 + e);
                switch (connectivities[position]) {
                    case TETRA_4: {
                        this->gridData->tetrahedronConnectivity.emplace_back(std::array<int, 5>());
                        std::copy_n(std::begin(element), 5, std::begin(this->gridData->tetrahedronConnectivity.back()));
                        break;
                    }
                    case HEXA_8: {
                        this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                        std::copy_n(std::begin(element), 9, std::begin(this->gridData->hexahedronConnectivity.back()));
                        break;
                    }
                    case PENTA_6: {
                        this->gridData->prismConnectivity.emplace_back(std::array<int, 7>());
                        std::copy_n(std::begin(element), 7, std::begin(this->gridData->prismConnectivity.back()));
                        break;
                    }
                    case PYRA_5: {
                        this->gridData->pyramidConnectivity.emplace_back(std::array<int, 6>());
                        std::copy_n(std::begin(element), 6, std::begin(this->gridData->pyramidConnectivity.back()));
                        break;
                    }
                    case TRI_3: {
                        this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                        std::copy_n(std::begin(element), 4, std::begin(this->gridData->triangleConnectivity.back()));
                        break;
                    }
                    case QUAD_4: {
                        this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                        std::copy_n(std::begin(element), 5, std::begin(this->gridData->quadrangleConnectivity.back()));
                        break;
                    }
                }
                position += numberOfVertices + 1;
            }
            break;
        }
        case TETRA_4: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->tetrahedronConnectivity.emplace_back(std::array<int, 5>());
                auto& tetrahedron = this->gridData->tetrahedronConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    tetrahedron[k] = connectivities[e * numberOfVertices + k] - 1;
                tetrahedron.back() = elementStart - 1 + e;
            }
            break;
        }
        case HEXA_8: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                auto& hexahedron = this->gridData->hexahedronConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    hexahedron[k] = connectivities[e * numberOfVertices + k] - 1;
                hexahedron.back() = elementStart - 1 + e;
            }
            break;
        }
        case PENTA_6: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->prismConnectivity.emplace_back(std::array<int, 7>());
                auto& prism = this->gridData->prismConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    prism[k] = connectivities[e * numberOfVertices + k] - 1;
                prism.back() = elementStart - 1 + e;
            }
            break;
        }
       case PYRA_5: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->pyramidConnectivity.emplace_back(std::array<int, 6>());
                auto& pyramid = this->gridData->pyramidConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    pyramid[k] = connectivities[e * numberOfVertices + k] - 1;
                pyramid.back() = elementStart - 1 + e;
            }
            break;
        }
        case TRI_3: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                auto& triangle = this->gridData->triangleConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    triangle[k] = connectivities[e * numberOfVertices + k] - 1;
                triangle.back() = elementStart - 1 + e;
            }
            break;
        }
        case QUAD_4: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                auto& quadrangle = this->gridData->quadrangleConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    quadrangle[k] = connectivities[e * numberOfVertices + k] - 1;
                quadrangle.back() = elementStart - 1 + e;
            }
            break;
        }
        case BAR_2: {
            for (int e = 0; e < numberOfElements; ++e) {
                this->gridData->lineConnectivity.emplace_back(std::array<int, 3>());
                auto& line = this->gridData->lineConnectivity.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    line[k] = connectivities[e * numberOfVertices + k] - 1;
                line.back() = elementStart - 1 + e;
            }
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
