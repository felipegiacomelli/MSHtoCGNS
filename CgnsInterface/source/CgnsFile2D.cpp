#include <CgnsInterface/CgnsFile2D.hpp>

CgnsFile2D::CgnsFile2D(const GridData& gridData, const std::string& folderPath) : 
	CgnsFile(gridData, folderPath) {
	this->setupFile();
	this->initialize();
}

void CgnsFile2D::defineGeometryType() {
	if (this->gridData.triangleConnectivity.size() > 0 && this->gridData.quadrangleConnectivity.size() == 0) {
		this->geometry = TRI_3;
		this->numberOfElements = this->gridData.triangleConnectivity.size();
	}
	else if (this->gridData.triangleConnectivity.size() == 0 && this->gridData.quadrangleConnectivity.size() > 0) {
		this->geometry = QUAD_4;
		this->numberOfElements = this->gridData.quadrangleConnectivity.size();
	}
	else throw std::runtime_error("Geometry type not supported");
}

void CgnsFile2D::setupFile() {
	this->defineGeometryType();
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->numberOfNodes) + std::string("n_") + std::to_string(this->numberOfElements) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsFile2D::writeCoordinates() {
	double coordinatesX[this->numberOfNodes];
	double coordinatesY[this->numberOfNodes];
	double coordinatesZ[this->numberOfNodes];
	for (cgsize_t i = 0; i < this->numberOfNodes; i++) {
		coordinatesX[i] = this->gridData.coordinates[i][0];
		coordinatesY[i] = this->gridData.coordinates[i][1];
		coordinatesZ[i] = 0.0; 
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]);
}

void CgnsFile2D::writeSections() {
	switch (this->geometry) {
		case TRI_3:  {
			cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData.triangleConnectivity);
			for (unsigned int i = 0; i < this->gridData.triangleConnectivity.size()*this->gridData.triangleConnectivity[0].size(); i++) connectivities[i]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TRI_3, 1, this->numberOfElements, zoneSizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		case QUAD_4: {
			cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData.quadrangleConnectivity);
			for (unsigned int i = 0; i < this->gridData.quadrangleConnectivity.size()*this->gridData.quadrangleConnectivity[0].size(); i++) connectivities[i]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", QUAD_4, 1, this->numberOfElements, zoneSizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		default:
			throw std::runtime_error("Geometry type not supported");
			cg_error_exit();
			break;
	}

	for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
		cgsize_t elementStart = this->numberOfElements + 1;
		cgsize_t elementEnd = elementStart + this->gridData.boundaries[i].lineConnectivity.size() - 1;
		cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData.boundaries[i].lineConnectivity);
		for (unsigned int j = 0; j < this->gridData.boundaries[i].lineConnectivity.size()*this->gridData.boundaries[i].lineConnectivity[0].size(); j++) connectivities[j]++;
		cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), BAR_2, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
		delete connectivities;
	}
}

void CgnsFile2D::writeBoundaryConditions() {
	for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
		std::set<int> vertices;
		for (auto j = this->gridData.boundaries[i].lineConnectivity.cbegin(); j != this->gridData.boundaries[i].lineConnectivity.cend(); j++) {
			for (auto k = j->cbegin(); k != j->cend(); k++) {
				vertices.insert(*k+1);
			}
		}
		cgsize_t* indices = determine_array_1d<cgsize_t>(vertices); 
		cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), BCWall, PointList, vertices.size(), indices, &this->boundaryIndices[i]);
		delete indices;
	}
}
