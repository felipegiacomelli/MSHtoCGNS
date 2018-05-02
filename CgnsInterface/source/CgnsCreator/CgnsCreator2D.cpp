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
	elementConnectivities.insert(elementConnectivities.end(), this->gridData->triangleConnectivity.begin(), this->gridData->triangleConnectivity.end());
	elementConnectivities.insert(elementConnectivities.end(), this->gridData->quadrangleConnectivity.begin(), this->gridData->quadrangleConnectivity.end());
	int elementStart = 1;
	int elementEnd = 0;

	for (unsigned i = 0; i < this->gridData->regions.size(); i++) {
		this->sectionIndices.emplace_back(0);

		std::vector<std::vector<int>> regionConnectivities(elementConnectivities.cbegin() + this->gridData->regions[i].elementsOnRegion.front(), 
															elementConnectivities.cbegin() + this->gridData->regions[i].elementsOnRegion.back() + 1);
	 	for (unsigned j = 0; j < regionConnectivities.size(); j++) {
			regionConnectivities[j].pop_back(); 
			for (unsigned k = 0; k < regionConnectivities[j].size(); k++)
				regionConnectivities[j][k]++;		
	 	}
	 	elementEnd += regionConnectivities.size();
//////////////////////////////////////////////////////////
	 	ElementType_t elementType;
	 	if (std::all_of(regionConnectivities.cbegin(), regionConnectivities.cend(), [](const auto& connectivity){return connectivity.size() == 3u;})) 
	 		elementType = TRI_3;
	 	else if (std::all_of(regionConnectivities.cbegin(), regionConnectivities.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);})) 
	 		elementType = QUAD_4;
		else 
			elementType = MIXED;

		if (elementType != MIXED) {
			std::vector<int> connectivities = linearize(regionConnectivities);
			
			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), elementType, 
									elementStart, elementEnd, sizes[2], &connectivities[0], &this->sectionIndices.back())) 
				throw std::runtime_error("CgnsCreator2D: Could not write element section " + std::to_string(i+1));

			elementStart = elementEnd + 1;
		}
		else {
			if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), elementType, 
											elementStart, elementEnd, sizes[2], &this->sectionIndices.back())) 
			throw std::runtime_error("CgnsCreator2D: Could not partial write element section " + std::to_string(i+1));
		
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
					throw std::runtime_error("CgnsCreator2D: Could not write element " + std::to_string(elementStart) + " in section " + std::to_string(i+1));
	
				elementStart++;
			}
		}
	}
}

void CgnsCreator2D::writeBoundaries() {
	int elementStart = this->sizes[1] + 1;
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		std::vector<std::vector<int>> boundaryConnectivities(this->gridData->boundaries[i].lineConnectivity.cbegin(), 
																this->gridData->boundaries[i].lineConnectivity.cend());
		for (unsigned j = 0; j < boundaryConnectivities.size(); j++) 
			boundaryConnectivities[j].pop_back(); 
		int elementEnd = elementStart + this->gridData->boundaries[i].lineConnectivity.size() - 1;
		
		std::vector<int> connectivities = linearize(boundaryConnectivities);
		for (unsigned j = 0; j < connectivities.size(); j++) 
			connectivities[j]++;

		if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BAR_2, 
								elementStart, elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back())) 
			throw std::runtime_error("CgnsCreator2D: Could not write facet section " + std::to_string(i+1));
		
		elementStart = elementEnd + 1;
	}
}