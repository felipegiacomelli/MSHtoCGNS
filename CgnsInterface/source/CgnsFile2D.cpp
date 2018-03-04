#include <CgnsInterface/CgnsFile2D.hpp>

CgnsFile2D::CgnsFile2D(GridDataShared gridData, const std::string& folderPath) : 
	CgnsFile(gridData, folderPath) {
	this->setupFile();
	this->initialize();
}

void CgnsFile2D::defineGeometryType() {
	if (this->gridData->triangleConnectivity.size() > 0 && this->gridData->quadrangleConnectivity.size() == 0) {
		this->geometry = TRI_3;
		this->numberOfElements = this->gridData->triangleConnectivity.size();
	}
	else if (this->gridData->triangleConnectivity.size() == 0 && this->gridData->quadrangleConnectivity.size() > 0) {
		this->geometry = QUAD_4;
		this->numberOfElements = this->gridData->quadrangleConnectivity.size();
	}
	else if (this->gridData->triangleConnectivity.size() > 0 && this->gridData->quadrangleConnectivity.size() > 0) {
		this->geometry = MIXED;
		this->numberOfElements = this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
	}
	else throw std::runtime_error("CgnsFile2D: Geometry type not supported!!");
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
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = 0.0; 
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]);
}

void CgnsFile2D::writeSections() {
	std::vector<std::vector<int>> conn;
	conn.insert(conn.end(), this->gridData->triangleConnectivity.begin(), this->gridData->triangleConnectivity.end());
	conn.insert(conn.end(), this->gridData->quadrangleConnectivity.begin(), this->gridData->quadrangleConnectivity.end());
	cgsize_t elementStart = 1;
	cgsize_t elementEnd = 0;

	for (unsigned i = 0; i < this->gridData->regions.size(); i++) {
		std::vector<std::vector<int>> a(conn.cbegin() + this->gridData->regions[i].elementsOnRegion.front(), conn.cbegin() + this->gridData->regions[i].elementsOnRegion.back() + 1);
	 	elementEnd += a.size();
		switch (this->gridData->regions[i].elementType) {
			case 1:  {
				cgsize_t* connectivities = determine_array_1d<cgsize_t>(a);
				std::transform(&connectivities[0], &connectivities[a.size()*3], &connectivities[0], [](const cgsize_t& x){return x + 1;});
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), TRI_3, elementStart, elementEnd, zoneSizes[2], connectivities, &sectionIndices.back());
				delete connectivities;
				break;
			}
			case 2: {
				cgsize_t* connectivities = determine_array_1d<cgsize_t>(a);
				std::transform(&connectivities[0], &connectivities[a.size()*4], &connectivities[0], [](const cgsize_t& x){return x + 1;});
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), QUAD_4, elementStart, elementEnd, zoneSizes[2], connectivities, &sectionIndices.back());
				delete connectivities;
				break;
			}
			default:
				throw std::runtime_error("CgnsFile2D: Geometry type not supported");
				cg_error_exit();
		}
		elementStart = elementEnd + 1;
	}
	
	elementStart = this->numberOfElements + 1;
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		elementEnd = elementStart + this->gridData->boundaries[i].lineConnectivity.size() - 1;
		cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData->boundaries[i].lineConnectivity);
		std::transform(&connectivities[0], &connectivities[this->gridData->boundaries[i].lineConnectivity.size()*2], &connectivities[0], [](const cgsize_t& x){return x + 1;});
		cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BAR_2, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
		delete connectivities;
		elementStart = elementEnd + 1;
	}
}

void CgnsFile2D::writeBoundaryConditions() {
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		int numberOfVertices = this->gridData->boundaries[i].vertices.size();
		cgsize_t* indices = determine_array_1d<cgsize_t>(this->gridData->boundaries[i].vertices); 
		std::transform(&indices[0], &indices[numberOfVertices], &indices[0], [](const cgsize_t& x){return x + 1;});
		cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BCWall, PointList, numberOfVertices, indices, &this->boundaryIndices[i]);
		delete indices;
	}
}