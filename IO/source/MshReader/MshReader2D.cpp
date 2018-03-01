#include <IO/MshReader2D.hpp>

MshReader2D::MshReader2D(const std::string& filePath) : 
	MshReader(filePath) {
	this->gridData->dimension = 2;
	this->readNodes();
	this->readPhysicalEntities();
	this->readConnectivities();
	this->addElements();
	this->defineBoundaryVertices();
}

void MshReader2D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("MshReader2D: There is no Physical Entities data in the grid file");
	this->file >> this->numberOfPhysicalEntities;
	std::vector<int> entitiesTypes;			int type;
	std::vector<int> entitiesIndices;		int index;
	std::vector<std::string> entitiesNames; std::string name;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		file >> type >> index >> name;
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
				regionsIndices.push_back(entitiesIndices[i]);
				break;
			}
			default: 
				throw std::runtime_error("MshReader2D: Non supported physical entity found");
		}
	}

	this->numberOfFacets = boundaryIndices.size();
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

void MshReader2D::addElements() {
	for (unsigned i = 0; i < this->physicalEntitiesElementIndices.size(); i++) {
		for (unsigned j = 0; j < this->physicalEntitiesElementIndices[i].size(); j++) {
			int index = this->physicalEntitiesElementIndices[i][j];
			int type  = this->connectivities[index][0];
			auto first = this->connectivities[index].cbegin() + 2;
			auto last  = this->connectivities[index].cend();
			std::vector<int> connectivity(first, last); 
			std::transform(std::begin(connectivity), std::end(connectivity), std::begin(connectivity), [](const int& x){return x - 1;});
			switch (type) {
				case 1: 
					this->gridData->boundaries[i].lineConnectivity.emplace_back(std::move(connectivity));
					break;
				case 2: 
					this->gridData->triangleConnectivity.emplace_back(std::move(connectivity));
					break;
				case 3: 
					this->gridData->quadrangleConnectivity.emplace_back(std::move(connectivity));
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