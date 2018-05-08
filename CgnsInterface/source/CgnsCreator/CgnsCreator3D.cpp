#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <cgnslib.h>

CgnsCreator3D::CgnsCreator3D(GridDataShared gridData, const std::string& folderPath) : CgnsCreator(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size();
	this->sizes[2] = 0;
	this->defineGeometryType();
	this->setupFile();
	this->initialize();
}

void CgnsCreator3D::defineGeometryType() {
	if (this->gridData->tetrahedronConnectivity.size() > 0 && this->gridData->hexahedronConnectivity.size() == 0) {
		this->geometry = 4;
		this->boundary = 3;
	}
	else if (this->gridData->tetrahedronConnectivity.size() == 0 && this->gridData->hexahedronConnectivity.size() > 0) {
		this->geometry = 8;
		this->boundary = 4;
	}
	else throw std::runtime_error("CgnsCreator3D: Geometry type not supported");
}

void CgnsCreator3D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	double coordinatesZ[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2];
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]))
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateX");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]))
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateY");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]))
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateZ");

}

void CgnsCreator3D::writeRegions() {
	this->sectionIndices.emplace_back(0);

	switch (this->geometry) {
		case 4: {
			std::vector<std::vector<int>> regionConnectivities;
			for (auto i = this->gridData->tetrahedronConnectivity.cbegin(); i != this->gridData->tetrahedronConnectivity.cend(); i++) {
				std::vector<int> tetrahedron(i->cbegin(), i->cend()-1);
				for (unsigned j = 0; j < tetrahedron.size(); j++)
					tetrahedron[j]++;
				regionConnectivities.emplace_back(std::move(tetrahedron));
			}

			std::vector<int> connectivities = linearize(regionConnectivities);

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TETRA_4,
									1, this->sizes[1], sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(1));

			break;
		}
		case 8: {
			std::vector<std::vector<int>> regionConnectivities;
			for (auto i = this->gridData->hexahedronConnectivity.cbegin(); i != this->gridData->hexahedronConnectivity.cend(); i++) {
				std::vector<int> hexahedron(i->cbegin(), i->cend()-1);
				for (unsigned j = 0; j < hexahedron.size(); j++)
					hexahedron[j]++;
				regionConnectivities.emplace_back(std::move(hexahedron));
			}

			std::vector<int> connectivities = linearize(regionConnectivities);

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", HEXA_8,
									1, this->sizes[1], sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(1));

			break;
		}
		default:
			throw std::runtime_error("CgnsCreator3D: Geometry type not supported");
	}
}

void CgnsCreator3D::writeBoundaries() {
	this->sectionIndices.emplace_back(0);
	this->elementStart = this->sizes[1] + 1;

	switch (this->boundary) {
		case 3: {
			std::vector<std::vector<int>> facetConnectivity;
			for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++) {
				std::vector<int> triangle(i->cbegin(), i->cend()-1);
				for (unsigned j = 0; j < triangle.size(); j++)
					triangle[j]++;
				facetConnectivity.emplace_back(std::move(triangle));
			}

			for (auto boundary = this->gridData->boundaries.cbegin(); boundary != this->gridData->boundaries.cend(); boundary++) {
				std::vector<std::vector<int>> boundaryConnectivities(facetConnectivity.cbegin() + boundary->facetsOnBoundary.front() - this->sizes[1],
																facetConnectivity.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->sizes[1]);

				this->elementEnd = this->elementStart + boundaryConnectivities.size() - 1;

				std::vector<int> connectivities = linearize(boundaryConnectivities);

				if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), TRI_3,
										this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back()))
					throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(this->sectionIndices.size()));

				this->elementStart = this->elementEnd + 1;
			}
			break;
		}
		case 4: {
			std::vector<std::vector<int>> facetConnectivity;
			for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++) {
				std::vector<int> quadrangle(i->cbegin(), i->cend()-1);
				for (unsigned j = 0; j < quadrangle.size(); j++)
					quadrangle[j]++;
				facetConnectivity.emplace_back(std::move(quadrangle));
			}

			for (auto boundary = this->gridData->boundaries.cbegin(); boundary != this->gridData->boundaries.cend(); boundary++) {
				std::vector<std::vector<int>> boundaryConnectivities(facetConnectivity.cbegin() + boundary->facetsOnBoundary.front() - this->sizes[1],
																facetConnectivity.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->sizes[1]);

				this->elementEnd = this->elementStart + boundaryConnectivities.size() - 1;

				std::vector<int> connectivities = linearize(boundaryConnectivities);

				if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), QUAD_4,
										this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back()))
					throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(this->sectionIndices.size()));

				this->elementStart = this->elementEnd + 1;
			}
			break;
		}
		default:
			throw std::runtime_error("CgnsCreator3D: Boundary type not supported");
	}
}