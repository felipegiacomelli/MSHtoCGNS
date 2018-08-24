#include <SpecialCgnsCreator3D.hpp>
#include <cgnslib.h>

SpecialCgnsCreator3D::SpecialCgnsCreator3D(GridDataShared gridData, const std::string& folderPath) : CgnsCreator(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size() + this->gridData->prismConnectivity.size() + this->gridData->pyramidConnectivity.size();
	this->sizes[2] = 0;
	this->checkDimension();
	this->setupFile();
	this->initialize();
}

void SpecialCgnsCreator3D::checkDimension() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error("SpecialCgnsCreator3D: gridData dimension must be equal to 3 and not " + std::to_string(this->gridData->dimension));
}

void SpecialCgnsCreator3D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	double coordinatesZ[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2];
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]))
		throw std::runtime_error("SpecialCgnsCreator3D: Could not write CoordinateX");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]))
		throw std::runtime_error("SpecialCgnsCreator3D: Could not write CoordinateY");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]))
		throw std::runtime_error("SpecialCgnsCreator3D: Could not write CoordinateZ");

}

void SpecialCgnsCreator3D::buildElementConnectivities() {
	for (auto i = this->gridData->tetrahedronConnectivity.cbegin(); i != this->gridData->tetrahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->hexahedronConnectivity.cbegin(); i != this->gridData->hexahedronConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->prismConnectivity.cbegin(); i != this->gridData->prismConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->pyramidConnectivity.cbegin(); i != this->gridData->pyramidConnectivity.cend(); i++) {
		this->elementConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->elementConnectivities.back()), [](auto x){return x + 1;});
	}
	std::stable_sort(this->elementConnectivities.begin(), this->elementConnectivities.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});
	for (unsigned i = 0; i < this->elementConnectivities.size(); i++)
		this->elementConnectivities[i].pop_back();
}

void SpecialCgnsCreator3D::writeRegions() {
	this->buildElementConnectivities();

	for (auto& region : this->gridData->regions) {
		this->sectionIndices.emplace_back(0);

		auto regionBegin = this->elementConnectivities.cbegin() + region.elementsOnRegion.front();
		auto regionEnd = this->elementConnectivities.cbegin() + region.elementsOnRegion.back() + 1;
	 	this->elementEnd += (regionEnd - regionBegin);

	 	ElementType_t elementType = ElementType_t(0);
	 	if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
	 		elementType = TETRA_4;
	 	else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 8u;}))
	 		elementType = HEXA_8;
	 	else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 6u;}))
	 		elementType = PENTA_6;
	 	else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 5u;}))
	 		elementType = PYRA_5;
		else
			elementType = MIXED;

		if (elementType != MIXED) {
			std::vector<int> connectivities;
			append(regionBegin, regionEnd, std::back_inserter(connectivities));

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType,
									this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("SpecialCgnsCreator3D: Could not write element section " + std::to_string(this->sectionIndices.size()));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType,
											this->elementStart, this->elementEnd, sizes[2], &this->sectionIndices.back()))
				throw std::runtime_error("SpecialCgnsCreator3D: Could not partial write element section " + std::to_string(this->sectionIndices.size()));


			std::set<unsigned> shapeType;
			for (auto i = regionBegin; i != regionEnd; i++)
				shapeType.insert(i->size());

			std::vector<unsigned> shapeBegin;
			for (auto i = shapeType.cbegin(); i != shapeType.cend(); i++) {
				unsigned value = *i;
				auto first = std::find_if(regionBegin, regionEnd, [=](const auto& connectivity){return connectivity.size() == value;});
				shapeBegin.emplace_back(first - regionBegin);
			}
			std::stable_sort(shapeBegin.begin(), shapeBegin.end());
			shapeBegin.emplace_back(regionEnd - regionBegin);
			
			std::vector<int> connectivities;
			for (unsigned i = 0; i < shapeBegin.size()-1; i++) {
				std::vector<std::vector<int>> shapeConnectivities(regionBegin + shapeBegin[i], regionBegin + shapeBegin[i+1]);

				switch (shapeConnectivities[0].size()) {
					case 4: {
						elementType = TETRA_4;
						break;
					}
					case 8: {
						elementType = HEXA_8;
						break;
					}
					case 5: {
						elementType = PYRA_5;
						break;
					}
					case 6: {
						elementType = PENTA_6;
						break;
					}
					default:
						throw std::runtime_error("SpecialCgnsCreator3D: Element type not supported");
				}

				for (unsigned j = 0; j < shapeConnectivities.size(); j++)
					shapeConnectivities[j].insert(shapeConnectivities[j].begin(), elementType);

				append(shapeConnectivities.cbegin(), shapeConnectivities.cend(), std::back_inserter(connectivities));
			}
			
			if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndices.back(), this->elementStart, this->elementEnd, &connectivities[0]))
					throw std::runtime_error("SpecialCgnsCreator3D: Could not write element " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndices.size()));

			this->elementStart = this->elementEnd + 1;
		}
	}
}

void SpecialCgnsCreator3D::buildFacetConnectivities() {
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

void SpecialCgnsCreator3D::writeBoundaries() {
	this->buildFacetConnectivities();
	this->elementStart = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size() + this->gridData->prismConnectivity.size() + this->gridData->pyramidConnectivity.size() + 1;

	for (auto& boundary : this->gridData->boundaries) {
		this->sectionIndices.emplace_back(0);

		auto boundaryBegin = this->facetConnectivities.cbegin() + boundary.facetsOnBoundary.front() - this->sizes[1];
		auto boundaryEnd = this->facetConnectivities.cbegin() + boundary.facetsOnBoundary.back() + 1 - this->sizes[1];
		this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

		ElementType_t elementType;
	 	if (std::all_of(boundaryBegin, boundaryEnd, [](const auto& connectivity){return connectivity.size() == 3u;}))
	 		elementType = TRI_3;
	 	else if (std::all_of(boundaryBegin, boundaryEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
	 		elementType = QUAD_4;
		else
			elementType = MIXED;

		if (elementType != MIXED) {
			std::vector<int> connectivities;
			append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType,
									this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("SpecialCgnsCreator3D: Could not write facet section " + std::to_string(this->sectionIndices.size()));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType,
											this->elementStart, this->elementEnd, sizes[2], &this->sectionIndices.back()))
			throw std::runtime_error("SpecialCgnsCreator3D: Could not partial write facet section " + std::to_string(this->sectionIndices.size()));

			std::set<unsigned> shapeType;
			for (auto i = boundaryBegin; i != boundaryEnd; i++)
				shapeType.insert(i->size());

			std::vector<unsigned> shapeBegin;
			for (auto i = shapeType.cbegin(); i != shapeType.cend(); i++) {
				unsigned value = *i;
				auto first = std::find_if(boundaryBegin, boundaryEnd, [=](const auto& connectivity){return connectivity.size() == value;});
				shapeBegin.emplace_back(first - boundaryBegin);
			}
			std::stable_sort(shapeBegin.begin(), shapeBegin.end());
			shapeBegin.emplace_back(boundaryEnd - boundaryBegin);
			
			std::vector<int> connectivities;
			for (unsigned i = 0; i < shapeBegin.size()-1; i++) {
				std::vector<std::vector<int>> shapeConnectivities(boundaryBegin + shapeBegin[i], boundaryBegin + shapeBegin[i+1]);

				switch (shapeConnectivities[0].size()) {
					case 3: {
						elementType = TRI_3;
						break;
					}
					case 4: {
						elementType = QUAD_4;
						break;
					}
					default:
						throw std::runtime_error("SpecialCgnsCreator3D: Element type not supported");
				}

				for (unsigned j = 0; j < shapeConnectivities.size(); j++)
					shapeConnectivities[j].insert(shapeConnectivities[j].begin(), elementType);

				append(shapeConnectivities.cbegin(), shapeConnectivities.cend(), std::back_inserter(connectivities));
			}
			
			if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndices.back(), this->elementStart, this->elementEnd, &connectivities[0]))
					throw std::runtime_error("SpecialCgnsCreator3D: Could not write element " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndices.size()));

			this->elementStart = this->elementEnd + 1;
		}
	}
}