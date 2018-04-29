#include <IO/MshReader2D.hpp>

MshReader2D::MshReader2D(const std::string& filePath) : MshReader(filePath) {
	this->gridData->dimension = 2;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->divideConnectivities();
	this->processConnectivities();
	this->addFacets();
	this->addElements();
	this->defineBoundaryVertices();
}

void MshReader2D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) 
		this->file >> this->buffer;
	if (this->file.eof()) 
		throw std::runtime_error("MshReader2D: There is no Physical Entities data in the grid file");
	
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
	
	this->numberOfBoundaries = boundaryIndices.size();
	this->gridData->boundaries.resize(boundaryIndices.size());
	for (unsigned i = 0; i < boundaryIndices.size(); i++) 
		this->gridData->boundaries[i].name = entitiesNames[boundaryIndices[i]];

	this->numberOfRegions = regionsIndices.size();
	this->gridData->regions.resize(regionsIndices.size());
	for (unsigned i = 0; i < regionsIndices.size(); i++) 
		this->gridData->regions[i].name = entitiesNames[regionsIndices[i]];

	std::iota(boundaryIndices.begin(), boundaryIndices.end(), 0);
	std::iota(regionsIndices.begin(), regionsIndices.end(), 0);
}

void MshReader2D::divideConnectivities() {
	int numberOfFacets = 0;
	for (unsigned i = 0; i < this->connectivities.size(); i++) {
		if (this->connectivities[i][0] != 0) 
			break;
		else 
			numberOfFacets++;
	}
	this->facets   = std::vector<std::vector<int>>(this->connectivities.begin()                 , this->connectivities.begin() + numberOfFacets);
	this->elements = std::vector<std::vector<int>>(this->connectivities.begin() + numberOfFacets, this->connectivities.end());	
}

void MshReader2D::processConnectivities() {
	int counter = 0;
	std::vector<unsigned> regionStart;
	regionStart.emplace_back(0);
	for (unsigned i = 0; i < this->elements.size()-1; i++) {
		if (this->elements[i][1] == this->elements[i+1][1]) 
			counter++;
		else {
			counter++;
			regionStart.push_back(counter);
		}
	}
	regionStart.push_back(this->elements.size());
	
	for (unsigned i = 0; i < regionStart.size()-1; i++) 
		for (unsigned j = regionStart[i]; j < regionStart[i+1]; j++) 
			this->elements[j][1] = i;

	this->regionElements.resize(this->numberOfRegions, std::vector<int>());
	for (unsigned i = 0; i < this->elements.size(); i++)
		this->regionElements[this->elements[i][1]].push_back(i);

	printf("\n");
	print(elements, "elements");
	printf("\n");

	printf("\n");
	print(regionElements, "regionElements");
	printf("\n");

	this->boundaryFacets.resize(this->numberOfBoundaries, std::vector<int>());
	for (unsigned i = 0; i < this->facets.size(); i++)
		this->boundaryFacets[this->facets[i][1]].push_back(i);

	printf("\n");
	print(facets, "facets");
	printf("\n");

	printf("\n");
	print(boundaryFacets, "boundaryFacets");
	printf("\n");
}

void MshReader2D::addFacets() {
	for (unsigned i = 0; i < this->boundaryFacets.size(); i++) {
		for (unsigned j = 0; j < this->boundaryFacets[i].size(); j++) {
			int index = this->boundaryFacets[i][j];
			int type  = this->facets[index][0];
			auto first = this->facets[index].cbegin() + 2;
			auto last  = this->facets[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 0: {
					this->gridData->boundaries[i].lineConnectivity.emplace_back(std::move(connectivity));
					break;
				}
				default: 
					throw std::runtime_error("MshReader2D: Non supported facet found");
			}
		}
	}
}

void MshReader2D::addElements() {
	for (unsigned i = 0; i < this->regionElements.size(); i++) {
		for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
			int index = this->regionElements[i][j];
			int type  = this->elements[index][0];
			auto first = this->elements[index].cbegin() + 2;
			auto last  = this->elements[index].cend();
			std::vector<int> connectivity(first, last); 
			switch (type) {
				case 1: {
					this->gridData->triangleConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->regionElements[i];
					break;
				}
				case 2: {
					this->gridData->quadrangleConnectivity.emplace_back(std::move(connectivity));
					this->gridData->regions[i].elementsOnRegion = this->regionElements[i];
					break;
				}
				default: 
					throw std::runtime_error("MshReader2D: Non supported element found");
			}
		}
	}
}

void MshReader2D::defineBoundaryVertices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		for (auto j = boundary->lineConnectivity.cbegin(); j != boundary->lineConnectivity.cend(); j++) 
			for (auto k = j->cbegin(); k != j->cend(); k++) 
				vertices.insert(static_cast<int>(*k));
		boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
	}
}