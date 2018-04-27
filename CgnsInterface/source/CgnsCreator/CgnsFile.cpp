#include <CgnsInterface/CgnsCreator/CgnsFile.hpp>
#include <cgnslib.h>

CgnsFile::CgnsFile(GridDataShared gridData, const std::string& folderPath) : gridData(gridData), folderPath(folderPath) {
	this->baseName = "Base";
	this->zoneName = "Zone";
	this->physicalDimension = this->gridData->dimension;
	this->cellDimension = this->gridData->dimension;

	this->numberOfVertices = this->gridData->coordinates.size();
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
	this->coordinateIndices.resize(3);
	this->sectionIndices.resize(this->gridData->boundaries.size() + 1);
	this->boundaryIndices.resize(this->gridData->boundaries.size());
}

void CgnsFile::writeBase() {
	if (cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex)) {
		throw std::runtime_error("CgnsFile: Could not write base");
	}
}

void CgnsFile::writeZone() {
	this->sizes[0] = this->numberOfVertices;
	this->sizes[1] = this->numberOfElements;
	this->sizes[2] = 0;	
	if (cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), &this->sizes[0], Unstructured, &this->zoneIndex)) {
		throw std::runtime_error("CgnsFile: Could not write zone");
	}
}

void CgnsFile::writeBoundaryConditions() {
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		int numberOfVertices = this->gridData->boundaries[i].vertices.size();
		int* indices = determine_array_1d<int>(this->gridData->boundaries[i].vertices); 
		for (unsigned j = 0; j < this->gridData->boundaries[i].vertices.size(); j++) indices[j]++;
		if (cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BCWall, PointList, numberOfVertices, indices, &this->boundaryIndices[i])) {
			throw std::runtime_error("CgnsFile: Could not write boundary condition");
		}
		delete indices;
	}
}

std::string CgnsFile::getFileName() const {
	return this->fileName;
}

CgnsFile::~CgnsFile() {
	cg_close(this->fileIndex);
}