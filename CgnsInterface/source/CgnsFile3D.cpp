#include <CgnsInterface/CgnsFile3D.hpp>

CgnsFile3D::CgnsFile3D(GridDataShared gridData, const std::string& folderPath) : 
	CgnsFile(gridData, folderPath) {
	this->setupFile();
	this->initialize();
}

void CgnsFile3D::defineGeometryType() {
	if (this->gridData->tetrahedronConnectivity.size() > 0 && this->gridData->hexahedronConnectivity.size() == 0) {
		this->geometry = TETRA_4;
		this->numberOfElements = this->gridData->tetrahedronConnectivity.size();
	}
	else if (this->gridData->tetrahedronConnectivity.size() == 0 && this->gridData->hexahedronConnectivity.size() > 0) {
		this->geometry = HEXA_8;
		this->numberOfElements = this->gridData->hexahedronConnectivity.size();
	}
	else throw std::runtime_error("CgnsFile3D: Geometry type not supported");
}

void CgnsFile3D::defineBoundaryType() {
	std::vector<bool> boundaryTypes;	
	for (auto i = this->gridData->boundaries.cbegin(); i < this->gridData->boundaries.cend(); i++) {
		if (i->lineConnectivity.size() == 0) {
			if      (i->triangleConnectivity.size()  > 0 && i->quadrangleConnectivity.size() == 0) boundaryTypes.emplace_back(true);
			else if (i->triangleConnectivity.size() == 0 && i->quadrangleConnectivity.size() >  0) boundaryTypes.emplace_back(false);
			else std::runtime_error("CgnsFile3D: Boundary is empty");
		}
		else throw std::runtime_error("CgnsFile3D: Line boundary is not supported");
	}
		
	if (std::all_of(boundaryTypes.cbegin(), boundaryTypes.cend(), [](const bool& i){return i == true;})) this->boundary = TRI_3;
	else this->boundary = QUAD_4;
}

void CgnsFile3D::setupFile() {
	this->defineGeometryType();
	this->defineBoundaryType();
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->numberOfVertices) + std::string("v_") + std::to_string(this->numberOfElements) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsFile3D::writeCoordinates() {
	double coordinatesX[this->numberOfVertices];
	double coordinatesY[this->numberOfVertices];
	double coordinatesZ[this->numberOfVertices];
	for (cgsize_t i = 0; i < this->numberOfVertices; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2]; 
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]);
}

void CgnsFile3D::writeSections() {
	switch (this->geometry) {
		case TETRA_4: {
			cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData->tetrahedronConnectivity);
			for (unsigned j = 0; j < this->gridData->tetrahedronConnectivity.size()*4; j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TETRA_4, 1, this->numberOfElements, zoneSizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		case HEXA_8: {
			cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData->hexahedronConnectivity);
			for (unsigned j = 0; j < this->gridData->hexahedronConnectivity.size()*8; j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", HEXA_8, 1, this->numberOfElements, zoneSizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		default: 
			throw std::runtime_error("CgnsFile3D: Geometry type not supported");
	}

	switch (this->boundary) {
		case TRI_3: {
			cgsize_t elementStart = this->numberOfElements + 1;
			cgsize_t elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				elementEnd = elementStart + this->gridData->boundaries[i].triangleConnectivity.size() - 1;
				cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData->boundaries[i].triangleConnectivity);
				for (unsigned j = 0; j < this->gridData->boundaries[i].triangleConnectivity.size()*3; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), TRI_3, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
				delete connectivities;
				elementStart = elementEnd + 1;
			}
			break;
		}
		case QUAD_4: {
			cgsize_t elementStart = this->numberOfElements + 1;
			cgsize_t elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				elementEnd = elementStart + this->gridData->boundaries[i].quadrangleConnectivity.size() - 1;
				cgsize_t* connectivities = determine_array_1d<cgsize_t>(this->gridData->boundaries[i].quadrangleConnectivity);
				for (unsigned j = 0; j < this->gridData->boundaries[i].quadrangleConnectivity.size()*4; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), QUAD_4, elementStart, elementEnd, this->zoneSizes[2], connectivities, &this->sectionIndices[i+1]);
				delete connectivities;
				elementStart = elementEnd + 1;
			}
			break;
		}
		default: 
			throw std::runtime_error("CgnsFile3D: Boundary type not supported");
	}
}