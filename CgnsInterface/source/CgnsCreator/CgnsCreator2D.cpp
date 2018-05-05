#include <CgnsInterface/CgnsCreator/CgnsCreator2D.hpp>
#include <cgnslib.h>

#include <Utilities/Print.hpp>

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

void CgnsCreator2D::writeRegions() {
	std::vector<std::vector<int>> elementConnectivities;
	for (auto i = this->gridData->triangleConnectivity.begin(); i < this->gridData->triangleConnectivity.end(); i++) {
		std::vector<int> triangle(i->begin(), i->end()-1);
		for (unsigned j = 0; j < triangle.size(); j++)
			triangle[j]++;
		elementConnectivities.emplace_back(std::move(triangle));
	}
	for (auto i = this->gridData->quadrangleConnectivity.begin(); i < this->gridData->quadrangleConnectivity.end(); i++) {
		std::vector<int> quadrangle(i->begin(), i->end()-1);
		for (unsigned j = 0; j < quadrangle.size(); j++)
			quadrangle[j]++;
		elementConnectivities.emplace_back(std::move(quadrangle));
	}

	int elementStart = 1;
	int elementEnd = 0;

	for (auto region = this->gridData->regions.begin(); region < this->gridData->regions.end(); region++) {
		this->sectionIndices.emplace_back(0);

		std::vector<std::vector<int>> regionConnectivities(elementConnectivities.cbegin() + region->elementsOnRegion.front(),
															elementConnectivities.cbegin() + region->elementsOnRegion.back() + 1);
	 	elementEnd += regionConnectivities.size();

	 	ElementType_t elementType;
	 	if (std::all_of(regionConnectivities.cbegin(), regionConnectivities.cend(), [](const auto& connectivity){return connectivity.size() == 3u;}))
	 		elementType = TRI_3;
	 	else if (std::all_of(regionConnectivities.cbegin(), regionConnectivities.cend(), [](const auto& connectivity){return connectivity.size() == 4u;}))
	 		elementType = QUAD_4;
		else
			elementType = MIXED;

		if (elementType != MIXED) {
			std::vector<int> connectivities = linearize(regionConnectivities);

			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType,
									elementStart, elementEnd, sizes[2], &connectivities[0], &this->sectionIndices.back()))
				throw std::runtime_error("CgnsCreator2D: Could not write element section " + std::to_string(region - this->gridData->regions.begin()));

			elementStart = elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region->name.c_str(), elementType,
											elementStart, elementEnd, sizes[2], &this->sectionIndices.back()))
			throw std::runtime_error("CgnsCreator2D: Could not partial write element section " + std::to_string(region - this->gridData->regions.begin()));

			for (unsigned j = 0; j < regionConnectivities.size(); j++) {
				std::vector<int> connectivities = regionConnectivities[j];

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
										elementStart, elementStart, &connectivities[0]))
					throw std::runtime_error("CgnsCreator2D: Could not write element " + std::to_string(elementStart) + " in section " +
												std::to_string(region - this->gridData->regions.begin()));

				elementStart++;
			}
		}
	}
}

void CgnsCreator2D::writeBoundaries() {
	int elementStart = this->sizes[1] + 1;
	std::vector<std::vector<int>> facetConnectivity;
	for (auto i = this->gridData->lineConnectivity.begin(); i < this->gridData->lineConnectivity.end(); i++) {
		std::vector<int> line(i->begin(), i->end());
		facetConnectivity.emplace_back(std::move(line));
	}

	for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
		this->sectionIndices.emplace_back(0);
		std::vector<std::vector<int>> boundaryConnectivities(facetConnectivity.cbegin() + boundary->facetsOnBoundary.front() - this->sizes[1],
																facetConnectivity.cbegin() + boundary->facetsOnBoundary.back() + 1 - this->sizes[1]);
		for (unsigned j = 0; j < boundaryConnectivities.size(); j++) {
			boundaryConnectivities[j].pop_back();
			for (unsigned k = 0; k < boundaryConnectivities[j].size(); k++)
				boundaryConnectivities[j][k]++;
		}
		int elementEnd = elementStart + boundaryConnectivities.size() - 1;

		std::vector<int> connectivities = linearize(boundaryConnectivities);

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary->name.c_str(), BAR_2,
								elementStart, elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back()))
			throw std::runtime_error("CgnsCreator2D: Could not write facet section " + std::to_string(boundary - this->gridData->boundaries.begin()));

		elementStart = elementEnd + 1;
	}
}