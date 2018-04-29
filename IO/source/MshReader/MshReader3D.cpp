#include <IO/MshReader3D.hpp>

MshReader3D::MshReader3D(const std::string& filePath) : MshReader(filePath) {
	this->gridData->dimension = 3;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->determineNumberOfFacets();
	this->divideConnectivities();
	this->assignElementsToRegions();
	this->assignFacetsToBoundaries();
	this->addRegions();
	this->addBoundaries();
	this->defineBoundaryVertices();
}

void MshReader3D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) 
		this->file >> this->buffer;
	if (this->file.eof()) 
		throw std::runtime_error("MshReader3D: There is no Physical Entities data in the grid file");
	
	this->file >> this->numberOfPhysicalEntities;
	std::vector<int> entitiesTypes;			
	std::vector<int> entitiesIndices;		
	std::vector<std::string> entitiesNames; 
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		int type, index;
		std::string name;
		this->file >> type >> index >> name;
		type--;
		index--;
		name.erase(name.begin()); name.erase(name.end()-1);
 		entitiesTypes.push_back(type);
		entitiesIndices.push_back(index);
		entitiesNames.push_back(name);
	}

	std::vector<int> regionsIndices, boundaryIndices;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		switch(entitiesTypes[i]) {
			case 1: {
				boundaryIndices.push_back(entitiesIndices[i]);
				break;
			}
			case 2: {
				regionsIndices.push_back(entitiesIndices[i]);;
				break;
			}
			default: 
				throw std::runtime_error("MshReader3D: Non supported physical entity found");
		}
	}
	if (regionsIndices.size() != 1) throw std::runtime_error("MshReader3D: One and only one geometry supported");
	
	this->numberOfBoundaries = boundaryIndices.size();
	this->gridData->boundaries.resize(boundaryIndices.size());
	for (unsigned i = 0; i < boundaryIndices.size(); i++) 
		this->gridData->boundaries[i].name = entitiesNames[boundaryIndices[i]];

	this->numberOfRegions = regionsIndices.size();
	this->gridData->regions.resize(regionsIndices.size());
	for (unsigned i = 0; i < regionsIndices.size(); i++) 
		this->gridData->regions[i].name = entitiesNames[regionsIndices[i]];

	std::iota(boundaryIndices.begin(), boundaryIndices.end(), 0);
}

void MshReader3D::determineNumberOfFacets() {
	this->numberOfFacets = 0;
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		if (this->connectivities[i][0] != 1 && this->connectivities[i][0] != 2)  
			break;
		else 
			this->numberOfFacets++;
	}
}

void MshReader3D::addRegions() {
	for (unsigned i = 0; i < this->regionElements.size(); i++) {
		for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
			int index = this->regionElements[i][j];
			int type  = this->elements[index][0];
			auto first = this->elements[index].cbegin() + 2;
			auto last  = this->elements[index].cend();
			std::vector<int> connectivity(first, last);
			this->gridData->regions[i].elementsOnRegion = this->regionElements[i];
			switch (type) {
				case 3: {
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(connectivity));
					break;
				}
				case 4: { 
					this->gridData->hexahedronConnectivity.emplace_back(std::move(connectivity));
					break;
				}
				default: 
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::addBoundaries() {
	for (unsigned i = 0; i < this->boundaryFacets.size(); i++) {
		for (unsigned j = 0; j < this->boundaryFacets[i].size(); j++) {
			int index = this->boundaryFacets[i][j];
			int type  = this->facets[index][0];
			auto first = this->facets[index].cbegin() + 2;
			auto last  = this->facets[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 1: {
					this->gridData->boundaries[i].triangleConnectivity.emplace_back(std::move(connectivity));
					break;
				}
				case 2: { 
					this->gridData->boundaries[i].quadrangleConnectivity.emplace_back(std::move(connectivity));
					break;
				}
				default: 
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::defineBoundaryVertices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary != this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		if (boundary->triangleConnectivity.size() > 0) {
			for (auto j = boundary->triangleConnectivity.cbegin(); j != boundary->triangleConnectivity.cend(); j++) 
				for (auto k = j->cbegin(); k != j->cend()-1; k++) 
					vertices.insert(*k);
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
		else {
			for (auto j = boundary->quadrangleConnectivity.cbegin(); j != boundary->quadrangleConnectivity.cend(); j++) 
				for (auto k = j->cbegin(); k != j->cend()-1; k++) 
					vertices.insert(*k);
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
	}
}