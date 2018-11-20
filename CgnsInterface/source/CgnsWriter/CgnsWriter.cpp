#include <CgnsInterface/CgnsWriter.hpp>
#include <cgnslib.h>

CgnsWriter::CgnsWriter(std::string filePath, std::string solutionLocation) : filePath(filePath), isFinalized(false) {
    if (solutionLocation == std::string("Vertex"))
        this->gridLocation = 2;
    else if (solutionLocation == std::string("CellCenter"))
        this->gridLocation = 3;
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Solution location must be either Vertex or CellCenter");

    this->checkFile();
    this->readBase();
    this->readZone();
}

void CgnsWriter::checkFile() {
    boost::filesystem::path input(this->filePath);
    if (!boost::filesystem::exists(input.parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path does not exist");

    if (!boost::filesystem::exists(this->filePath))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    if (cg_open(this->filePath.c_str(), CG_MODE_MODIFY, &this->fileIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not open the file " + this->filePath);

    float version;
    if (cg_version(this->fileIndex, &version))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could read file version");

    if (version <= 3.10)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(version) + ") is older than 3.11");
}

void CgnsWriter::readBase() {
    if (cg_nbases(this->fileIndex, &this->baseIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of bases");

    if (this->baseIndex != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has more than one base");
}

void CgnsWriter::readZone() {
    if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of zones");

    if (this->zoneIndex != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has more than one zone");
}

void CgnsWriter::writePermanentSolution(std::string solutionName) {
    if (cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), GridLocation_t(this->gridLocation), &this->permanentSolutionIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write permanent solution " + solutionName);
}


void CgnsWriter::writePermanentField(std::string fieldName, const std::vector<double>& fieldValues){
    if (cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->permanentSolutionIndex, RealDouble, fieldName.c_str(), &fieldValues[0], &this->permanentFieldIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + fieldName);
}

void CgnsWriter::writeTransientSolution(const double& timeInstant) {
    this->timeInstants.push_back(timeInstant);
    this->solutionIndices.emplace_back(0);
    std::string solutionName = std::string("TimeStep") + std::to_string(timeInstants.size());
    cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), GridLocation_t(this->gridLocation), &this->solutionIndices.back());
}

void CgnsWriter::writeTransientField(const std::vector<double>& fieldValues, std::string fieldName) {
    this->fieldsIndices.emplace_back(0);
    cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndices.back(), RealDouble, fieldName.c_str(), &fieldValues[0], &this->fieldsIndices.back());
}

void CgnsWriter::finalizeTransient() {
    if (!this->isFinalized) {
        this->isFinalized = true;
        if (this->timeInstants.size() > 0) {
            int numberOfTimeSteps = this->timeInstants.size();
            cg_biter_write(this->fileIndex, this->baseIndex, "TimeIterativeValues", this->timeInstants.size());
            cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, nullptr);
            cg_array_write("TimeValues", RealDouble, 1, &numberOfTimeSteps, &this->timeInstants[0]);
            cg_simulation_type_write(this->fileIndex, this->baseIndex, TimeAccurate);
        }
        cg_close(this->fileIndex);
    }
}

CgnsWriter::~CgnsWriter() {
    this->finalizeTransient();
}
