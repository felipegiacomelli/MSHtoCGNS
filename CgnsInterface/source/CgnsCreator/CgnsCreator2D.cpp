#include <CgnsInterface/CgnsCreator/CgnsCreator2D.hpp>
#include <cgnslib.h>

CgnsCreator2D::CgnsCreator2D(GridDataShared gridData, const std::string& folderPath) : CgnsCreator(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
	this->sizes[2] = 0;
	this->setupFile();
	this->initialize();
}

void CgnsCreator2D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]))
		throw std::runtime_error("CgnsCreator2D: Could not write CoordinateX");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]))
		throw std::runtime_error("CgnsCreator2D: Could not write CoordinateY");
}

void CgnsCreator2D::buildElementConnectivities() {
	for (auto i = this->gridData->triangleConnectivity.cbegin(); i != this->gridData->triangleConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->quadrangleConnectivity.cbegin(); i != this->gridData->quadrangleConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void CgnsCreator2D::writeRegions() {
	this->buildElementConnectivities();
	for (auto region = this->gridData->regions.cbegin(); region != this->gridData->regions.cend(); region++) {
		this->sectionIndices.emplace_back(0);

		auto regionBegin = this->elementConnectivities.cbegin() + region->elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + region->elementsOnRegion.back() + 1;
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

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType,
									this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("CgnsCreator2D: Could not write element section " + std::to_string(this->sectionIndices.size()));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType,
											this->elementStart, this->elementEnd, sizes[2], &this->sectionIndices.back()))
			throw std::runtime_error("CgnsCreator2D: Could not partial write element section " + std::to_string(this->sectionIndices.size()));

			for (auto j = regionBegin; j != regionEnd; j++) {
				std::vector<int> connectivities = *j;

				switch (connectivities.size()) {
					case 3: {
						connectivities.insert(connectivities.begin(), TRI_3);
						break;
					}
					case 4: {
						connectivities.insert(connectivities.begin(), QUAD_4);
						break;
					}
					default:
						throw std::runtime_error("CgnsCreator2D: Element type not supported");
				}

				if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndices.back(),
												this->elementStart, this->elementStart, &connectivities[0]))
					throw std::runtime_error("CgnsCreator2D: Could not write element " + std::to_string(this->elementStart) + " in section " +
												std::to_string(this->sectionIndices.size()));

				this->elementStart++;
			}
		}
	}
}

void CgnsCreator2D::buildFacetConnectivities() {
	for (auto i = this->gridData->lineConnectivity.cbegin(); i != this->gridData->lineConnectivity.cend(); i++) {
		this->facetConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->facetConnectivities.back()), [](auto x){return x + 1;});
	}
	std::stable_sort(this->facetConnectivities.begin(), this->facetConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
	for (unsigned i = 0; i < this->facetConnectivities.size(); i++)
		this->facetConnectivities[i].pop_back();
}

void CgnsCreator2D::writeBoundaries() {
	this->buildFacetConnectivities();
	for (auto boundary = this->gridData->boundaries.cbegin(); boundary != this->gridData->boundaries.cend(); boundary++) {
		this->sectionIndices.emplace_back(0);

		auto boundaryBegin = this->facetConnectivities.cbegin() + boundary->facetsOnBoundary.front() - this->sizes[1];
		auto boundaryEnd = this->facetConnectivities.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->sizes[1];
		this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

		std::vector<int> connectivities;
		append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), BAR_2,
								this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back()))
			throw std::runtime_error("CgnsCreator2D: Could not write facet section " + std::to_string(this->sectionIndices.size()));

		this->elementStart = this->elementEnd + 1;
	}
}