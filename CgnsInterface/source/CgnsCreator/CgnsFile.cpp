#include <CgnsInterface/CgnsCreator/CgnsFile.hpp>

CgnsFile::CgnsFile(GridDataShared gridData, const std::string& folderPath) : 
	gridData(gridData), folderPath(folderPath), baseName("Base"), zoneName("Zone"), physicalDimension(3) {
	this->numberOfVertices = this->gridData->coordinates.size();
	this->cellDimension = this->gridData->dimension;
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
	this->sectionIndices.resize(this->gridData->boundaries.size() + 1);
	this->boundaryIndices.resize(this->gridData->boundaries.size());
}

void CgnsFile::writeBase() {
	cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex);
}

void CgnsFile::writeZone() {
	this->zoneSizes[0] = this->numberOfVertices;
	this->zoneSizes[1] = this->numberOfElements;
	this->zoneSizes[2] = 0;	
	cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), &this->zoneSizes[0], Unstructured, &this->zoneIndex);
}

void CgnsFile::writeBoundaryConditions() {
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		int numberOfVertices = this->gridData->boundaries[i].vertices.size();
		cgsize_t* indices = determine_array_1d<cgsize_t>(this->gridData->boundaries[i].vertices); 
		for (unsigned j = 0; j < this->gridData->boundaries[i].vertices.size(); j++) indices[j]++;
		cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BCWall, PointList, numberOfVertices, indices, &this->boundaryIndices[i]);
		delete indices;
	}
}

std::string CgnsFile::getFileName() const {
	return this->fileName;
}

CgnsFile::~CgnsFile() {
	cg_close(this->fileIndex);
}