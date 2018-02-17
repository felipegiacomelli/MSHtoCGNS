#include <CgnsInterface/CgnsFile.hpp>

CgnsFile::CgnsFile(const GridData& gridData, const std::string& folderPath) : gridData(gridData), folderPath(folderPath), baseName("Base"), zoneName("Zone"), physicalDimension(3) {
	this->numberOfNodes = this->gridData.coordinates.size();
	this->cellDimension = this->gridData.dimension;
}

void CgnsFile::initialize() {
	this->resizeVectors();
	this->writeBase();
	this->writeZone();
	this->writeCoordinates();
	this->writeSections();
	this->writeBoundaryConditions();
}

void CgnsFile::resizeVectors() {
	this->zoneSizes.resize(3);
	this->coordinateIndices.resize(3);
	this->sectionIndices.resize(this->gridData.boundaries.size() + 1);
	this->boundaryIndices.resize(this->gridData.boundaries.size());
}

void CgnsFile::writeBase() {
	cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex);
}

void CgnsFile::writeZone() {
	this->zoneSizes[0] = this->numberOfNodes;
	this->zoneSizes[1] = this->numberOfElements;
	this->zoneSizes[2] = 0;	
	cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), &this->zoneSizes[0], Unstructured, &this->zoneIndex);
}

std::string CgnsFile::getFileName() const {
	return this->fileName;
}

CgnsFile::~CgnsFile() {
	cg_close(this->fileIndex);
}