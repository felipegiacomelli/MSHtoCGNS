#include <IO/MshReader3D.hpp>

MshReader3D::MshReader3D(const std::string& filePath) : 
	MshReader(filePath) {
	this->gridData->dimension = 3;
	this->readNodes();
	this->readPhysicalEntities();
	this->readElements();
	this->addElements();
	this->defineBoundaryVerticesIndices();
}

void MshReader3D::readPhysicalEntities() {
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$PhysicalNames") && !this->file.eof()) this->file >> this->buffer;
	if (this->file.eof()) throw std::runtime_error("MshReader3D: There is no Physical Entities data in the grid file");
	this->file >> this->numberOfPhysicalEntities;
	std::vector<int> entitiesTypes;			int type;
	std::vector<int> entitiesNumbers;		int number;
	std::vector<std::string> entitiesNames; std::string name;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		file >> type >> number >> name;
		number--;
		name.erase(name.begin()); name.erase(name.end()-1);
 		entitiesTypes.push_back(type);
		entitiesNumbers.push_back(number);
		entitiesNames.push_back(name);
	}

	std::vector<int> geometryNumbers, boundaryNumbers;
	for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
		switch(entitiesTypes[i]) {
			case 2: {
				boundaryNumbers.push_back(entitiesNumbers[i]);
				break;
			}
			case 3: {
				geometryNumbers.push_back(entitiesTypes[i]);;
				break;
			}
			default: 
				throw std::runtime_error("MshReader3D: Non supported physical entity found");
		}
	}
	if (geometryNumbers.size() != 1) throw std::runtime_error("MshReader3D: One and only one geometry supported");

	this->gridData->boundaries.resize(boundaryNumbers.size());
	for (unsigned i = 0; i < boundaryNumbers.size(); i++) {
		this->gridData->boundaries[i].name = entitiesNames[boundaryNumbers[i]];
	}
}

void MshReader3D::addElements() {
	for (unsigned i = 0; i < this->physicalEntitiesElementIndices.size(); i++) {
		for (unsigned j = 0; j < this->physicalEntitiesElementIndices[i].size(); j++) {
			int index = this->physicalEntitiesElementIndices[i][j];
			int type  = this->elements[index][0];
			auto first = this->elements[index].cbegin() + 2;
			auto last  = this->elements[index].cend();
			std::vector<int> element(first, last);
			std::transform(std::begin(element), std::end(element), std::begin(element), [](const int& x){return x - 1;});
			
			switch (type) {
				case 2: 
					this->gridData->boundaries[i].triangleConnectivity.emplace_back(std::move(element));
					break;
				case 3: 
					this->gridData->boundaries[i].quadrangleConnectivity.emplace_back(std::move(element));
					break;
				case 4: 
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(element));
					break;
				case 5: 
					this->gridData->hexahedronConnectivity.emplace_back(std::move(element));
					break;
				default: 
					throw std::runtime_error("MshReader3D: Non supported element found");
			}
		}
	}
}

void MshReader3D::defineBoundaryVerticesIndices() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary != this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		if (boundary->triangleConnectivity.size() > 0) {
			for (auto j = boundary->triangleConnectivity.cbegin(); j != boundary->triangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(*k);
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
		else {
			for (auto j = boundary->quadrangleConnectivity.cbegin(); j != boundary->quadrangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(*k);
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
	}
}