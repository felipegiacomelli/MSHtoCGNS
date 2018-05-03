#include <CgnsInterface/CgnsWriter.hpp>
#include <cgnslib.h>

CgnsWriter::CgnsWriter(const std::string& filePath) : filePath(filePath) {
	this->checkFile();
	this->readBase();
	this->readZone();
}

void CgnsWriter::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path()))
		throw std::runtime_error("CgnsWriter: The parent path does not exist");

	if (!boost::filesystem::exists(this->filePath))
		throw std::runtime_error("CgnsWriter: There is no file in the given path");

	if (input.extension() != ".cgns")
		throw std::runtime_error("CgnsWriter: The file extension is not .cgns");

	if (cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex))
		throw std::runtime_error("CgnsWriter: Could not open the file " + this->filePath);
}

void CgnsWriter::readBase() {
	if (cg_nbases(this->fileIndex, &this->baseIndex))
		throw std::runtime_error("CgnsWriter: Could not read number of bases");

	if (this->baseIndex != 1)
		throw std::runtime_error("CgnsWriter: The CGNS file has more than one base");

	if (cg_base_read(this->fileIndex, this->baseIndex, this->buffer, &this->cellDimension, &this->physicalDimension))
		throw std::runtime_error("CgnsWriter: Could not read base");
}

void CgnsWriter::readZone() {
	if (cg_nzones(this->fileIndex, this->baseIndex, &this->zoneIndex))
		throw std::runtime_error("CgnsWriter: Could not read number of zones");

	if (this->zoneIndex != 1)
		throw std::runtime_error("CgnsWriter: The CGNS file has more than one zone");

	ZoneType_t zoneType;
	if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &zoneType))
		throw std::runtime_error("CgnsWriter: Could not read zone type");

	if (zoneType != Unstructured)
		throw std::runtime_error("CgnsWriter: Only unstructured zones are supported");

	if (cg_zone_read(this->fileIndex, this->baseIndex, this->zoneIndex, this->buffer, &this->sizes[0]))
		throw std::runtime_error("CgnsWriter: Could not read zone");
}

void CgnsWriter::writePermanentSolution() {
	cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Solution", Vertex, &this->solutionIndex);
}

void CgnsWriter::writePermanentField(const std::vector<double>& fieldValues, const std::string& fieldName) {
	cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, RealDouble, fieldName.c_str(), &fieldValues[0], &this->fieldIndex);
}

void CgnsWriter::writeTimeStep(const double& timeInstant) {
	this->timeSteps.push_back(timeInstant);
	this->solutionIndices.emplace_back(0);
	std::string solutionName = std::string("TimeStep") + std::to_string(timeSteps.size());
	cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), Vertex, &this->solutionIndices.back());
}

void CgnsWriter::writeTransientField(const std::vector<double>& field, std::string&& fieldName) {
	this->fieldsIndices.emplace_back(0);
	cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndices.back(), RealDouble, fieldName.c_str(), &field[0], &this->fieldsIndices.back());
}

void CgnsWriter::finalize() {
	if (this->timeSteps.size() > 0) {
 		int numberOfTimeSteps = this->timeSteps.size();
 	    cg_biter_write(this->fileIndex, this->baseIndex, "TimeIterativeValues", this->timeSteps.size());
 	    cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, nullptr);
		cg_array_write("TimeValues", RealDouble, 1, &numberOfTimeSteps, &this->timeSteps[0]);
    	cg_simulation_type_write(this->fileIndex, this->baseIndex, TimeAccurate);
	}
}

CgnsWriter::~CgnsWriter() {
	this->finalize();
	cg_close(this->fileIndex);
}