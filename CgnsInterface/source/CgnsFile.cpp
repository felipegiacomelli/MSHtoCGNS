#include <CgnsInterface/CgnsFile.hpp>

CgnsFile::CgnsFile(const Grid& grid, const std::string& folderPath) : grid(grid), folderPath(folderPath), baseName("Base"), zoneName("Zone"), physicalDimension(3) {
	this->zoneSizes.resize(3);
}

void CgnsFile::initialize() {
	this->writeBase();
	this->writeZone();
	this->writeCoordinates();
	this->writeSections();
	this->writeBoundaryConditions();
}

void CgnsFile::writePermanentField(const std::vector<double>& field, const std::string& fieldName) {
	cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, fieldName.c_str(), Vertex, &this->solutionIndex);
	cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, RealDouble, fieldName.c_str(), &field[0], &this->fieldIndex);
}

void CgnsFile::writeTimeSteps(const std::vector<double>& timeSteps) {
	this->numberOfTimeSteps = timeSteps.size();
	this->solutionIndices.resize(this->numberOfTimeSteps);

	for (int i = 0; i < this->numberOfTimeSteps; i++) {
		std::string solutionName = std::string("TimeStep") + std::to_string(i+1);
		cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, solutionName.c_str(), Vertex, &this->solutionIndices[i]);
	}
    
    cg_biter_write(this->fileIndex, this->baseIndex, "TimeIterativeValues", this->numberOfTimeSteps);
    cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, "end");

    cg_array_write("TimeValues", RealDouble, 1, &this->numberOfTimeSteps, &timeSteps[0]);
    cg_simulation_type_write(this->fileIndex, this->baseIndex, TimeAccurate);
}

void CgnsFile::writeTransientField(const std::vector<std::vector<double>>& field, const std::string& fieldName) {
	if (field.size() != this->numberOfTimeSteps) throw std::runtime_error("field and timeSteps sizes mismatch.");
	
	this->fieldsIndices.emplace_back(0);
	int fieldIndex = this->fieldsIndices.size() - 1;	
	cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndices[0], RealDouble, fieldName.c_str(), &field[0][0], &this->fieldsIndices[fieldIndex]);
	
	for (int i = 1; i < this->numberOfTimeSteps; i++) {
		cg_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndices[i], RealDouble, fieldName.c_str(), &field[i][0], &this->fieldsIndices[fieldIndex]);
	}
}