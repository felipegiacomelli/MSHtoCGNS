#include <CgnsInterface/CgnsFile3D.hpp>

CgnsFile3D::CgnsFile3D(const GridData& gridData, const std::string& folderPath) : 
	CgnsFile(gridData, folderPath) {
	this->defineGeometryType();
	this->defineBoundaryType();
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->numberOfNodes) + std::string("n_") + std::to_string(this->numberOfElements) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsFile3D::defineGeometryType() {
	if (this->gridData.tetrahedronConnectivity.size() > 0 && this->gridData.hexahedronConnectivity.size() == 0) {
		this->tetrahedralGrid = true;
		this->hexahedralGrid  = false;
		this->numberOfElements = this->gridData.tetrahedronConnectivity.size();
	}
	else if (this->gridData.tetrahedronConnectivity.size() == 0 && this->gridData.hexahedronConnectivity.size() > 0) {
		this->tetrahedralGrid = false;
		this->hexahedralGrid  = true;
		this->numberOfElements = this->gridData.hexahedronConnectivity.size();
	}
	else throw std::runtime_error("Geometry type not supported");
}

void CgnsFile3D::defineBoundaryType() {
	std::vector<bool> boundaryTypes;
	for (auto i = this->gridData.boundaries.cbegin(); i < this->gridData.boundaries.cend(); i++) {
		if (i->lineConnectivity.size() == 0) {
			if      (i->triangleConnectivity.size()  > 0 && i->quadrangleConnectivity.size() == 0) boundaryTypes.emplace_back(true);
			else if (i->triangleConnectivity.size() == 0 && i->quadrangleConnectivity.size() >  0) boundaryTypes.emplace_back(false);
			else std::runtime_error("Boundary is empty");
		}
		else throw std::runtime_error("Line boundary is not supported");
	}

	if (std::all_of(boundaryTypes.cbegin(), boundaryTypes.cend(), [](bool i){return i == true;})) {
		this->triangularBoundary   = true;
		this->quadrangularBoundary = false;
	}
	else {
		this->triangularBoundary   = false;
		this->quadrangularBoundary = true;
	}
}

void CgnsFile3D::writeBase() {
	cg_base_write(this->fileIndex, this->baseName.c_str(), this->cellDimension, this->physicalDimension, &this->baseIndex);
}

void CgnsFile3D::writeZone() {
	this->zoneSizes[0] = this->numberOfNodes;
	this->zoneSizes[1] = this->numberOfElements;
	this->zoneSizes[2] = 0;	
	cg_zone_write(this->fileIndex, this->baseIndex, this->zoneName.c_str(), &this->zoneSizes[0], Unstructured, &this->zoneIndex);
}

void CgnsFile3D::writeCoordinates() {
	double coordinatesX[this->numberOfNodes];
	double coordinatesY[this->numberOfNodes];
	double coordinatesZ[this->numberOfNodes];
	for (cgsize_t i = 0; i < this->numberOfNodes; i++) {
		coordinatesX[i] = this->gridData.coordinates[i][0];
		coordinatesY[i] = this->gridData.coordinates[i][1];
		coordinatesZ[i] = this->gridData.coordinates[i][2]; 
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]);
}

void CgnsFile3D::writeSections() {
	cgsize_t elementStart = 1;
	cgsize_t elementEnd = this->numberOfElements;
	if (this->tetrahedralGrid) {
		cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData.tetrahedronConnectivity);
		for (unsigned int i = 0; i < this->gridData.tetrahedronConnectivity.size()*this->gridData.tetrahedronConnectivity[0].size(); i++) connectivities[i]++;
		cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TETRA_4, elementStart, elementEnd, zoneSizes[2], connectivities, &sectionIndices[0]);
		delete connectivities;

		for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
			elementStart = elementEnd + 1;
			elementEnd = elementStart + this->gridData.boundaries[i].triangleConnectivity.size() - 1;
			connectivities = determine_array_1d<cgsize_t>(this->gridData.boundaries[i].triangleConnectivity);
			for (unsigned int j = 0; j < this->gridData.boundaries[i].triangleConnectivity.size()*this->gridData.boundaries[i].triangleConnectivity[0].size(); j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), TRI_3, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
			delete connectivities;
		}
	}
	if (this->hexahedralGrid) {
		cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData.hexahedronConnectivity);
		for (unsigned int i = 0; i < this->gridData.hexahedronConnectivity.size()*this->gridData.hexahedronConnectivity[0].size(); i++) connectivities[i]++;
		cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", HEXA_8, elementStart, elementEnd, zoneSizes[2], connectivities, &sectionIndices[0]);
		delete connectivities;

		for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
			elementStart = elementEnd + 1;
			elementEnd = elementStart + this->gridData.boundaries[i].quadrangleConnectivity.size() - 1;
			connectivities = determine_array_1d<cgsize_t>(this->gridData.boundaries[i].quadrangleConnectivity);
			for (unsigned int j = 0; j < this->gridData.boundaries[i].quadrangleConnectivity.size()*this->gridData.boundaries[i].quadrangleConnectivity[0].size(); j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), TRI_3, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
			delete connectivities;
		}
	}
}

void CgnsFile3D::writeBoundaryConditions() {
	if (this->triangularBoundary) {
		for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
			std::set<int> vertices;
			for (auto j = this->gridData.boundaries[i].triangleConnectivity.cbegin(); j != this->gridData.boundaries[i].triangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(*k+1);
				}
			}
			cgsize_t* indices = determine_array_1d<cgsize_t>(vertices); 
			cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), BCWall, PointList, vertices.size(), indices, &this->boundaryIndices[i]);
			delete indices;
		}
	}
	if (this->quadrangularBoundary) {
		for (unsigned int i = 0; i < this->gridData.boundaries.size(); i++) {
			std::set<int> vertices;
			for (auto j = this->gridData.boundaries[i].quadrangleConnectivity.cbegin(); j != this->gridData.boundaries[i].quadrangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(*k+1);
				}
			}
			cgsize_t* indices = determine_array_1d<cgsize_t>(vertices); 
			cg_boco_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData.boundaries[i].name.c_str(), BCWall, PointList, vertices.size(), indices, &this->boundaryIndices[i]);
			delete indices;		
		}
	}
}