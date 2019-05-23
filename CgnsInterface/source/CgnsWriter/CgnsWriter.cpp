#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"
#include <cgnslib.h>

CgnsWriter::CgnsWriter(std::string path, std::string gridLocation) : path(path), isFinalized(false) {
    if (gridLocation == std::string("Vertex"))
        this->gridLocation = 2;
    else if (gridLocation == std::string("CellCenter"))
        this->gridLocation = 3;
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Solution location must be either Vertex or CellCenter");

    this->checkFile();
    this->readBase();
    this->readZone();
}

void CgnsWriter::checkFile() {
    if (!boost::filesystem::exists(boost::filesystem::path(this->path).parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path does not exist");

    if (!boost::filesystem::exists(this->path))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    if (cg_open(this->path.c_str(), CG_MODE_MODIFY, &this->fileIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not open the file " + this->path);

    float version;
    if (cg_version(this->fileIndex, &version))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could read file version");

    if (version <= 3.10)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(version) + ") is older than 3.11");
}

void CgnsWriter::readBase() {
    int numberOfBases;
    if (cg_nbases(this->fileIndex, &numberOfBases))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of bases");

    if (numberOfBases < 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has no base");

    this->baseIndex = 1;
}

void CgnsWriter::readZone() {
    if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of zones");

    if (this->zoneIndex != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has more than one zone");
}

void CgnsWriter::writePermanentSolution(std::string name) {
    if (cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, name.c_str(), GridLocation_t(this->gridLocation), &this->permanentSolutionIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write permanent solution " + name);
}

void CgnsWriter::writePermanentField(std::string name, const std::vector<double>& values) {
    if (cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->permanentSolutionIndex, RealDouble, name.c_str(), &values[0], &this->permanentFieldIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + name);
}

void CgnsWriter::writePermanentField(std::string name, const std::vector<int>& values) {
    if (cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->permanentSolutionIndex, Integer, name.c_str(), &values[0], &this->permanentFieldIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + name);
}

void CgnsWriter::writeTransientSolution(double timeInstant) {
    this->timeInstants.push_back(timeInstant);
    this->solutionIndices.emplace_back(0);
    std::string solutionName = std::string("TimeStep") + std::to_string(timeInstants.size());
    cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), GridLocation_t(this->gridLocation), &this->solutionIndices.back());
}

void CgnsWriter::writeTransientField(std::string name, const std::vector<double>& values) {
    this->fieldsIndices.emplace_back(0);
    cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndices.back(), RealDouble, name.c_str(), &values[0], &this->fieldsIndices.back());
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
