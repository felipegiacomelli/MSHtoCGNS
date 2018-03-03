#include <IO/MshReader2D.hpp>

MshReader2D::MshReader2D(const std::string& filePath) : 
	MshReader(filePath) {
	this->gridData->dimension = 2;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->addFacets();
	this->addElements();
	this->defineBoundaryVertices();
}

void MshReader2D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("MshReader2D: There is no Physical Entities data in the grid file");
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
			case 0: {
				boundaryIndices.push_back(entitiesIndices[i]);
				break;
			}
			case 1: {
				regionsIndices.push_back(entitiesIndices[i]);
				break;
			}
			default: 
				throw std::runtime_error("MshReader2D: Non supported physical entity found");
		}
	}
	std::iota(boundaryIndices.begin(), boundaryIndices.end(), 0);
	std::iota(regionsIndices.begin(), regionsIndices.end(), 0);

	print(boundaryIndices, "boundaryIndices");
	print(regionsIndices, "regionsIndices");

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
}

void MshReader2D::addFacets() {
	for (unsigned i = 0; i < this->facetsOnBoundary.size(); i++) {
		for (unsigned j = 0; j < this->facetsOnBoundary[i].size(); j++) {
			int index = this->facetsOnBoundary[i][j];
			int type  = this->facets[index][0];
			auto first = this->facets[index].cbegin() + 2;
			auto last  = this->facets[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 0: 
					this->gridData->boundaries[i].lineConnectivity.emplace_back(std::move(connectivity));
					break;
				default: 
					throw std::runtime_error("MshReader2D: Non supported facet found");
			}
		}
	}
}

void MshReader2D::addElements() {
	for (unsigned i = 0; i < this->elementsOnRegion.size(); i++) {
		for (unsigned j = 0; j < this->elementsOnRegion[i].size(); j++) {
			int index = this->elementsOnRegion[i][j];
			int type  = this->elements[index][0];
			auto first = this->elements[index].cbegin() + 2;
			auto last  = this->elements[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 1: 
					this->gridData->triangleConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->elementsOnRegion[i];
					break;
				case 2: 
					this->gridData->quadrangleConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->elementsOnRegion[i];
					break;
				default: 
					throw std::runtime_error("MshReader2D: Non supported element found");
			}
		}
	}
}

void MshReader2D::defineBoundaryVertices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		for (auto j = boundary->lineConnectivity.cbegin(); j != boundary->lineConnectivity.cend(); j++) {
			for (auto k = j->cbegin(); k != j->cend(); k++) {
				vertices.insert(*k);
			}
		}
		boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
	}
}