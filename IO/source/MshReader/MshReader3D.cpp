#include <IO/MshReader3D.hpp>

MshReader3D::MshReader3D(const std::string& filePath) : 
	MshReader(filePath) {
	this->gridData->dimension = 3;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->processConnectivities();
	this->addFacets();
	this->addElements();
	this->defineBoundaryVertices();
}

void MshReader3D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("MshReader3D: There is no Physical Entities data in the grid file");
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
	for (unsigned i = 0; i < boundaryIndices.size(); i++) {
		this->gridData->boundaries[i].name = entitiesNames[boundaryIndices[i]];
	}

	this->numberOfRegions = regionsIndices.size();
	this->gridData->regions.resize(regionsIndices.size());
	for (unsigned i = 0; i < regionsIndices.size(); i++) {
		this->gridData->regions[i].name = entitiesNames[regionsIndices[i]];
	}

	std::iota(boundaryIndices.begin(), boundaryIndices.end(), 0);
}

void MshReader3D::processConnectivities() {
	int numberOfFacets = 0;
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		if (this->connectivities[i][0] != 1 && this->connectivities[i][0] != 2) break;
		else numberOfFacets++;
	}
	this->facets   = std::vector<std::vector<int>>(this->connectivities.begin()                 , this->connectivities.begin() + numberOfFacets);
	this->elements = std::vector<std::vector<int>>(this->connectivities.begin() + numberOfFacets, this->connectivities.end());
	
	int counter = 0;
	std::vector<unsigned> regionStart;
	regionStart.emplace_back(0);
	for (unsigned i = 0; i < elements.size()-1; i++) {
		if (elements[i][1] == elements[i+1][1]) counter++;
		else {
			counter++;
			regionStart.push_back(counter);
		}
	}
	regionStart.push_back(elements.size());
	for (unsigned i = 0; i < regionStart.size()-1; i++) {
		for (unsigned j = regionStart[i]; j < regionStart[i+1]; j++) {
			elements[j][1] = i;
		}
	}

	this->facetsOnBoundary.resize(this->numberOfBoundaries, std::vector<int>());
	for (unsigned i = 0; i < this->facets.size(); i++) {
		facetsOnBoundary[facets[i][1]].push_back(i);
	}
	
	this->elementsOnRegion.resize(this->numberOfRegions, std::vector<int>());
	for (unsigned i = 0; i < this->elements.size(); i++) {
		elementsOnRegion[elements[i][1]].push_back(i);
	}
}

void MshReader3D::addFacets() {
	for (unsigned i = 0; i < this->facetsOnBoundary.size(); i++) {
		for (unsigned j = 0; j < this->facetsOnBoundary[i].size(); j++) {
			int index = this->facetsOnBoundary[i][j];
			int type  = this->facets[index][0];
			auto first = this->facets[index].cbegin() + 2;
			auto last  = this->facets[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 1: 
					this->gridData->boundaries[i].triangleConnectivity.emplace_back(std::move(connectivity));
					break;
				case 2: 
					this->gridData->boundaries[i].quadrangleConnectivity.emplace_back(std::move(connectivity));
					break;
				default: 
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::addElements() {
	for (unsigned i = 0; i < this->elementsOnRegion.size(); i++) {
		for (unsigned j = 0; j < this->elementsOnRegion[i].size(); j++) {
			int index = this->elementsOnRegion[i][j];
			int type  = this->elements[index][0];
			auto first = this->elements[index].cbegin() + 2;
			auto last  = this->elements[index].cend();
			std::vector<int> connectivity(first, last);
			switch (type) {
				case 3: 
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->elementsOnRegion[i];
					break;
				case 4: 
					this->gridData->hexahedronConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->elementsOnRegion[i];
					break;
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
			for (auto j = boundary->triangleConnectivity.cbegin(); j != boundary->triangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(static_cast<int>(*k));
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
		else {
			for (auto j = boundary->quadrangleConnectivity.cbegin(); j != boundary->quadrangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(static_cast<int>(*k));
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
	}
}