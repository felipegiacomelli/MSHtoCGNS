#include <CgnsInterface/CgnsCreator/CgnsCreator2D.hpp>
#include <cgnslib.h>

CgnsCreator2D::CgnsCreator2D(GridDataShared gridData, std::string folderPath) : CgnsCreator(gridData, folderPath) {
	this->checkDimension();
	this->setDimensions();
	this->setupFile();
	this->initialize();
}

void CgnsCreator2D::checkDimension() {
	if (this->gridData->dimension != 2)
		throw std::runtime_error("CgnsCreator2D: gridData dimension must be equal to 2 and not " + std::to_string(this->gridData->dimension));
}

void CgnsCreator2D::setDimensions() {
	this->physicalDimension = this->gridData->dimension;
	this->cellDimension = this->gridData->dimension;
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
	this->sizes[2] = 0;
}

void CgnsCreator2D::writeCoordinates() {
	std::vector<double> coordinatesX(this->sizes[0]);
	std::vector<double> coordinatesY(this->sizes[0]);
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", &coordinatesX[0], &this->coordinateIndex))
		throw std::runtime_error("CgnsCreator2D: Could not write CoordinateX");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", &coordinatesY[0], &this->coordinateIndex))
		throw std::runtime_error("CgnsCreator2D: Could not write CoordinateY");
}

void CgnsCreator2D::buildGlobalConnectivities() {
	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}

	for (auto i = this->gridData->lineConnectivity.cbegin(); i != this->gridData->lineConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}

	std::stable_sort(this->globalConnectivities.begin(), this->globalConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

	for (unsigned i = 0; i < this->globalConnectivities.size(); i++)
		this->globalConnectivities[i].pop_back();
}

void CgnsCreator2D::writeRegions() {
	for (auto region = this->gridData->regions.cbegin(); region != this->gridData->regions.cend(); region++) {

		auto regionBegin = this->globalConnectivities.begin() + region->elementsOnRegion.front();
		auto regionEnd = this->globalConnectivities.begin() + region->elementsOnRegion.back() + 1;
	 	this->elementEnd += (regionEnd - regionBegin);

	 	ElementType_t elementType;
	 	if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 3u;}))
	 		elementType = TRI_3;
	 	else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
	 		elementType = QUAD_4;
		else
			elementType = MIXED;

		if (elementType != MIXED) {
			std::vector<int> connectivities;
			append(regionBegin, regionEnd, std::back_inserter(connectivities));

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
				throw std::runtime_error("CgnsCreator2D: Could not write element section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &this->sectionIndex))
				throw std::runtime_error("CgnsCreator2D: Could not partial write element section " + std::to_string(this->sectionIndex));

			for (auto element = regionBegin; element != regionEnd; element++) {
				switch (element->size()) {
					case 3 : {
						element->insert(element->begin(), TRI_3);
						break;
					}
					case 4: {
						element->insert(element->begin(), QUAD_4);
						break;
					}
					default:
						throw std::runtime_error("SpecialCgnsCreator3D: Element type not supported");
				}
			}

			std::vector<int> connectivities;
			append(regionBegin, regionEnd, std::back_inserter(connectivities));

			if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndex, this->elementStart, this->elementEnd, &connectivities[0]))
					throw std::runtime_error("CgnsCreator2D: Could not write element " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
	}
}

void CgnsCreator2D::writeBoundaries() {
	for (auto boundary = this->gridData->boundaries.cbegin(); boundary != this->gridData->boundaries.cend(); boundary++) {

		auto boundaryBegin = this->globalConnectivities.cbegin() + boundary->facetsOnBoundary.front();
		auto boundaryEnd = this->globalConnectivities.cbegin() + boundary->facetsOnBoundary.back() + 1;
		this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

		std::vector<int> connectivities;
		append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), BAR_2, this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndex))
			throw std::runtime_error("CgnsCreator2D: Could not write facet section " + std::to_string(this->sectionIndex));

		this->elementStart = this->elementEnd + 1;
	}
}