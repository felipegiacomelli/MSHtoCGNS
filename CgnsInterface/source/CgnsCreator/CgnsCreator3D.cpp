#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <cgnslib.h>

CgnsCreator3D::CgnsCreator3D(GridDataShared gridData, std::string folderPath) : CgnsCreator(gridData, folderPath) {
	this->checkDimension();
	this->setDimensions();
	this->defineGeometryType();
	this->setupFile();
	this->initialize();
}

void CgnsCreator3D::checkDimension() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error("CgnsCreator3D: gridData dimension must be equal to 3 and not " + std::to_string(this->gridData->dimension));
}

void CgnsCreator3D::setDimensions() {
	this->physicalDimension = this->gridData->dimension;
	this->cellDimension = this->gridData->dimension;
	this->coordinateIndices.resize(this->gridData->dimension);
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size();
	this->sizes[2] = 0;
}

void CgnsCreator3D::defineGeometryType() {
	if (this->gridData->tetrahedronConnectivity.size() > 0 && this->gridData->hexahedronConnectivity.size() == 0) {
		this->geometry = 10;
		if (this->gridData->triangleConnectivity.size() > 0 && this->gridData->quadrangleConnectivity.size() == 0)
			this->boundary = 5;
		else
			throw std::runtime_error("CgnsCreator3D: Boundary type not supported for given element type");
	}
	else if (this->gridData->tetrahedronConnectivity.size() == 0 && this->gridData->hexahedronConnectivity.size() > 0) {
		this->geometry = 17;
		if (this->gridData->triangleConnectivity.size() == 0 && this->gridData->quadrangleConnectivity.size() > 0)
			this->boundary = 7;
		else
			throw std::runtime_error("CgnsCreator3D: Boundary type not supported for given element type");
	}
	else
		throw std::runtime_error("CgnsCreator3D: Element type not supported");
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

void CgnsCreator3D::buildElementConnectivities() {
	for (auto i = this->gridData->tetrahedronConnectivity.cbegin(); i != this->gridData->tetrahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->hexahedronConnectivity.cbegin(); i != this->gridData->hexahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void CgnsCreator3D::writeRegions() {
	this->buildElementConnectivities();
	this->sectionIndices.emplace_back(0);

	std::vector<int> connectivities;
	append(this->elementConnectivities.cbegin(), this->elementConnectivities.cend(), std::back_inserter(connectivities));

	if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", ElementType_t(this->geometry),
							1, this->sizes[1], sizes[2], &connectivities[0], &this->sectionIndices.back()))
		throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(this->sectionIndices.size()));
}

void CgnsCreator3D::buildFacetConnectivities() {
	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++) {
		this->facetConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->facetConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++) {
		this->facetConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->facetConnectivities.back()), [](auto x){return x + 1;});
	}
	std::stable_sort(this->facetConnectivities.begin(), this->facetConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
	for (unsigned i = 0; i < this->facetConnectivities.size(); i++)
		this->facetConnectivities[i].pop_back();
}

void CgnsCreator3D::writeBoundaries() {
	this->buildFacetConnectivities();
	this->elementStart = this->sizes[1] + 1;

	for (auto boundary = this->gridData->boundaries.cbegin(); boundary != this->gridData->boundaries.cend(); boundary++) {
		this->sectionIndices.emplace_back(0);

		auto boundaryBegin = this->facetConnectivities.cbegin() + boundary->facetsOnBoundary.front() - this->sizes[1];
		auto boundaryEnd = this->facetConnectivities.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->sizes[1];
		this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

		std::vector<int> connectivities;
		append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), ElementType_t(this->boundary),
								this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back()))
			throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(this->sectionIndices.size()));

		this->elementStart = this->elementEnd + 1;
	}
}