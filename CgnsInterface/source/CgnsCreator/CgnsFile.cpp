#include <CgnsInterface/CgnsCreator/CgnsCreator.hpp>
#include <cgnslib.h>

CgnsCreator::CgnsCreator(GridDataShared gridData, const std::string& folderPath) : gridData(gridData), folderPath(folderPath) {
	this->baseName = "Base";
	this->zoneName = "Zone";
	this->physicalDimension = this->gridData->dimension;
	this->cellDimension = this->gridData->dimension;
	this->coordinateIndices.resize(this->gridData->dimension);
}

void CgnsCreator::initialize() {
	this->writeBase();
	this->writeZone();
	this->writeCoordinates();
	this->writeSections();
	this->writeBoundaryConditions();
}

void CgnsCreator::writeBase() {
	if (cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex)) {
		throw std::runtime_error("CgnsCreator: Could not write base");
	}
}

void CgnsCreator::writeZone() {
	if (cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), &this->sizes[0], Unstructured, &this->zoneIndex)) {
		throw std::runtime_error("CgnsCreator: Could not write zone");
	}
}

void CgnsCreator::writeBoundaryConditions() {
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		this->boundaryIndices.emplace_back(0);
		int numberOfVertices = this->gridData->boundaries[i].vertices.size();
		int* indices = determine_array_1d<int>(this->gridData->boundaries[i].vertices); 
		for (unsigned j = 0; j < this->gridData->boundaries[i].vertices.size(); j++) indices[j]++;
		
		if (cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BCWall, 
							PointList, numberOfVertices, indices, &this->boundaryIndices.back())) {
			throw std::runtime_error("CgnsCreator: Could not write boundary condition " + std::to_string(i+1));
		}
		if (cg_goto(this->fileIndex, this->baseIndex, "Zone_t", 1, "ZoneBC_t", 1, "BC_t", i+1, nullptr)) {
			throw std::runtime_error("CgnsCreator: Could go to boundary condition " + std::to_string(i+1) + " location");
		}
		if (cg_famname_write(this->gridData->boundaries[i].name.c_str())) {
			throw std::runtime_error("CgnsCreator: Could not write boundary condition " + std::to_string(i+1) + " family name");
		}
		delete indices;
	}
}

std::string CgnsCreator::getFileName() const {
	return this->fileName;
}

CgnsCreator::~CgnsCreator() {
	cg_close(this->fileIndex);
}