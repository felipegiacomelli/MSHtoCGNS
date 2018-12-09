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
    boost::filesystem::path input(this->filePath);
    if (!boost::filesystem::exists(input.parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path " + input.parent_path().string() + " does not exist");

    if (!boost::filesystem::exists(this->filePath))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    int fileType;
    if (cg_is_cgns(this->filePath.c_str(), &fileType))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The file is not a valid cgns file");

    if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not open the file " + this->filePath);

    float version;
    if (cg_version(this->fileIndex, &version))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read file version");

    if (version <= 3.10)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(version) + ") is older than 3.11");
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

void CgnsReader::addRegion(std::string&& name, int elementStart, int end) {
    RegionData region;
    region.name = name;
    region.begin = elementStart;
    region.end = end;
    this->gridData->regions.emplace_back(std::move(region));
}

void CgnsReader::addBoundary(std::string&& name, int elementStart, int end) {
    BoundaryData boundary;
    boundary.name = name;
    boundary.begin = elementStart;
    boundary.end = end;
    this->gridData->boundaries.emplace_back(std::move(boundary));
}

int CgnsReader::readSolutionIndex(std::string solutionName) {
    int numberOfSolutions;
    if (cg_nsols(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfSolutions))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of solutions");

    int solutionIndex;
    for (solutionIndex = 1; solutionIndex <= numberOfSolutions; solutionIndex++) {
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
