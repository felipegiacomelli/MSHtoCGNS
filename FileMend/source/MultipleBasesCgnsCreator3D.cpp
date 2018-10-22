#include <FileMend/MultipleBasesCgnsCreator3D.hpp>
#include <cgnslib.h>

MultipleBasesCgnsCreator3D::MultipleBasesCgnsCreator3D(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::string folderPath) : CgnsCreator(nullptr, folderPath), gridDatas(gridDatas), baseNames(baseNames), firstCall(true) {
	this->initialize();
}

void MultipleBasesCgnsCreator3D::initialize() {
	for (unsigned i = 0; i < this->gridDatas.size(); i++) {
		this->gridData = this->gridDatas[i];

		this->checkDimension();
		this->setDimensions();

		if (firstCall) {
			firstCall = false;
			this->setupFile();
		}

		this->baseName = this->baseNames[i];
		this->zoneName = this->baseNames[i];

		this->writeBase();
		this->writeZone();
		this->writeCoordinates();
		this->buildGlobalConnectivities();
		this->writeSections();
		this->writeBoundaryConditions();

		this->globalConnectivities.clear();
		this->globalConnectivities.clear();
		this->globalConnectivities.clear();

		this->elementStart = 1;
		this->elementEnd = 0;
	}
}

void MultipleBasesCgnsCreator3D::checkDimension() {
	if (this->gridData->dimension != 3)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be equal to 3 and not " + std::to_string(this->gridData->dimension));
}

void MultipleBasesCgnsCreator3D::setDimensions() {
	this->physicalDimension = this->gridData->dimension;
	this->cellDimension = this->gridData->dimension;
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size() + this->gridData->prismConnectivity.size() + this->gridData->pyramidConnectivity.size();
	this->sizes[2] = 0;
}

void MultipleBasesCgnsCreator3D::writeCoordinates() {
	std::vector<double> coordinatesX(this->sizes[0]);
	std::vector<double> coordinatesY(this->sizes[0]);
	std::vector<double> coordinatesZ(this->sizes[0]);
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2];
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", &coordinatesX[0], &this->coordinateIndex))
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateX");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", &coordinatesY[0], &this->coordinateIndex))
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateY");

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", &coordinatesZ[0], &this->coordinateIndex))
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateZ");
}

void MultipleBasesCgnsCreator3D::writeSections() {
	this->writeRegions();
	this->writeBoundaries();
	this->writeWells();
}

void MultipleBasesCgnsCreator3D::buildGlobalConnectivities() {
	for (auto i = this->gridData->tetrahedronConnectivity.cbegin(); i != this->gridData->tetrahedronConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->hexahedronConnectivity.cbegin(); i != this->gridData->hexahedronConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->prismConnectivity.cbegin(); i != this->gridData->prismConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}
	for (auto i = this->gridData->pyramidConnectivity.cbegin(); i != this->gridData->pyramidConnectivity.cend(); i++) {
		this->globalConnectivities.emplace_back(std::vector<int>());
		std::transform(i->cbegin(), i->cend(), std::back_inserter(this->globalConnectivities.back()), [](auto x){return x + 1;});
	}

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

void MultipleBasesCgnsCreator3D::writeRegions() {
	for (auto region : this->gridData->regions) {

		auto regionBegin = this->globalConnectivities.begin() + region.elementBegin;
		auto regionEnd = this->globalConnectivities.begin() + region.elementEnd;
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

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write element section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &this->sectionIndex))
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not partial write element section " + std::to_string(this->sectionIndex));

			for (auto element = regionBegin; element != regionEnd; element++) {
				switch (element->size()) {
					case 4: {
						element->insert(element->begin(), TETRA_4);
						break;
					}
					case 8: {
						element->insert(element->begin(), HEXA_8);
						break;
					}
					case 5: {
						element->insert(element->begin(), PYRA_5);
						break;
					}
					case 6: {
						element->insert(element->begin(), PENTA_6);
						break;
					}
					default:
						throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Element type not supported");
				}
			}

			std::vector<int> connectivities;
			append(regionBegin, regionEnd, std::back_inserter(connectivities));

			if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndex, this->elementStart, this->elementEnd, &connectivities[0]))
					throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write element " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
	}
}

void MultipleBasesCgnsCreator3D::writeBoundaries() {
	for (auto boundary : this->gridData->boundaries) {

		auto boundaryBegin = this->globalConnectivities.begin() + boundary.facetBegin;
		auto boundaryEnd = this->globalConnectivities.begin() + boundary.facetEnd;
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

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write facet section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &this->sectionIndex))
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not partial write facet section " + std::to_string(this->sectionIndex));

			for (auto facet = boundaryBegin; facet != boundaryEnd; facet++) {
				switch (facet->size()) {
					case 3: {
						facet->insert(facet->begin(), TRI_3);
						break;
					}
					case 4: {
						facet->insert(facet->begin(), QUAD_4);
						break;
					}
					default:
						throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Facet type not supported");
				}
			}

			std::vector<int> connectivities;
			append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

			if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndex, this->elementStart, this->elementEnd, &connectivities[0]))
					throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write facet " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndex));

			this->elementStart = this->elementEnd + 1;
		}
	}
}

void MultipleBasesCgnsCreator3D::writeWells() {
	for (auto& well : this->gridData->wells) {

		auto wellBegin = this->globalConnectivities.cbegin() + well.lineBegin;
		auto wellEnd = this->globalConnectivities.cbegin() + well.lineEnd;
		this->elementEnd = this->elementStart + (wellEnd - wellBegin) - 1;

		std::vector<int> connectivities;
		append(wellBegin, wellEnd, std::back_inserter(connectivities));

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, well.name.c_str(), BAR_2, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write well section " + std::to_string(this->sectionIndex));

		this->elementStart = this->elementEnd + 1;
	}
}