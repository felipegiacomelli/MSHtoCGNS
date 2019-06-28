#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"
#include <cgnslib.h>

CgnsWriter::CgnsWriter(std::string filePath, std::string gridLocation) : CgnsOpener(filePath, "Modify") {
    this->setGridLocation(gridLocation);
}

void CgnsWriter::setGridLocation(std::string gridLocation) {
    if (gridLocation == std::string("Vertex"))
        this->gridLocation = 2;
    else if (gridLocation == std::string("CellCenter"))
        this->gridLocation = 3;
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Solution location must be either Vertex or CellCenter");
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

void CgnsWriter::writeTransientSolution(double timeValue) {
    this->timeValues.push_back(timeValue);
    std::string solutionName = std::string("TimeStep") + std::to_string(timeValues.size());
    cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), GridLocation_t(this->gridLocation), &this->solutionIndex);
}

void CgnsWriter::writeTransientField(std::string name, const std::vector<double>& values) {
    cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, RealDouble, name.c_str(), &values[0], &this->fieldIndex);
}

void CgnsWriter::finalizeTransient() {
    if (!this->isFinalized) {
        this->isFinalized = true;
        if (this->timeValues.size() > 0) {
            int numberOfTimeSteps = this->timeValues.size();
            cg_biter_write(this->fileIndex, this->baseIndex, "TimeIterativeValues", this->timeValues.size());
            cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, nullptr);
            cg_array_write("TimeValues", RealDouble, 1, &numberOfTimeSteps, &this->timeValues[0]);
            cg_simulation_type_write(this->fileIndex, this->baseIndex, TimeAccurate);
        }
    }
}

CgnsWriter::~CgnsWriter() {
    this->finalizeTransient();
}
